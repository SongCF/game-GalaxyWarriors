#include "About.h"
#include "Resource.h"
#include "StartLayer.h"
#include <string>
using std::string;

CCScene* About::scene()
{
    CCScene *scene = CCScene::create();
    About *about = About::create();
    scene->addChild(about);
    return scene;
}

bool About::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *sp = CCSprite::create(s_loading);
    sp->setAnchorPoint(ccp(0, 0));
    addChild(sp, 0, 1);
    
    CCTexture2D *imageCache = CCTextureCache::sharedTextureCache()->addImage(s_menuTitle);
    CCSprite *title = CCSprite::createWithTexture(imageCache, CCRectMake(0, 36, 100, 34));
    title->setPosition(ccp(winSize.width/2, winSize.height - 60));
    addChild(title);
    
	string fileName = "Font/string.txt";
	unsigned long len;
	fileName = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());
	unsigned char *data = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "r", &len);
	string temp; //bug： data 结尾后还跟了些乱码？？
	for (int i=0; i<len; ++i)
		temp.push_back(*(data+i));

	if (len > 0)
	{
		CCLabelTTF *about = CCLabelTTF::create(temp.c_str(), "Arial", 18, CCSizeMake(winSize.width * 0.9f, 0), kCCTextAlignmentLeft);
		about->setPosition(ccp(winSize.width / 2, 360));
		about->setAnchorPoint(ccp(0.5, 1));
		addChild(about);
	}
    
    
    CCLabelBMFont *backLb = CCLabelBMFont::create("Go Back", s_font);
    CCMenuItemLabel *goBack = CCMenuItemLabel::create(backLb, this, menu_selector(About::goBack));
    goBack->setScale(0.6);
    CCActionInterval *fadeIn = CCFadeTo::create(1, 80);
    CCActionInterval *fadeOut = CCFadeTo::create(1, 255);
    CCEaseSineInOut *ease1 = CCEaseSineInOut::create(fadeIn);
    CCEaseSineInOut *ease2 = CCEaseSineInOut::create(fadeOut);
    CCActionInterval *seq = CCSequence::create(ease1, ease2, NULL);

    goBack->runAction(CCRepeatForever::create(seq));
    
    CCMenu *menu = CCMenu::create(goBack, NULL);
    menu->setPosition(winSize.width / 2, 50);
    addChild(menu);
    
    return  true;
}

void About::goBack(CCObject* pSender)
{
    CCScene *scene = StartLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));
}
