//
//  CTriggerConfigData.h
//  91.cube
//
//  Created by phileas on 13-9-10.
//
//

#ifndef ___1_cube__CTriggerConfigData__
#define ___1_cube__CTriggerConfigData__

#include "CSingleton.h"
#include "cocos2d.h"

using namespace cocos2d;

class CPtTrigger : public CCObject
{
public:
    static CPtTrigger* create();
public:
    CPtTrigger();
    virtual ~CPtTrigger();
    CC_SYNTHESIZE(int, m_nTriggerId, TriggerId);
    CC_SYNTHESIZE(int, m_nTriggerStep, TriggerStep);
    CC_SYNTHESIZE(int, m_nStoryId, StoryId);
    CC_SYNTHESIZE(int, m_nSpecialEventId, SpecialEventId);
};


class CPtTriggerConfigData : public CCObject
{
public:
    CPtTriggerConfigData();
    virtual ~CPtTriggerConfigData();
    CPtTrigger* getTriggerById(int inId);
    
protected:
    void initialTriggerConfigData();
protected:
    int m_nCurrentTriggerId;
    CPtTrigger *m_pCurrentTrigger;

};

typedef Singleton<CPtTriggerConfigData>  SingleTriggerConfig;
#endif /* defined(___1_cube__CTriggerConfigData__) */
