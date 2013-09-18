//
//  SGamePlayerData.h
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#ifndef ___1_cube__SGamePlayerData__
#define ___1_cube__SGamePlayerData__

#include <iostream>
#include <string>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
struct SLevelPlayer;
class CGamePlayerData {
public:
    CGamePlayerData();
    ~CGamePlayerData();
    void setLevelConfig(SLevelPlayer * level);
    void decodeDictnory(CCDictionary *dict);
public:
    std::string m_susername;
    int    m_icoin;
    int    m_icash;
    int    m_iexp;
    int    m_ilevel;
    int    m_ienergy; //体力
    int    m_iVipScore;
    int    m_ienergy_update_time;
    int    m_iupdate_time;
    int    m_create_time;
    int    m_guide_step;
    int    m_sign;
    int    m_friend_count;
    int    m_irvc;
    int    m_iHp;   //神力
    struct SLevelPlayer *m_sLevelPlayer;
    int    m_nOpenGridCount;
};

#endif /* defined(___1_cube__SGamePlayerData__) */
