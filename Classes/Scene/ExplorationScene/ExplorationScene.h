//
//  ExplorationScene.h
//  91.cube
//
//  Created by phileas on 13-6-9.
//
//

#ifndef ___1_cube__ExplorationScene__
#define ___1_cube__ExplorationScene__

#include "cocos2d.h"
#include "LayoutLayer.h"

#include "CTriggerConfigData.h"
#include "CPtButtonWidget.h"
#include "CPtSectionConfigData.h"
#include "gameStruct.h"

using namespace cocos2d;

#define LEFT_TOUCH_TAG (3001)
#define CENTER_TOUCH_TAG (LEFT_TOUCH_TAG+1)
#define RIGHT_TOUCH_TAG (CENTER_TOUCH_TAG+1)

// demo:
//extern  int g_nLevle;
//extern  int g_array[3];
//extern  int g_index;

class CExploration : public CCLayer
{
public:
    static SECTION_DATA s_SectionData ;
    static CCScene *scene();
    CREATE_FUNC(CExploration);
    
public:
    CExploration();
    virtual ~CExploration();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    bool initExploration();
    void initData();
    void initEvent();
    void handlerTouch();
    void handlerEvent();
    void handlerSuccess();
    void handlerTrigger();
  //  void randonArrows(const int inLevle);
    void attachConfirm();

    void updateUI();
    
    void backHall();
    
    void onReceiveTaskInfo(CCObject *pObject);
    void hanlderLoadTaskInfo();
    
    void callBack(float dt);
    
protected:
    bool m_bCanTouch;
    bool m_bLoadTaskInfo;
    
    int m_nWhichEvent;
    int m_nTouchTag;
    CPtButtonWidget* m_pBtn[3];
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    CCSprite * m_pTouchSprite;
    CCLabelTTF * m_pProgress;
    
    CPtTrigger * m_pTrigger;
    
    
    
    
// connect server:
    
protected:
    // api.php?m=Part&a=recordPart&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&chapter_id=1(章)&part_id=1(节)&item_id=1(道具ID)
    void onSaveSectionProgress();
    void onReceiveSaveMsg(CCObject *pObject);
    void onParseSaveMsgByDictionary(CCDictionary *pResultDict);
    
    //模块 ：api.php?m=Part&a=commonEvent&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&chapter_id=1(章)&part_id=1(节)&step=1(小节中第几步)
    void onSendEventRequest();
    void onReceiveEventRequetMsg(CCObject *pObject);
    void onParseEventRequestMsg(CCDictionary *pResultDict);

    
};


#endif /* defined(___1_cube__ExplorationScene__) */
