//
//  PVPRankRewardConfig.cpp
//  91.cube
//
//  Created by phileas on 14-1-10.
//
//

#include "PVPRankRewardConfig.h"
#include "gameConfig.h"

PVPRankRewardConfig * PVPRankRewardConfig::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new PVPRankRewardConfig();
    }
    return s_pInstance;
}

void PVPRankRewardConfig::releaseConfigData()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}
PVPRankRewardConfig* PVPRankRewardConfig::s_pInstance = NULL;

void PVPRankRewardConfig::getPvPRankRewardByRank(int inIndex, int &outKanHun,int& outCoin)
{
    outCoin = 0;
    outKanHun = 0;
    for (int i = 0; i < m_nDataExtentCount; i++)
    {
        if (m_pPvPRankRewards[i].startrank >= inIndex && inIndex <= m_pPvPRankRewards[i].endrank )
        {
            outKanHun = m_pPvPRankRewards[i].kahun;
            outCoin = m_pPvPRankRewards[i].coin;
            break;
        }
    }

}
void PVPRankRewardConfig::getPvPRankRewardById(int inId, int &outKanHun,int &outCoin)
{
    outCoin = 0;
    outKanHun = 0;
    for (int i = 0; i < m_nDataExtentCount; i++)
    {
        if (m_pPvPRankRewards[i].rewardId >= inId )
        {
            outKanHun = m_pPvPRankRewards[i].kahun;
            outCoin = m_pPvPRankRewards[i].coin;
            break;
        }
    }
}

PVPRankReward * PVPRankRewardConfig::getPvPRankRewardByRank(int inIndex)
{
    PVPRankReward * result = NULL;
    for (int i = 0; i < m_nDataExtentCount; i++)
    {

        if (m_pPvPRankRewards[i].startrank <= inIndex && inIndex <= m_pPvPRankRewards[i].endrank )
        {
    
            result = m_pPvPRankRewards+i;
            break;
        }
    }
    return result;
}
PVPRankReward * PVPRankRewardConfig::getPvPRankRewardById(int inId)
{
    PVPRankReward * result = NULL;
    for (int i = 0; i < m_nDataExtentCount; i++)
    {
        if (m_pPvPRankRewards[i].rewardId >= inId )
        {
            result = m_pPvPRankRewards+i;
            break;
        }
    }
    return result;
}
PVPRankRewardConfig::PVPRankRewardConfig()
{
    m_pPvPRankRewards = NULL;
    loadData();
}
PVPRankRewardConfig::~PVPRankRewardConfig()
{
    CC_SAFE_DELETE_ARRAY(m_pPvPRankRewards);
    m_pPvPRankRewards = NULL;
}

void PVPRankRewardConfig::loadData()
{
    CCDictionary *tmpDict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "charts_pvp_reward_config.plist"));
    if (tmpDict)
    {
        int count = tmpDict->count();
        m_pPvPRankRewards = new PVPRankReward[count];
        m_nDataExtentCount = count;
        CCDictElement *element = NULL;
        CCDictionary *elementDict = NULL;
        int index = 0;
        CCDICT_FOREACH(tmpDict, element)
        {
            elementDict = (CCDictionary*) element->getObject();
            m_pPvPRankRewards[index].coin = GameTools::intForKey("coin", elementDict);
            m_pPvPRankRewards[index].kahun = GameTools::intForKey("card_soul", elementDict);
            m_pPvPRankRewards[index].startrank = GameTools::intForKey("min", elementDict);
            m_pPvPRankRewards[index].endrank = GameTools::intForKey("max", elementDict);
            m_pPvPRankRewards[index].rewardId = GameTools::intForKey("id", elementDict);
            index++;
        }
        for (int i = 0; i < m_nDataExtentCount; i++)
        {
            CCLog("the coin: %d", m_pPvPRankRewards[i].coin);
        }
    }
}