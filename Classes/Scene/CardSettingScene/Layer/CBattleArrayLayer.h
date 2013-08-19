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
#include "CCardEvolutionLayer.h"

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
    void addEvolution();
    
    void removeLeft();
protected:
    void initCards();
    void resetState();

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
    CCardEvolutionLayer *m_pEvolutionPanel;
    CCardSellLayer * m_pSellPanel;


    CGamePlayer * m_pGamePlayer;
    CPtListViewWidget *m_pCards;
    
    
    CC_SYNTHESIZE(bool, m_bTableClikEnable, TableClickEnable);
    CC_SYNTHESIZE(bool, m_bTableClickMove, TableClickMove);
    
    CC_SYNTHESIZE(int, m_nCurrentTab, CurrentTab);
    CC_SYNTHESIZE(CPtDisPlayCard*, m_pPreCardManifier, PreCardManifier);
    CC_SYNTHESIZE(bool, m_bActionEnable, ActionEnable);
 
};

class CPtBattleArrayItem : public CPtTableItem
{
public:
    CPtBattleArrayItem(){m_pDelegateLayer = NULL;m_bSellPanel = false,m_bClickManifier =false;};
    CREATE_FUNC(CPtBattleArrayItem);
    virtual bool init(){ return CPtTableItem::init();};
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void setDelegateLayer(CBattleArrayLayer * inLayer){ m_pDelegateLayer = inLayer;};
    
    
    bool onEnhanceBegin(CCTouch *pTouch, CCEvent *pEvent);
    void onEnhanceEnd(CCTouch *pTouch, CCEvent *pEvent);
    
    bool onSellBegin(CCTouch * pTouch, CCEvent *pEvent);
    void onSellEnd(CCTouch *pTouch, CCEvent *pEvent);
    
    bool onEvolutionBegin(CCTouch *pTouch, CCEvent *pEvent);
    void onEvolutionEnd(CCTouch *pTouch, CCEvent *pEvent);
    
    bool onTeamArrayBegin(CCTouch *pTouch, CCEvent *pEvent);
    void onTeamArrayEnd(CCTouch *pTouch, CCEvent *pEvent);
    
    void createInfoLayer();
    void displayManifier(); 
    // handlerror
    void addCardFail();
    void removeCallBack(CCNode *pNode);
    void removeCallBack();
    void removeAction();
   
protected:
    bool isSameCardId(CPtDisPlayCard ** battleArray, const int &cardId,const int replaceIndex=-1);
    bool getCopyCard(bool inEnsumeAble = false, bool inBattleArray= true);
    
protected:
    CBattleArrayLayer * m_pDelegateLayer;
    bool m_bClickManifier;
    CC_SYNTHESIZE(bool, m_bSellPanel, SellPanel);



};

#endif /* defined(___1_cube__CBattleArrayLayer__) */
