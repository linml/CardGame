//
//  CGameAddFriend.h
//  91.cube
//
//  Created by linminglu on 11/15/13.
//
//

#ifndef ___1_cube__CCustomerDialog__
#define ___1_cube__CCustomerDialog__

#include <iostream>
#include "cocos2d.h"
#include "CGameDialogLayer.h"
using namespace std;
using namespace cocos2d;

class CCustomerDialog :public CCLayer
{
public:
    static CCustomerDialog* create(CCObject *inLeftTarget, CCObject *inRightTarget, SEL_CallFuncO  inLeftSelector, SEL_CallFuncO  inRightSelector, CCObject *inLeftParam, CCObject *inRightParam);
public:
    CCustomerDialog();
    virtual ~CCustomerDialog();
    
    void setDialog(CCObject *inLeftTarget, CCObject *inRightTarget, SEL_CallFuncO  inLeftSelector, SEL_CallFuncO  inRightSelector, CCObject *inLeftParam, CCObject *inRightParam);
    void setLeftHandler(CCObject *inLeftTarget,SEL_CallFuncO  inLeftSelector,  CCObject *inLeftParam = NULL);
    void setRightHandler(CCObject *inRightTarget,SEL_CallFuncO  inRightSelector,CCObject *inRightParam = NULL);
    void setButtonText(std::string inLeftText, std::string inRightText);
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    bool initDialog(CCObject *inLeftTarget, CCObject *inRightTarget, SEL_CallFuncO  inLeftSelector, SEL_CallFuncO  inRightSelector, CCObject *inLeftParam, CCObject *inRightParam);
    void handlerTouch();
    void loadResource();
protected:
    int m_nTouchTag;
    vector<TouchRect> m_cTouches;
    
    CCLabelTTF *m_pTip;
    CCLabelTTF *m_pLeft;
    CCLabelTTF *m_pRight;
    CCObject *m_pLeftHandler;
    CCObject *m_pRightHandler;
    SEL_CallFuncO m_pRightSelector;
    SEL_CallFuncO m_pLeftSelector;
    CCObject * m_pRightParam;
    CCObject * m_pLeftParam;
    CCSpriteFrame *m_pFrame[2];
    CCSprite* m_pBtn[2];
};
#endif /* defined(___1_cube__CCustomerDialog__) */
