//
//  CScreenHandBookLayer.h
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#ifndef ___1_cube__CScreenHandBookLayer__
#define ___1_cube__CScreenHandBookLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
using namespace std;
USING_NS_CC;
using namespace extension;
class CFightCard;
class CHandBookLayerCell:public CCLayer
{
public:
    CHandBookLayerCell();
    ~CHandBookLayerCell();
    static CHandBookLayerCell *Create(vector<CFightCard *>CardList,int begin,int end);
    bool initCreate(vector<CFightCard *>CardList,int begin,int end);
    int checkTouchCell(CCTouch *pTouch);
    CCPoint getCardPosition(int nIndex);
};

class CGamePlayer;

class CScreenHandBookLayer :public CCLayer,CCTableViewDataSource,CCTableViewDelegate
{
public:
    CScreenHandBookLayer();
    ~CScreenHandBookLayer();
    CREATE_FUNC(CScreenHandBookLayer);
    bool init();
    void createBackGroud();
    void createTableView();
    void createQuitButton();

protected:
    // implement interface of CCTableViewDataSource & CCTableViewDelegate
    CCSize cellSizeForTable(CCTableView *table);
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx) ;
    unsigned int numberOfCellsInTableView(CCTableView *table) ;
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    
    void initCellItem(CCTableViewCell*cell, unsigned int idx);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    //void onEnter(void);
    void registerWithTouchDispatcher(void);
    void onExit();
    
private:
    void sendGetTuJian();
    void loadCardVector();
    void decodeGetData(CCObject *object);
    vector<CFightCard *>m_vCardList;
    CGamePlayer *m_tempPlayer;
    bool isLoading;
};

#endif /* defined(___1_cube__CScreenHandBookLayer__) */
