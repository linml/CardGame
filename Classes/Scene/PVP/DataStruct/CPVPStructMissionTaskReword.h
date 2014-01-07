//
//  CPVPStructMissionTaskReword.h
//  91.cube
//
//  Created by linminglu on 14-1-7.
//
//

#ifndef ___1_cube__CPVPStructMissionTaskReword__
#define ___1_cube__CPVPStructMissionTaskReword__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CPVPStructMissionTaskReword :public cocos2d::CCObject
{
public:
    CPVPStructMissionTaskReword();
    ~CPVPStructMissionTaskReword();
    CC_SYNTHESIZE(int , m_nMissionTaskID, MissionTaskID);
    CC_SYNTHESIZE(int , m_nMissionTaskType, MissionTaskType);
    CC_SYNTHESIZE(int , m_nMissionTaskNum, MissionTaskNum);
    CC_SYNTHESIZE(int , m_nMissionTaskSoul, MissionTaskSoul);
    CC_SYNTHESIZE(int , m_nMissionTaskCoin, MissionTaskCoin);
    CC_SYNTHESIZE(int , m_nMissionTaskCash, MissionTaskCash);
    CC_SYNTHESIZE(int , m_nMissionTaskIDItem, MissionTaskItem);
    CC_SYNTHESIZE(int , m_nMissionTaskIDCardId, MissionTaskCardId);
    CC_SYNTHESIZE(bool, m_bMissionTaskLingQu, MissionTaskLingQu);
    CC_SYNTHESIZE(bool, m_bMissionTaskKeYiLingQu, MissionTaskKeYiLingQu);
};

class CPVPStructMissionTaskRewordManager :public cocos2d::CCObject
{
public:
    CPVPStructMissionTaskRewordManager();
    ~CPVPStructMissionTaskRewordManager();
public:
    vector<CPVPStructMissionTaskReword *>m_vtaskRewordManager;
private:
    void initLoadConfigFile();
    
    
};




#endif /* defined(___1_cube__CPVPStructMissionTaskReword__) */
