#include "client.hpp"
#include "dispatcher.hpp"
#include "component.hpp"

bool Client::Connect(const std::string& ip)
{
    if (IsConnected())
    {
        return false;
    }

    websocketpp::lib::error_code ec;
    auto ptr = client_.get_connection(ip, ec);
    if (ptr == nullptr)
    {
        if (ip.find("ws://") == std::string::npos)
        {
            OmpWebsocketComponent::getInstance()->getCore()->logLn(LogLevel::Error, "Failed to connect to \"%s\": You only can connect using \"ws://\"", ec.message().c_str());
        }
        else
        {
            OmpWebsocketComponent::getInstance()->getCore()->logLn(LogLevel::Error, "Failed to connect to \"%s\": %s", ip.c_str(), ec.message().c_str());
        }
        return false;
    }

    connection_ = ptr->get_handle();
    client_.connect(ptr);

    thread_ = std::make_unique<asio::thread>(std::bind(&WebsocketClient::run, &client_));
    return true;
}

bool Client::Disconnect()
{
    if (!IsConnected())
    {
        return false;
    }

    client_.stop();
    thread_->join();

    isConnected_ = false;
    return client_.stopped();
}

bool Client::IsConnected() const
{
    return isConnected_;
}

bool Client::Send(const std::string& text, bool isBinary)
{
    if (!IsConnected())
    {
        return false;
    }

    auto opcode = isBinary ? websocketpp::frame::opcode::binary : websocketpp::frame::opcode::text;
    auto connection = client_.get_con_from_hdl(connection_);
    
    if (connection == nullptr)
    {
        return false;
    }
    return !connection->send(text, opcode);
}

int Client::GetID() const
{
    return id_;
}

void Client::SetID(int id)
{
    id_ = id;
}

Client::Client(
    AMX* amx,
    const std::string& connectFunc,
    const std::string& failFunc,
    const std::string& disconnectFunc,
    const std::string& messageFunc
) : amx_(amx),
    connectFunc_(connectFunc),
    failFunc_(failFunc),
    disconnectFunc_(disconnectFunc),
    messageFunc_(messageFunc)
{
    client_.set_access_channels(websocketpp::log::alevel::none);
    client_.set_error_channels(websocketpp::log::alevel::none);

    client_.set_open_handler([&](WebsocketConnection)
    {
        isConnected_ = true;

        int idx = GetID();
        AMX* amx = amx_;
        std::string func = connectFunc_;

        Dispatcher::Get().Dispatch([amx, idx, func]()
        {
            int funcIDX = 0;
            if (!amx_FindPublic(amx, func.c_str(), &funcIDX))
            {
                amx_Push(amx, idx);
                amx_Exec(amx, NULL, funcIDX);
            }
        });
    });

    client_.set_fail_handler([&](WebsocketConnection)
    {
        isConnected_ = false;

        int idx = GetID();
        AMX* amx = amx_;
        std::string func = failFunc;

        Dispatcher::Get().Dispatch([amx, idx, func]()
        {
            int funcIDX = 0;
            if (!amx_FindPublic(amx, func.c_str(), &funcIDX))
            {
                amx_Push(amx, idx);
                amx_Exec(amx, NULL, funcIDX);
            }
        });
    });

    client_.set_close_handler([&](WebsocketConnection)
    {
        isConnected_ = false;

        int idx = GetID();
        AMX* amx = amx_;
        std::string func = disconnectFunc_;

        Dispatcher::Get().Dispatch([amx, idx, func]()
        {
            int funcIDX = 0;
            if (!amx_FindPublic(amx, func.c_str(), &funcIDX))
            {
                amx_Push(amx, idx);
                amx_Exec(amx, NULL, funcIDX);
            }
        });
    });

    client_.set_message_handler([&](WebsocketConnection, WebsocketMessage msg)
    {
        std::string data = msg->get_payload();

        int idx = GetID();
        AMX* amx = amx_;
        std::string func = messageFunc_;

        Dispatcher::Get().Dispatch([amx, idx, func, data]()
        {
            int funcIDX = 0;
            if (!amx_FindPublic(amx, func.c_str(), &funcIDX))
            {
                cell addr = 0;

                amx_PushString(amx, &addr, NULL, data.c_str(), NULL, NULL);
                amx_Push(amx, idx);
                amx_Exec(amx, NULL, funcIDX);
                amx_Release(amx, addr);
            }
        });
    });

    client_.init_asio();
}

Client::~Client()
{
    Disconnect();
}