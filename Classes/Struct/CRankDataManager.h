//
//  CRankDataManager.h
//  91.cube
//
//  Created by phileas on 13-12-3.
//
//

#ifndef ___1_cube__CRankDataManager__
#define ___1_cube__CRankDataManager__

#include <vector>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
typedef struct
{
    std::string name;
    int value;
    short rank;
}CRankData;

class CRankDataManager
{
public:
    static CRankDataManager *getInstance();
    static void releaseDataManager();
protected:
    static CRankDataManager *s_pIntance;
    
protected:
    CRankDataManager();
    virtual ~CRankDataManager();

    void initData();
    void relaseData();
protected:
    vector<CRankData *>* m_pFightRanks;
    vector<CRankData *>* m_pLevelRanks;
    vector<CRankData *>* m_pRechargeRanks;

};

#endif /* defined(___1_cube__CRankDataManager__) */
