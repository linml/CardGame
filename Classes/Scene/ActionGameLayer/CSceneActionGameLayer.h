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
enum EN_CSceneActionGameLayerStatus {
    EN_CSceneActionGameLayerStatus_NONE  = 0,
    EN_CSceneActionGameLayerStatus_RUNNING = 1,
    EN_CSceneActionGameLayerStatus_END
    };

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
    void createShowNoHaveActivity();
    void removeShowNoHaveActivity();
    CCScrollView *getScrollView();
    void adjustScrollView(float t);
protected:
    CCSize cellSizeForTable(CCTableView *table) ;
    CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx) ;
    unsigned int numberOfCellsInTableView(CCTableView *table) ;
    void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    void scrollViewDidScroll(CCScrollView* view);
    void scrollViewDidZoom(CCScrollView* view);
    void initCellItem(CCTableViewCell*cell, unsigned int idx);
protected:
    void onEnter();
    void onExit();
    // default implements are used to call script callback if exist
     bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
     void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
     void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
     void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void createTableView();
    void removeTableView();
    void reloadTableView();
    void createRightLayer(CStructGameActionData *data);
protected:
    vector<CStructGameActionData *>m_vActionList;
    EN_CSceneActionGameLayerStatus m_enStatus;
    CCSize size;
    CCTouch *m_pTouchEnd;
    CStructGameActionData *m_pBackData;
     CCPoint m_touchPoint,m_touchOffset ;
    
};


#endif /* defined(___1_cube__CSceneActionGameLayer__) */
