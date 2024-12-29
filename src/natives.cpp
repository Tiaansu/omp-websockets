#include <sdk.hpp>
#include <Server/Components/Pawn/Impl/pawn_natives.hpp>

#include "component.hpp"
#include "client-manager.hpp"
#include "server-manager.hpp"

// Client natives
SCRIPT_API(CreateWSClient, int(std::string const& connectFunc, std::string const& failFunc, std::string const& disconnectFunc, std::string const& messageFunc))
{
    auto& manager = ClientManager::Get();
    int id = manager.Create(GetAMX(), connectFunc, failFunc, disconnectFunc, messageFunc);

    const auto& client = manager.GetClient(id);
    if (id != -1 && client != nullptr)
    {
        client->SetID(id);
    }
    return id;
}

SCRIPT_API(DestroyWSClient, bool(int id))
{
    return ClientManager::Get().Destroy(id);
}

SCRIPT_API(WSClientConnect, bool(int id, std::string const& ip))
{
    const auto& client = ClientManager::Get().GetClient(id);
    return client != nullptr ? client->Connect(ip) : false;
}

SCRIPT_API(WSClientDisconnect, bool(int id))
{
    const auto& client = ClientManager::Get().GetClient(id);
    return client != nullptr ? client->Disconnect() : false;
}

SCRIPT_API(WSClientIsConnected, bool(int id))
{
    const auto& client = ClientManager::Get().GetClient(id);
    return client != nullptr ? client->IsConnected() : false;
}

SCRIPT_API(WSClientSend, bool(int id, std::string const& text, bool isBinary))
{
    const auto& client = ClientManager::Get().GetClient(id);
    return client != nullptr ? client->Send(text, isBinary) : false;
}

// Server natives
SCRIPT_API(CreateWSServer, int(std::string const& connectFunc, std::string const& disconnectFunc, std::string const& messageFunc))
{
    auto& manager = ServerManager::Get();
    int id = manager.Create(GetAMX(), connectFunc, disconnectFunc, messageFunc);

    if (id != -1)
    {
        manager.GetServer(id)->SetID(id);
    }
    return id;
}

SCRIPT_API(DestroyWSServer, bool(int id))
{
    return ServerManager::Get().Destroy(id);
}

SCRIPT_API(WSServerStartListen, bool(int id, std::string const& host, std::string const& port))
{
    const auto& server = ServerManager::Get().GetServer(id);
    return server != nullptr ? server->Listen(host, port) : false;
}

SCRIPT_API(WSServerStopListen, bool(int id))
{
    const auto& server = ServerManager::Get().GetServer(id);
    return server != nullptr ? server->StopListen() : false;
}

SCRIPT_API(WSServerIsListening, bool(int id))
{
    const auto& server = ServerManager::Get().GetServer(id);
    return server != nullptr ? server->IsListening() : false;
}

SCRIPT_API(WSServerIsConnected, bool(int serverId, int clientId))
{
    const auto& server = ServerManager::Get().GetServer(serverId);
    if (server == nullptr)
    {
        return false;
    }
    return server->GetClient(clientId) != nullptr;
}
SCRIPT_API(WSServerSend, bool(int serverId, int clientId, std::string const& text, bool isBinary))
{
    const auto& server = ServerManager::Get().GetServer(serverId);
    if (server == nullptr)
    {
        return false;
    }

    auto client = server->GetClient(clientId);
    if (client == nullptr)
    {
        return false;
    }

    auto opcode = isBinary ? websocketpp::frame::opcode::binary : websocketpp::frame::opcode::text;
    return !client->send(text, opcode);
}

SCRIPT_API(WSServerSendToAll, int(int id, std::string const& text, bool isBinary))
{
    int clientsSent = 0;

    const auto& server = ServerManager::Get().GetServer(id);
    if (server == nullptr)
    {
        return -1;
    }

    auto opcode = isBinary ? websocketpp::frame::opcode::binary : websocketpp::frame::opcode::text;

    auto clients = server->GetClients();
    for (const auto& client : clients)
    {
        auto client_ = server->GetClient(client.first);
        if (client_ == nullptr)
        {
            continue;
        }

        client_->send(text, opcode);
        clientsSent ++;
    }
    return clientsSent;
}

SCRIPT_API(WSServerGetIP, bool(int serverId, int clientId, OutputOnlyString& ip))
{
    const auto& server = ServerManager::Get().GetServer(serverId);
    if (server == nullptr)
    {
        return false;
    }

    const auto& client = server->GetClient(clientId);
    if (client == nullptr)
    {
        return false;
    }

    ip = client->get_socket().remote_endpoint().address().to_string();
    return true;
}

SCRIPT_API(WSServerKick, bool(int serverId, int clientId, int code, std::string const& reason))
{
    const auto& server = ServerManager::Get().GetServer(serverId);
    if (server == nullptr)
    {
        return false;
    }

    const auto& client = server->GetClient(clientId);
    if (client == nullptr)
    {
        return false;
    }
    client->close(code, reason);
    return true;
}