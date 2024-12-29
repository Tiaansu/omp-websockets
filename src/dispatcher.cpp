#include "dispatcher.hpp"

Dispatcher& Dispatcher::Get()
{
    static Dispatcher instance;
    return instance;
}

void Dispatcher::Dispatch(Callback cb)
{
    std::lock_guard<std::mutex> lg(mutex_);
    queue_.push(cb);
}

void Dispatcher::Process()
{
    std::lock_guard<std::mutex> lg(mutex_);
    while (!queue_.empty())
    {
        queue_.front()();
        queue_.pop();
    }
}

Dispatcher::Dispatcher()
{
}