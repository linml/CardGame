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
#include "Biforest.h"

#include "CTriggerConfigData.h"
#include "CPtButtonWidget.h"
#include "CPtSectionConfigData.h"
#include "gameStruct.h"
#include "gamePlayer.h"
#include "CEventConfigData.h"
#include "CEventBoxConfigData.h"
#include "CEventBoxReward.h"
#include "CEventBoxDialog.h"
#include "CEventConfigData.h"


using namespace cocos2d;

#define LEFT_TOUCH_TAG (3001)
#define CENTER_TOUCH_TAG (LEFT_TOUCH_TAG+1)
#define RIGHT_TOUCH_TAG (CENTER_TOUCH_TAG+1)


class CExploration : public CCLayer
{
    

public:
    static CCScene *scene();
    CREATE_FUNC(CExploration);
    
    static void resetStaticInfo();
    static void setSectionData(int inCurrentStep, const EVENTDATA &inEventData, CPtSection * inSectionData = NULL);
    static void setExplorationInfo(CPtSection * inSectionData);
    static void setEvents(const EVENTDATA& inEventData);
    static void addCurrentStep();
    static int getCurrentStep(); //步
    static void setCurrentStep( const int & inCurrentStep);
    static const EVENTDATA &getEventData();
    static CPtSection *getExplorationSectionInfo();
    static SECTION_DATA s_SectionData ;
public:
    static void setCurrentEvnetId(int inEventId);
    static int  getCurrentEventId(); //事件ID
    static int getCurrentChapterId(); //章ID
    static int getCurrentSectionId(); //节ID
    static int getCurrentTaskId(); //任务ID
    static void setNextEventByDict(CCDictionary *inEventInfo);
    static void setRewardsByDict(CCDictionary *inReward);
    
protected:
    static int s_CurrentEventId;
    
 
public:
    CExploration();
    virtual ~CExploration();
public:
    virtual bool init();
    virtual void onEnter();
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
    void updateBtn();
    void attachConfirm();
    void createOrUpdatePlayerData();

    void updateUI();
    
    void backHall();
    
    void onReceiveTaskInfo(CCObject *pObject);
    void hanlderLoadTaskInfo();
    
    void callBack(float dt);
    void requestCallBack(float dt);
    void goSection(float dt);
    
    // reword:
    void addForwordReword();
    
    void getTriggers();
    
    void dispatchEventByEventId(const int &inEventId);
    
    void updateEventData();
    
    
    // handler event:
    void handlerEmptyEvent();
    void handlerFightEvent(int inType);
    void hanlderEventBox(int eventBoxId);
    
    void createEventBoxDialogByType(int inEventBoxId,int inType);
    void createEventBoxDialogByType(CEventBoxData *inEventBoxData, int inType);
    
    void setVisiable();
    void setInsiable();
    
    void getBiforest();
protected:
    bool m_bCanTouch;
    bool m_bLoadTaskInfo;
    
    int m_nWhichEvent;
    int m_nTouchTag;
    
    int m_nEventType;
    CPtButtonWidget* m_pBtn[3];
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    CCSprite * m_pTouchSprite;
    CCLabelTTF * m_pProgress;
    CCArray *m_pTriggers;
    
    CGamePlayer *m_pPlayer;
    
    CEventData *m_aEvents[3];
    CEventDataConfig * m_pEventData;
    CEventBoxConfigData *m_pEventBoxData;
    
    int m_nEventBoxSelectType; // 0---> cancle 1---> confirm
    
// connect server:
    
protected:
    // api.php?m=Part&a=recordPart&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&chapter_id=1(章)&part_id=1(节)&item_id=1(道具ID)
    void onSaveSectionProgress();
    void onReceiveSaveMsg(CCObject *pObject);
    void onParseSaveMsgByDictionary(CCDictionary *pResultDict);
    void dispatchParaseEvent(CCDictionary *pEventInfo, int inType);
    
    //模块 ：api.php?m=Part&a=commonEvent&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&chapter_id=1(章)&part_id=1(节)&step=1(小节中第几步)
    void onSendEventRequest();
    void onReceiveEventRequetMsg(CCObject *pObject);
    void onParseEventRequestMsg(CCDictionary *pResultDict);
    
    void onFishEventRequest(int inType = 1);
    void onReceiveFishEventRequestMsg(CCObject * pObject);
    void onParseFishEventRequestMsg(CCDictionary * pResultDict);
    void dispatchParaseFinishEvent(CCDictionary *pResult, int inType);
    
    
    void onCancleCallback(CCObject *pObject);
    void onCanfirmCallback(CCObject *pObject);

    void onOpenCallBack(CCObject* pObject);
};


#endif /* defined(___1_cube__ExplorationScene__) */
