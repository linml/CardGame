//
//  CGameBufferTip.h
//  91.cube
//
//  Created by phileas on 13-10-24.
//
//

#ifndef ___1_cube__CGameBufferTip__
#define ___1_cube__CGameBufferTip__

#include "cocos2d.h"
using namespace cocos2d;

#define CGAMEBUFFERTIPDIALOG_TOUCH_PRORITY 50000

class CGameBufferTipLayer : public CCLayerColor
{
public:
    static CGameBufferTipLayer * create(int inBufferKey);
public:
    CGameBufferTipLayer();
    virtual ~CGameBufferTipLayer();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    bool initCGameBufferTipLayer(int inBufferKey);
    
    void setCloseHanlder(CCObject *inTarget, SEL_CallFuncO inSelector);
    
protected:
    void initCGameBufferTipUI();
    
    void createBufferTipUI();
    void createDebufferTipUI();
    void loadResource();
    void  handlerTouch(CCTouch* pTouch);
    void  onClickClose();


protected:
    int m_nTouchTag;
    int m_nBufferKey;
    int m_nBufferType;
    CCSprite *m_pCloseBtn;
    
    CCObject *m_pCloseHandler;
    SEL_CallFuncO m_pCloseSelector;
    
};

#endif /* defined(___1_cube__CGameBufferTip__) */
