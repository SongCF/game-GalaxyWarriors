#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "Bullet.h"
#include "Resource.h"
#include "Config.h"
#include "Enemy.h"
#include "Effect.h"
#include "GameOver.h"
#include "PauseLayer.h"

#define DoubleClickedTime 0.5f



#define FIX_POS(_pos, _min, _max) \
	if (_pos < _min)        \
	_pos = _min;        \
else if (_pos > _max)   \
	_pos = _max;        \


using namespace cocos2d;
using namespace CocosDenshion;


bool isPaused = false;

GameLayer::GameLayer():m_state(statePlaying),m_time(0),m_ship(NULL),m_backSky(NULL),m_backSkyHeight(0),m_backSkyRe(NULL),m_backTileMap(NULL),m_backTileMapHeight(0),m_backTileMapRe(NULL),m_isBackSkyReload(false),m_isBackTileReload(false),m_lifeCount(NULL),
    m_tempScore(0)
{
    mClickedTime.tv_sec = 0;
	mClickedTime.tv_usec = 0;
	mHasFirstClicked = false;
}
GameLayer::~GameLayer()
{
    if (m_levelManager) {
        delete m_levelManager;
    }
    
    play_bullet->release();
    enemy_bullet->release();
    enemy_items->release();
}

bool GameLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    // 开启触摸
    this->setTouchEnabled(true);
    
    // 创建数组，需要retain一下
    play_bullet = CCArray::create();
    play_bullet->retain();
    
    enemy_bullet = CCArray::create();
    enemy_bullet->retain();
    
    enemy_items = CCArray::create();
    enemy_items->retain();
    
    m_state = statePlaying;//statePlaying=0
    
    Enemy::sharedEnemy();
    Effect::sharedExplosion();
    
    Config::sharedConfig()->resetConfig();
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    m_levelManager = new LevelManager(this);
    
    //初始化背景
    initBackground();

    m_screenRec = CCRectMake(0, 0,  winSize.width, winSize.height + 10);
    
    // score
	CCLabelBMFont *scoreLabel = CCLabelBMFont::create("SCORE: ", s_arial14_fnt);
	scoreLabel->setAnchorPoint(ccp(0, 0));
	addChild(scoreLabel, 1000);
	scoreLabel->setColor(ccc3(255, 240, 188));
	scoreLabel->setPosition(100, winSize.height - 30);
	{
		//  1 1 000
		m_lbScore1 = CCLabelBMFont::create("", s_arial14_fnt);
		m_lbScore1->setAnchorPoint(ccp(0, 0));
		m_lbScore1->setColor(ccc3(200,0,200));
		addChild(m_lbScore1, 1000);
		m_lbScore1->setPosition(scoreLabel->getPositionX()+scoreLabel->getContentSize().width, scoreLabel->getPositionY());
		//
		m_lbScore2 = CCLabelBMFont::create("", s_arial14_fnt);
		m_lbScore2->setAnchorPoint(ccp(0, 0));
		m_lbScore2->setColor(ccYELLOW);
		addChild(m_lbScore2, 1000);
		m_lbScore2->setPosition(m_lbScore1->getPositionX()+m_lbScore1->getContentSize().width, m_lbScore1->getPositionY());
		//
		m_lbScore3 = CCLabelBMFont::create("", s_arial14_fnt);
		m_lbScore3->setAnchorPoint(ccp(0, 0));
		addChild(m_lbScore3, 1000);
		m_lbScore3->setPosition(m_lbScore2->getPositionX()+m_lbScore2->getContentSize().width, m_lbScore2->getPositionY());
	}
    
    // ship life
    CCTexture2D *shipTexture = CCTextureCache::sharedTextureCache()->addImage(s_ship01);
    CCSprite *life = CCSprite::createWithTexture(shipTexture, CCRectMake(0, 0, 60, 38));
    life->setScale(0.6);
    life->setPosition(ccp(30,winSize.height-23));
    addChild(life, 1, 5);
    
    // ship life count
    char lifecount[2];
    sprintf(lifecount, "%d",Config::sharedConfig()->getLifeCount()-1);
    m_lifeCount = CCLabelTTF::create(lifecount, "Arial", 20);
    m_lifeCount->setPosition(ccp(60, winSize.height-20));
    m_lifeCount->setColor(ccc3(255,0, 0));
    addChild(m_lifeCount, 1000);
    
    // ship
    m_ship = Ship::create();
    addChild(m_ship, m_ship->getZoder(), 1001);
    
	//pause
    CCMenuItemImage *pause = CCMenuItemImage::create("pause.png", "pause.png", this, menu_selector(GameLayer::doPause));
    pause->setAnchorPoint(ccp(1, 1));
	pause->setScale(0.8);
	pause->setPosition(ccp(winSize.width, winSize.height));
    CCMenu *menu = CCMenu::create(pause, NULL);
    menu->setAnchorPoint(ccp(0, 0));
    addChild(menu, 1, 10);
    menu->setPosition(CCPointZero);
    
    // 调 update函数
    scheduleUpdate();
    
    // 每秒调一次 scoreCounter函数
    schedule(schedule_selector(GameLayer::scoreCounter), 1);
    
    if (Config::sharedConfig()->getAudioState()) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(s_bgMusic, true);
    }
    
    return true;
}



void GameLayer::update(float dt)
{
    if (m_state == statePlaying) {
        checkIsCollide();
        removeInactiveUnit(dt);
        checkIsReborn();
        updateUI();
    }
    
}

void GameLayer::scoreCounter(float dt)
{
    if (m_state == statePlaying) {
        m_time++;
        m_levelManager->loadLevelResource(m_time);
    }
}

void GameLayer::checkIsCollide()
{
    CCObject *units;
    CCObject *bullets;
	CCObject *enemybs;
	CCArray* deleteEnemys = CCArray::create();
	CCArray* deleteEnemyBullets = CCArray::create();
	CCArray* deletePlayerBullets = CCArray::create();
    CCARRAY_FOREACH(enemy_items, units)
    {
        UnitSprite *enemy = (UnitSprite*)(units);
        CCARRAY_FOREACH(play_bullet, bullets)
        {
            UnitSprite *bullet = (UnitSprite*)(bullets);
            if (this->collide(enemy, bullet)) {
                enemy->hurt();
                bullet->hurt();
            }
            if (!(m_screenRec.intersectsRect(bullet->boundingBox()))) {
//				bullet->destroy();
				deletePlayerBullets->addObject(bullet);
            }
        }
        if (collide(enemy, m_ship)) {
            if (m_ship->isActive()) {
                enemy->hurt();
                m_ship->hurt();
            }
            
        }
        if (!(m_screenRec.intersectsRect(enemy->boundingBox()))) {
//			enemy->destroy();
			deleteEnemys->addObject(enemy);
        }
    }
    
    CCARRAY_FOREACH(enemy_bullet, enemybs)
    {
        UnitSprite *enemyb = (UnitSprite*)(enemybs);
        if (enemyb) {
            if (collide(enemyb, m_ship)) {
                if (m_ship->isActive()) {
                    enemyb->hurt();
                    m_ship->hurt();
                }
            }
            if (!m_screenRec.intersectsRect(enemyb->boundingBox())) {
//				enemyb->destroy();
				deleteEnemyBullets->addObject(enemybs);
            }
        }
        
	}

	//////////////////////////////////////////////////////////////////////////
	//统一删除, 避免在遍历数组的时候动态删除
	int x = 0;
	CCARRAY_FOREACH(deletePlayerBullets, bullets)
	{
		play_bullet->removeObject(bullets);
		((Bullet*)bullets)->removeFromParent();
	}
	deletePlayerBullets->removeAllObjects();

	CCARRAY_FOREACH(deleteEnemyBullets, enemybs)
	{
		enemy_bullet->removeObject(enemybs);
		((Bullet*)enemybs)->removeFromParent();
	}
	deleteEnemyBullets->removeAllObjects();

	CCARRAY_FOREACH(deleteEnemys, units)
	{
		enemy_items->removeObject(units);
		((Enemy*)units)->removeFromParent();
	}
	deleteEnemys->removeAllObjects();

// 	//道具
// 	if(m_tools != NULL)
// 	{
// 		if (collide(m_tools, m_ship)) 
// 		{
// 			if (m_ship->isActive()) 
// 			{
// 				m_tools->destroy();
// 				m_ship->setPowerValue(2);
// 				m_tools = NULL;
// 			}
// 		}		
// 	}
}

void GameLayer::removeInactiveUnit(float dt)
{
        
    CCArray *children = this->getChildren();
    for (unsigned int i = 0; i < children->count(); ++i) 
	{
        CCSprite *selChild =  dynamic_cast<CCSprite*>(children->objectAtIndex(i));
        if (selChild) 
		{
            selChild->update(dt);
            int tag = selChild->getTag();

            if (( tag == 900) || (tag == 901 )|| (tag == 1000)) 
			{
                if (!((UnitSprite*)selChild)->isActive())
				{
                    ((UnitSprite*)selChild)->destroy();
                }
            }
        }
    }
    
    if (m_ship) {
        if (!m_ship->isActive()) {
            m_ship->destroy();
            m_ship = NULL;
        }
    }
    
}

void GameLayer::checkIsReborn()
{    
    if (Config::sharedConfig()->getLifeCount() > 0) {
        if (!m_ship) {
            
                m_ship = Ship::create();
                this->addChild(m_ship, m_ship->getZoder(), 1001);
        }
        
    }
    else if (Config::sharedConfig()->getLifeCount() <= 0 ) {
        m_state = stateGameOver;
        m_ship = NULL;
        CCCallFunc *gameOver = CCCallFunc::create(this, callfunc_selector(GameLayer::gameOver));
        this->runAction(CCSequence::create(CCDelayTime::create(0.2), gameOver, NULL));
    }
    
}

bool GameLayer::collide(UnitSprite *a, UnitSprite *b)
{
    if(!a || !b)
    {
        return false;
    }
    CCRect aRect = a->collideRect();
    CCRect bRect = b->collideRect();
    if (aRect.intersectsRect(bRect)) {
        return true;
    }
    return false;
}

void GameLayer::updateUI()
{
    if (m_tempScore < Config::sharedConfig()->getScoreValue()) {
        m_tempScore += 5;
    }

    char buf[20];
	if (m_tempScore > 9999)
	{
		sprintf(buf, "%01d", m_tempScore / 10000);
		m_lbScore1->setString(buf);
	}
	if (m_tempScore > 999)
	{
		sprintf(buf, "%01d", m_tempScore / 1000 % 10);
		m_lbScore2->setString(buf);
		m_lbScore2->setPositionX(m_lbScore1->getPositionX()+m_lbScore1->getContentSize().width);
	}
	if (m_tempScore > 0)
	{
		sprintf(buf, "%03d", m_tempScore % 1000);
		m_lbScore3->setString(buf);
		m_lbScore3->setPositionX(m_lbScore2->getPositionX()+m_lbScore2->getContentSize().width);
	}


    char lifecount[3];
    sprintf(lifecount, "%d",Config::sharedConfig()->getLifeCount()-1);
    m_lifeCount->setString(lifecount);   
}

void GameLayer::onEnter()
{
	CCLayer::onEnter();
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	if (Config::sharedConfig()->getAcclerateState())
		pDirector->getAccelerometer()->setDelegate(this); // == this->setIsAccelerometerEnabled(true)
}

void GameLayer::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	pDirector->getAccelerometer()->setDelegate(NULL);// == this->setIsAccelerometerEnabled(false)
    CCLayer::onExit();
}


bool GameLayer::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	cc_timeval curTime;
	CCTime::gettimeofdayCocos2d(&curTime, 0);

	if (! mHasFirstClicked)
	{
		mClickedTime = curTime;
	//	mHasFirstClicked = true; // when end
	}
	else
	{
		double deltaTime = CCTime::timersubCocos2d(&mClickedTime, &curTime);
		if (deltaTime > DoubleClickedTime * 1000)  //两次点击各得太久
		{
			mClickedTime = curTime;
			mHasFirstClicked = false;

			CCLog("%.3f", deltaTime / 1000);
		}
	}

    return true;
}

void GameLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if ((m_state == statePlaying) && m_ship) {
    CCPoint pos = touch->getDelta();
    CCPoint currentPos = m_ship->getPosition();
    currentPos = ccpAdd(currentPos, pos);
    currentPos = ccpClamp(currentPos, CCPointZero, ccp(winSize.width, winSize.height));
    m_ship->setPosition(currentPos);
    }
    
}

void GameLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
	cc_timeval curTime;
	CCTime::gettimeofdayCocos2d(&curTime, 0);

	if (mHasFirstClicked)
	{
		double deltaTime = CCTime::timersubCocos2d(&mClickedTime, &curTime);
		if (deltaTime < DoubleClickedTime * 1000)
		{
			m_ship->Skill_Destroy();
		}

		mClickedTime.tv_sec = 0;
		mClickedTime.tv_usec = 0;
		mHasFirstClicked = false;
	}
	else
	{
		mHasFirstClicked = true;
	}
}


void GameLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
	mClickedTime.tv_sec = 0;
	mClickedTime.tv_usec = 0;
	mHasFirstClicked = false;
}

// 无限滚动地图，采用两张图循环加载滚动
void GameLayer::initBackground()
{
    m_backSky = CCSprite::create(s_bg01);
    m_backSky->setAnchorPoint(ccp(0, 0));
    m_backSkyHeight = m_backSky->getContentSize().height;
    addChild(m_backSky, -10);
    
    // Tile map
    m_backTileMap = CCTMXTiledMap::create(s_level01);
    addChild(m_backTileMap, -9);
    m_backTileMapHeight = m_backTileMap->getMapSize().height * m_backTileMap->getTileSize().height;
    
    
    
    m_backSkyHeight -= 48;
    m_backTileMapHeight -= 200;
    m_backSky->runAction(CCMoveBy::create(3, ccp(0, -48)));
    m_backTileMap->runAction(CCMoveBy::create(3, ccp(0, -200)));
    
    schedule(schedule_selector(GameLayer:: movingBackground),3);
    
}

// 这里就是视差背景了
void GameLayer::movingBackground(float dt)
{
    m_backSky->runAction(CCMoveBy::create(3, ccp(0, -48)));
    m_backTileMap->runAction(CCMoveBy::create(3, ccp(0, -200)));
    
    // 每次移动48
    m_backSkyHeight -= 48;
    
    // 每次移动200
    m_backTileMapHeight -= 200;
    

    // 图的顶部到达屏幕顶部时
    if (m_backSkyHeight <= winSize.height) {
        if (!m_isBackSkyReload) {
            
            // 如果另一张图还没加载则create一个
            m_backSkyRe = CCSprite::create(s_bg01);
            m_backSkyRe->setAnchorPoint(ccp(0, 0));
            addChild(m_backSkyRe, -10);
            m_backSkyRe->setPosition(ccp(0, winSize.height));
            
            // 反转标志位
            m_isBackSkyReload = true;
        }
        // 第二张图紧接着第一张图滚动
        m_backSkyRe->runAction(CCMoveBy::create(3, ccp(0, -48)));
    }
    
    // 第一张图完全经过屏幕
    if (m_backSkyHeight <= 0) {
        m_backSkyHeight = m_backSky->getContentSize().height;
        // 移除第一张的精灵
        this->removeChild(m_backSky, true);
        
        // 指向第二张图的精灵
        m_backSky = m_backSkyRe;
        
        // 第二张的精灵指针置空
        m_backSkyRe = NULL;
        
        // 反转标志位
        m_isBackSkyReload = false;
    }
    
    if (m_backTileMapHeight <= winSize.height) {
        if (!m_isBackTileReload) {
            m_backTileMapRe = CCTMXTiledMap::create(s_level01);
            this->addChild(m_backTileMapRe, -9);
            m_backTileMapRe->setPosition(0, winSize.height);
            m_isBackTileReload = true;
        }
        m_backTileMapRe->runAction(CCMoveBy::create(3, ccp(0, -200)));
    }
    
    if (m_backTileMapHeight <= 0) {
        m_backTileMapHeight = m_backTileMap->getMapSize().height * m_backTileMap->getTileSize().height;
        this->removeChild(m_backTileMap, true);
        m_backTileMap = m_backTileMapRe;
        m_backTileMapRe = NULL;
        m_isBackTileReload = false;
    }
}

void GameLayer::gameOver()
{
    CCScene * scene = GameOver::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));
}

void GameLayer::doPause(CCObject *pSender)
{
    CCDirector::sharedDirector()->pause();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();

    PauseLayer *pauseLayer = PauseLayer::create();
    addChild(pauseLayer,9999);
}

Ship* GameLayer::getShip()
{
    return m_ship;
}

void GameLayer::didAccelerate(CCAcceleration* pAccelerationValue)
{
	if ( m_ship == NULL ) {
		return;
	}

	CCDirector* pDir = CCDirector::sharedDirector();
	CCPoint visOrigin = pDir->getVisibleOrigin();
	CCSize visSize = pDir->getVisibleSize();

	CCSize shipSize  = m_ship->getContentSize();

	CCPoint ptNow  = m_ship->getPosition();
	CCPoint ptTemp = pDir->convertToUI(ptNow);

	ptTemp.x += pAccelerationValue->x * 10.0f;
	ptTemp.y -= pAccelerationValue->y * 10.0f;

	CCPoint ptNext = pDir->convertToGL(ptTemp);
	FIX_POS(ptNext.x, (visOrigin.x+shipSize.width / 2.0), (visOrigin.x+visSize.width - shipSize.width / 2.0));
	FIX_POS(ptNext.y, (visOrigin.y+shipSize.height / 2.0), (visOrigin.y+visSize.height - shipSize.height / 2.0));
	m_ship->setPosition(ptNext);
}
