//
//  CSceneFriendHaoyouliebiao.h
//  91.cube
//
//  Created by linminglu on 11/15/13.
//
//

#ifndef ___1_cube__CSceneFriendHaoyouliebiao__
#define ___1_cube__CSceneFriendHaoyouliebiao__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
using namespace std;
using namespace cocos2d;
using namespace extension;

class CFriend;
class CGamePlayer;

class CSceneFriendHaoyouliebiao :public CCLayer,public cocos2d::extension::CCTableViewDataSource,public cocos2d::extension::CCTableViewDelegate

{
public:
    CSceneFriendHaoyouliebiao();
    ~CSceneFriendHaoyouliebiao();
    bool init();
    CREATE_FUNC(CSceneFriendHaoyouliebiao);
public:
    void selectButton();
    void allZan();
public:
    void createTableView();
    void removeTableView();
    void sendPostGetFriend();
    void decodeGetFriend(CCObject *object);
    void cleanFriendVector();
    void sendPostSendZanMei(int Uid);
    void decodeDataGetFriendLy(CCObject *object);
protected:
    CCSize cellSizeForTable(CCTableView *table) ;
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx) ;
    unsigned int numberOfCellsInTableView(CCTableView *table) ;
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    void  initCellItem(CCTableViewCell*cell, unsigned int idx);
protected:
    vector<CFriend *> *m_vNewFriendList;
    CGamePlayer *m_tempGamePlayer;
    int m_nUpdateValue;
    bool isSendServerSingleFriend;

};

#endif /* defined(___1_cube__CSceneFriendHaoyouliebiao__) */
