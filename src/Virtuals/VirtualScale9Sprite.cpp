#include "VirtualRGBA.hpp"
#include <geode.devtools/include/API.hpp>

class VirtualScale9Sprite : public VirtualRGBA, RegisterDOM<VirtualScale9Sprite, "Scale 9 Sprite"> {
    std::string m_spriteName = "GJ_square01.png";
    bool m_frameDirty;
public:
    VirtualScale9Sprite() : VirtualRGBA() {
        m_tether = CCScale9Sprite::create(m_spriteName.c_str());
        m_tether->setContentSize({100, 100});
        setContentSize({100, 100});
        setAnchorPoint({0.5, 0.5});
    }

    void settings() override {
        VirtualNode::settings();
        m_frameDirty = devtools::property("Sprite Name", m_spriteName);
        devtools::sameLine();
		devtools::button(reinterpret_cast<const char*>(u8"\ue930"), [&]{
            file::FilePickOptions options;
            options.filters.push_back({"Image Files", {"*.png", "*.jpg", "*.jpeg", "*.webp", "*.pvr", "*.tga", "*.bmp"}});
        
            file::pick(file::PickMode::OpenFile, options).listen(
                [this](Result<std::filesystem::path>* result) {
                    if (result->isOk()) {
                        auto path = result->unwrap();

                        m_spriteName = path.string().c_str();
                        replaceTether(createSprite(path.string().c_str()));
                    }
                }
            );
        });
    }

    matjson::Value exportJSON() override {
        auto obj = VirtualRGBA::exportJSON();

        obj["type"] = "Scale 9 Sprite";
        obj["spriteName"] = m_spriteName;

        return obj;
    }

    void importJSON(matjson::Value obj) override {
        m_spriteName = obj["spriteName"].asString().unwrapOr("GJ_square01.png");

        auto spr = CCSprite::create(m_spriteName.c_str());
        if (!spr || spr->getUserObject("geode.texture-loader/fallback")) 
            m_spriteName = "GJ_square01.png";

        
        m_tether = CCScale9Sprite::create(m_spriteName.c_str());
        VirtualRGBA::importJSON(obj);
    }

    std::string emitCode(int indent = 0) override {
        std::string ind(indent, ' ');
        std::string out = fmt::format("{}Build<CCScale9Sprite>::create(\"{}\")\n", ind, m_spriteName);

        out += VirtualRGBA::emitAttributes(exportJSON(), indent + 4);

        return out;
    }

    void updateTether() override {
        if (m_frameDirty) {
            auto spr = CCSprite::create(m_spriteName.c_str());
            if (spr && !spr->getUserObject("geode.texture-loader/fallback")) {
                replaceTether(CCScale9Sprite::create(m_spriteName.c_str()));
            }
        }

        VirtualRGBA::updateTether();
    }
};
