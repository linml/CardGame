//
//  CGameErrorConfig.h
//  91.cube
//
//  Created by linminglu on 13-8-15.
//
//

#ifndef ___1_cube__CPtDialog__
#define ___1_cube__CPtDialog__

#include <iostream>
#include <string>
#include <map>
#include "cocos2d.h"
#include <vector>
#include "Utility.h"
using namespace std;

#define CONFIRM_TAG 2002
#define CANCEL_TAG 2001

class CPtDialog :public cocos2d::CCLayer
{
public:
    CREATE_FUNC(CPtDialog);
    static CPtDialog* create(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inConfirmSelector, SEL_CallFuncO  inCancelSelector, CCObject *inConfirmParam, CCObject *inCancleParam);
public:
    CPtDialog();
    virtual ~CPtDialog();
    
    void setDialog(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inConfirmSelector, SEL_CallFuncO  inCancelSelector, CCObject *inConfirmParam, CCObject *inCancleParam);
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void initDialog();
    void handlerTouch();
protected:
    int m_nTouchTag;
    vector<TouchRect> m_cTouches;
    CCLabelTTF *m_pTip;
    CCObject *m_pHandler;
    SEL_CallFuncO m_pConfirmSelector;
    SEL_CallFuncO m_pCancelSelector;
    CCObject * m_pConfirmParam;
    CCObject * m_pCancleParam;

};
#endif /* defined(___1_cube__CPtDialog__) */
