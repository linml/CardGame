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
#include "CGameButtonControl.h"
#include "CSkillData.h"
#include "CPlayerBufferManager.h"
#include "CBufferLogo.h"
using namespace cocos2d;

#define CGAMEBUFFERTIPDIALOG_TOUCH_PRORITY -50000

class CAltarBufferLogo : public CBufferLogo
{
public:
  static  CAltarBufferLogo *create(AltarBuffer& inAltarBuffer);
public:
    CAltarBufferLogo(AltarBuffer &inAltarBuffer);
    ~CAltarBufferLogo();
    bool initCAltarBufferLog(int inSkillEffectId);
};

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
    
    void createBufferTipUI(CCSprite *inBg);
    void createDebufferTipUI(CCSprite *inBg);
    CCNode *createShopItem(int inPropId);
    void loadResource();
    void  handlerTouch(CCTouch* pTouch);
    void  onClickClose();
    void  onClickUse();
    
    void  onSendCancelBufferRequest();
    void  onReceiveMsg(CCObject *pObject);
    


protected:
    int m_nTouchTag;
    int m_nBufferKey;
    int m_nBufferType;
    int m_nTipId;
    int m_nPropId;
    CCSprite *m_pCloseBtn;
    CGameButtonControl *m_pUseBtn;
    CCObject *m_pCloseHandler;
    SEL_CallFuncO m_pCloseSelector;
    CImapact* m_pImapact;
    
    
};

#endif /* defined(___1_cube__CGameBufferTip__) */
