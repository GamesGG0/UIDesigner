#pragma once
#include <Geode/utils/cocos.hpp>
#include <VirtualDOM.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

class VirtualNode : public CCNode, RegisterDOM<VirtualNode, "Node"> {
    friend class VirtualDOMManager;
    int m_nodeSelection = 0;

protected:
    Ref<CCNode> m_tether;
public:
    VirtualNode();
    VirtualNode(VirtualNode&);
    
    void addChild(CCNode*, int, int) override;
    void removeFromParent() override;
    void update(float dt) override;
    void replaceTether(CCNode*);

    virtual void settings();
    virtual void updateTether();
    virtual std::string emitCode(int indent = 0);
    virtual void addTetherChild(VirtualNode*);
    virtual VirtualNode* duplicate();

    virtual matjson::Value exportJSON();
    virtual void importJSON(matjson::Value value);

    std::string emitAttributes(matjson::Value json, int indent = 0);
};
