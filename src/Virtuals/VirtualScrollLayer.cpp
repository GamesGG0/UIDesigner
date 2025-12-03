#include <VirtualNode.hpp>
#include "VirtualRGBA.hpp"
#include <Geode/ui/ScrollLayer.hpp>
#include <geode.devtools/include/API.hpp>

using namespace geode::prelude;

class VirtualContentLayer : public VirtualRGBA, RegisterDOM<VirtualContentLayer, "Content"> {

public:
    CCContentLayer* m_CCContentLayerObject;

    VirtualContentLayer() : VirtualRGBA() {
        m_CCContentLayerObject = CCContentLayer::create(ccc4(m_color.r, m_color.g, m_color.b, m_opacity), 50, 50);
        m_tether = m_CCContentLayerObject;

        setAnchorPoint({0.5, 0.5});
        setContentSize({100., 100.});
    }

    matjson::Value exportJSON() override {
        auto obj = VirtualRGBA::exportJSON();
        obj["type"] = "Content";
        return obj;
    }

    std::string emitCode(int indent = 0) override {
        std::string ind(indent, ' ');
        std::string output = fmt::format("{}Build<CCContentLayer>::create()\n", ind);

        output += VirtualRGBA::emitAttributes(exportJSON(), indent + 4);

        return output;
    }
};


class VirtualScrollLayer : public VirtualNode, RegisterDOM<VirtualScrollLayer, "Scroll Layer"> {
    bool m_vertical = true;
    bool m_scrollWheelEnabled = true;

    cocos2d::CCRect m_scrollLimit = {0, 0, 300, 500};

public:
    VirtualScrollLayer(){
        auto scrollLayer = ScrollLayer::create(m_scrollLimit, m_scrollWheelEnabled, m_vertical);
        m_tether = scrollLayer;

        auto contentLayer = new VirtualContentLayer();
        scrollLayer->addChild(contentLayer->m_CCContentLayerObject);
        scrollLayer->m_contentLayer = contentLayer->m_CCContentLayerObject;

        setContentSize({200, 100});
        setAnchorPoint({0.5, 0.5});
    }

    void settings() override {
        VirtualNode::settings();

        devtools::property("ScrollLimit", m_scrollLimit);

        devtools::property("Is Vertical Scrolling", m_vertical);
        devtools::property("Scroll Wheel Enabled", m_scrollWheelEnabled);
    }

    matjson::Value exportJSON() override {
        auto obj = VirtualNode::exportJSON();
        obj["type"] = "ScrollLayer";

        obj["vertical"] = m_vertical;
        obj["scrollWheelEnabled"] = m_scrollWheelEnabled;

        matjson::Value limit = matjson::makeObject({
            { "x", matjson::Value(m_scrollLimit.origin.x) },
            { "y", matjson::Value(m_scrollLimit.origin.y) },
            { "width", matjson::Value(m_scrollLimit.size.width) },
            { "height", matjson::Value(m_scrollLimit.size.height) },
        });

        obj["scrollLimit"] = limit;

        return obj;
    }

    std::string emitCode(int indent = 0) override { 
        std::string indentStr(indent, ' ');
        return fmt::format(
            "{}auto scrollLayer = ScrollLayer::create({{{}, {}, {}, {}}}, {}, {});\n",
            indentStr,
            m_scrollLimit.origin.x,
            m_scrollLimit.origin.y,
            m_scrollLimit.size.width,
            m_scrollLimit.size.height,
            m_scrollWheelEnabled,
            m_vertical
        );
    }

    void importJSON(matjson::Value json) override {
        VirtualNode::importJSON(json);

        m_vertical = json["vertical"].as<bool>().unwrap();
        m_scrollWheelEnabled = json["scrollWheelEnabled"].as<bool>().unwrap();

        auto limit = json["scrollLimit"];

        m_scrollLimit = cocos2d::CCRect(
            limit["x"].as<double>().unwrap(),
            limit["y"].as<double>().unwrap(),
            limit["width"].as<double>().unwrap(),
            limit["height"].as<double>().unwrap()
        );
    }


    void updateTether() override { 
        auto tether = static_cast<geode::ScrollLayer*>(m_tether.data());
        if (! tether) return; 

        VirtualNode::updateTether();
    }
};
