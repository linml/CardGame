//
//  PVPDataManager.h
//  91.cube
//
//  Created by phileas on 14-1-10.
//
//

#ifndef ___1_cube__PVPDataManager__
#define ___1_cube__PVPDataManager__
#include "cocos2d.h"

#include <vector>
#include <string>
#include "PVPRankRewardConfig.h"

#include "CPVPPublicStruct.h"
using namespace cocos2d;
#define HEADCOUNT 3
#define NOINRANKTAG 3001
#define INITJIFEN 500




class PVPDataManager : public CCObject
{
public:
    static PVPDataManager*getInstance();
    static void releasManager();
protected:
    static PVPDataManager* s_pIntance;
    static bool s_bIsHandlerNetwork;
public:
    void releasAllRankInfo();
    /*
     * @breif:
     * @param inHandler is retain align
     */
    void getPVPMainUIInfo(CCObject *inHandler, SEL_CallFunc inCallback)
    {
        CC_SAFE_RELEASE_NULL(m_pHandler);
        m_pHandler = inHandler;
        m_pCallback = inCallback;
        CC_SAFE_RETAIN(m_pHandler);
        getPVPMainUIInfoFromServer();
    }
    
    void getPVPRankInfo(CCObject *inHandler, SEL_CallFunc inCallback)
    {
        CC_SAFE_RELEASE_NULL(m_pHandler);
        m_pHandler = inHandler;
        m_pCallback = inCallback;
        CC_SAFE_RETAIN(m_pHandler);
        getPVPRankInfoFromServer();
    }

    PVPRankData *getHeadRankInfo(int &outCount)
    {
        outCount = m_nHeadRankCount;
        return m_aHeadRank;
    }
    
    PVPRankData *getUserRankInfo()
    {
        return &m_cPVPUserInfo;
    }
    
    PVPRankData *getAllRankInfo(int &outCount)
    {
        outCount = m_nAllRankCount;
        return m_pAllRankInfoes;
    }
    
    PVPRankData *getPVPUserRankInfo()
    {
        return  &m_cPVPUserRankInfo;
    }
    
    void subOverTime()
    {
        m_nOverTime--;
    }
    bool getRankReward(PVPRankReward *& outRanReward);
    
   
    
protected:
    PVPDataManager();
    virtual~PVPDataManager();
    
    //get pvp main ui info:
    void getPVPMainUIInfoFromServer();
    void parsePVPMainUIInfo(CCObject *pObject);
    
    //get pvp rank info:
    void getPVPRankInfoFromServer();
    void parsePVPRankInfo(CCObject *pObject);
    
    void parsePVPHeadRank(CCArray *inArray);
    void parsePVPAllRank(CCArray *inArray);
    
    //parse and get pvp rank info:
    /*
     * @breif 获取pvp排行榜排名信息
     *  1.主界面上获取前三名的信息
     *  2.全部排行榜上的排名信息
     * @param : outDataContainer:
     */
    void parsePVPRankInfo(CCArray* inArrayData, int& outCountInContainer, PVPRankData *outDataContainer);
    
    void parsePVPRankReward(CCArray *inArray);
    void parsePVPUserInfo(CCDictionary *inUserDict); //主界面种玩家的信息(实时的)
    void parsePVPUserRankInfo(CCDictionary *inPVPInfoDict); // 排行榜种玩家的信息（定时刷新的）
    
 
    void callBack();
protected:
    CCObject *m_pHandler;
    SEL_CallFunc m_pCallback;
    PVPRankData m_aHeadRank[HEADCOUNT];
    PVPRankData *m_pAllRankInfoes;
    PVPRankData m_cPVPUserInfo;
    PVPRankData m_cPVPUserRankInfo;
    std::vector<int> m_vRankReward; //
    int m_nHeadRankCount;
    int m_nAllRankCount;
    //int m_nOverTime; // -1表示未开始
    CC_SYNTHESIZE(int, m_nOverTime, OverTime);

};

#endif /* defined(___1_cube__PVPDataManager__) */
