//
//  CSellerShopDialog.h
//  91.cube
//
//  Created by phileas on 13-10-18.
//
//

#ifndef ___1_cube__CSellerShopDialog__
#define ___1_cube__CSellerShopDialog__

#include "cocos2d.h"
#include "gameStruct.h"
#include "CPtPropConfigData.h"
#include "CGameButtonControl.h"
#include "gamePlayer.h"
#include <string>
using namespace cocos2d;
using std::string;

class CSellerShopDialog : public CCLayerColor
{
public:
    static CSellerShopDialog* create(SELLER_DATA inSellData, int inSellerType, int inShopId, string inSellerName);
public:
    CSellerShopDialog();
    virtual ~CSellerShopDialog();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    bool initCSellerShopDialog(SELLER_DATA inSellData, int inSellerType,  int inShopId, string inSellerName);
    void setCloseHandler(CCObject *inTarget, SEL_CallFuncO inSelector);
    
protected:
    void handlerTouch(CCTouch *pTouch);
    
    void initCSellerShopUI(const char *inSellerName=NULL);
    CCNode *createShopItem(const SELLER_SHOP &shopItem, int inIndex);
    int getPalyerMoney(); // 获取玩家的金币或现金币－－－ 通过 m_sSellData 的priceType
    void onClickBuyBtn(int inIndex);
    void onClickExtract(int inIndex);
    void onClickClose();
    void loadResouce();
    
    void onSendBuyRequest(CCObject *pObject);
    void onReceiveBuyRequest(CCObject *pObject);
    void onHandlerBuySuccess(CCDictionary *inResultDict);
    void onHandlerBuyProp(CCDictionary *inResultDict);
    void onHandlerExtractCard(CCDictionary *inResultDict);
    
    void updateLimitNum(CCLabelTTF * inLimitLabel, int inLimitNumber);
    void updateTotalNum();
protected:
    int m_nSellerType;
    int m_nTouchTag;
    int m_nShopId;
    int m_nSelectPropId;
    int m_nLimitCount;
    
    SELLER_DATA m_sSellData;
    CPtPropConfigData *m_pPropData;
    CGameButtonControl *m_pBuyBtn[3];
    CCLabelTTF * m_pLimitLabel[3];
    CCSprite *m_pCloseBtn;
    CCSpriteFrame *m_pCurrency; // 币种
    CCLabelTTF *m_pTotalMoney;
    CGamePlayer *m_pPlayer;
    
    CCObject *m_pCloseHandler;
    SEL_CallFuncO m_pCloseSelector;

};


#endif /* defined(___1_cube__CSellerShopDialog__) */
