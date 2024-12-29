#pragma once

#include <queue>
#include <mutex>
#include <functional>

#include <types.hpp>

class Dispatcher : public NoCopy
{
    typedef std::function<void()> Callback;
    typedef std::queue<Callback> CallbackQueue;

private:
    CallbackQueue queue_;
    std::mutex mutex_;

    explicit Dispatcher();

public:
    static Dispatcher& Get();

    void Dispatch(Callback cb);
    void Process();
};