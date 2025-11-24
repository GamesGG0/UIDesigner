#pragma once
#include <Geode/utils/cocos.hpp>
#include <VirtualDOMAPI.hpp>
#include <matjson.hpp>

#define MY_MOD_ID "camila314.uidesigner"
using namespace geode::prelude;

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

        inline bool init() override;
        inline void addChild(CCNode* a, int b, int c) override;

        inline void removeFromParent() override;
        inline void update(float dt) override;
        void replaceTether(CCNode*) GEODE_EVENT_EXPORT_NORES(&VirtualNode::replaceTether, (this, std::declval<CCNode*>()));

        inline virtual void settings();
        inline virtual void updateTether();
        inline virtual std::string emitCode(int indent = 0);
        inline virtual void addTetherChild(VirtualNode* a);
        inline virtual matjson::Value exportJSON();
        inline virtual void importJSON(matjson::Value value);

        std::string emitAttributes(matjson::Value json, int indent = 0) GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitAttributes, (this, json, indent));
    };

    bool VirtualNode::init() GEODE_EVENT_EXPORT_NORES(&VirtualNode::init, (this));
    void VirtualNode::addChild(CCNode* a, int b, int c) GEODE_EVENT_EXPORT_NORES(&VirtualNode::addChild, (this, a, b, c));
    void VirtualNode::removeFromParent() GEODE_EVENT_EXPORT_NORES(&VirtualNode::removeFromParent, (this));
    void VirtualNode::update(float dt) GEODE_EVENT_EXPORT_NORES(&VirtualNode::update, (this, dt));
    void VirtualNode::settings() GEODE_EVENT_EXPORT_NORES(&VirtualNode::settings, (this));
    void VirtualNode::updateTether() GEODE_EVENT_EXPORT_NORES(&VirtualNode::updateTether, (this));
    std::string VirtualNode::emitCode(int indent) GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitCode, (this, indent));
    void VirtualNode::addTetherChild(VirtualNode* a) GEODE_EVENT_EXPORT_NORES(&VirtualNode::addTetherChild, (this, a));
    matjson::Value VirtualNode::exportJSON() GEODE_EVENT_EXPORT_NORES(&VirtualNode::exportJSON, (this));
    void VirtualNode::importJSON(matjson::Value value) GEODE_EVENT_EXPORT_NORES(&VirtualNode::importJSON, (this, value));
}
