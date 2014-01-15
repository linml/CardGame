//
//  CPVPReportItemData.h
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#ifndef ___1_cube__CPVPReportItemData__
#define ___1_cube__CPVPReportItemData__

#include <iostream>
#include <string>
#include "CPVPPublicStruct.h"
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CFightCard;
class CPVPReportItemData :public CCObject
{
public:
    CPVPReportItemData();
    ~CPVPReportItemData();
    int time;
    int status;
    int fuchouID;
    PVPRankData m_pvpRankData;
    vector<CFightCard *>m_vCardList;
    
};

#endif /* defined(___1_cube__CPVPReportItemData__) */
