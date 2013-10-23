//
//  CEventBoxDialog.h
//  91.cube
//
//  Created by phileas on 13-9-12.
//
//

#ifndef ___1_cube__CEventBoxDialog__
#define ___1_cube__CEventBoxDialog__

#include "cocos2d.h"
#include "LayoutLayer.h"
#include "CEventBoxConfigData.h"

class CEventBoxLayer : public CCLayer
{
public:
    
    static CEventBoxLayer* create(int inEventBoxId);
    static CEventBoxLayer* create(CEventBoxData *inEventBoxData);
public:
    
    CEventBoxLayer();
    virtual ~CEventBoxLayer();
    void setHanlder(CCObject *inTarget, SEL_CallFuncO inOpenSelector, SEL_CallFuncO inSkipSelector);
    
public:
    virtual bool init(CEventBoxData *inEventBoxData);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initEventBoxLayer(CEventBoxData *inEventBoxData);
    void handlerTouch();
    void loadResource();
    
    // test:
    void testUpdateString();
protected:
    
    
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
   
    CCLabelTTF *tips;
    int m_nTouchTag;
    bool m_bMove;
    
    CCSprite * m_pBtn[2];
   // CCRect m_cTouchSpriteFrameRect[2];
    CCSpriteFrame *m_pState[2];
    
    SEL_CallFuncO m_pOpenSelector;
    SEL_CallFuncO m_pSkipSelector;
    CCObject *m_pTarget;
    
    CC_SYNTHESIZE(bool, m_bGPEnough, GPEnough);
};


#endif /* defined(___1_cube__CEventBoxDialog__) */
