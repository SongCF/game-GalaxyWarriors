#include "UnitSprite.h"
#include "ExitLayer.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

UnitSprite::UnitSprite()
{
    
}
UnitSprite::~UnitSprite()
{
    
}

void CostomeKeyPadDelegae::keyBackClicked()
{
	CCDirector::sharedDirector()->pause();
//	CCDirector::sharedDirector()->getRunningScene()->pauseSchedulerAndActions();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();

	CCLayer *exitLayer = ExitLayer::create();
	CCDirector::sharedDirector()->getRunningScene()->addChild(exitLayer, 1024);
}