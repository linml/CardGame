//
//  CPVPStructMissionTaskRewordSprite.h
//  91.cube
//
//  Created by linminglu on 14-1-8.
//
//

#ifndef ___1_cube__CPVPStructMissionTaskRewordSprite__
#define ___1_cube__CPVPStructMissionTaskRewordSprite__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
class CPVPStructMissionTaskReword;
//PVP任务奖励 单独的一个item
class CPVPStructMissionTaskRewordSprite :public cocos2d::CCNode
{
public:
    CPVPStructMissionTaskRewordSprite();
    ~CPVPStructMissionTaskRewordSprite();
    static CPVPStructMissionTaskRewordSprite *CreateByPvpData(CPVPStructMissionTaskReword *pvp);
    bool init(CPVPStructMissionTaskReword *pvp);
    CC_SYNTHESIZE(CPVPStructMissionTaskReword * , m_pCPVPStructMissionTaskReword, PVPStructMissionTaskReword);
private:
    void createBackgroud();
    void createItemIcon();
    void createTitle();
    void createContext();
    void createJindu();
    void createFlag();
};

#endif /* defined(___1_cube__CPVPStructMissionTaskRewordSprite__) */
