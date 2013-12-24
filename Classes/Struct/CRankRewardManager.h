//
//  CRankRewardManager.h
//  91.cube
//
//  Created by phileas on 13-12-20.
//
//

#ifndef ___1_cube__CRankRewardManager__
#define ___1_cube__CRankRewardManager__

#include <string>
#include "cocos2d.h"
#include "CPtStarConfigData.h"

using std::string;
using namespace cocos2d;

#define LEVELRANKREWARD   1
#define FIGHTRANKREWARD  2
#define RECHARGRANKREWARD 3
#define LEVELCOUNT 5



struct RankReward
{
    // [startIndex,endIndex]
    int startIndex;
    int endIndex;
    int exp;
    int coin;
    PropItem item;
    void setData(int inStartIndex, int inEndIndex, int inExp, int inCoin, int inItemId, int inItemCount)
    {
        startIndex = inStartIndex;
        endIndex = inEndIndex;
        exp = inExp;
        coin = inCoin;
        item.propId = inItemId;
        item.propCount = inItemCount;
    }
    
};
class CRankRewardManager
{
public:
    static CRankRewardManager* getInstance();
    static void releaseDataManager();
    
protected:
    static CRankRewardManager *s_pInstance;
    
public:
    int getMedalKind(int inType, int inRank);
    /*
     * @notice:
     *   1.if CRandRewarkManager's instance is released ,you can't use the return object
     *   2.you need relase the return object by yourself.
     */
    const RankReward* getRankRewardByRank(int inType, int inRank);
    const RankReward *getRankRewardByIndex(int inType, int inIndex);
protected:
    CRankRewardManager();
    ~CRankRewardManager();
    void parseDataFromFile();
    void getWhichRank(int inType, RankReward *&outPointer);
    
protected:
    RankReward m_aFRankReward[LEVELCOUNT]; // fight point
    RankReward m_aLRankReward[LEVELCOUNT]; // level
    RankReward m_aRRankReward[LEVELCOUNT]; // recharge
};

#endif /* defined(___1_cube__CRankRewardManager__) */
