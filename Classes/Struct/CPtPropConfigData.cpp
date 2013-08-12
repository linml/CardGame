//
//  CPtPropConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-8-12.
//
//

#include "CPtPropConfigData.h"
#include "gameConfig.h"

CPtPropConfigData::CPtPropConfigData()
{
    m_pConfigData = CCDictionary::createWithContentsOfFile(g_str_prop_config.c_str());
    if (m_pConfigData)
    {
        m_pConfigData->retain();
    }
    m_nPropId = -1;

}

CPtPropConfigData::~CPtPropConfigData()
{
    if(m_pConfigData)
    {
        m_pConfigData->release();
    }
}

bool CPtPropConfigData::getPropDataById(const int &inPropId)
{
    if(inPropId < 0)
    {
        return false;
    }
    if (m_nPropId == inPropId)
    {
        CCLog("don't update propconfig file");
        return true;
    }
    char buffer[20] = {0};
    sprintf(buffer, "%d", inPropId);
    if (m_pConfigData)
    {
        CCDictionary * tmp = (CCDictionary *) m_pConfigData->objectForKey(buffer);
        if (tmp == NULL)
        {
            return false;
        }
        // getData:
        m_strPropName = GameTools::valueForKey("name", tmp);
        m_nPropType  = GameTools::intForKey("type", tmp);
        m_nUnLockLevel = GameTools::intForKey("unlock_level", tmp);
        m_nUseDelete = GameTools::intForKey("use_delete", tmp);
        m_nQuality = GameTools::intForKey("quality", tmp);
        m_nPrice =  GameTools::intForKey("price", tmp);
        m_nSkillId = GameTools::intForKey("skill_id", tmp);
        m_nSellCoin = GameTools::intForKey("sell_coin", tmp);
        m_nCoin = GameTools::intForKey("coin",tmp);
        m_strIconName = GameTools::valueForKey("icon", tmp);
        m_nLimitNum = GameTools::intForKey("limit_num", tmp);
        m_nMaxNum = GameTools::intForKey("max_num", tmp);
        m_strTips = GameTools::valueForKey("tips", tmp);
        
        m_nPropId = inPropId;
        return true;
    }
    return false;
}

