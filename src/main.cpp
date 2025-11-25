#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include "Geode/cocos/CCDirector.h"
#include "VirtualDOM.hpp"
#include "VirtualNode.hpp"

using namespace geode::prelude;

struct DesignerLayer : public CCLayer {
	using CCLayer::CCLayer;
	VirtualNode* root;

	static DesignerLayer* create() {
		auto ret = new DesignerLayer();
		ret->init();
		ret->autorelease();
		ret->setKeypadEnabled(true);

		ret->setContentSize(CCDirector::sharedDirector()->getWinSize());
		ret->setAnchorPoint({0, 0});
		ret->setLayout(AnchorLayout::create());
		ret->schedule(schedule_selector(DesignerLayer::saveTemp), 5.f);

		Build<CCSprite>::create("GJ_gradientBG.png")
			.scaleToMatch(ret->getContentSize())
			.color(100, 86, 255)
			.setAnchorOpts(Anchor::Center)
			.parent(ret);

		ret->root = VirtualDOMManager::get()->initialize(ret);

		if (auto data = file::readJson(Mod::get()->getSaveDir() / "temp.json"))
			ret->root->importJSON(data.unwrap());
		else {
			log::info("yeahhhhh");
			ret->root->CCNode::addChild(VirtualDOMManager::get()->createFromJSON(matjson::parse(R"(
			{
				"type": "Label",
				"text": "Welcome to UI Designer!\nEnter DevTools and locate the VirtualRoot to get started.\nPress Esc to exit.",
				"alignment": 1,
				"anchor": [0.5, 0.5],
				"scale": 0.45
			}
			)").unwrap()));
		}

		return ret;
	}

	void keyBackClicked() override {
		CCDirector::sharedDirector()->popSceneWithTransition(0.5, cocos2d::kPopTransitionFade);
		saveTemp(0.f);
	}

	void saveTemp(float) {
		if (!file::writeToJson(Mod::get()->getSaveDir() / "temp.json", root->exportJSON()))
			log::warn("Failed to save temporary JSON!");
	}
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
	bool init() {
		MenuLayer::init();

		auto btn = Build<CCSprite>::create("button.png"_spr)
			.id("uidesigner-btn"_spr)
			.intoMenuItem([] {
				Build<DesignerLayer>::create()
					.updateLayout()
					.intoScene()
					.fadeIn(0.5)
					.pushScene();
			});

		auto parent = getChildByID("right-side-menu");
		if (parent) {
			parent->addChild(btn);
			parent->updateLayout();
		}

		return true;
	}
};
