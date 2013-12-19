//
//  CSceneGameShopLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#ifndef ___1_cube__CSceneGameShopLayer__
#define ___1_cube__CSceneGameShopLayer__

#include <iostream>
#include "cocos2d.h"
#include "CCScrollView.h"
#include "gamePlayer.h"
using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

class CSceneGameShopLayer : public cocos2d::CCLayer
{
    typedef  cocos2d::CCLayer Super;
public:
    CSceneGameShopLayer();
    ~CSceneGameShopLayer();
    bool  init();
    CREATE_FUNC(CSceneGameShopLayer);
protected:
    void createQuNiuDanButton();
    void createQuChongZhi();
    void createLoadLayer();
    void createShopName();
    void tishiGoumaiChenggong();
    void updateQianBi();
    void schudelUpdateQianbi(float t);
    void initLayerData();
    void createShopTitle();
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();
protected:
    void swithChangeTab(int changeTab);
    void createShopItemLayer(CStructShopInfo *info);
    int getCheckTouchTab(CCPoint point);
private:
    CCNode *m_tempTouchNode;
    CCLayer *m_currentShopLayer;
    int m_currentTabIndex;
    vector<CStructShopInfo *>m_vSaveShop;
    CGamePlayer *m_pPlayer; // change by phileas
    CCArray *m_tabArray;
};

#endif /* defined(___1_cube__CSceneGameShopLayer__) */
