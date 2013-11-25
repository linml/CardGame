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

/*
 * @breif 活动的状态
 */
enum EN_CSceneActionGameLayerStatus
{
    EN_CSceneActionGameLayerStatus_NONE  = 0, //无效状态
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
    
    //从服务端获活动信息之前的loading界面
    void createLoadingLayer();
    void removeLoadingLayer();
    // 从服务端获取活动的信息
    void sendHttpToGetAction(float t);
    void decodeHttpToGetAction(CCObject *object);
    
    void createShowNoHaveActivity();
    void removeShowNoHaveActivity();
    
    CCScrollView *getScrollView();
    void adjustScrollView(float t);
protected:
    // implement interface of CCTableViewDataSource & CCTableViewDelegate
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
    
    vector<CStructGameActionData *>m_vActionList; // 存储当前活动的信息
    EN_CSceneActionGameLayerStatus m_enStatus;
    CCSize size;
    bool m_bTouchScrollView;
    bool m_bTouchEventEnable;
    CCTouch *m_pTouchEnd;
    CStructGameActionData *m_pBackData;
    CCPoint m_touchPoint,m_touchOffset ;
    float m_fOffsetX;
    
};


#endif /* defined(___1_cube__CSceneActionGameLayer__) */
