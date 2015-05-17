#include "Effect.h"
#include "Resource.h"
#include <string>
USING_NS_CC;
using namespace std;

Effect::Effect()
{
}

Effect::~Effect()
{
}

Effect* Effect::create()
{
    Effect* effect = new Effect();
    if (effect) {
        effect->autorelease();
        return effect;
    }else{
        delete effect;
        effect = NULL;
        return NULL;
    }
}
//执行完这个函数跳到StartMenu::newGame()开始游戏
void Effect::flareEffect(CCNode *parent, CCCallFunc *callback)
{
	int n=0;
	CCLog("begin>>>Effect::flareEffect()",n);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // 特效图片
    CCSprite *flare = CCSprite::create(s_flare);
    
    // 设置混合模式
    ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    flare->setBlendFunc(cbl);
    
    // 添加到父节点
    parent->addChild(flare, 10);
    
    // 设置初始透明度
    flare->setOpacity(0);
    
    // 设置初始位置
    flare->setPosition(ccp(-30, winSize.height - 130));
    
    // 设置初始角度
    flare->setRotation(-120);
    
    // 设置初始放大系数
    flare->setScale(0.2);
    
    //1 透明度渐变
    CCActionInterval *opacityAnim = CCFadeTo::create(0.5, 255);
    CCActionInterval *opacDim = CCFadeTo::create(1, 0);
    
    //2 大小渐变    
    CCActionInterval *bigAnim = CCScaleBy::create(0.7, 1.2, 1.2);   
    CCEaseSineOut *biggerEase = CCEaseSineOut::create(bigAnim);

	//3 移动渐变
    CCActionInterval *moveAnim = CCMoveBy::create(0.5, ccp(328, 0));
    CCEaseSineOut *moveEase = CCEaseSineOut::create(moveAnim);
    
    //4 角度旋转    // 角度旋转速度，指数变化
    CCActionInterval *roteAnim = CCRotateBy::create(2.5, 90);  
    CCEaseExponentialOut * rotateEase = CCEaseExponentialOut::create(roteAnim);
    
    //5 放大到原大小
    CCScaleTo *bigger = CCScaleTo::create(0.5, 1);
    
    
    // 执行动画             
    flare->runAction(CCSequence::create(opacityAnim, biggerEase, opacDim, callback,  NULL));
    flare->runAction(moveEase);
    flare->runAction(rotateEase);
    flare->runAction(bigger);
	CCLog("end>>>Effect::flareEffect()",n);
}

void Effect::explode(CCNode *parent, CCPoint pos)
{
    // 第一帧
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("explosion_01.png");
    CCSprite *explosion = CCSprite::createWithSpriteFrame(pFrame);
    parent->addChild(explosion);
    explosion->setPosition(pos);
    
    CCSize cs = explosion->getContentSize();
    
    // 完成之后的回调
    CCCallFuncN *removeFunc =  CCCallFuncN::create(explosion, callfuncN_selector(Effect::killSprite));
    
    // 爆炸动画
    CCAnimation *animation = CCAnimationCache::sharedAnimationCache()->animationByName("Explosion");
    explosion->runAction(CCSequence::create(CCAnimate::create(animation), removeFunc, NULL));
    
}

// 闪光动画
void Effect::spark(CCPoint pos, CCNode *parent, float scale, float duration)
{
    // spark 精灵
    CCSprite *one = CCSprite::create(s_explode1);
    CCSprite *two = CCSprite::create(s_explode2);
    CCSprite *three = CCSprite::create(s_explode3);
    
    // 混合模式
    ccBlendFunc cb = { GL_SRC_ALPHA, GL_ONE };
    one->setBlendFunc(cb);
    two->setBlendFunc(cb);
    three->setBlendFunc(cb);
    
    one->setPosition(pos);
    two->setPosition(pos);
    three->setPosition(pos);
    
    parent->addChild(one);
    parent->addChild(two);
    parent->addChild(three);
    
    one->setScale(scale);
    two->setScale(scale);
    three->setScale(scale);
    
    three->setRotation(CCRANDOM_0_1() * 360);
    
    CCRotateBy *left = CCRotateBy::create(duration, -45);
    CCRotateBy *right = CCRotateBy::create(duration, 45);
    CCScaleBy *scaleBy = CCScaleBy::create(duration, 3);
    CCFadeOut *fadeOut = CCFadeOut::create(duration);
    

    //JS写这个地方真是爽，这里却要写3个回调和3个动作
    CCCallFuncN *removeOne =  CCCallFuncN::create(this, callfuncN_selector(Effect::killSprite));
    CCCallFuncN *removeTwo =  CCCallFuncN::create(this, callfuncN_selector(Effect::killSprite));
    CCCallFuncN *removeThree =  CCCallFuncN::create(this, callfuncN_selector(Effect::killSprite));
    CCAction *seqOne = CCSequence::create(fadeOut, removeOne, NULL);
    CCAction *seqTwo = CCSequence::create((CCFadeOut*)fadeOut->copy(), removeTwo, NULL);
    CCAction *seqThree = CCSequence::create((CCFadeOut*)fadeOut->copy(), removeThree, NULL);
    
    one->runAction(left);
    two->runAction(right);
    
    one->runAction(scaleBy);
    two->runAction((CCScaleBy*)scaleBy->copy());
    three->runAction((CCScaleBy*)scaleBy->copy());
    
    one->runAction(seqOne);
    two->runAction(seqTwo);
    three->runAction(seqThree);
    
    
}

//动画加入缓存
void Effect::sharedExplosion()
{
	int n=0;
	CCLog("begin>>>Effect::sharedExplosion");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_explosion_plist);
    CCArray *animFrames = CCArray::create();
    
    char str[64] = {0};
    
    for (int i = 1; i < 35; ++i)
	{
        sprintf(str, "explosion_%02d.png", i);
        CCLOG("%s",str);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
        animFrames->addObject(frame);
    }
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.04);
    
    // 帧动画命名
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "Explosion");
    CCLog("end>>>Effect::sharedExplosion");
}

void Effect::killSprite(CCNode *pSender)
{
//    CCLOG("remove effect");
    pSender->removeFromParent();
}
