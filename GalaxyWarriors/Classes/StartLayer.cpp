#include "StartLayer.h"
#include "Resource.h"
#include "SimpleAudioEngine.h"
#include "Effect.h"
#include "GameLayer.h"
#include "About.h"
#include "Options.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* StartLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StartLayer *layer = StartLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartLayer::init()
{
    //////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // 添加Android的按键处理，这里加了全局代理
    this->setKeypadEnabled(true);
    CostomeKeyPadDelegae *keyDelegate = new CostomeKeyPadDelegae();
    keyDelegate->autorelease();
    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(keyDelegate);
    
    //////////////////
    // 2. add main menu ,logo, loading, and ships
    winSize = CCDirector::sharedDirector()->getWinSize();
    //添加背景 0 ；1；
    CCSprite* sl = CCSprite::create(s_loading);
    sl->setAnchorPoint(ccp(0, 0));
    this->addChild(sl,0,1);
    //添加logo 10 ；1；
    CCSprite* logo = CCSprite::create(s_logo);
    logo->setAnchorPoint(ccp(0.5, 1));
    logo->setPosition(ccp(winSize.width/2, winSize.height-50));
    this->addChild(logo, 10, 1);
    //添加开始，设置，关于 三个按钮
    CCSprite* newGameNormal = CCSprite::create(s_menu, CCRectMake(0, 0, 126, 33));
    CCSprite* newGameSelected = CCSprite::create(s_menu, CCRectMake(0, 33, 126, 33));
    CCSprite* newGameDisabled = CCSprite::create(s_menu, CCRectMake(0, 33*2, 126, 33));
    
    CCSprite* gameSettingNormal = CCSprite::create(s_menu, CCRectMake(126, 0, 126, 33));
    CCSprite* gameSettingNSelected = CCSprite::create(s_menu, CCRectMake(126, 33, 126, 33));
    CCSprite* gameSettingDesabled = CCSprite::create(s_menu, CCRectMake(126, 33*2, 126, 33));
    
    CCSprite* aboutNormal = CCSprite::create(s_menu, CCRectMake(252, 0, 126, 33));
    CCSprite* aboutSelected = CCSprite::create(s_menu, CCRectMake(252, 33, 126, 33));
    CCSprite* aboutDesabled = CCSprite::create(s_menu, CCRectMake(252, 33*2, 126, 33));
    
    
    CCMenuItemSprite* newGame = CCMenuItemSprite::create(newGameNormal, newGameSelected, newGameDisabled,
		this, menu_selector(StartLayer::flareEffect));//StartMenu::flareEffect

    CCMenuItemSprite* gameSetting = CCMenuItemSprite::create(gameSettingNormal, gameSettingNSelected, gameSettingDesabled,
		this, menu_selector(StartLayer::menuCallback));//StartMenu::menuCallback
	//“设置”按钮tag为20
    gameSetting->setTag(20);

    CCMenuItemSprite* about = CCMenuItemSprite::create(aboutNormal, aboutSelected, aboutDesabled, 
		this, menu_selector(StartLayer::menuCallback));//StartMenu::menuCallback
	//“关于”按钮tag为21
    about->setTag(21);

    // 最后一个参数要是NULL
    CCMenu* menu = CCMenu::create(newGame, gameSetting, about, NULL);
    menu->alignItemsVerticallyWithPadding(20);
    this->addChild(menu, 1, 2);
    menu->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 80));

	//添加update回调函数
    this->schedule(schedule_selector(StartLayer::update), 0.1);
    
    // 添加ships
    CCTexture2D *textCache = CCTextureCache::sharedTextureCache()->addImage(s_ship01);
    m_ship = CCSprite::createWithTexture(textCache, CCRectMake(0, 45, 60, 38));
    this->addChild(m_ship, 0, 4);
	//设置飞船位置 移动
    CCPoint position = ccp(CCRANDOM_0_1() * winSize.width, 0);
    m_ship->setPosition(position);
    m_ship->runAction(CCMoveBy::create(2, ccp(CCRANDOM_0_1() * winSize.width, position.y + winSize.height + 100)));

    
    if (Config::sharedConfig()->getAudioState())
	{
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.7);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(s_mainMainMusic, true);

    }
    
    return true;
}
//选择按钮时的音乐
void StartLayer::onButtonEffect()
{
    if (Config::sharedConfig()->getAudioState()) {
         SimpleAudioEngine::sharedEngine()->playEffect(s_buttonEffect);
    }
   
}
//选择开始游戏之后的效果
void StartLayer::flareEffect(CCObject* pSender)
{
    onButtonEffect();
	int n=0;
	CCLog("begin>>>StartMenu::flareEffect",n);
    Effect* flareEffect = Effect::create();
    CCCallFunc *callback =  CCCallFunc::create(this, callfunc_selector(StartLayer::newGame));
	CCLog("begin>>>StartMenu::flareEffect>>1",n);
    flareEffect->flareEffect(this, callback);
	CCLog("begin>>>StartMenu::flareEffect>>2",n);

}

void StartLayer::newGame()
{
	int n=0;
	CCLog("begin>>>newGame",n);
    CCScene *scene = CCScene::create();
    scene->addChild(GameLayer::create());
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));
	CCLog("end>>>newGame",n);
}

void StartLayer::menuCallback(CCObject* pSender)
{
    onButtonEffect();
    int tag = dynamic_cast<CCNode*>(pSender)->getTag();
    if (tag == 20) 
	{
		//转到"Options"
        CCScene *scene = Options::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));

    }
    else if(tag == 21)
    {
		//转到"About"
        CCScene *scene = About::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));
    }
    
}
//飞船在初始界面移动函数
void StartLayer::update(float dt)
{
    if (m_ship->getPosition().y > winSize.height) 
	{
        CCPoint pos = ccp(CCRANDOM_0_1() * winSize.width, 10);
        m_ship->setPosition(pos);
        m_ship->runAction(CCMoveBy::create(floor(5 * CCRANDOM_0_1()), ccp(CCRANDOM_0_1() * winSize.width, pos.y + winSize.height)));
											
    }
}
