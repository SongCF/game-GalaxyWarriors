#ifndef __MoonWarriorsx__About__
#define __MoonWarriorsx__About__

#include "cocos2d.h"
USING_NS_CC;

class About : public CCLayer {
    
public:
    static CCScene* scene();
    virtual bool init();
    void goBack(CCObject* pSender);
    CREATE_FUNC(About);
    
};

#endif /* defined(__MoonWarriorsx__About__) */