//
//  CPVPReportLayer.h
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#ifndef ___1_cube__CPVPReportLayer__
#define ___1_cube__CPVPReportLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class CCustomerTableView;
class CPVPReportItemData ;

class CPVPReportLayer :public CCLayer,CCTableViewDelegate,CCTableViewDataSource
{
public:
    CPVPReportLayer();
    ~CPVPReportLayer();
public:
    bool init();
    CREATE_FUNC(CPVPReportLayer);
private:
    void createBackGround();
    void createQuitButton();
    void createTableView();
    void createInfo();
public:
    void sendInitTable();
    void sendgetNextPage();
    void callBackNextPage(CCObject *object);
    void callBackInitTableValue(CCObject *object);
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
    bool checkisCanGetData(CPVPReportItemData *pvp);
private:
    CCustomerTableView *m_pCustomTable;
    int m_nTaskId;
    CCSize size;
    int m_nVectorIndex;
    int m_nCellIndex;
    vector<CPVPReportItemData *>m_vPvpReport;
};

#endif /* defined(___1_cube__CPVPReportLayer__) */
