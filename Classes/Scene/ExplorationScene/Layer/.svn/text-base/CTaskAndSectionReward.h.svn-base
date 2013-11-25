//
//  CTaskAndSectionReward.h
//  91.cube
//
//  Created by phileas on 13-9-24.
//
//

#ifndef ___1_cube__CTaskAndSectionReward__
#define ___1_cube__CTaskAndSectionReward__

#include "cocos2d.h"

using namespace cocos2d;

class CTaskAndSectionReward : public CCLayerColor
{
public:
    static CTaskAndSectionReward * create();
    CTaskAndSectionReward();
    virtual ~CTaskAndSectionReward();
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initData();
    void loadResource();
protected:
    CCSprite * m_pBtn;
    CCRect m_cTouchSpriteFrameRect[2];
    SEL_CallFuncO m_pConfirmSelector;
    CCObject *m_pTarget;
};

#endif /* defined(___1_cube__CTaskAndSectionReward__) */
