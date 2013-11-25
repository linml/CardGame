//
//  CGameSelectDialog.h
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#ifndef ___1_cube__CGameSelectDialog__
#define ___1_cube__CGameSelectDialog__


#include <vector>
#include "cocos2d.h"
#include "Utility.h"
using namespace cocos2d;


#define SELECT_MIDDLE_TAG  2002
#define SELECT_LEFT_TAG    2001
#define SELECT_RIGHT_TAG   2003

class CGameSelectDialog :public cocos2d::CCLayerColor
{
public:
    static CGameSelectDialog* create(std::string inTipContent);
public:
    CGameSelectDialog();
    virtual ~CGameSelectDialog();
    
    void setLeftHandler(CCObject *inLeftTarget,SEL_CallFuncO  inLeftSelector,  CCObject *inLeftParam = NULL);
    void setRightHandler(CCObject *inRightTarget,SEL_CallFuncO  inRightSelector,CCObject *inRightParam = NULL);
    void setMiddleHanlder(CCObject *inMiddleTarget,SEL_CallFuncO  inMiddleSelector,  CCObject *inMiddleParam = NULL);
    void setButtonText(const char *inLeftText,const char *inMiddleText, const char *inRightText);
    void setContentTip(const char *inContentTip);
    void setCloseCallBack(CCObject *pObject, SEL_CallFunc closeCallback);
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void initData();
    void initCGameSelectDialog();
    void handlerTouch();
    void onClose();
    void loadResource();
  
    
protected:
    int m_nTouchTag;
    vector<TouchRect> m_cTouches;
    
    CCLabelTTF *m_pTip;
    CCLabelTTF *m_pLeft;
    CCLabelTTF *m_pRight;
    CCLabelTTF *m_pMiddle;
    
    CCObject *m_pLeftHandler;
    CCObject *m_pRightHandler;
    CCObject *m_pMiddleHandler;
    
    SEL_CallFuncO m_pRightSelector;
    SEL_CallFuncO m_pLeftSelector;
    SEL_CallFuncO m_pMiddleSelector;
    
    CCObject *m_pCloseHandler;
    SEL_CallFunc m_pCloseSelector;

    
    CCObject * m_pRightParam;
    CCObject * m_pLeftParam;
    CCObject * m_pMiddleParam;
    
    // CCRect m_cTouchSpriteFrameRect[2];
    CCSpriteFrame *m_pFrame[2];
    CCSprite* m_pBtn[3];
    
};


#endif /* defined(___1_cube__CGameSelectDialog__) */
