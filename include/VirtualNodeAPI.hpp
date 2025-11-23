#pragma once
#include <Geode/utils/cocos.hpp>
#include <VirtualDOMAPI.hpp>
#include <matjson.hpp>

#define MY_MOD_ID "camila314.uidesigner"
using namespace geode::prelude;

class VirtualNode : public CCNode {
    friend class VirtualDOMManager;
    struct Impl final {
        int m_nodeSelection = 0;
    };
    std::unique_ptr<Impl> m_extraData;
protected:
    Ref<CCNode> m_tether;
public:
    void copyTraits(VirtualNode& vnode) GEODE_EVENT_EXPORT_NORES(&VirtualNode::copyTraits, (this, vnode));
    VirtualNode() { init(); }

    bool init() override GEODE_EVENT_EXPORT_NORES(&VirtualNode::init, (this));
    void addChild(CCNode* a, int b, int c) override GEODE_EVENT_EXPORT_NORES(&VirtualNode::addChild, (this, a, b, c));
    void removeFromParent() override GEODE_EVENT_EXPORT_NORES(&VirtualNode::removeFromParent, (this));
    void update(float dt) override GEODE_EVENT_EXPORT_NORES(&VirtualNode::update, (this, dt));
    void replaceTether(CCNode*) GEODE_EVENT_EXPORT_NORES(&VirtualNode::replaceTether, (this, std::declval<CCNode*>()));

    virtual void settings() GEODE_EVENT_EXPORT_NORES(&VirtualNode::settings, (this));
    virtual void updateTether() GEODE_EVENT_EXPORT_NORES(&VirtualNode::updateTether, (this));
    virtual std::string emitCode(int indent = 0) GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitCode, (this, indent));
    virtual void addTetherChild(VirtualNode* a) GEODE_EVENT_EXPORT_NORES(&VirtualNode::addTetherChild, (this, a));
    virtual matjson::Value exportJSON() GEODE_EVENT_EXPORT_NORES(&VirtualNode::exportJSON, (this));
    virtual void importJSON(matjson::Value value) GEODE_EVENT_EXPORT_NORES(&VirtualNode::importJSON, (this, value));

    std::string emitAttributes(matjson::Value json, int indent = 0) GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitAttributes, (this, json, indent));
};
