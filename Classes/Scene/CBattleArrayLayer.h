//
//  CBattleArrayLayer.h
//  91.cube
//
//  Created by phileas on 13-7-8.
//
//

#ifndef ___1_cube__CBattleArrayLayer__
#define ___1_cube__CBattleArrayLayer__

#include "cocos2d.h"
#include "CPtBatterArray.h"
#include "gamePlayer.h"
#include "CPtListViewWidget.h"
#include "CPtTableItem.h"

using namespace cocos2d;


class CBattleArrayLayer : public CCLayer
{
    
public:
    CREATE_FUNC(CBattleArrayLayer);
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void removeCallBack(CCNode *pNode);
protected:
    void initCBattleArray();
    void initCards();
    void initBatterArray();
    
    // test:
    
    void load();
    void callback(float dt);
    

//protected:
public:
    float m_fDistance;
    int m_nCurrentIndex;
    int m_nCount;

    bool m_bAdd;
    bool m_bReplace;
    CCNode *m_pMoveCard;
    CCNode * m_pArray[5];
    CCPoint m_cPrePoint;
    
    CPtBattleArrayPanel * panel;
    CGamePlayer * m_pGamePlayer;
    CPtListViewWidget *m_pCards;
    

    
 
};

class CPtBattleArrayItem : public CPtTableItem
{
public:
    CPtBattleArrayItem(){m_pDelegateLayer = NULL;};
    CREATE_FUNC(CPtBattleArrayItem);
    virtual bool init(){ return CPtTableItem::init();};
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void setDelegateLayer(CBattleArrayLayer * inLayer){ m_pDelegateLayer = inLayer;};
protected:
    CBattleArrayLayer * m_pDelegateLayer;
    
};

#endif /* defined(___1_cube__CBattleArrayLayer__) */
