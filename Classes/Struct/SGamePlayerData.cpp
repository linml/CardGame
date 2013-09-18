//
//  SGamePlayerData.cpp
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#include "SGamePlayerData.h"
#include "CGamePlayerStruct.h"
#include "gameTools.h"
CGamePlayerData::CGamePlayerData()
{
    m_icoin=0;
    m_icash=0;
    m_iexp=0;
    m_ilevel=0;
    m_ienergy=0; //精力
    m_iVipScore=0;
    m_ienergy_update_time=0;
    m_iupdate_time=0;
    m_create_time=0;
    m_guide_step=0;
    m_sign=0;
    m_friend_count=0;
    m_sLevelPlayer=NULL;
    m_irvc=0;
    m_nOpenGridCount = 9;
}
void CGamePlayerData::decodeDictnory(CCDictionary *dict)
{
    m_icoin=GameTools::intForKey("coin", dict);
    m_icash=GameTools::intForKey("cash", dict);
    m_iexp=GameTools::intForKey("exp", dict);
    m_ilevel=GameTools::intForKey("level", dict);
    m_ienergy=GameTools::intForKey("energy", dict); //体力
    m_ienergy_update_time=GameTools::intForKey("energy_update_time", dict);
    m_iVipScore=GameTools::intForKey("vip_score", dict);
    m_irvc=GameTools::intForKey("leader", dict);
    m_iupdate_time=GameTools::intForKey("update_time",dict);
    m_create_time=GameTools::intForKey("create_time", dict);
    m_guide_step=GameTools::intForKey("guide_step", dict);
    m_sign=GameTools::intForKey("sign",dict);
    m_friend_count=GameTools::intForKey("friend_max",dict);
    m_susername=GameTools::valueForKey("username", dict);
    m_nOpenGridCount = GameTools::intForKey("max_bag", dict);
    m_iHp=GameTools::intForKey("hp", dict);
    
}
void CGamePlayerData::setLevelConfig(SLevelPlayer * pSlevel)
{
    this->m_sLevelPlayer=pSlevel;
}
CGamePlayerData::~CGamePlayerData()
{
    
}