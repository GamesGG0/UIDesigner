#include <VirtualNode.hpp>
#include <UIBuilder.hpp>

struct VirtualRGBA : public VirtualNode, CCRGBAProtocol {
    ccColor3B m_color = ccc3(255, 255, 255);
    GLubyte m_opacity = 255;
    bool m_cascadeColorEnabled = false;
    bool m_cascadeOpacityEnabled = false;
    bool m_opacityModifyRGB = false;

    using VirtualNode::VirtualNode;

    auto tether() { return typeinfo_cast<CCRGBAProtocol*>(m_tether.data()); }

    void setColor(const ccColor3B& color) override { m_color = color; }
    const ccColor3B& getColor(void) override { return m_color; }
    const ccColor3B& getDisplayedColor(void) override { return m_color; }
    GLubyte getDisplayedOpacity(void) override { return m_opacity; }
    void setOpacity(GLubyte opacity) override { m_opacity = opacity; }
    GLubyte getOpacity(void) override { return m_opacity; }
    void setOpacityModifyRGB(bool bValue) override { m_opacityModifyRGB = bValue; }
    bool isOpacityModifyRGB(void) override { return m_opacityModifyRGB; }
    void updateDisplayedColor(const ccColor3B& color) override { m_color = color; }
    void setCascadeColorEnabled(bool cascadeColorEnabled) override { m_cascadeColorEnabled = cascadeColorEnabled; }
    bool isCascadeColorEnabled(void) override { return m_cascadeColorEnabled; }
    void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) override { m_cascadeOpacityEnabled = cascadeOpacityEnabled; }
    bool isCascadeOpacityEnabled(void) override { return m_cascadeOpacityEnabled; }
    void updateDisplayedOpacity(GLubyte opacity) override { m_opacity = opacity; }

    void updateTether() override {
        VirtualNode::updateTether();

        tether()->setColor(m_color);
        tether()->setOpacity(m_opacity);
        tether()->setCascadeColorEnabled(m_cascadeColorEnabled);
        tether()->setCascadeOpacityEnabled(m_cascadeOpacityEnabled);
        tether()->setOpacityModifyRGB(m_opacityModifyRGB);
    }

    matjson::Value exportJSON() override {
        auto obj = VirtualNode::exportJSON();

        if (m_color != ccc3(255, 255, 255))
            obj["color"] = std::vector { m_color.r, m_color.g, m_color.b };
        if (m_opacity != 255)
            obj["opacity"] = m_opacity;
        if (m_cascadeColorEnabled)
            obj["cascadeColorEnabled"] = m_cascadeColorEnabled;
        if (m_cascadeOpacityEnabled)
            obj["cascadeOpacityEnabled"] = m_cascadeOpacityEnabled;
        if (m_opacityModifyRGB)
            obj["opacityModifyRGB"] = m_opacityModifyRGB;

        return obj;
    
    }

    void importJSON(matjson::Value obj) override {
        VirtualNode::importJSON(obj);

        m_color = ccc3(
            obj["color"][0].asInt().unwrapOr(255),
            obj["color"][1].asInt().unwrapOr(255),
            obj["color"][2].asInt().unwrapOr(255)
        );

        m_opacity = obj["opacity"].asInt().unwrapOr(255);
        m_cascadeColorEnabled = obj["cascadeColorEnabled"].asBool().unwrapOr(false);
        m_cascadeOpacityEnabled = obj["cascadeOpacityEnabled"].asBool().unwrapOr(false);
        m_opacityModifyRGB = obj["opacityModifyRGB"].asBool().unwrapOr(false);
    }

    std::string emitAttributes(matjson::Value json, int indent = 0) {
        std::string out;
        std::string ind(indent, ' ');

        if (auto arr = json["color"]; arr.isArray()) {
            out += fmt::format("{}.color({}, {}, {})\n", ind,
                arr[0].asInt().unwrapOr(255),
                arr[1].asInt().unwrapOr(255),
                arr[2].asInt().unwrapOr(255)
            );
        }

        if (auto obj = json["opacity"].asInt(); obj && obj.unwrap() != 255)
            out += fmt::format("{}.opacity({})\n", ind, obj.unwrap());
        if (auto obj = json["cascadeColorEnabled"]; obj.isBool())
            out += fmt::format("{}.cascadeColor({})\n", ind, obj.asBool().unwrap() ? "true" : "false");
        if (auto obj = json["cascadeOpacityEnabled"]; obj.isBool())
            out += fmt::format("{}.cascadeOpacity({})\n", ind, obj.asBool().unwrap() ? "true" : "false");

        return out + VirtualNode::emitAttributes(json, indent);
    }
};
