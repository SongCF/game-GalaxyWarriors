#ifndef __MoonWarriorsx__Options__
#define __MoonWarriorsx__Options__

#include "cocos2d.h"
USING_NS_CC;

class Options : public CCLayer {
    
public:
    static CCScene* scene();

    void setOptions(CCObject* pSender);
	void setSkill(CCObject *psender);
	void setAccelerate(CCObject *pSender);
    void goBack(CCObject *pSender);

    virtual bool init();
    CREATE_FUNC(Options);
    
};

#endif /* defined(__MoonWarriorsx__Options__) */