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
#include <map>
using namespace cocos2d;
using namespace std;
struct SLevelPlayer;
class CGamePlayerData {
public:
    CGamePlayerData();
    ~CGamePlayerData();
    void clearPlayerData();
    void decodeDictnory(CCDictionary *dict);
    int appendAp(int value);
    int appendGp(int value);
    int getGpMax();
    int getApMax();
    int getExpMax();
    int getCardBagAppend();
    void settestInit(int level);
    void logicValue(int &Value,int inValue, char logicChar);
public:
    std::string m_susername;
    int    m_icoin;
    int    m_icash;
    int    m_iexp;
    int    m_ilevel;
    int    m_iAp; //体力
    int    m_iVipScore;
    int    m_iAp_update_time;
    int    m_iupdate_time;
    int    m_create_time;
    int    m_guide_step;
    int    m_sign;
    int    m_friend_count;
    int    m_irvc;
    int    m_iGp;   //神力
    bool   m_bFirstRecharge;
    int     m_nFriendly;
    struct SLevelPlayer *m_sLevelPlayer;
    int    m_nOpenGridCount;
    int    m_nCardAppend;
    std::map<int ,SLevelPlayer *>m_gvPlayerLevel;
};

#endif /* defined(___1_cube__SGamePlayerData__) */
