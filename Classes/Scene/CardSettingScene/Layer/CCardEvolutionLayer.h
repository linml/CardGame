//
//  CCardEvolutionLayer.h
//  91.cube
//
//  Created by phileas on 13-8-1.
//
//

#ifndef ___1_cube__CCardEvolutionLayer__
#define ___1_cube__CCardEvolutionLayer__

#include "cocos2d.h"
#include "CPtBatterArray.h"
#include "CPtListViewWidget.h"
#include "CPtStarConfigData.h"
using namespace cocos2d;

class CCardEvolutionLayer : public CCLayer
{
public:
    CREATE_FUNC(CCardEvolutionLayer);
    
public:
    CCardEvolutionLayer();
    virtual ~CCardEvolutionLayer();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
     void addCard(CPtDisPlayCard *inCard);
protected:
    void initEvolution();
    void handlerTouch(CCTouch * pTouch);
  
   
    void removeCard();
    
    void updateEvolution();
    void updateData();
    void updateTexture();
    void save();
    void saveOnClick();
    void receiveCallBack(CCObject *pSender);
    
    void clearPanel();
protected:

    LayoutLayer *m_cMaps;
    CGamePlayer *m_pPlayer;
    CCSprite *m_pSaveButton;
    CPtDisPlayCard *m_pSrcCard;
    CPtDisPlayCard *m_pDesCard;
    CPtStarConfigData *m_pStarData;
    CCLayer * m_pMapLayer[2];
    CCPoint m_aPoint[2];
    
    CCLabelTTF *label[5];
    CC_SYNTHESIZE(CPtListViewWidget*, m_pCardBag, CardBag);
    CFightCard * m_pFightCard;
    
    int m_nTouchTag;
    int m_nCostConin;
    int m_nAddAtk;
    int m_nAddRvc;
    int m_nAddDef;
    int m_nAddHp;
    
};


#endif /* defined(___1_cube__CCardEvolutionLayer__) */
