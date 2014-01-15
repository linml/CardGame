//
//  CPVPMonsterData.h
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#ifndef ___1_cube__CPVPMonsterData__
#define ___1_cube__CPVPMonsterData__

#include <iostream>
#include "cocos2d.h"
#include "CPVPPublicStruct.h"
using namespace std;
using namespace cocos2d;
class CFightCard;
class CPVPMonsterData :public CCObject
{
public:
    CPVPMonsterData();
    ~CPVPMonsterData();
    void clearFightingCardData();
    CC_SYNTHESIZE(PVPRankData *, m_pvpUserData, PVPUserData);
    vector<CFightCard *> m_vCardList;
    int fuchouID;
};

#endif /* defined(___1_cube__CPVPMonsterData__) */
