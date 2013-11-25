//
//  CCardBagFullDialog.h
//  91.cube
//
//  Created by phileas on 13-11-13.
//
//

#ifndef ___1_cube__CCardBagFullDialog__
#define ___1_cube__CCardBagFullDialog__

#include "CGameSelectDialog.h"

#define CARDBAGCLOSE_TAG 2004
class CCardBagFullDialog :public CGameSelectDialog
{
public:    
    static CCardBagFullDialog* create();
public:
    CCardBagFullDialog();
    virtual ~CCardBagFullDialog();
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void initCCardBagFullDialog();
    
    // detail event handler:
    
    void onClickGoEnhance(CCObject *pObject);
    void onClickGoSell(CCObject *pObject);
    void onClickExtentCardBag(CCObject *pObject);
    void onClickCloseBtn();
    
protected:
    CCSprite *m_pCloseBtn;
    CCSpriteFrame *m_pCloseFrame[2];
};


#endif /* defined(___1_cube__CCardBagFullDialog__) */
