#pragma once

#include <asio/thread.hpp>
#include <functional>
#include <memory>
#include <types.hpp>
#include <amx/amx.h>

#include "websocket.hpp"
#include "indexed-vector.hpp"

class Server : public NoCopy
{
private:
    WebsocketServer server_;
    IndexedVector<WebsocketConnection> clients_;
    std::unique_ptr<asio::thread> thread_;

    int id_ = -1;
    AMX* const amx_;
    const std::string connectFunc_;
    const std::string disconnectFunc_;
    const std::string messageFunc_;

    bool isListening_ = false;

public:
    Server(AMX* amx, const std::string& connectFunc, const std::string& disconnectFunc, const std::string& messageFunc);
    ~Server();

    bool Listen(const std::string& host, const std::string& port);
    bool StopListen();
    bool IsListening() const;

    bool GetID() const;
    void SetID(int id);

    WebsocketConnectionPtr GetClient(int id);
    IndexedVector<WebsocketConnection> GetClients() const;

    int GetClientIDByConnection(WebsocketConnection connection);
};