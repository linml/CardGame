//
//  CEventBoxReward.h
//  91.cube
//
//  Created by phileas on 13-9-13.
//
//

#ifndef ___1_cube__CEventBoxReward__
#define ___1_cube__CEventBoxReward__

#include "cocos2d.h"
#include "LayoutLayer.h"

using namespace cocos2d;

class CEventBoxRewordLayer : public CCLayer
{
public:
    
    CREATE_FUNC(CEventBoxRewordLayer);
public:
    
    CEventBoxRewordLayer();
    virtual ~CEventBoxRewordLayer();
    void setHanlder(CCObject *inTarget, SEL_CallFuncO inCanfirmSelector);
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initEventBoxRewordLayer();
    void handlerTouch();
    void loadResource();
    
    
    void createReWordDialog();
    void createConfirmDialog();
protected:
    
    LayoutLayer *m_cMaps;    
    int m_nTouchTag;
    bool m_bMove;
    
    CCSprite * m_pBtn;
    CCRect m_cTouchSpriteFrameRect[2];
    SEL_CallFuncO m_pConfirmSelector;
    CCObject *m_pTarget;
};


#endif /* defined(___1_cube__CEventBoxReward__) */
