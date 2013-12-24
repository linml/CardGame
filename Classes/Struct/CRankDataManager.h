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
#include "CRankRewardManager.h"
using namespace cocos2d;
using namespace std;

#define INTERVALTIME 150 // 5分钟

#define FIGHTRANK 0
#define LEVELRANK 1
#define RECHARGERANK 2
#define GETRANKSTATE(inRank) (inRank) >=0 ? 1 : 2

class CRankData
{
public:
    CRankData(const char *inName,int inUid,int inValue,int inRank):name(inName),value(inValue), uid(inUid){ rank = GETRANKSTATE(inRank);};
    CRankData(){};
    void setData(const char *inName,int inUid,int inValue, int inRank){name =inName,value=inValue, uid =inUid,rank = GETRANKSTATE(inRank); };
    std::string getRankChangeIconPath();
    std::string name;
    int value;
    int uid;
    short rank; // rank state: 0(没变化),1(上升),2(下降)
    short stage; //排名段 1-5
};

struct CRankRewardData
{
    const RankReward *rankRewardData = NULL;
    bool hasGet = false;
};

class CRankDataManager : public CCObject
{
public:
    static CRankDataManager *getInstance();
    static void releaseDataManager();
protected:
    static CRankDataManager *s_pIntance;

public:
    void startRankDataManger();
    vector<CRankData*> * sendRequest(int inTab, CCObject *pTarget, SEL_CallFunc pCallabck);
    vector<CRankData*> * getDataByType(int inTab);
    CRankData *getRankDataByUid(int inUid, int inType, int &outRank);
    CRankRewardData *getRankRewardByType(int inType){ return (m_pRankRewards+inType);};
protected:
    CRankDataManager();
    virtual ~CRankDataManager();

    void initData();
    void allocalVector(vector<CRankData *>* &data);
    void releaseData(bool inDeleteContainer = true);
    void releaseRankData(int inTab);
    
    void callBackFunc();
    void parseRankData(int inType,CCArray *pArray);
    void parseRankReward(int inType, CCObject *pObject);
    // connect server:
    
    void onSendRequestGetFightRank();
    void onSendRequestGetLevelRank();
    void onSendRequestGetRechargeRank();
    
    void onReceiveDataFightRank(CCObject *pObject);
    void onReceiveDataLevelRank(CCObject *pObject);
    void onReceiveDataRechageRank(CCObject *pObject);
    
    void resetReward(int inType)
    {
        m_pRankRewards[inType].hasGet =false;
        m_pRankRewards[inType].rankRewardData = NULL;
    }
    
protected:
    time_t m_tstartTime;
    
    bool m_bFightReady;
    bool m_bLevelReady;
    bool m_bRechargeReady;
    
    vector<CRankData *>* m_pFightRanks;
    vector<CRankData *>* m_pLevelRanks;
    vector<CRankData *>* m_pRechargeRanks;
    
    CRankRewardData m_pRankRewards[3];

    
    SEL_CallFunc m_pDataCallback;
    CCObject* m_pDataCallbackHander;
    CRankRewardManager * m_pRewardInstance;
};

#endif /* defined(___1_cube__CRankDataManager__) */
