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

#define INTERVALTIME 150 // 5分钟

#define FIGHTRANK 0
#define LEVELRANK 1
#define RECHARGERANK 2

class CRankData;
class CRankData
{
public:
    CRankData(const char *inName,int inUid,int inValue):name(inName),value(inValue), uid(inUid){};
    CRankData(){};
    void setData(const char *inName,int inUid,int inValue){name =inName,value=inValue, uid =inUid;};
    std::string name;
    int value;
    int uid;
    short rank;
    
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
protected:
    CRankDataManager();
    virtual ~CRankDataManager();

    void initData();
    void allocalVector(vector<CRankData *>* &data);
    void releaseData(bool inDeleteContainer = true);
    void releaseRankData(int inTab);
    
    void callBackFunc();
    void paraRankData(int inType,CCArray *pArray);
    // connect server:
    
    void onSendRequestGetFightRank();
    void onSendRequestGetLevelRank();
    void onSendRequestGetRechargeRank();
    
    void onReceiveDataFightRank(CCObject *pObject);
    void onReceiveDataLevelRank(CCObject *pObject);
    void onReceiveDataRechageRank(CCObject *pObject);
    
protected:
    time_t m_tstartTime;
    
    bool m_bFightReady;
    bool m_bLevelReady;
    bool m_bRechargeReady;
    
    vector<CRankData *>* m_pFightRanks;
    vector<CRankData *>* m_pLevelRanks;
    vector<CRankData *>* m_pRechargeRanks;
    
    SEL_CallFunc m_pDataCallback;
    CCObject* m_pDataCallbackHander;
};

#endif /* defined(___1_cube__CRankDataManager__) */
