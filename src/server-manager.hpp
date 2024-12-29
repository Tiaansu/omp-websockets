#pragma once

#include <string>
#include <memory>

#include "server.hpp"
#include "indexed-vector.hpp"

class ServerManager
{
    typedef std::shared_ptr<Server> ServerPtr;

private:
    IndexedVector<ServerPtr> servers_;
    
    explicit ServerManager();

public:
    static ServerManager& Get();

    int Create(AMX* amx, const std::string& connectFunc, const std::string& disconnectFunc, const std::string& messageFunc);
    bool Destroy(int id);
    void DestroyAll();

    ServerPtr GetServer(int id);
    
    const IndexedVector<ServerPtr>& GetServers() const;
};