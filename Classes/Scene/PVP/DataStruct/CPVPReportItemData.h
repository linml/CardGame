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
class CPVPReportItemData :public CCObject
{
public:
    CPVPReportItemData();
    ~CPVPReportItemData();
    int level;
    int zhandouli;
    int time;
    string username;
    int status;
    int userid;
    
};

#endif /* defined(___1_cube__CPVPReportItemData__) */
