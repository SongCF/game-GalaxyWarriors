#ifndef MoonWarriorsx_Config_h
#define MoonWarriorsx_Config_h

#include "cocos2d.h"
#include <vector>

USING_NS_CC;
using namespace std;

extern cocos2d::CCArray *play_bullet;
extern cocos2d::CCArray *enemy_bullet;
extern cocos2d::CCArray *enemy_items;

typedef struct Enemies
{
    string showType;
    int showTime;
    int types[3];
    Enemies()
    {
        showType = "Repeate";
        showTime = 0;
        types[0] = 0;
        types[1] = 0;
        types[2] = 0;
        
    }
}EnemyInfo;

typedef struct {
    int type;
    string textureName;
    string bulletType;
    int hp;
    int moveType;
    int scoreValue;
}EnemyType;

class Config : public CCObject{
private:
    vector<EnemyInfo> m_enemies;
    vector<EnemyType> m_enemyTypes;
    int m_scoreValue;
    int m_life;
    bool m_isAudioOn;
	bool m_canUseAccelerate;

	bool m_HasSkill;
	int mUsedSkillNum; //已用的skill个数
	int mAlreadySkillCount;//总的skill个数 （包含已用的）

    Config();
public:
	static Config* sharedConfig();
	~Config();
	void resetConfig();

    vector<EnemyInfo> getEnemise();
    vector<EnemyType> getEnemyType();

    bool getAudioState();
    void updateAudioState(bool isOn);

	void updateLifeCount();
	int getLifeCount();

	void setScoreValue(int score);
	int getScoreValue();

	void setCanUseSkill(bool b) {m_HasSkill = b;}
	bool getCanUseSkill(){return m_HasSkill;}
	void useSkill();

	bool getAcclerateState() {return m_canUseAccelerate;}
	void setAcclerateState(bool state) {m_canUseAccelerate = state;}
};


#endif