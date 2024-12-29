#pragma once

#include <asio/thread.hpp>
#include <string>
#include <memory>
#include <types.hpp>
#include <amx/amx.h>

#include "websocket.hpp"

class Client : public NoCopy
{
private:
    int id_ = -1;
    bool isConnected_ = false;

    WebsocketClient client_;
    WebsocketConnection connection_;
    std::unique_ptr<asio::thread> thread_;

    AMX* const amx_;
    const std::string connectFunc_;
    const std::string failFunc_;
    const std::string disconnectFunc_;
    const std::string messageFunc_;

public:
    Client(AMX* amx, const std::string& connectFunc, const std::string& failFunc, const std::string& disconnectFunc, const std::string& messageFunc);
    ~Client();

    bool Connect(const std::string& ip);
    bool Disconnect();
    bool IsConnected() const;

    bool Send(const std::string& text, bool isBinary);

    int GetID() const;
    void SetID(int id);
};