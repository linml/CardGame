//
//  CRankDataManager.cpp
//  91.cube
//
//  Created by phileas on 13-12-3.
//
//

#include "CRankDataManager.h"


CRankDataManager * CRankDataManager::getInstance()
{
    if (s_pIntance == NULL)
    {
        s_pIntance = new CRankDataManager();
    }
    return s_pIntance;
}
void CRankDataManager::releaseDataManager()
{
    if (s_pIntance)
    {
        delete s_pIntance;
        s_pIntance = NULL;
    }
}

CRankDataManager * CRankDataManager::s_pIntance = NULL;
