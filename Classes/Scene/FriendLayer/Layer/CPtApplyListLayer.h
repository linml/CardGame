//
//  CPtApplyListLayer.h
//  91.cube
//
//  Created by phileas on 13-11-15.
//
//

#ifndef ___1_cube__CPtApplyListLayer__
#define ___1_cube__CPtApplyListLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CFriend.h"
#include "CGameFriedItem.h"
#include "CPtTableItem.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class CPtApplyItem : public CPtTableItem
{
public:
    CREATE_FUNC(CPtApplyItem);
public:
    CPtApplyItem();
    virtual ~CPtApplyItem();
    virtual bool init();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void setHandler(CCObject *pTarget,SEL_CallFuncO cancel, SEL_CallFuncO consent);
    void setIndex(int inIndex);
protected:
    void initApplayItemData();
    
protected:
    int m_nTag;
    int m_nIndex;
    CCObject *m_pHanlder;
    SEL_CallFuncO m_pCancelSelector;
    SEL_CallFuncO m_pConsentSelector;
    CC_SYNTHESIZE(bool, m_bHasConsent, HasConsent);
};


class CPtApplyListLayer : public CCLayer
{
public:
    CREATE_FUNC(CPtApplyListLayer);
public:
    CPtApplyListLayer();
    virtual ~CPtApplyListLayer();
    virtual bool init();
    bool getHandlerHttp(){return m_bHandlerHttp;};
    // test:
    void testData();
    
    void onClickAllConsent();
    void onClickAllCancel();
protected:
    void initData();
    void initCPtApplyListLayer();
    void initUI();
    void releaseResource();
    void deleteApplyListData();
    void onSendGetApplyListData();
    void onReceiveGetApplyMsg(CCObject *pObject);
    
    void onClickCancle(CCObject *pObject);
    void onClickConsent(CCObject *pObject);
    
    void onSendApplyRequest(const vector<int> &array, int inType);
    void onReceiveApplyMsg(CCObject *pObject);
    
    void updateData();
protected:
    vector<CFriendMM*> *m_pApplyList;
    CCArray *m_pApplyItems;
    CCLayer * m_pContainer;
    bool m_bHandlerHttp;
    bool m_bAllOperator;
    int m_nSelectIndex;
    CFriendManager *m_pFriendManager;
};

#endif /* defined(___1_cube__CPtApplyListLayer__) */
