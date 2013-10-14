//
//  CSceneGameShopLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#include "CSceneGameShopLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"

CSceneGameShopLayer::CSceneGameShopLayer()
{
    
}

CSceneGameShopLayer::~CSceneGameShopLayer()
{
    
}

bool  CSceneGameShopLayer::init()
{
    //his->m_bTimerStatus=EN_GLOBALTIMER_STOP;
  
    long value=clock();
    LayoutLayer *m_cMaps= LayoutLayer::create();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "shangdianjiemian.plist"));
    CCLog("stop:%f",(clock()-value)*1.0/CLOCKS_PER_SEC);
    return true;
}