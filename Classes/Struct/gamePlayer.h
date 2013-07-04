//
//  gamePlayer.h
//  91.cube
//
//  Created by xianbei1987 on 13-5-24.
//
//

#ifndef _1_cube_gamePlayer_h
#define _1_cube_gamePlayer_h
#include "gameStruct.h"
#include "CSingleton.h"
#include "CCard.h"
#include "cocos2d.h"
#include <vector>
using namespace std;
struct SLevelPlayer {
    int m_iLevel;
    int m_iCard_max;
    int m_iLeadership;
    int m_iFriend_max; //好友上线
    int m_iHP;
    int m_iMp;
    int m_iexp;
};
class CGamePlayer : cocos2d::CCObject {
public:
    void forTestCard();
    void forTestMonsterCard();
 public:
    CGamePlayer();
    ~CGamePlayer();
    void initGames();
    void loadGamesConfig();
 public:    
   //读取卡牌表格
    void clearAllCard();
    void initAllCard(const char *cardFileName);
    map<int, CCard *>m_hashmapAllCard;
    vector<CCard *>m_hashmapFight;
    vector<CCard *>m_hashmapMonster;
    
    //读取玩家等级表格子
    void clearPlayerTable();
    void initPlayerTable(const char *playerFileName);
  
    
    
    vector<SLevelPlayer *>m_gvPlayerLevel;
    vector<CFightCard *>m_hashmapFightingCard;
    vector<CFightCard *>m_hashmapMonsterCard;
    void initByServerDictorny(cocos2d::CCDictionary *dict);
    void initFightingCardByserverDictorny(cocos2d::CCArray *dict);
    bool isLoadServer;
private:
    int m_iCurrentExp;
    int m_iCurrentHp;
    int m_iCurrentMp;
    int m_iCurrentLeader;
    int m_iLevel;
    int m_iCardBagNum;
    struct SLevelPlayer *m_sLevelPlayer;
public:
    int  getFriendMax();
    bool isCanUpdateLevel(int nExp);
    bool UpdateLevel(int nExp);
    int getTotoalHp();
    int getCurrentHp();
    int getTotoalMp();
    int getCurrentMp();
    int getTotalExp();
    int getCurrentExp();
    void setCurrentHp(int nHp);
    void setCurrentMp(int nMp);
    void setCurrentExp(int nExp);
    void initPlayerStatusZero();
    //获得服务端的数据并init下数据
    void getSeverPlayerInfo(CCObject *object);

};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
