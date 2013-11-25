//
//  CEventConfigData.h
//  91.cube
//
//  Created by phileas on 13-9-11.
//
//

#ifndef ___1_cube__CEventConfigData__
#define ___1_cube__CEventConfigData__

#include <string>
#include <map>

#include "CSingleton.h"
#include "cocos2d.h"

using std::string;
using std::map;
using namespace cocos2d;

class CEventData : public CCObject
{
public:
    CREATE_FUNC(CEventData);
public:

    CEventData();
    virtual ~CEventData();
    virtual bool init();
    CC_SYNTHESIZE(int , m_nEventId, EventId);
    CC_SYNTHESIZE(string,m_sEventName, EventName);
    CC_SYNTHESIZE(int, m_nEventTipId, EventTipId);
    CC_SYNTHESIZE(int, m_nEventType, EventType);
    CC_SYNTHESIZE(string, m_sEventEffect, EventEffect);
    CC_SYNTHESIZE(string, m_sSoundEffect, SoundEffect);
    CC_SYNTHESIZE(int, m_nBackStep, BackStep);
    CC_SYNTHESIZE(int, m_nCostHP, CostHP);

    CC_SYNTHESIZE(int, m_nParamOne, ParamOne);
    CC_SYNTHESIZE(int, m_nParamTwo, ParamTwo);
    CC_SYNTHESIZE(int, m_nParamThree, ParamThree);
    CC_SYNTHESIZE(int, m_nParamFouth, ParamFouth);
    CC_SYNTHESIZE(int, m_nParamFive, ParamFive);
};



class CEventDataConfig : public CCObject
{
public:
    
    CEventDataConfig();
    virtual ~CEventDataConfig();
    
    CEventData* getEventById(int inId);

protected:
    void loadEventConfigData();
protected:
    map<int, CEventData*> m_cAllEvents;
    
};

typedef Singleton<CEventDataConfig> SingleEventDataConfig;


typedef struct
{
    int eventId = 0;
    int wordId;
    string iconPic;
}SpecialEventButton;

#define SPECIALEVENTBUTTONCOUNT 3
class CSpecialEvent : public CCObject
{
public:
    CREATE_FUNC(CSpecialEvent);
public:
    CSpecialEvent();
    virtual ~CSpecialEvent();
    virtual bool init();
    
    CC_SYNTHESIZE(int, m_nSpecialEventId, SpecialEventId);
    void setLeftEvent(int inPosition, int inEventId, int inWordId, const char * inPic);
protected:
    SpecialEventButton m_vSpecitialButtons[SPECIALEVENTBUTTONCOUNT];
};


class CSpecialEventConfigData: public CCObject
{
    
public:
    CSpecialEventConfigData();
    virtual ~CSpecialEventConfigData();
    
    CSpecialEvent *getSpecialEventById(int inId);
protected:
    int m_nCurrentSpecialEventId;

    CSpecialEvent* m_pCurrentSpecialEvent;
};

#endif /* defined(___1_cube__CEventConfigData__) */
