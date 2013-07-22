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
class CImapact;
class CSkillData;

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
   //开启程序需要读取下列的几个选项
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
    
    //读取技能表格
    void loadAllSkillInfo(const char *skillFileName);
    void clearAllSkillInfo();
    CSkillData *getPutongGongji();
    bool isHaveSkillBySkillId(int skillID);
    CSkillData *getSkillBySkillId(int skillId);
    vector<CSkillData *>m_vSkillInfo;
    
    //读取效果表格
    void loadAllEffectInfo(const char *effectFileName);
    void clearAllEffectInfo();
    CImapact *findByImpactId( int tempImpactId);
    vector< CImapact * >m_vImpactInfo;

 
public:
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
public: //读取卡包的信息
    void loadServerCardBag();
    void clearServerCardBag();
    void parseCardBagJson(CCObject *obj);
    void deleteFromCardBag(vector<int>user_CardId);
    bool isCardBagContainUserCardList(vector<int>User_CardId);
    bool isLoadCardBagEnd;
public: //读取英雄的信息
    void loadServerPlayerInfo();
    void loadServerPlayerInfoCallBack(CCObject *obj);
    bool isLoadPlayerInfoEnd;
    CFightCard *findFightCardByCard_User_ID(int  carduserid);
public: //读取 卡队列的信息
    void loadCardTeamInfo();
    void loadCardTeamInfoCallBack(CCObject *obj);
    bool isLoadEndCardTeam;
public:
    //读取对战别的英雄对战的阵容
    void loadRival(int  rivalUid);
    void parseRival(CCObject *object);
    
public:
    //获取 卡包 内容的卡
    vector<CFightCard *>m_vCardBag;
    //获取当前阵容（0~2),01攻击  　２防御
    vector<vector<CFightCard*> >m_vvBattleArray;
    int m_iCurrentBattle;


};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
