//
//  CGameRechargeLayer.h
//  91.cube
//
//  Created by phileas on 13-10-15.
//
//

#ifndef ___1_cube__CGameRechargeLayer__
#define ___1_cube__CGameRechargeLayer__

#include "cocos2d.h"
#include "LayoutLayer.h"
#include "CSceneGameShopLayer.h"


using namespace cocos2d;
using namespace std;
#define RECHARGE_ADD_TAG 2
#define RECHARGE_SUB_TAG 1


class CGameRechargeLayer : public CCLayer
{
public:
    
    static CGameRechargeLayer* create(CStructShopSellItem *inShopItem, int priceType = 1);
public:
    
    CGameRechargeLayer();
    virtual ~CGameRechargeLayer();
    void setHanlder(CCObject *inTarget, SEL_CallFuncO inConfirmSelector, SEL_CallFuncO inCancelSelector);
    
public:
    virtual bool init(CStructShopSellItem *item);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    int getPropId(){return m_nPropId;};
    int getBuyNum(){return m_nCurrentCount;};
    
protected:
    void initCGameRechargeLayer(CStructShopSellItem *item);
    void handlerTouch();
    void updateTexture();
    
    void startLongPress(int inType);
    void stopLongPress();
    void longTouchCallBack(float dt);
    int getMaxCount(CStructShopSellItem* inItem);
protected:
    
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    CCLabelTTF * m_pNumberLabel;
    CCLabelTTF * m_pCash;
    
    float m_fTotalTime;
    
    int m_nTouchTag;
    int m_nMaxCount;
    int m_nCurrentCount;
    int m_nPropId;
    int m_nType;
    int m_nCashPerItem;

    bool m_bLongPress;
    bool m_bCanDrag;
    
    
    CCSprite * m_pBtn[2];
    CCRect m_cTouchSpriteFrameRect[6];
    
    
    
    SEL_CallFuncO m_pConfirmSelector;
    SEL_CallFuncO m_pCancelSelector;
    CCObject *m_pTarget;
    
    
    CC_SYNTHESIZE(int , m_nPriceType, PriceType);
};


#endif /* defined(___1_cube__CGameRechargeLayer__) */
