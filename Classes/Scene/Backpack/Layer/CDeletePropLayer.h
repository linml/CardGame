//
//  CDeletePropLayer.h
//  91.cube
//
//  Created by phileas on 13-8-29.
//
//

#ifndef ___1_cube__CDeletePropLayer__
#define ___1_cube__CDeletePropLayer__

#include "cocos2d.h"

#include "LayoutLayer.h"
#include "CPtStarConfigData.h"
#include "CPtSliderWidget.h"

#include <vector>

using namespace cocos2d;
using namespace std;

#define ADD_TAG 2
#define SUB_TAG 1

class CDeletePropLayer : public CCLayer
{
public:
    
    static CDeletePropLayer* create(PropItem *inPropItem);
public:
   
    CDeletePropLayer();
    virtual ~CDeletePropLayer();
    void setHanlder(CCObject *inTarget, SEL_CallFuncO m_pConfirmSelector, SEL_CallFuncO m_pCancelSelector);
    
public:
    virtual bool init(PropItem *item);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initDeletePropLayer(PropItem *item);
    void handlerTouch();
    void updateTexture(bool sliderEnable = false, bool inEnd = false);
    
    void startLongPress(int inType);
    void stopLongPress();
    void longTouchCallBack(float dt);
protected:
  
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    CCLabelTTF * m_pNumberLabel;
    
    float m_fTotalTime;
    
    int m_nTouchTag;
    int m_nMaxCount;
    int m_nCurrentCount;
    int m_nPropId;
    int m_nType;
    
    bool m_bMove;
    bool m_bLongPress;
    bool m_bCanDrag;
    
    
    CCSprite * m_pBtn[2];
    CCRect m_cTouchSpriteFrameRect[6];
    
    CPtSliderWidget *m_pSlider;
    CCSprite * m_pThum;
    
    
    SEL_CallFuncO m_pConfirmSelector;
    SEL_CallFuncO m_pCancelSelector;
    CCObject *m_pTarget;
};



#endif /* defined(___1_cube__CDeletePropLayer__) */
