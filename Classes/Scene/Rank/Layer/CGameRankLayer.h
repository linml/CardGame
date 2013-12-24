//
//  CGameRankLayer.h
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#ifndef ___1_cube__CGameRankLayer__
#define ___1_cube__CGameRankLayer__

#include <iostream>
#include  "cocos2d.h"
#include "CUtilityTableView.h"
#include "CRankDataManager.h"

using namespace std;
using namespace cocos2d;

//其实这个界面跟好友界面是差不多的。。 这里不写派生的原因是怕未来的UI会更改变


class CGameRankLayer :public cocos2d::CCLayer,CCTableViewDataSource, public CCTableViewDelegate ,public CUtilityTableViewDelegate
{
public:
    CGameRankLayer();
    ~CGameRankLayer();
    void callBackFunc(void);//让对方去回调的数据;
    bool init();
    CREATE_FUNC(CGameRankLayer);
private:
    void createBackGround();
    void createRankTypeButton();
    void createQuitButton();
    void setGunDongTiaoPtr();
    
    void createPlayerRankInfo();
    void updatePlayerRankInfo();
private:
    bool switchToTab(int index);
    void onClickZongZhandouTabItem();
    void onClickDengJiTabItem();
    void onClickFuhaoTabItem();
private:
    void createTableView(int index);
    void removeTableView(int index);
    CCPoint getTabPositionByIndex(int index);
    bool addTab(const char* label,int index);
private:
    void reloadTableViewData(int index);
    void scrollViewBar(CCTableView* table);
protected: //tableview 
    void tableScrolBarView(CCTableView *tableVeiw);
    void tablecellTouchNode(CCTableViewCell *cell,CCTouch *pTouch);
    
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    
    unsigned int numberOfCellsInTableView(CCTableView *table);
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    CCSize cellSizeForTable(CCTableView *table);
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    //CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    void initCellItem(CCTableViewCell*cell, unsigned int idx);

protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher(void);
    void onExit();
    void handleTagCallBack(int tagValue);
    int checkTouchTableIndex(CCPoint point);
    int checkTouchQuitButton(CCPoint point);
    void updateRankCount();
    
    //领取排名奖励
    void createRankReward();
    void updateRankReward();
    
    void onClickGetRankReward();
    void onSendGetRankReward();
    void onReceiveGetRankRewardMsg(CCObject *pObject);
    
private:
    int m_currentTabIndex;
    vector<CRankData *> *m_vTableViewData;
    CCSize size;
    CCArray*          m_tabs;
    CCSprite *m_pScrollViewGuanDongTiao;
    float m_fOldScrollBarPosiontYtop;
    float m_foldscrollBarPosiontylow;
    CCSize m_pTableCellSize;
    CCSprite *tempSprite;
    CRankDataManager * m_pRankManager;
    
    CRankRewardData *m_pRankRewardData;
    int m_nPlayerUid;
    int m_nRankCount;
    CCLabelTTF * m_pPlayerRankInfo;
    
    CCLabelTTF * m_pPlayerRankRewardInfo;
    CCSprite * m_pGetRankRewardBtn;
    
};

#endif /* defined(___1_cube__CGameRankLayer__) */
