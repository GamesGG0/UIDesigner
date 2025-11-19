#pragma once
#include <Geode/utils/cocos.hpp>
#include <VirtualDOMAPI.hpp>
#include <matjson.hpp>

#define MY_MOD_ID "camila314.uidesigner"

#ifndef GEODE_DEFINE_EVENT_EXPORTS
#define IMPORT_INLINE inline
#else
#define IMPORT_INLINE
#endif

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
    IMPORT_INLINE void copyTraits(VirtualNode& vnode) GEODE_EVENT_EXPORT_NORES(&VirtualNode::copyTraits, (this, vnode));
    IMPORT_INLINE VirtualNode() { init(); }
    IMPORT_INLINE VirtualNode(VirtualNode& vnode) : VirtualNode() { copyTraits(vnode); }
    
    IMPORT_INLINE bool init() override GEODE_EVENT_EXPORT_NORES(&VirtualNode::init, (this));
    IMPORT_INLINE void addChild(CCNode* a, int b, int c) override GEODE_EVENT_EXPORT_NORES(&VirtualNode::addChild, (this, a, b, c));
    IMPORT_INLINE void removeFromParent() override GEODE_EVENT_EXPORT_NORES(&VirtualNode::removeFromParent, (this));
    IMPORT_INLINE void update(float dt) override GEODE_EVENT_EXPORT_NORES(&VirtualNode::update, (this, dt));
    IMPORT_INLINE void replaceTether(CCNode*) GEODE_EVENT_EXPORT_NORES(&VirtualNode::replaceTether, (this, std::declval<CCNode*>()));

    IMPORT_INLINE virtual void settings() GEODE_EVENT_EXPORT_NORES(&VirtualNode::settings, (this));
    IMPORT_INLINE virtual void updateTether() GEODE_EVENT_EXPORT_NORES(&VirtualNode::updateTether, (this));
    IMPORT_INLINE virtual std::string emitCode(int indent = 0) GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitCode, (this, indent));
    IMPORT_INLINE virtual void addTetherChild(VirtualNode* a) GEODE_EVENT_EXPORT_NORES(&VirtualNode::addTetherChild, (this, a));
    IMPORT_INLINE virtual VirtualNode* duplicate() GEODE_EVENT_EXPORT_NORES(&VirtualNode::duplicate, (this));
    IMPORT_INLINE virtual matjson::Value exportJSON() GEODE_EVENT_EXPORT_NORES(&VirtualNode::exportJSON, (this));
    IMPORT_INLINE virtual void importJSON(matjson::Value value) GEODE_EVENT_EXPORT_NORES(&VirtualNode::importJSON, (this, value));

    IMPORT_INLINE std::string emitAttributes(matjson::Value json, int indent = 0) GEODE_EVENT_EXPORT_NORES(&VirtualNode::emitAttributes, (this, json, indent));
};
