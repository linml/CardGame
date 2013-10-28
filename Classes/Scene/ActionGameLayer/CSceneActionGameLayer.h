//
//  CSceneActionGameLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#ifndef ___1_cube__CSceneActionGameLayer__
#define ___1_cube__CSceneActionGameLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class CStructGameActionData ;


class CSceneActionGameLayer :public CCLayer,CCTableViewDataSource,CCTableViewDelegate
{
public:
    CSceneActionGameLayer();
    ~CSceneActionGameLayer();
    bool init();
    CREATE_FUNC(CSceneActionGameLayer);
protected:
    void createBackGroud();
    void createLoadingLayer();
    void removeLoadingLayer();
    void sendHttpToGetAction(float t);
    void decodeHttpToGetAction(CCObject *object);
protected:
    CCSize cellSizeForTable(CCTableView *table) ;
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx) ;
    unsigned int numberOfCellsInTableView(CCTableView *table) ;
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    void initCellItem(CCTableViewCell*cell, unsigned int idx);
protected:
    void createTableView();
    void removeTableView();
    void reloadTableView();
protected:
    vector<CStructGameActionData *>m_vActionList;
   
    
};


#endif /* defined(___1_cube__CSceneActionGameLayer__) */
