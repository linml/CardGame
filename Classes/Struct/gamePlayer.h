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
#include "CPtPropConfigData.h"
#include "CGameEmailData.h"
using namespace std;
class CImapact;
class CSkillData;
class SEveryATKData;
class CFightCardFightingBuffer;
class CGamePlayerData;

#define OPENGGRIDLEVLE 20

class CGamePlayer : cocos2d::CCObject
{
 public:
    CGamePlayer();
    ~CGamePlayer();
    void loadGamesConfig();
    void onExitGameApp();
public:
     CCard *getCardByCardId(int cardid);
     SLevelPlayer *getPlayerDataByLevel(int level);
     CSkillData *getPutongGongji();
     CSkillData *getSkillBySkillId(int skillId);
     CImapact *getEffectTableByEffectId(int effectId);
     bool isHaveSkillBySkillId(int skillID);
    
     string getBufferPlistByEffectId(int effectID);
     string getBufferPngByEffectId(int effectID);
     CImapact *findByImpactId( int tempImpactId);
private:
   //读取卡牌表格
    void clearAllCard();
    void initAllCard(const char *cardFileName);
   
    //读取玩家等级表
    void clearPlayerTable();
    void initPlayerTable(const char *playerFileName);

    //读取技能表格
    void loadAllSkillInfo(const char *skillFileName);
    void clearAllSkillInfo();

    //读取效果表格
    void loadAllEffectInfo(const char *effectFileName);
    void clearAllEffectInfo();

public:
    vector<CFightCard *>m_hashmapFightingCard;
    vector<CFightCard *>m_hashmapMonsterCard;
    //模拟战斗时候调用的 数据
    bool isLoadServer;
private:
    void initByServerDictorny(cocos2d::CCDictionary *dict);
    void initFightingCardByserverDictorny(cocos2d::CCArray *dict);
public:
    int m_FightUid;
public:
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
    int  isLoadCardBagEnd;
    void decodeCardDict(CCDictionary *dict);
public: //读取英雄的信息
    void loadServerPlayerInfo();
    void loadServerPlayerInfoCallBack(CCObject *obj);
    bool isLoadPlayerInfoEnd;
    CFightCard *findFightCardByCard_User_ID(int  carduserid);
public:
    //系统初始化信息
    void onGameBegin();
    void onGameBeginCallBack(CCObject *object);
    void parseJsonUserInfo(CCDictionary *dict);
    int  gameInitStatus;
    
public: //读取 卡队列的信息
    void loadCardTeamInfo();
    void loadCardTeamInfoCallBack(CCObject *obj);
    void loadCardTeamInfoCallBackByDict(CCDictionary *dictresult);
    int  isLoadEndCardTeam;
public:
    //读取对战别的英雄对战的阵容
    void loadRival(int  usid,int troop);
    void parseRival(CCObject *object);
    int  isLoadFightTeam;
    void randRomIndexAdd();
    void logicRandValue(int &value,bool needAdd=true);
    std::vector<int >m_getRandom_data;
    int m_currRandRomIndex;
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

   
// 领取邮件：
public:
    void receiveEmail(CGameEmailData * inEmailData);
    void receiveEmail(map<int, int> inProps, int inAddPlayerExp, int inAddPlayerCoin);
// interface of prop:
    void addProp(int inPropId, int inAddNum);
    int subProp(int inPropId, int inSubNum);
    map<int,int> getPlayerProps();
    
//获取玩家基本信息
public:
    const char* getUserId();
    void setUserId(string strUid);
    const char* getUserSig();
    void setUserSig(string sig);
public:
    vector<CFightCard *>m_vCardBag;
    vector<vector<CFightCard*> >m_vvBattleArray;
    int m_iCurrentBattleTeam;  //!< 当前的选择 战斗的阵容
// 道具背包:
public:
    //读取道具背包信息
    void loadPropsInfo();
    void parsePropsInfo(CCObject *pObject);
    void parseProsInfoByDict(CCDictionary *dict);
 
    vector<int> getCanAddToBackPackEmals(vector<EMAIL_DATA> inEmailDatas);
    int getOpenGridCount();
    int AddOpenGridCount(int inAddCount);
    int getUseGridCount();
    void updateProps();
    bool getLoadPropEnd(){return  m_bLoadProps;};
    
    bool addGridBySys();
protected:
    map<int, CPtProp*> &m_rAllProps; // 静态配置中道具信息
    map<int, int> m_vProps;          // 用户道具列表
    bool m_bLoadProps;
protected:
    int isCanAddToBackPack(map<int, int> &tmpProps, map<int, int> &inAddProps , int inUserGridCount);
    void mergeProps(map<int, int> &tmpProps, map<int, int> &inAddProps);    
    
    
    // play info:
public:
    int getCoin();
    int getPlayerPrice();
    int getRVC();
    int getPlayerExp();
    int getPlayerLevel();
    
    void addPlayerPrice(int inAddPrice);
    void subPlayerPrice(int inSubPrice);
    
    void addPalyerExp(int inAddExp);
    void subPlayerExp(int inSubExp);
    
    void addRVC(const int &inAddValue);
    void addCoin(const int &inAddValue);
    void ReduceRVC(const int &inReduceRVC);
    void ReduceCoin(const int &inReduceCoin);
    
protected:
    CGamePlayerData *m_gGamePlayerData;
private:
    map<int, CCard *>m_hashmapAllCard;
    vector<SLevelPlayer *>m_gvPlayerLevel;
    map<int ,CSkillData *>m_vSkillInfo;
    vector< CImapact * >m_vImpactInfo;
    string m_strSig;
    string m_strUid;

};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
