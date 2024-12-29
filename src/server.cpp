#include "server.hpp"
#include "dispatcher.hpp"
#include "component.hpp"

bool Server::Listen(const std::string& host, const std::string& port)
{
    if (IsListening())
    {
        return false;
    }

    try
    {
        server_.listen(host, port);
        server_.start_accept();

        thread_ = std::make_unique<asio::thread>(std::bind(&WebsocketServer::run, &server_));

        isListening_ = true;
        return true;
    }
    catch (const std::exception& e)
    {
        OmpWebsocketComponent::getInstance()->getCore()->logLn(LogLevel::Error, "Failed to listen: %s", e.what());
        return false;
    }
}

bool Server::StopListen()
{
    if (!IsListening())
    {
        return false;
    }

    for (auto& client : clients_)
    {
        auto client_ = GetClient(client.first);
        if (client_ == nullptr)
        {
            continue;
        }
        client_->close(1000, "Stop listening");
    }

    clients_.clear();
    std::error_code ec;
    server_.stop_listening(ec);
    server_.stop();

    thread_->join();

    isListening_ = false;
    return true;
}

bool Server::IsListening() const
{
    return isListening_;
}

bool Server::GetID() const
{
    return id_;
}

void Server::SetID(int id)
{
    id_ = id;
}

WebsocketConnectionPtr Server::GetClient(int id)
{
    auto it = clients_.find(id);
    if (it == clients_.end())
    {
        return nullptr;
    }
    return server_.get_con_from_hdl(it->second);
}

IndexedVector<WebsocketConnection> Server::GetClients() const
{
    return clients_;
}

int Server::GetClientIDByConnection(WebsocketConnection connection)
{
    auto handle = server_.get_con_from_hdl(connection);

    for (const auto& client : clients_)
    {
        if (server_.get_con_from_hdl(client.second) == handle)
        {
            return client.first;
        }
    }
    return -1;
}

Server::Server(
    AMX* amx,
    const std::string& connectFunc,
    const std::string& disconnectFunc,
    const std::string& messageFunc
) : amx_(amx)
    , connectFunc_(connectFunc)
    , disconnectFunc_(disconnectFunc)
    , messageFunc_(messageFunc)
{
    server_.set_access_channels(websocketpp::log::alevel::none);
    server_.set_error_channels(websocketpp::log::elevel::none);

    server_.init_asio();

    server_.set_open_handler([&](WebsocketConnection hdl)
    {
        int clientID = (clients_ += hdl);
        int serverID = GetID();
        AMX* amx = amx_;
        std::string func = connectFunc_;

        Dispatcher::Get().Dispatch([amx, clientID, serverID, func]()
        {
            int funcIDX = 0;
            if (!amx_FindPublic(amx, func.c_str(), &funcIDX))
            {
                amx_Push(amx, clientID);
                amx_Push(amx, serverID);
                amx_Exec(amx, NULL, funcIDX);
            }
        });
    });

    server_.set_close_handler([&](WebsocketConnection hdl)
    {
        int clientID = GetClientIDByConnection(hdl);
        if (clientID != -1)
        {
            clients_ -= clientID;
        }

        int serverID = GetID();
        AMX* amx = amx_;
        std::string func = disconnectFunc_;

        Dispatcher::Get().Dispatch([amx, clientID, serverID, func]()
        {
            int funcIDX = 0;
            if (!amx_FindPublic(amx, func.c_str(), &funcIDX))
            {
                amx_Push(amx, clientID);
                amx_Push(amx, serverID);
                amx_Exec(amx, NULL, funcIDX);
            }
        });
    });

    server_.set_message_handler([&](WebsocketConnection hdl, WebsocketMessage msg)
    {
        int clientID = GetClientIDByConnection(hdl);
        std::string data = msg->get_payload();

        int serverID = GetID();
        AMX* amx = amx_;
        std::string func = messageFunc_;

        Dispatcher::Get().Dispatch([amx, clientID, serverID, func, data]()
        {
            int funcIDX = 0;
            if (!amx_FindPublic(amx, func.c_str(), &funcIDX))
            {
                cell addr = 0;

                amx_PushString(amx, &addr, NULL, data.c_str(), NULL, NULL);
                amx_Push(amx, clientID);
                amx_Push(amx, serverID);
                amx_Exec(amx, NULL, funcIDX);
                amx_Release(amx, addr);
            }
        });
    });
}

Server::~Server()
{
    StopListen();
}