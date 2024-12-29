#include "client-manager.hpp"

ClientManager& ClientManager::Get()
{
    static ClientManager instance;
    return instance;
}

int ClientManager::Create(AMX* amx, const std::string& connectFunc, const std::string& failFunc, const std::string& disconnectFunc, const std::string& messageFunc)
{
    return clients_ += std::make_shared<Client>(amx, connectFunc, failFunc, disconnectFunc, messageFunc);
}

bool ClientManager::Destroy(int id)
{
    auto it = clients_.find(id);
    return it != clients_.end() ? clients_.erase(it->first) : false;
}

void ClientManager::DestroyAll()
{
    clients_.clear();
}

ClientManager::ClientPtr ClientManager::GetClient(int id)
{
    auto it = clients_.find(id);
    return it != clients_.end() ? it->second : nullptr;
}

ClientManager::ClientManager()
{
}