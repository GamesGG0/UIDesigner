#include "VirtualNode.hpp"
#include <geode.devtools/include/API.hpp>

class VirtualScale9Button : public VirtualNode, RegisterDOM<VirtualScale9Button, "Scale 9 Button"> {
	std::string m_spriteName = "GJ_button_01.png";
	bool m_frameDirty = false;

	CCScale9Sprite* m_sprite;
public:
	VirtualScale9Button() {
		setContentSize({ 150, 40 });
		setAnchorPoint({ 0.5, 0.5 });
		
		m_sprite = CCScale9Sprite::create(m_spriteName.c_str());
		m_sprite->setContentSize({ 150, 40 });
		m_sprite->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));

		m_tether = CCMenuItemExt::createSpriteExtra(m_sprite, +[](CCMenuItemSpriteExtra* mitem) {
			log::info("Button Pressed: ", mitem->getID());
		});

		setLayout(AnchorLayout::create());
	}

	VirtualScale9Button(VirtualScale9Button& src) : VirtualNode(src) {
		m_spriteName = src.m_spriteName;

		m_sprite = CCScale9Sprite::create(m_spriteName.c_str());
		m_sprite->setContentSize(getContentSize());
		m_sprite->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));

		replaceTether(CCMenuItemExt::createSpriteExtra(m_sprite, +[](CCMenuItemSpriteExtra* mitem) {
			log::info("Button Pressed: ", mitem->getID());
		}));
	}

	void settings() override {
		VirtualNode::settings();

		m_frameDirty = devtools::property("Sprite Name", m_spriteName);
	}

	matjson::Value exportJSON() override {
		auto obj = VirtualNode::exportJSON();

		obj["type"] = "Scale 9 Button";
		obj["spriteName"] = m_spriteName;

		return obj;
	}

	void importJSON(matjson::Value obj) override {
		VirtualNode::importJSON(obj);

		m_spriteName = obj["spriteName"].asString().unwrapOr(m_spriteName);
		m_frameDirty = true;
	}

	std::string emitCode(int indent = 0) override {
		std::string ind(indent, ' ');
		std::string out = fmt::format("{}Build<CCScale9Sprite>::create(\"{}\")\n", ind, m_spriteName);

		matjson::Value preJson = matjson::Value::object();
		auto json = exportJSON();

		for (auto& key : {"scale", "rotation", "anchor", "ignoreAnchor", "skew", "size"}) {
			if (json.contains(key)) {
				preJson[key] = json[key];
				json.erase(key);
			}
		}

		out += VirtualNode::emitAttributes(preJson, indent + 4);
		out += fmt::format("{}    .intoMenuItem([](auto) {{ log::info(\"Button Pressed\"); }})\n", ind);
		out += VirtualNode::emitAttributes(exportJSON(), indent + 4);
		return out;
	}

	void updateTether() override {
		if (m_frameDirty) {
		    auto spr = CCSprite::create(m_spriteName.c_str());
		    if (spr) {
		    	m_sprite = CCScale9Sprite::create(m_spriteName.c_str());
		    	m_sprite->setContentSize(getContentSize());
		    	m_sprite->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));

		    	replaceTether(CCMenuItemExt::createSpriteExtra(m_sprite, +[](CCMenuItemSpriteExtra* mitem) {
		    		log::info("Button Pressed: ", mitem->getID());
		    	}));
		    }
		}

		m_sprite->setContentSize(getContentSize());

		auto prevScale = m_tether->getScale();
		VirtualNode::updateTether();
		m_tether->setScale(prevScale);

		m_sprite->setScale(getScale());

		if (m_sprite->getZOrder() != -1)
			m_sprite->setZOrder(-1);
	}

	VirtualNode* duplicate() override {
		return new VirtualScale9Button(*this);
	}
};

VirtualNode* createLabel();

$execute {
	VirtualDOMManager::get()->registerType("Text Button", +[]() -> VirtualNode* {

		auto btn = new VirtualScale9Button();
		btn->setID("Text Button");
		auto label = createLabel();
		label->setScale(0.8);
		label->setID("Button Label");
		label->setLayoutOptions(AnchorLayoutOptions::create()->setAnchor(Anchor::Center));
		btn->addChild(label, 0, 0);

		return btn;
	});
};


