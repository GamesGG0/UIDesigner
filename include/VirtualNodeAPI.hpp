#pragma once
#include <Geode/utils/cocos.hpp>
#include <VirtualDOMAPI.hpp>
#include <matjson.hpp>

#define MY_MOD_ID "camila314.uidesigner"
using namespace geode::prelude;

#ifndef GEODE_DEFINE_EVENT_EXPORTS
#define $_export(...) GEODE_EVENT_EXPORT_NORES(__VA_ARGS__)
#else
#define $_export(...)
#endif

namespace uidesigner {
    class VirtualNode : public CCNode {
        friend class VirtualDOMManager;
        struct Impl final {
            int m_nodeSelection = 0;
        };
        std::unique_ptr<Impl> m_extraData;
    protected:
        Ref<CCNode> m_tether;
    public:
        VirtualNode() { init(); }

        bool init() override $_export(&VirtualNode::init, (this));
        void addChild(CCNode* a, int b, int c) override $_export(&VirtualNode::addChild, (this, a, b, c));
        void removeFromParent() override $_export(&VirtualNode::removeFromParent, (this));
        void update(float dt) override $_export(&VirtualNode::update, (this, dt));

        void replaceTether(CCNode*) GEODE_EVENT_EXPORT_NORES(&VirtualNode::replaceTether, (this, std::declval<CCNode*>()));

        virtual void settings() $_export(&VirtualNode::settings, (this));
        virtual void updateTether() $_export(&VirtualNode::updateTether, (this));
        virtual std::string emitCode(int indent = 0) $_export(&VirtualNode::emitCode, (this, indent));
        virtual void addTetherChild(VirtualNode* a) $_export(&VirtualNode::addTetherChild, (this, a));
        virtual matjson::Value exportJSON() $_export(&VirtualNode::exportJSON, (this));
        virtual void importJSON(matjson::Value value) $_export(&VirtualNode::importJSON, (this, value));

        std::string emitAttributes(matjson::Value json, int indent = 0) GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitAttributes, (this, json, indent));
    };

    #ifdef GEODE_DEFINE_EVENT_EXPORTS
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::init, (this));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::addChild, (this, a, b, c));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::removeFromParent, (this));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::update, (this, dt));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::settings, (this));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::updateTether, (this));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitCode, (this, indent));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::addTetherChild, (this, a));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::exportJSON, (this));
        GEODE_EVENT_EXPORT_NORES(&VirtualNode::importJSON, (this, value));
    #endif
}
