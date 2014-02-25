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
#include "CGameTimerManager.h"
#include "CStructShopInfo.h"
#include "CStructShopSellItem.h"
using namespace std;
class CImapact;
class CSkillData;
class SEveryATKData;
class CFightCardFightingBuffer;
class CGamePlayerData;
class CGlobalUpdateAp;
class CGlobalUpdateGp;
class CGlobalGetGongGao;
class CGlobalUpdateHospital;
class CStructStrips;
class CAnnouncementDataManager;
class CDrawGonggaoTable;
class CEveryDayLoginData;

#define OPENGGRIDLEVLE 20
#define MAXLEVEL 100

class CGamePlayer : cocos2d::CCObject
{
 public:
    CGamePlayer();
    virtual ~CGamePlayer();
    void loadGamesConfig();
    void onExitGameApp();
    void clearShangchengData();
    void releasAllResource();
public:
     map<int ,CCard *> *getCardMap();
     CCard *getCardByCardId(int cardid);
     CSkillData *getPutongGongji(CFightCard *pCard);
     CSkillData *getSkillBySkillId(int skillId);
     CImapact *getEffectTableByEffectId(int effectId);
     string getBufferPlistByEffectId(int effectID);
     string getBufferPngByEffectId(int effectID);
     CImapact *findByImpactId( int tempImpactId);
protected:
    void initData();
    void restData();
    
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
    
    void clearSkillStrip();
    
    //读取CardGongGao
    void loadCardGonggao();

public:
    vector<CFightCard *> m_hashmapFightingCard;
    vector<CFightCard *> m_hashmapMonsterCard;
private:
    void initByServerDictorny(cocos2d::CCDictionary *dict);
    void initFightingCardByserverDictorny(cocos2d::CCArray *dict);
public:
    CC_SYNTHESIZE(int, m_FightUid, FightUid);
    CC_SYNTHESIZE(bool, m_bFightFuChou, Fuchou);
    CC_SYNTHESIZE(int, m_nFightFuchouId, FuChouId);
    CC_SYNTHESIZE(int ,m_bIsFightWithTeam, IsFightWithTeam); //0 npc //1  team
    CC_SYNTHESIZE(int, gameInitStatus, GameInitStatus) ;
    CC_SYNTHESIZE(int, isLoadEndCardTeam, LoadEndCardTeam);

public:
    //获得服务端的数据并init下数据
    void getSeverPlayerInfo(CCObject *object);

public: //读取卡包的信息
    void clearServerCardBag();
    void deleteFromCardBag(vector<int>user_CardId);
    bool isCardBagContainUserCardList(vector<int>User_CardId);
    int  isLoadCardBagEnd;
    void decodeCardDictAppendCardBag(CCDictionary *dict);
    void appendCardToCardBag(const int &inCardItemId, const int &inCardId, int inSuit, int inLevel = 1, int inExp = 0);
    void subCardByUserId(const int &inUserId);
    void subCardByIndex(const int inIdex);
    void subCardByIterator(vector<CFightCard*>::iterator inIterator);
    int  getMaxCardCount();
    int  getCurrentCardCount();
    void appendCardBag();
    bool canAppendCardBag();
    CFightCard *findFightCardByCard_User_ID(int  carduserid);
    bool cardBagIsMoreThanConfig();
public:
    //系统初始化信息
    void onGameBegin(const char* pchNickname, const int nGender);
    void onGameBeginCallBack(CCObject *object);
    void parseJsonUserInfo(CCDictionary *dict);
    void parseTaskInfo(CCDictionary *dict);

public: //读取 卡队列的信息
    void loadCardTeamInfoCallBackByDict(CCDictionary *dictresult);
public:
    //读取对战别的英雄对战的阵容
    void loadRival(int  usid,int troop);
    void loadNpcCardTeam(int zhang,int jie,int bu,int dijige);
    void parseRival(CCObject *object);
    void parseNpcCard(CCObject *object);
    void parseNpcCard(CCDictionary *resultDictresult);
    void parseRandomData(CCArray* inRandowArray);
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
    CStructStrips *getSkillStripNeed(int id);
    bool isCheckNeedSatisfied(int nlineIndex,EN_LEFTTEAMORRIGHTTEAM enValue);//判断左边的阵容是否满足需求
   
// 领取邮件：
public:
    void receiveEmail(CGameEmailData * inEmailData);
    void receiveEmail(map<int, int> inProps, int inAddPlayerExp, int inAddPlayerCoin);
    
    // reward走邮件：
    void addEmails(CCDictionary *inbox_info);
    
    void addProp(int inPropId, int inAddNum);
    int subProp(int inPropId, int inSubNum);
    map<int,int> getPlayerProps();
    
    
    
//获取玩家基本信息
public:
    const char* getUserId();
    void setUserId(string strUid);
    const char* getUserSig();
    void setUserSig(string sig);
    const int getPlayerGender();
public:
    vector<CFightCard *> & getCardBagVector();
    vector<vector<CFightCard *> > &getCardBattleArray();

// 道具背包:
public:
    //读取道具背包信息
    void parseProsInfoByDict(CCDictionary *dict);
 
    vector<int> getCanAddToBackPackEmals(vector<EMAIL_DATA> inEmailDatas);
    int getOpenGridCount();
    int AddOpenGridCount(int inAddCount);
    void setOpenGridCount(int inOpenGridCount);
    int getUseGridCount();
    void updateProps();
    bool addGridBySys();
    
    int getPropCountFromBag(int inPropId);
    int getPropMaxCountAddToBag(int inPropId);
    int haveEnoughPropById(int inPropId, int inNum);
protected:
    int isCanAddToBackPack(map<int, int> &tmpProps, map<int, int> &inAddProps , int inUserGridCount);
    void mergeProps(map<int, int> &tmpProps, map<int, int> &inAddProps);    
    
    
// 探索信息:
public:
    int  getMaxChapterId(){return m_nMaxChapterId ;};
    int  getMaxSectionid(){return m_nMaxSectionId;};
    void setCurrentTaskId(int inTaskId);
    int  getCurrentTaskId(){return m_nCurrentTaskId;};
    void openChapterAndSection(int inChapterId, int inSectionId){ m_nMaxChapterId = inChapterId; m_nMaxSectionId = inSectionId;};

protected:
    void setChapterAndSectionByTask();
public:
    bool hasFullGP();
    bool hasFullAP();
    
    
    // new add by phileas:
    int getTotalFightPoint();
    int getTotalRechargeValue();
    
    void setTotalFightPoint(int inFightPoint);
    void setTotalRechargeValue(int inRechargeValue);
    void addRechargeValue(int inAddRechargeValue);
    
    int getCoin();          //金币
    int getPlayerCash();  //现金
    int getRVC();          //领导力
    int getPlayerExp();    //经验
    int getPlayerLevel();  //等级
    int getPlayerGp();     //神力
    int getPlayerAp();  //体力
    int getFriendly(); //友情点
    bool getPlayrHadRecharged();//是否首充
    const char* getPlayerName();//获取玩家昵称
    
    int getGpMax();
    int getApMax();
    int getExpMax();
    int getFriendMax();
    

    int setPlayerGp(int iValue); //神力
    int setPlayerAp(int iValue);
    int setPlayerCash(int iValue);
    bool setPlayrHadRecharged(bool var);
  
    void addPlayerGp(int inAddHp);
    void subPlayerGp(int inSubHp);
    
    
    void addPlayerAp(int inAddEnergy);
    void subPlayerAp(int inSubEnergy);
      
    void addPlayerCash(int iValue);
    void subPlayerCash(int iValue);

    
    bool addPlayerExp(int inAddExp);
    void subPlayerExp(int inSubExp);
    
    void setPlayerExp(int inExpValue);
    
    
    
    void addCoin(const int &inAddValue);
    void subCoin(const int &inSubValue);
    
    
    void addRVC(const int &inAddValue);
    void subRVC(const int &insubRVC);
    
    void addFriendly(int inFriendly);
    void subFriendly(int inFriendly);
    
    
    
    //add player info 卡魂
    void addKaHun(int inAddKaHun);
    void subKaHun(int inSubKaHun);
    
    int  getPlayerKaHun();
    void setPlayerKanHun(int inKaHun);
    
    
    int getPVPTotalNum();
    int getCostCashPerAddPVPCount();
    int  getPlayerPVPCount();
    void setPlayerPVPCount(int number);
    void addPlayerPVPCount();
    void subPlayerPVPCount();
    
    void addTaskOperator(int taskType,std::vector<int> *targetIdVector);
    void subTaskOperator(int taskType,std::vector<int> *targetIdVector);
    bool isSuccessFinishTask();
    int  getCurrentTaskType();
    void getNextTaskInfo();
    void setInitCurrentTaskTargetNumber(int number); //程序启动的时候 需要载入当前任务进度
    /**
     @paramid1 taskid  当前完成的任务ID
     @paramid2 object  调用某个layer的类
     @paramid3 selector 回调函数 ，同上个参数一样类中
     @strcallback 回调字符串
     */
    void postCompleteTask(int taskId, CCObject *object, SEL_CallFuncO selector,const char *strCallback);
    /**
     
     */
    void postAddTask(int taskNextId, CCObject *object, SEL_CallFuncO selector,const char *strCallback); //当本地
    bool isHaveSendComplate();
    int getShopItemCount(int shopID);
    int getShopType(int shopID); // priceType
    string getShopName(int shopID);
    CStructShopSellItem *getShopItemById(int shopId,int itemID);
    void  setShopItemByItemId(int shopID,int itemID,int nNowValue);
    void setReBackXiangLiang(int shopId);
    void setReBackAll();
    
    //机器猫说商城中买的道具ID都是唯一的
    CStructShopSellItem *getShopItemByPropId(int inPropId);
private:
    void setTaskTotalNumberOnFinishSectionTask(int Value);
public:
    void sendUpdateGp();
    void sendUpdateAp();
    void decodeDataGp(CCObject *object);
    void decodeDataAp(CCObject *object);
    void sendGetGonggao();
    void decodeDataGonggao(CCObject *object);
    bool isCheckSameGongGao(CCArray *array);

    
public:
    // 或取当前阵容的全部领导力
    int getAllRvcBattlerArray(const int& inType);
    
    
// data section:
protected:
    CGamePlayerData *m_gGamePlayerData;
    map<int, CPtProp*> &m_rAllProps; // 静态配置中道具信息
    map<int, int> m_vProps;          // 用户道具列表
    
private:
    map<int, CCard *>m_hashmapAllCard; // 静态配置表里的所有卡的信息
    map<int ,CSkillData *>m_vSkillInfo;
    vector< CImapact * >m_vImpactInfo;
    map<int ,CCard *>m_hashmapNpcAllCard;
    map<int,CStructStrips *>m_haspMapSkillStrips;
    string m_strSig;
    string m_strUid;
    std::vector<int >m_getRandom_data;
    int m_currRandRomIndex;
    //添加当前任务的判断值得
    CPtTaskLogic *m_pTaskLogic;
protected:
    int m_nCurrentTaskId;
    int m_nMaxChapterId;
    int m_nMaxSectionId;
    vector<CFightCard *>m_vCardBag;
    vector<vector<CFightCard*> >m_vvBattleArray;
    CC_SYNTHESIZE(bool, m_bAllTaskCompleted, AllTaskCompleted);
    CC_SYNTHESIZE(int , m_iCurrentBattleTeam, CurrentBattleTeam) ;  //!< 当前的选择 战斗的阵容
    CC_SYNTHESIZE(EN_GAMEFIGHTSTATUS, m_enWinStatus, WinOrLoseStatus);
    CC_SYNTHESIZE(bool, m_bFightKuaijin, FightKuaijin);
    vector<SEveryATKData*>m_vHpAngry;
    vector<CFightCardFightingBuffer *>m_vCFightCardFightingBuffer;
    CGlobalUpdateAp  *m_pUpdateAp;
    CGlobalUpdateGp  *m_pUpdateGp;   //其实用一个类就可以做的。
    CGlobalGetGongGao *m_pUpdateGetGonggao;
    CStructShopInfoManager  *m_gameShopManager;
    int m_nActionGonggao;
    vector<int>m_vGonggaoId;
public:
    void updatePlayerDataWithExp();
protected:
    void sendLeveleUpRequest();
    void receiveLevelUpRequestMsg(CCObject *pObject);
    void parseLevelUpInfoByDict(CCDictionary *inDict);
    void changePlayerInfoWithLevelUp(CCDictionary *inDict);
    void levelUpData(int level);
public:
    list<int>m_vPlayerBufferList;
    int zhongzuCount[8];//0,1,2,3我方的人员个数   4,5,6,7,敌方的种族个数
    void initZhongZu();
public:
    CAnnouncementDataManager *m_pAnno;
    CCArray *m_gonggaoCard;
    CEveryDayLoginData *m_pEveryDataLogin;
    bool m_bIsLogin;
    bool isTodayHaveGet();
    void initDayInfo(string str);
    
};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
