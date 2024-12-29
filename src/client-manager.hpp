#pragma once

#include <memory>
#include <types.hpp>

#include "client.hpp"
#include "indexed-vector.hpp"

class ClientManager : public NoCopy
{
    typedef std::shared_ptr<Client> ClientPtr;

private:
    IndexedVector<ClientPtr> clients_;

    explicit ClientManager();

public:
    static ClientManager& Get();

    int Create(AMX* amx, const std::string& connectFunc, const std::string& failFunc, const std::string& disconnectFunc, const std::string& messageFunc);
    bool Destroy(int id);
    void DestroyAll();

    ClientPtr GetClient(int id);
};