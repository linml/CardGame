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
class SEveryATKData;
class CFightCardFightingBuffer;

class CGamePlayer : cocos2d::CCObject
{
 public:
    CGamePlayer();
    ~CGamePlayer();
    void initGames();
    void loadGamesConfig();
    void onExitGameApp();
 public:
   //开启程序需要读取下列的几个选项
   //读取卡牌表格
    void clearAllCard();
    void initAllCard(const char *cardFileName);
    map<int, CCard *>m_hashmapAllCard;
    
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
    string getBufferPngByEffectId(int effectID);
    
    //读取效果表格
    void loadAllEffectInfo(const char *effectFileName);
    void clearAllEffectInfo();
    CImapact *findByImpactId( int tempImpactId);
    vector< CImapact * >m_vImpactInfo;
public:
    vector<CFightCard *>m_hashmapFightingCard;
    vector<CFightCard *>m_hashmapMonsterCard;
    //模拟战斗时候调用的 数据
    bool isLoadServer;
    
private:
    void initByServerDictorny(cocos2d::CCDictionary *dict);
    void initFightingCardByserverDictorny(cocos2d::CCArray *dict);

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
    void initPlayerStatusZero();
    //获得服务端的数据并init下数据
    void getSeverPlayerInfo(CCObject *object);
//以后这个通讯的会单独抽离在通讯的框架里面。
    
    
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
    void loadRival(int  usid,int troop);
    void parseRival(CCObject *object);
    bool isLoadFightTeam;
public:
    void backUpFightTeam(int index);
    void appendAtkData(SEveryATKData * data);
    void appendCFightCardFightingBuffer(CFightCardFightingBuffer *data);
    void onFightInterScene();
    void onFightExitScene();
    void deleteFightMonsterCard();
    EN_GAMEFIGHTSTATUS  m_enWinStatus;
    vector<SEveryATKData*>m_vHpAngry;
    vector<CFightCardFightingBuffer *>m_vCFightCardFightingBuffer;
public:
    //获取 卡包 内容的卡
    vector<CFightCard *>m_vCardBag;
    //获取当前阵容（0~2),01攻击  　２防御
    vector<vector<CFightCard*> >m_vvBattleArray;
    int m_iCurrentBattleTeam; //当前的选择 战斗的阵容


    
    // play info:
public:
    int getCoin();
    int getRVC();
    
    void addRVC(const int &inAddValue);
    void addCoin(const int &inAddValue);
    void ReduceRVC(const int &inReduceRVC);
    void ReduceCoin(const int &inReduceCoin);
    
    void testPlayInfoData();
protected:
    int m_nCoin;
    int m_nRvc;

};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
