#include "PauseLayer.h"
#include "SimpleAudioEngine.h"
#include <limits>
#include "resource.h"
#include "StartLayer.h"

using namespace CocosDenshion;


bool PauseLayer::init()
{
    // ccc4的最后一个参数才是透明度
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 100))) {
        return  false;
	}

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);




	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemImage *play = CCMenuItemImage::create("play.png", "play.png", this, menu_selector(PauseLayer::doResume));
	play->setScale(3);
	play->setPosition(ccp(winSize.width/2, winSize.height/2));

	//
	CCLabelBMFont *backLb = CCLabelBMFont::create("Main Layer", s_font);
	CCMenuItemLabel *goBack = CCMenuItemLabel::create(backLb, this, menu_selector(PauseLayer::gotoSartLayer));
	goBack->setScale(0.6f);
	goBack->setPosition(ccp(winSize.width/2, goBack->getContentSize().height));

	mMenu = CCMenu::create(play, goBack, NULL);
	mMenu->setAnchorPoint(ccp(0, 0));
	addChild(mMenu, 1, 10);
	mMenu->setPosition(CCPointZero);

	mMenu->runAction(CCSequence::create(
		CCDelayTime::create(0.01f), 
		CCCallFunc::create(this, callfunc_selector(PauseLayer::schedule_menuPriority)),
		NULL));
    
    return true;
}


void PauseLayer::doResume(CCObject *pSender)
{
	CCLog("resume!");
	CCDirector::sharedDirector()->resume();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();


	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	removeFromParent();
}

void PauseLayer::schedule_menuPriority()
{
	mMenu->setHandlerPriority(-150);
}

void PauseLayer::gotoSartLayer(CCObject *pSender)
{
	CCDirector::sharedDirector()->resume();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);


	CCScene *scene = StartLayer::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.25f, scene));
}