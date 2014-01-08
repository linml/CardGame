//
//  CPVPStructMissionTaskRewordLayer.h
//  91.cube
//
//  Created by linminglu on 14-1-8.
//
//

#ifndef ___1_cube__CPVPStructMissionTaskRewordLayer__
#define ___1_cube__CPVPStructMissionTaskRewordLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class CCustomerTableView;
class CPVPStructMissionTaskRewordManager;
class CPVPStructMissionTaskReword;
class CPVPStructMissionTaskRewordLayer :public cocos2d::CCLayer,CCTableViewDelegate,CCTableViewDataSource
{
public:
    bool init();
    CPVPStructMissionTaskRewordLayer();
    ~CPVPStructMissionTaskRewordLayer();
    CREATE_FUNC(CPVPStructMissionTaskRewordLayer);
private:
    void createBackGround();
    void createQuitButton();
    void createTableView();
    void createInfo();
public:
    void sendLingqu();
    void sendInitTable();
    void callBackInitTableValue(CCObject *object);
    void callBackLingQuValue(CCObject *object);
protected:
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
    bool checkisCanGetData(CPVPStructMissionTaskReword *pvp);
private:
    CCustomerTableView *m_pCustomTable;
    CPVPStructMissionTaskRewordManager *m_pManager;
    int m_nTaskId;
    CCSize size;
    int m_nVectorIndex;
    int m_nCellIndex;

};
#endif /* defined(___1_cube__CPVPStructMissionTaskRewordLayer__) */
