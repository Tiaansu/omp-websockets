#include "server-manager.hpp"

ServerManager& ServerManager::Get()
{
    static ServerManager instance;
    return instance;
}

int ServerManager::Create(
    AMX* amx,
    const std::string& connectFunc,
    const std::string& disconnectFunc,
    const std::string& messageFunc
)
{
    return servers_ += std::make_shared<Server>(amx, connectFunc, disconnectFunc, messageFunc);
}

bool ServerManager::Destroy(int id)
{
    auto it = servers_.find(id);
    if (it == servers_.end())
    {
        return false;
    }
    return servers_.erase(it->first);
}

void ServerManager::DestroyAll()
{
    servers_.clear();
}

ServerManager::ServerPtr ServerManager::GetServer(int id)
{
    auto it = servers_.find(id);
    if (it == servers_.end())
    {
        return nullptr;
    }
    return it->second;
}

const IndexedVector<ServerManager::ServerPtr>& ServerManager::GetServers() const
{
    return servers_;
}

ServerManager::ServerManager()
{
}