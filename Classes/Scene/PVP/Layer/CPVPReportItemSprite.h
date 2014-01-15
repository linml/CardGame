//
//  CPVPReportItemSprite.h
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#ifndef ___1_cube__CPVPReportItemSprite__
#define ___1_cube__CPVPReportItemSprite__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CPVPReportItemData;
//PVP任务奖励 单独的一个item
class CPVPReportItemSprite :public cocos2d::CCNode
{
public:
    CPVPReportItemSprite();
    ~CPVPReportItemSprite();
    static CPVPReportItemSprite *CreateByPvpData(CPVPReportItemData *pvp);
    bool init(CPVPReportItemData *pvp);
    CC_SYNTHESIZE(CPVPReportItemData * , m_pCPVPReportItemData, PVPReportItemData);
private:
    void createBackgroud();
    void createItemIcon();
    void createTitle();
    void createContext();
    void createShiJian();
    void createName();
    void updateSetFuChou();
};

#endif /* defined(___1_cube__CPVPReportItemSprite__) */
