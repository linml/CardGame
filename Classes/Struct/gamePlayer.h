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
#include "CGamePlayerStruct.h"
using namespace std;

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
public:
    void loadServerCardBag();
    void clearServerCardBag();
    void parseCardBagJson(CCObject *obj);
    void deleteFromCardBag(vector<int>user_CardId);
    bool isCardBagContainUserCardList(vector<int>User_CardId);
    bool isLoadCardBagEnd;
    
public:
    //获取 卡包 内容的卡
    vector<CFightCard *>m_vCardBag;
    //获取当前阵容（0~2),01攻击  　２防御
    vector<vector<CFightCard*> >m_vvBattleArray;
    int m_iCurrentBattle;

};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
