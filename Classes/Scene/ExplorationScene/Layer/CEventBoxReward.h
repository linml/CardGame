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
#include "CEventBoxConfigData.h"

using namespace cocos2d;

#define GET_BOX 0
#define IGNORE_BOX 1
#define EMPTY_EVENT 2
class CEventBoxRewordLayer : public CCLayer
{
public:    
    static CEventBoxRewordLayer *create(CEventBoxData *inEventBoxData, int inType = IGNORE_BOX);
public:
    
    CEventBoxRewordLayer();
    virtual ~CEventBoxRewordLayer();
    void setHanlder(CCObject *inTarget, SEL_CallFuncO inCanfirmSelector);
    
public:
    virtual bool init(CEventBoxData *inEventBoxData,int inType);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initEventBoxRewordLayer(CEventBoxData *inEventBoxData,int inType);
    void handlerTouch();
    void loadResource();
    
    
    void createReWordDialog(CEventBoxData *inEventBoxData);
    void createConfirmDialog();
    void createEmptyDialog();
protected:
    
    LayoutLayer *m_cMaps;    
    int m_nTouchTag;
    int m_nType;
    bool m_bMove;
    
    CCSprite * m_pBtn;
    CCRect m_cTouchSpriteFrameRect[2];
    SEL_CallFuncO m_pConfirmSelector;
    CCObject *m_pTarget;
};


#endif /* defined(___1_cube__CEventBoxReward__) */
