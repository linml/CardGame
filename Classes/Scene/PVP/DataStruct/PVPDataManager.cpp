//
//  PVPDataManager.cpp
//  91.cube
//
//  Created by phileas on 14-1-10.
//
//

#include "PVPDataManager.h"

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
    
}

PVPDataManager* PVPDataManager::s_pIntance = NULL;


PVPDataManager::PVPDataManager()
{
    
}
PVPDataManager::~PVPDataManager()
{
    
}