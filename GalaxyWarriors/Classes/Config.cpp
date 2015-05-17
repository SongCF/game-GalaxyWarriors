#include "Config.h"
#include "UnitSprite.h"

CCArray *play_bullet;
CCArray *enemy_bullet;
CCArray *enemy_items;

static  Config *g_config = NULL;

/**这里用的单例做的全局参数。JS版的直接就定义了一堆全局数组，真是方便*/
Config::Config():m_scoreValue(0),m_life(0),m_HasSkill(true),m_isAudioOn(true),m_canUseAccelerate(false)
{
    CCAssert(g_config == NULL, "Attempted to allocate a second instance of a singleton.");
    EnemyInfo temp;
    temp.showType = "Repeate";
    temp.showTime = 2;
    temp.types[1] = 1;
    temp.types[2] = 2;
    m_enemies.push_back(temp);
    
    temp.showTime = 5;
    temp.types[0] = 3;
    temp.types[1] = 4;
    temp.types[2] = 5;
    m_enemies.push_back(temp);
    
    EnemyType enemyType;
    enemyType.type = 0;
    enemyType.textureName = "E0.png";
    enemyType.bulletType = "W2.png";
    enemyType.hp = 1;
    enemyType.moveType = 0;
    enemyType.scoreValue = 15;
    m_enemyTypes.push_back(enemyType);
    
    enemyType.type = 1;
    enemyType.textureName = "E1.png";
    enemyType.bulletType = "W2.png";
    enemyType.hp = 2;
    enemyType.moveType = 0;
    enemyType.scoreValue = 40;
    m_enemyTypes.push_back(enemyType);
    
    enemyType.type = 2 ;
    enemyType.textureName = "E2.png";
    enemyType.bulletType = "W2.png";
    enemyType.hp = 4;
    enemyType.moveType = 2;
    enemyType.scoreValue = 60;
    m_enemyTypes.push_back(enemyType);
    
    enemyType.type = 3;
    enemyType.textureName = "E3.png";
    enemyType.bulletType = "W2.png";
    enemyType.hp = 6;
    enemyType.moveType = 3;
    enemyType.scoreValue = 80;
    m_enemyTypes.push_back(enemyType);
    
    enemyType.type = 4;
    enemyType.textureName = "E4.png";
    enemyType.bulletType = "W2.png";
    enemyType.hp = 10;
    enemyType.moveType = 2;
    enemyType.scoreValue = 150;
    m_enemyTypes.push_back(enemyType);
    
    enemyType.type = 5;
    enemyType.textureName = "E5.png";
    enemyType.bulletType = "W2.png";
    enemyType.hp = 15;
    enemyType.moveType = 2;
    enemyType.scoreValue = 200;
    m_enemyTypes.push_back(enemyType);
    
    //血量少于50的  将会被大招秒杀
}
Config::~Config()
{
    play_bullet->release();
    enemy_bullet->release();
    enemy_items->release();
    enemy_items = NULL;
    play_bullet = NULL;
    enemy_bullet = NULL;
    CCLOG("release config");

}

Config* Config::sharedConfig()
{
    if (!g_config) {
        g_config = new Config();
    }
    return g_config;
}

vector<EnemyInfo> Config::getEnemise()
{
    return m_enemies;
}

vector<EnemyType> Config::getEnemyType()
{
    return m_enemyTypes;
}

int Config::getScoreValue()
{
    return m_scoreValue;
}

void Config::setScoreValue(int score)
{
    m_scoreValue += score;

	mAlreadySkillCount = m_scoreValue / 1000;
}

int Config::getLifeCount()
{
    return m_life;
}
void Config::updateLifeCount()
{
    m_life--;
}

bool Config::getAudioState()
{
    return m_isAudioOn;
}

void Config::updateAudioState(bool isOn)
{
    m_isAudioOn = isOn;
}

void Config::resetConfig()
{
    m_scoreValue = 0;
    m_life = 5;
//    m_isAudioOn = true;

	mAlreadySkillCount = 0;
	mUsedSkillNum = 0;
}

void Config::useSkill()
{
	if (mAlreadySkillCount <= mUsedSkillNum) return;

	++mUsedSkillNum;

	CCObject *units;
	CCObject *enemybs;
	CCARRAY_FOREACH(enemy_items, units)
	{
		UnitSprite *enemy = (UnitSprite*)(units);
		if (enemy) for (int i=0; i<50; ++i) enemy->hurt();
	}

	CCARRAY_FOREACH(enemy_bullet, enemybs)
	{
		UnitSprite *enemyb = (UnitSprite*)(enemybs);
		if (enemyb) for (int i=0; i<50; ++i) enemyb->hurt();
	}

	CCLayerColor *layer = CCLayerColor::create(ccc4(255,255,255,200));
	CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
	layer->setOpacity(0);
	layer->runAction(CCSequence::create(
		CCFadeTo::create(0.1f,255), CCFadeTo::create(0.1f,0), CCRemoveSelf::create(), NULL));
}