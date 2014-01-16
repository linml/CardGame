//
//  CPVPMonsterPlayerLayer.h
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#ifndef ___1_cube__CPVPMonsterPlayerLayer__
#define ___1_cube__CPVPMonsterPlayerLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class CPVPAddTiaoZhanCountLayer;
class CPVPMonsterData;
class CPVPMonsterPlayerLayer :public CCLayer,CCTableViewDelegate,CCTableViewDataSource
{
public:
    CPVPMonsterPlayerLayer();
    ~CPVPMonsterPlayerLayer();
    static CPVPMonsterPlayerLayer *CreateByUserID(CPVPMonsterData *pvp,bool isSearCh,int fuchouID=0);
    void setCustomerTouchProty(int nCukongdengji);
protected:
    
    bool initCreateByUserId(CPVPMonsterData *pvp,bool isSearCh,int fuchouID);
    void createTableView();
    void createBackGround();
    void createBackButton();
    void createZhanButton();
    void createSearchButton();
    void createCishuLayer();
    void createUserInfo();
    void createUserName();
    void getUserIdTeam(int UserId);
    void getRandTeam();
    void callBackGetRandTeam(CCObject *object);
    void callBackGetUserId(CCObject *object);
    void schudelUpdateTime(float t);
    void updateLayerData();
    void handleDealWithTag(int tag);
    void startFighting();
    void sendloadRival();
    void sendSetTeam();
    void callBackSetTeam(CCObject *object);
    void updateSchudelCC(float t);
    void createProtectTime();
    void updateDaoJishi(float t);
    void okDialogButton(CCObject *object);
    void cancelDialogButton(CCObject *object);
protected:
    void registerWithTouchDispatcher();
    void onExit();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    CCSize cellSizeForTable(CCTableView *table) ;
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx) ;
    unsigned int numberOfCellsInTableView(CCTableView *table) ;
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    void initCellItem(CCTableViewCell*cell, unsigned int idx);

private:
    int UserID;
    int nFuchouID;
    bool isSearch;
    CC_SYNTHESIZE(CPVPMonsterData *, m_pMonsterPvp, MonsterPvp);
    CCTableView *m_pCustomTable;
    CCSize size;
    CCArray *m_touchArray;
    bool bMeiYouDuiShou;
    CPVPAddTiaoZhanCountLayer *m_pAddTiaoZhanCountPanel ;
};

#endif /* defined(___1_cube__CPVPMonsterPlayerLayer__) */
