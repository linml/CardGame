//
//  PVPDataManager.cpp
//  91.cube
//
//  Created by phileas on 14-1-10.
//
//

#include "PVPDataManager.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"


PVPDataManager* PVPDataManager::getInstance()
{
    if (s_pIntance == NULL)
    {
        s_pIntance = new PVPDataManager();
    }
    return s_pIntance;
}
void PVPDataManager::releasManager()
{
    if (!s_bIsHandlerNetwork)
    {
        delete s_pIntance;
        s_pIntance = NULL;
    }
}

PVPDataManager* PVPDataManager::s_pIntance = NULL;
bool PVPDataManager::s_bIsHandlerNetwork = false;

void PVPDataManager::releasAllRankInfo()
{
    CC_SAFE_DELETE_ARRAY(m_pAllRankInfoes);
    m_nAllRankCount = 0;
    m_pAllRankInfoes = NULL;
}

PVPDataManager::PVPDataManager()
{
    m_pAllRankInfoes = NULL;
    m_pHandler = NULL;
    m_pCallback = NULL;
    m_cPVPUserInfo.name = SinglePlayer::instance()->getPlayerName();
    m_cPVPUserRankInfo.name = m_cPVPUserInfo.name;
    m_cPVPUserInfo.uid = atoi(SinglePlayer::instance()->getUserId());
    m_cPVPUserRankInfo.uid = m_cPVPUserInfo.uid;
    
}
PVPDataManager::~PVPDataManager()
{
    releasAllRankInfo();
    
}

void PVPDataManager::getPVPMainUIInfoFromServer()
{
    s_bIsHandlerNetwork = true;
    char buffer[100]={0};
    snprintf(buffer, sizeof(buffer),"sig=%s",STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETPVPMAINUIINFO(194), "CALLBACK_PVPDataManager::getPVPMainUIInfoFromServer", "REQUEST_PVPDataManager::getPVPMainUIInfoFromServer", buffer, callfuncO_selector(PVPDataManager::parsePVPMainUIInfo));
    
}

bool PVPDataManager::getRankReward(PVPRankReward * & outRanReward)
{
    bool bRet =false;
    if (m_vRankReward.size()>0)
    {
        outRanReward = PVPRankRewardConfig::getInstance()->getPvPRankRewardByRank(*m_vRankReward.begin());
        m_vRankReward.erase(m_vRankReward.begin());
        bRet = true;
    }else
    {
        outRanReward = PVPRankRewardConfig::getInstance()->getPvPRankRewardByRank(m_cPVPUserInfo.rank);
    }
    return bRet;
}

void PVPDataManager::parsePVPMainUIInfo(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_PVPDataManager::getPVPMainUIInfoFromServer");
    s_bIsHandlerNetwork = false;
    char *buffer = (char *) pObject;
    if(buffer)
    {
        CCLog(CCString::createWithFormat("the : buffer: %s", buffer)->getCString());
        CCDictionary *tmpDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (tmpDict)
        {
            int code = GameTools::intForKey("code", tmpDict);
            if(code == 0)
            {
                CCDictionary *resultDict = (CCDictionary*)tmpDict->objectForKey("result");
                parsePVPHeadRank((CCArray*) resultDict->objectForKey("charts"));
                parsePVPRankReward((CCArray*) resultDict->objectForKey("charts_reward"));
                parsePVPUserInfo((CCDictionary*)resultDict->objectForKey("user_info"));
                m_nOverTime = GameTools::intForKey("pvp_over_time", resultDict);
                SinglePlayer::instance()->setPlayerPVPCount(GameTools::intForKey("pvp_num",resultDict));
            }
        }
    }
    callBack();
    
}


void PVPDataManager::getPVPRankInfoFromServer()
{
    s_bIsHandlerNetwork = true;
    char buffer[100]={0};
    snprintf(buffer, sizeof(buffer),"sig=%s",STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETPVPRANKINFO(194), "CALLBACK_PVPDataManager::getPVPRankInfoFromServer", "REQUEST_PVPDataManager::getPVPRankInfoFromServer", buffer, callfuncO_selector(PVPDataManager::parsePVPRankInfo));

}
void PVPDataManager:: parsePVPRankInfo(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_PVPDataManager::getPVPRankInfoFromServer");
    s_bIsHandlerNetwork = false;
    char *buffer = (char *) pObject;
    if(buffer)
    {
        CCLog(CCString::createWithFormat("the : buffer: %s", buffer)->getCString());
        CCDictionary *tmpDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (tmpDict)
        {
            int code = GameTools::intForKey("code", tmpDict);
            if(code == 0)
            {
                CCDictionary *resultDict = (CCDictionary*)tmpDict->objectForKey("result");
                parsePVPAllRank((CCArray*) resultDict->objectForKey("charts"));
                parsePVPUserRankInfo((CCDictionary*)resultDict->objectForKey("pvp_info"));
                
            }
        }
    }
    callBack();

}


void PVPDataManager::parsePVPHeadRank(CCArray *inArray)
{
    m_nHeadRankCount = 0;
    if(inArray && inArray->count() > 0 && inArray->count() <= 3)
    {
        parsePVPRankInfo(inArray, m_nHeadRankCount, m_aHeadRank);
    }
   
    
}
void PVPDataManager::parsePVPAllRank(CCArray *inArray)
{
    releasAllRankInfo();
    if (inArray && inArray->count()> 0)
    {
        m_pAllRankInfoes = new PVPRankData[inArray->count()];
        parsePVPRankInfo(inArray, m_nAllRankCount, m_pAllRankInfoes);
    }
    
   
}

void PVPDataManager::parsePVPRankInfo(CCArray* inArrayData, int& outCountInContainer, PVPRankData *outDataContainer)
{

    if(inArrayData)
    {
        outCountInContainer =  inArrayData->count();
        CCDictionary *tmpDict = NULL;
        for (int i = 0; i < outCountInContainer; i++)
        {
            tmpDict = (CCDictionary*)(inArrayData->objectAtIndex(i));
            outDataContainer[i].uid = GameTools::intForKey("uid", tmpDict);
            outDataContainer[i].credits = GameTools::intForKey("point", tmpDict);
            outDataContainer[i].fightpointer = GameTools::intForKey("power", tmpDict);
            outDataContainer[i].level= GameTools::intForKey("level", tmpDict);
            outDataContainer[i].rank = i+1;
            outDataContainer[i].name =GameTools::valueForKey("name", tmpDict);
            
        }
    }
}

void PVPDataManager::parsePVPRankReward(cocos2d::CCArray *inArray)
{
    m_vRankReward.clear();
    if (inArray)
    {
        CCObject* elementObject;
        CCDictionary *elementDict;
        CCARRAY_FOREACH(inArray, elementObject)
        {
            elementDict = (CCDictionary*) elementObject;
            m_vRankReward.push_back(GameTools::intForKey("num", elementDict));
        }
        
    }
}
void PVPDataManager::parsePVPUserInfo(CCDictionary *inUserDict)
{
  
    m_cPVPUserInfo.level = SinglePlayer::instance()->getPlayerLevel();
    m_cPVPUserInfo.credits = INITJIFEN;
    m_cPVPUserInfo.rank = NOINRANKTAG;
    if (inUserDict)
    {
        m_cPVPUserInfo.fightpointer = GameTools::intForKey("power", inUserDict);
        m_cPVPUserInfo.credits = GameTools::intForKey("point", inUserDict);
        m_cPVPUserInfo.rank = GameTools::intForKey("num", inUserDict);
    }
}

void PVPDataManager::parsePVPUserRankInfo(CCDictionary *inPVPInfoDict) // 排行榜种玩家的信息（定时刷新的）
{
    m_cPVPUserRankInfo.credits = INITJIFEN;
    m_cPVPUserRankInfo.rank = NOINRANKTAG;
    if (inPVPInfoDict)
    {
        m_cPVPUserRankInfo.fightpointer = GameTools::intForKey("power", inPVPInfoDict);
        m_cPVPUserRankInfo.credits = GameTools::intForKey("point", inPVPInfoDict);
        m_cPVPUserRankInfo.rank = GameTools::intForKey("num", inPVPInfoDict);
        m_cPVPUserRankInfo.level = GameTools::intForKey("level", inPVPInfoDict);
        
    }

}

void PVPDataManager::callBack()
{
    if (m_pHandler && m_pCallback)
    {
        (m_pHandler->*m_pCallback)();
        CC_SAFE_RELEASE_NULL(m_pHandler);
        m_pCallback = NULL;
    }
}