#pragma once

#include <sdk.hpp>
#include <Server/Components/Pawn/pawn.hpp>

#include "interface.hpp"

class OmpWebsocketComponent final
    : public IOmpWebsocketComponent
    , public PawnEventHandler
    , public CoreEventHandler
{
private:
    ICore* core_ = nullptr;
    IPawnComponent* pawn_ = nullptr;
    inline static OmpWebsocketComponent* instance_ = nullptr;

public:
    ~OmpWebsocketComponent();

    StringView componentName() const override
    {
        return "Websocket";
    }

    SemanticVersion componentVersion() const override
    {
        return SemanticVersion(0, 0, 1, 0);
    }

    void onLoad(ICore* c) override;
    void onInit(IComponentList* components) override;
    void onReady() override;
    void onFree(IComponent* component) override;
    
    void free() override;
    void reset() override;

    void onAmxLoad(IPawnScript& script) override;
    void onAmxUnload(IPawnScript& script) override;

    void onTick(Microseconds elapsed, TimePoint now) override;

    static OmpWebsocketComponent* getInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new OmpWebsocketComponent();
        }
        return instance_;
    }

    ICore* getCore()
    {
        return core_;
    }
};