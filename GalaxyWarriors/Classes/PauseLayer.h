#ifndef __MoonWarriorsx__PauseLayer__
#define __MoonWarriorsx__PauseLayer__

#include "cocos2d.h"
USING_NS_CC;

class PauseLayer : public CCLayerColor {
    
public:
    virtual bool init();
/*    virtual void  registerWithTouchDispatcher();
    
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
*/
//    void doPause();
    void doResume(CCObject *pSender);
	void gotoSartLayer(CCObject *pSender);
    CREATE_FUNC(PauseLayer);


	void schedule_menuPriority();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event){return true;}

private:
	CCMenu *mMenu;
};


#endif /* defined(__MoonWarriorsx__PauseLayer__) */