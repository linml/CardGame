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
#include "CTaskConfigData.h"
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
     CSkillData *getPutongGongji(CFightCard *pCard);
     CSkillData *getSkillBySkillId(int skillId);
     CImapact *getEffectTableByEffectId(int effectId);
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
    
    //读取NPC效果表格
    void loadNpcCard(const char *npcCardName);
    void clearAllNpcCard();

public:
    vector<CFightCard *> m_hashmapFightingCard;
    vector<CFightCard *> m_hashmapMonsterCard;
private:
    void initByServerDictorny(cocos2d::CCDictionary *dict);
    void initFightingCardByserverDictorny(cocos2d::CCArray *dict);
public:
    CC_SYNTHESIZE(int, m_FightUid, FightUid);
    CC_SYNTHESIZE(int, gameInitStatus, GameInitStatus) ;
    CC_SYNTHESIZE(int, isLoadEndCardTeam, LoadEndCardTeam);

public:
    void initPlayerStatusZero();
    //获得服务端的数据并init下数据
    void getSeverPlayerInfo(CCObject *object);

public: //读取卡包的信息
    void clearServerCardBag();
    void deleteFromCardBag(vector<int>user_CardId);
    bool isCardBagContainUserCardList(vector<int>User_CardId);
    int  isLoadCardBagEnd;
    void decodeCardDictAppendCardBag(CCDictionary *dict);
    CFightCard *findFightCardByCard_User_ID(int  carduserid);
public:
    //系统初始化信息
    void onGameBegin();
    void onGameBeginCallBack(CCObject *object);
    void parseJsonUserInfo(CCDictionary *dict);

public: //读取 卡队列的信息
    void loadCardTeamInfoCallBackByDict(CCDictionary *dictresult);
public:
    //读取对战别的英雄对战的阵容
    void loadRival(int  usid,int troop);
    void loadNpcCardTeam(int zhang,int jie,int bu,int dijige);
    void parseRival(CCObject *object);
    void parseNpcCard(CCObject *object);
    void parseNpcCard(CCDictionary *resultDictresult);
    int  isLoadFightTeam;
    //战斗时候调用的随机值得。
    void randRomIndexAdd();
    void logicRandValue(int &value,bool needAdd=true);
public:
    void backUpFightTeam(int index);
    void appendAtkData(SEveryATKData * data);
    void appendCFightCardFightingBuffer(CFightCardFightingBuffer *data);
    void onFightInterScene();
    void onFightExitScene();
    void deleteFightMonsterCard();
    vector<SEveryATKData*> getHpAngryVectory()const;
    vector<CFightCardFightingBuffer *>getFightCardFightingBuffer()const;
   
// 领取邮件：
public:
    void receiveEmail(CGameEmailData * inEmailData);
    void receiveEmail(map<int, int> inProps, int inAddPlayerExp, int inAddPlayerCoin);
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
    vector<CFightCard *> & getCardBagVector();
    vector<vector<CFightCard *> > &getCardBattleArray();

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
    int isCanAddToBackPack(map<int, int> &tmpProps, map<int, int> &inAddProps , int inUserGridCount);
    void mergeProps(map<int, int> &tmpProps, map<int, int> &inAddProps);    
    
    
// 探索信息:
public:
    int getMaxChapterId(){return m_nMaxChapterId ;};
    int getMaxSectionid(){return m_nMaxSectionId;};
    void setCurrentTaskId(int inTaskId);
    int getCurrentTaskId(){return m_nCurrentTaskId;};
    void openChapterAndSection(int inChapterId, int inSectionId){ m_nMaxChapterId = inChapterId, m_nMaxSectionId = inSectionId;};
    // now is chapter and section:
    void onGetTaskInfo();
    void onParseTaskInfoByDictionary(CCDictionary *inDataDictionary);
protected:
    void onReceiveTaskInfo(CCObject *pObject);
    void setChapterAndSectionByTask();
    

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
    map<int, CPtProp*> &m_rAllProps; // 静态配置中道具信息
    map<int, int> m_vProps;          // 用户道具列表
    bool m_bLoadProps;
private:
    map<int, CCard *>m_hashmapAllCard;
    vector<SLevelPlayer *>m_gvPlayerLevel;
    map<int ,CSkillData *>m_vSkillInfo;
    vector< CImapact * >m_vImpactInfo;
    map<int ,CCard *>m_hashmapNpcAllCard;
    string m_strSig;
    string m_strUid;
    std::vector<int >m_getRandom_data;
    int m_currRandRomIndex;
protected:
    int m_nCurrentTaskId;
    int m_nMaxChapterId;
    int m_nMaxSectionId;
    vector<CFightCard *>m_vCardBag;
    vector<vector<CFightCard*> >m_vvBattleArray;
    CC_SYNTHESIZE(bool, m_bLoadTaskInfo, LoadTaskInfo);
    CC_SYNTHESIZE(int , m_iCurrentBattleTeam, CurrentBattleTeam) ;  //!< 当前的选择 战斗的阵容
    CC_SYNTHESIZE(EN_GAMEFIGHTSTATUS, m_enWinStatus, WinOrLoseStatus);
    vector<SEveryATKData*>m_vHpAngry;
    vector<CFightCardFightingBuffer *>m_vCFightCardFightingBuffer;

};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
