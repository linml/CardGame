//
//  CCardSellLayer.h
//  91.cube
//
//  Created by phileas on 13-7-31.
//
//

#ifndef ___1_cube__CCardSellLayer__
#define ___1_cube__CCardSellLayer__

#include "cocos2d.h"
#include "CPtListViewWidget.h"
#include "CPtBatterArray.h"

class CCardSellLayer : public CCLayer
{
public:
    CREATE_FUNC(CCardSellLayer);
    
public:
    CCardSellLayer();
    virtual ~CCardSellLayer();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    
public:
    void resetData();
    void addCoin(int conin);
    void subCoin(int conin);
    void clearCoin();
protected:
    void initSelll();
    void handlerTouch(CCTouch * pTouch);
    
    // action:
    void addCardAction(const int& inIndex, CPtDisPlayCard * inCard);
    void removeCardAction(const int& inIndex);
    
    void updateSell();
    void updateData();
    void updateTexture();
    void save();
    void saveOnClick();
    void receiveCallBack(CCObject *pSender);
    
    void restCardFlag();
    void removeCardInCardBag();
    void clearPanel();
protected:
    int m_nTouchTag; 
    int m_nMoney;
    LayoutLayer *m_cMaps;
    CGamePlayer *m_pPlayer;
    CCSprite *m_pSaveButton;
    CC_SYNTHESIZE(CPtListViewWidget*, m_pCardBag, CardBag);
    CC_SYNTHESIZE_READONLY(CPtListViewWidget*, m_pSellPackage, SellPackage);

    int m_nConin;
    CCLabelTTF *m_pConinlabel;
    

};


#endif /* defined(___1_cube__CCardSellLayer__) */
