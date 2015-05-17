#ifndef __MoonWarriorsx__ExitLayer__
#define __MoonWarriorsx__ExitLayer__
#include "cocos2d.h"
USING_NS_CC;

class ExitLayer : public CCLayer
{
public:
	ExitLayer() {}
	~ExitLayer() {}

	bool init();
	void goBack(CCObject* pSender);
	void exitGame(CCObject* pSender);

	CREATE_FUNC(ExitLayer);

};

#endif