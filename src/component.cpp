#include <sdk.hpp>
#include <Server/Components/Pawn/Impl/pawn_natives.hpp>
#include <Server/Components/Pawn/Impl/pawn_impl.hpp>

#include "component.hpp"
#include "dispatcher.hpp"
#include "client-manager.hpp"
#include "server-manager.hpp"

void OmpWebsocketComponent::onLoad(ICore* c)
{
    core_ = c;
    core_->getEventDispatcher().addEventHandler(this);
    setAmxLookups(core_);
}

void OmpWebsocketComponent::onInit(IComponentList* components)
{
    pawn_ = components->queryComponent<IPawnComponent>();
    if (pawn_)
    {
        setAmxFunctions(pawn_->getAmxFunctions());
        setAmxLookups(components);
        pawn_->getEventDispatcher().addEventHandler(this);
    }
}

void OmpWebsocketComponent::onReady()
{
}

void OmpWebsocketComponent::onFree(IComponent* component)
{
    if (component == pawn_)
    {
        pawn_ = nullptr;
        setAmxFunctions();
        setAmxLookups();
    }
}

void OmpWebsocketComponent::free()
{
    core_->printLn("[omp-websocket] Websocket releasing...");
    ClientManager::Get().DestroyAll();
    ServerManager::Get().DestroyAll();
    delete this;
    core_->printLn("[omp-websocket] Websocket released.");
}

void OmpWebsocketComponent::reset()
{
}

void OmpWebsocketComponent::onAmxLoad(IPawnScript& script)
{
    pawn_natives::AmxLoad(script.GetAMX());
}

void OmpWebsocketComponent::onAmxUnload(IPawnScript& script)
{
}

void OmpWebsocketComponent::onTick(Microseconds elapsed, TimePoint now)
{
    Dispatcher::Get().Process();
}

OmpWebsocketComponent::~OmpWebsocketComponent()
{
    if (pawn_)
    {
        pawn_->getEventDispatcher().removeEventHandler(this);
    }
    if (core_)
    {
        core_->getEventDispatcher().removeEventHandler(this);
    }
}