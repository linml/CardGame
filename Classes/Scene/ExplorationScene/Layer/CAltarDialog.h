//
//  CAltarDialog.h
//  91.cube
//
//  Created by phileas on 13-10-23.
//
//

#ifndef ___1_cube__CAltarDialog__
#define ___1_cube__CAltarDialog__

#include "cocos2d.h"

using namespace cocos2d;

#define CALTARRDIALOG_TOUCH_PRORITY 50000

class CAltarDialogLayer : public CCLayerColor
{
    
public:
    static CAltarDialogLayer* create(int inEventId);
public:
    CAltarDialogLayer();
    virtual ~CAltarDialogLayer();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    bool initCAltarDialogLayer(int inEventId);
    void setCloseHandler(CCObject *inTarget, SEL_CallFuncO inSelector);

protected:

    void iniCAltarDialogUI();

    void onCloseBtn();

    void loadResource();
    
    void handlerTouch(CCTouch *pTouch);

protected:
    int m_nTouchTag;
    CCSprite * m_pCloseBtn;

    CCObject *m_pCloseHandler;
    SEL_CallFuncO m_pCloseSelector;
    CC_SYNTHESIZE(int,m_nEventId, EventId)
};

#endif /* defined(___1_cube__CAltarDialog__) */
