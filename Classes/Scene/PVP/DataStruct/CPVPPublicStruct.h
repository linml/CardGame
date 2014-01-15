//
//  CPVPPublicStruct.h
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#ifndef _1_cube_CPVPPublicStruct_h
#define _1_cube_CPVPPublicStruct_h

typedef enum
{
    RuleBtn = 0,// 规则
    BattleLogBtn=1, // 战报
    ChallengeRewardBtn=2, //挑战奖励
    HuDianBtn=3, //魂殿
    SearchOpponentBtn=4, // 搜索对手
    AddPVPCountBtn=5, //增加挑战次数
    PVPRankRewardBtn=6, //领取排名奖励
    BackBtn=7,
    PVPRankBtn=8 //排行榜
}PVPSCENEBTNTYPE;

struct PVPRankData
{
    PVPRankData(){uid=0;fightpointer=0;credits=0;level=0;rank=0;name.clear();}
    int uid;
    int fightpointer;
    int credits;
    int level;
    int rank;
    std::string name;
} ;

typedef struct
{
    int rank;
    int coin;
    int kahun;
    
}PVPRankReward;

#endif
