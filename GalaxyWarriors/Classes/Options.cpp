#include "Options.h"
#include "Resource.h"
#include "Config.h"
#include "StartLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

CCScene* Options::scene()
{
    CCScene *scene = CCScene::create();
    Options *optionsLayer = Options::create();
    scene->addChild(optionsLayer);
    return scene;
}

bool Options::init()
{
    if (!CCLayer::init()) 
	{
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *sp = CCSprite::create(s_loading);
    sp->setAnchorPoint(CCPointZero);
    addChild(sp, 0, 1);
    
    CCTexture2D *imageCache = CCTextureCache::sharedTextureCache()->addImage(s_menuTitle);
    CCSprite *title = CCSprite::createWithTexture(imageCache, CCRectMake(0, 0, 134, 34));
    title->setPosition(ccp(winSize.width/2, winSize.height - 60));
    addChild(title);
	CCMenu *menu = CCMenu::create();
	addChild(menu,1);
	menu->setPosition(ccp(0,0));
	CCMenuItemFont::setFontSize(24);
	CCMenuItemFont::setFontName("Arial");

// 	//54 0  234
// 	//305   270
// 	CCMenuItemFont *skillTile = CCMenuItemFont::create("Skill");
// 	menu->addChild(skillTile);
// 	skillTile->setAnchorPoint(ccp(0, 0.5));
// 	skillTile->setEnabled(false);
// 	skillTile->setPosition(ccp(54, 305));
// 	CCMenuItemToggle *skilltoggle = CCMenuItemToggle::createWithTarget(this, menu_selector(Options::setSkill), 
// 		CCMenuItemFont::create("DoubleClick"),CCMenuItemFont::create("None"), NULL); 
// 	menu->addChild(skilltoggle);
// 	int skillsel = Config::sharedConfig()->getCanUseSkill()? 0 : 1;
// 	skilltoggle->setSelectedIndex(skillsel);
// 	skilltoggle->setPosition(ccp(234, 305));

	//
	CCMenuItemFont *skillTile = CCMenuItemFont::create("Skill");
	menu->addChild(skillTile);
	skillTile->setEnabled(false);
	skillTile->setAnchorPoint(ccp(0, 0.5));
	skillTile->setPosition(ccp(54, 305));
	CCMenuItemToggle *skilltoggle = CCMenuItemToggle::createWithTarget(this, menu_selector(Options::setSkill), 
		CCMenuItemFont::create("DoubleClick"),CCMenuItemFont::create("None"), NULL);    
	menu->addChild(skilltoggle);
	int skillsel = Config::sharedConfig()->getCanUseSkill()? 0 : 1;
	skilltoggle->setSelectedIndex(skillsel);
	skilltoggle->setPosition(ccp(234, 305));


	//
	CCMenuItemFont *soundTitle = CCMenuItemFont::create("Sound");
	menu->addChild(soundTitle);
	soundTitle->setEnabled(false);
	soundTitle->setAnchorPoint(ccp(0, 0.5));
	soundTitle->setPosition(ccp(54, 305-35));
    CCMenuItemToggle *soundToggle = CCMenuItemToggle::createWithTarget(this, menu_selector(Options::setOptions), 
		CCMenuItemFont::create("On"),CCMenuItemFont::create("Off"), NULL);    
	menu->addChild(soundToggle);
    int soundsel = Config::sharedConfig()->getAudioState()? 0 : 1;
	soundToggle->setSelectedIndex(soundsel);
	soundToggle->setPosition(ccp(234, 305-35));

	//
	CCMenuItemFont *accelTilte = CCMenuItemFont::create("Accelerate");
	menu->addChild(accelTilte);
	accelTilte->setEnabled(false);
	accelTilte->setAnchorPoint(ccp(0, 0.5));
	accelTilte->setPosition(ccp(54, 305-70));
	CCMenuItemToggle *accelToggle = CCMenuItemToggle::createWithTarget(this, menu_selector(Options::setAccelerate), 
		CCMenuItemFont::create("On"),CCMenuItemFont::create("Off"), NULL);    
	menu->addChild(accelToggle);
	int acclsel = Config::sharedConfig()->getAcclerateState()? 0 : 1;
	accelToggle->setSelectedIndex(acclsel);
	accelToggle->setPosition(ccp(234, 305-70));
   
	//142
    CCLabelBMFont *backLb = CCLabelBMFont::create("Go Back", s_font);
	CCMenuItemLabel *goBack = CCMenuItemLabel::create(backLb, this, menu_selector(Options::goBack));
	if (goBack) menu->addChild(goBack);
    goBack->setScale(0.6f);
	goBack->setPosition(ccp(winSize.width/2, 142));
    CCActionInterval *fadeIn = CCFadeTo::create(1, 80);
    CCActionInterval *fadeOut = CCFadeTo::create(1, 255);
    CCEaseSineInOut *ease1 = CCEaseSineInOut::create(fadeIn);
    CCEaseSineInOut *ease2 = CCEaseSineInOut::create(fadeOut);
    CCActionInterval *seq = CCSequence::create(ease1, ease2, NULL);
    goBack->runAction(CCRepeatForever::create(seq));

    return true;
}

void Options::setOptions(CCObject* pSender)
{
    bool tmpSound = Config::sharedConfig()->getAudioState();
    Config::sharedConfig()->updateAudioState(!tmpSound);
    
    if (Config::sharedConfig()->getAudioState()) {
        SimpleAudioEngine::sharedEngine()->resumeAllEffects();
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }else{
        SimpleAudioEngine::sharedEngine()->pauseAllEffects();
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    }
    
}

void Options::goBack(cocos2d::CCObject *pSender)
{
    CCScene *scene = StartLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));

}

void Options::setSkill(CCObject *psender)
{
	if (Config::sharedConfig()->getCanUseSkill())
	{
		Config::sharedConfig()->setCanUseSkill(false);
	}
	else
	{
		Config::sharedConfig()->setCanUseSkill(true);
	}
}

void Options::setAccelerate(CCObject *pSender)
{
	if (Config::sharedConfig()->getAcclerateState())
	{
		Config::sharedConfig()->setAcclerateState(false);
	}
	else
	{
		Config::sharedConfig()->setAcclerateState(true);
	}
}