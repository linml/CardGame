//
//  PVPRankRewardConfig.h
//  91.cube
//
//  Created by phileas on 14-1-10.
//
//

#ifndef ___1_cube__PVPRankRewardConfig__
#define ___1_cube__PVPRankRewardConfig__
#include "cocos2d.h"
using namespace cocos2d;
typedef struct 
{
    int rewardId;
    int startrank;
    int endrank;
    int kahun;
    int coin;
    
}PVPRankReward;

class PVPRankRewardConfig
{
public:
    static PVPRankRewardConfig *getInstance();
    static void releaseConfigData();
protected:
    static PVPRankRewardConfig *s_pInstance;

public:
    void getPvPRankRewardByRank(int inIndex, int &outKanHun,int &outCoin);
    void getPvPRankRewardById(int inId, int &outKanHun,int &outCoin);
    PVPRankReward *getPvPRankRewardByRank(int inIndex);
    PVPRankReward *getPvPRankRewardById(int inId);
    
protected:
    PVPRankRewardConfig();
    ~PVPRankRewardConfig();
    
    void loadData();
protected:
    int m_nDataExtentCount;
    PVPRankReward* m_pPvPRankRewards;
    
    
};

#endif /* defined(___1_cube__PVPRankRewardConfig__) */
