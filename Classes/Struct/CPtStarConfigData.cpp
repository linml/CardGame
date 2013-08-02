//
//  CPtStarConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-8-2.
//
//

#include "CPtStarConfigData.h"
#include "gameConfig.h"
#include "gameTools.h"

CPtStarConfigData::CPtStarConfigData()
{
    m_pConfigData = CCDictionary::createWithContentsOfFile(g_str_star_config.c_str());
    if (m_pConfigData)
    {
        m_pConfigData->retain();
    }
}

CPtStarConfigData::~CPtStarConfigData()
{
    if (m_pConfigData)
    {
        m_pConfigData->release();
        m_pConfigData = NULL;
    }
}

int CPtStarConfigData::getConstConin(const int &inCardId)
{
    int result = 0;
    
    if (m_pConfigData)
    {
        char buff [15]={0};
        sprintf(buff, "%d",inCardId);
        CCDictionary * tmp = (CCDictionary *)m_pConfigData->objectForKey(buff);
        if (tmp)
        {
            result = GameTools::intForKey("coin", tmp);
        }
    }
    return result;
    
}