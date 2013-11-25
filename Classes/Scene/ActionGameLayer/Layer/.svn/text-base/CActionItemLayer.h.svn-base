//
//  CActionItemLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-30.
//
//

#ifndef ___1_cube__CActionItemLayer__
#define ___1_cube__CActionItemLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include "CGameButtonControl.h"


using namespace std;
using namespace cocos2d;
class CStructGameActionData;

class CActionItemLayer:public cocos2d::CCLayer
{
public:
    static CActionItemLayer *Created(CStructGameActionData *data);
    bool  initCreate(CStructGameActionData *data);
public:
    CActionItemLayer();
    ~CActionItemLayer();
    

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

protected:
    
    bool handlerTouchEvent(CCTouch* pTouch);
    bool handlerSingleButton(CCTouch *pTouch);
    bool handlerMultiButtons(CCTouch *pTouch);
    
    void recoverButtons(CCTouch *pTouch);
    
    void handlerGoEncounter();
    void handlerGetReward();
    void handlerGoRecharge();
    
    int isTouchEvent(CCTouch *pTouch, int inType);
    
    void onClickGoEncounter();
    void onClickGoRecharge();
    void onClickGetItems(int inItemId);
    
    // connect with server:
    void onSendGetItemRequest(int inItemId);
    void onReceiveGetItemMsg(CCObject* pObject);
    
private:
    void createHead(string strHead);//创建一个 标题
    void createContext(string  strContext);//创建一个内容
    void createPng(string strPngName);
    void createTips(CStructGameActionData *data);
    void createFuben(CStructGameActionData *data);
    void createChongZhi(CStructGameActionData *data);
protected:
    
    int m_nTouchTag;
    int m_nRewardButtonCount;
    CGameButtonControl *m_pSingleButton;
    CGameButtonControl **m_pRewardButtons;
private:
    CCSize size;
    CStructGameActionData *m_pData;
    float height;
    
    
   
    
};

#endif /* defined(___1_cube__CActionItemLayer__) */
