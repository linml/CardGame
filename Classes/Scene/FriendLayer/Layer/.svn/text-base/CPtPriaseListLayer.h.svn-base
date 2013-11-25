//
//  CPtPriaseListLayer.h
//  91.cube
//
//  Created by phileas on 13-11-15.
//
//

#ifndef ___1_cube__CPtPriaseListLayer__
#define ___1_cube__CPtPriaseListLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CFriend.h"
#include "CPtTableItem.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class CPtPriaseItem :public CPtTableItem
{
public:
    static CPtPriaseItem* create(Friend inFriendData, int inIndex);
public:
    CPtPriaseItem(Friend inFriendData);
    virtual ~CPtPriaseItem();
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void setHandler(CCObject*pTarget, SEL_CallFuncO pSelector){m_pHandler = pTarget; m_pOweSelector = pSelector;};
protected:
    void initPtPriase();
protected:
    Friend m_sFriendData;
    CCSprite *m_pOweBtn;
    CCObject *m_pHandler;
    SEL_CallFuncO m_pOweSelector;
    CCTexture2D* texture[2];
    CC_SYNTHESIZE(int, m_nIndex, Index);
    
};


class CPtPriaseListLayer : public CCLayer
{
public:
    CREATE_FUNC(CPtPriaseListLayer);
public:
    CPtPriaseListLayer();
    virtual ~CPtPriaseListLayer();
    virtual bool init();
    bool getHandlerHttp(){return m_bHandlerHttp;}
    void onClickAllGanEn();
protected:
    void initData();
    void initUI();
    void deletePraseListInfo();
    void onSendGetPraiseListRequest();
    void onReceivePraiseListMsg(CCObject *pObject);
    
    void onSendGanEnRequest(const vector<int> &ids);
    void onReceiveGanEnMsg(CCObject *pObject);
    
    void onClickGanEn(CCObject *pObject);
    
    void updateData();
    void saveData(CCDictionary *inResDict);
    
    //test
    void testData();
protected:
    vector<Friend> * m_pAllPriaseListInfo;
    CFriendManager *m_pFriendManager;
    CCLayer *m_pContainer;
    CCArray *m_pPriases;
    bool m_bHandlerHttp;
    bool m_bHasAllGanEn;
    int m_nSelectIndex;

};

#endif /* defined(___1_cube__CPtPriaseListLayer__) */
