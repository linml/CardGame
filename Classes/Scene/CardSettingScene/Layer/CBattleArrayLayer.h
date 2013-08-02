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
#include "CCardEnhanceLayer.h"
#include "CCardSellLayer.h"
using namespace cocos2d;




class CBattleArrayLayer : public CCLayer
{
    
public:
    CREATE_FUNC(CBattleArrayLayer);
    
    CBattleArrayLayer();
    virtual ~CBattleArrayLayer();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void removeCallBack(CCNode *pNode);
    
    void addTeamArrayPanel();
    void addEnhance();
    void addSell();
    
    void removeLeft();
protected:
    void initCards();
    

//protected:
public:
    float m_fDistance;

    bool m_bAdd;
    bool m_bReplace;
    CPtDisPlayCard *m_pMoveCard;
    CCNode * m_pArray[5];
    CCPoint m_cPrePoint;
    
    CPtBattleArrayPanel * panel;
    CCardEnhanceLayer * m_pEnhancePanel;
    CCardSellLayer * m_pSellPanel;
    CGamePlayer * m_pGamePlayer;
    CPtListViewWidget *m_pCards;
    
    CC_SYNTHESIZE(bool, m_bTableClikEnable, TableClickEnable);
    CC_SYNTHESIZE(bool, m_bTableClickMove, TableClickMove);
    
    CC_SYNTHESIZE(int, m_nCurrentTab, CurrentTab);
 
};

class CPtBattleArrayItem : public CPtTableItem
{
public:
    CPtBattleArrayItem(){m_pDelegateLayer = NULL;m_bSellPanel = false;};
    CREATE_FUNC(CPtBattleArrayItem);
    virtual bool init(){ return CPtTableItem::init();};
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void setDelegateLayer(CBattleArrayLayer * inLayer){ m_pDelegateLayer = inLayer;};
    
    void onEnhanceBegin(CCTouch *pTouch, CCEvent *pEvent);
    void onEnhanceEnd(CCTouch *pTouch, CCEvent *pEvent);
    void onSellEnd(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    CBattleArrayLayer * m_pDelegateLayer;
    CC_SYNTHESIZE(bool, m_bSellPanel, SellPanel);
    
};

#endif /* defined(___1_cube__CBattleArrayLayer__) */
