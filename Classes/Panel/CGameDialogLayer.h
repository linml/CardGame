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

#define RIGHT_TAG 2002
#define LEFT_TAG 2001

class CPtDialog :public cocos2d::CCLayerColor
{
public:
    CREATE_FUNC(CPtDialog);
    static CPtDialog* create(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inLeftSelector, SEL_CallFuncO  inRightSelector, CCObject *inLeftParam, CCObject *inRightParam);
public:
    CPtDialog();
    virtual ~CPtDialog();
    
    void setDialog(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inLeftSelector, SEL_CallFuncO  inRightSelector, CCObject *inLeftParam, CCObject *inRightParam);
    void setButtonText(std::string inLeftText, std::string inRightText);
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void initDialog();
    void handlerTouch();
    void loadResource();
protected:
    int m_nTouchTag;
    vector<TouchRect> m_cTouches;
    
    CCLabelTTF *m_pTip;
    CCLabelTTF *m_pLeft;
    CCLabelTTF *m_pRight;
    CCObject *m_pHandler;
    SEL_CallFuncO m_pRightSelector;
    SEL_CallFuncO m_pLeftSelector;
    CCObject * m_pRightParam;
    CCObject * m_pLeftParam;
    
   // CCRect m_cTouchSpriteFrameRect[2];
    CCSpriteFrame *m_pFrame[2];
    CCSprite* m_pBtn[2];
    
};
#endif /* defined(___1_cube__CPtDialog__) */
