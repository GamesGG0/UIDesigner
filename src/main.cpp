#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>
#include "Geode/cocos/CCDirector.h"
#include "VirtualDOM.hpp"
using namespace geode::prelude;

struct ExitableLayer : public CCLayer {
	using CCLayer::CCLayer;
	static ExitableLayer* create() {
		auto ret = new ExitableLayer();
		ret->init();
		ret->autorelease();
		ret->setKeypadEnabled(true);
		return ret;
	}

	void keyBackClicked() override {
		CCDirector::sharedDirector()->popSceneWithTransition(0.5, cocos2d::kPopTransitionFade);
	}
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {
	bool init() {
		MenuLayer::init();

		auto btn = Build<CCSprite>::createSpriteName("btn_chatHistory_001.png")
			.intoMenuItem(this, &Self::designer);

		auto parent = getChildByID("right-side-menu");
		if (parent) {
			parent->addChild(btn);
			parent->updateLayout();
		}

		return true;
	}

	void designer(CCObject* a) {
		Build<ExitableLayer>::create()
			.contentSize(getContentSize())
			.anchorPoint({0, 0})
			.layout(AnchorLayout::create())
			.intoNewChild(CCSprite::create("GJ_gradientBG.png"))
				.scaleToMatch(getContentSize())
				.color(100, 86, 255)
				.setAnchorOpts(Anchor::Center)
				.intoParent<CCLayer>()
			.with([](auto lay) { VirtualDOMManager::get()->initialize(lay); })
			.updateLayout()
			.intoScene()
			.fadeIn(0.5)
			.pushScene();
	}
};
