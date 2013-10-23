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
class CGlobalUpdateAp;
class CGlobalUpdateGp;
class CStructShopInfo;
class CStructShopSellItem;

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
    CFightCard *findFightCardByCard_User_ID(int  carduserid);
    bool cardBagIsMoreThanConfig();
public:
    //系统初始化信息
    void onGameBegin();
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

    // test:

// play info:
public:
    int getCoin();          //金币
    int getPlayerCash();  //现金
    int getRVC();          //领导力
    int getPlayerExp();    //经验
    int getPlayerLevel();  //等级
    int getPlayerGp();     //神力
    int getPlayerAp();  //体力
    int getFriendly(); //友情点
    bool getPlayrHadRecharged();//是否首充
    
    int getGpMax();
    int getApMax();
    int getExpMax();
    
    
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

    
    bool addPalyerExp(int inAddExp);
    void subPlayerExp(int inSubExp);
    
    void setPlayerExp(int inExpValue);
    
    
    
    void addCoin(const int &inAddValue);
    void subCoin(const int &inSubValue);
    
    
    void addRVC(const int &inAddValue);
    void subRVC(const int &insubRVC);
    
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
    int getShopItemCount();
    int getShopType(); // priceType
    string getShopName();
    CStructShopSellItem *getShopItemById(int itemID);
    void  setShopItemByItemId(int itemID,int nNowValue);
    void setReBackXiangLiang();
private:
    void setTaskTotalNumberOnFinishSectionTask(int Value);
public:
    void sendUpdateGp();
    void sendUpdateAp();
    void decodeDataGp(CCObject *object);
    void decodeDataAp(CCObject *object);
    
public:
    // 或取当前阵容的全部领导力
    int getAllRvcBattlerArray(const int& inType);
    
protected:
    CGamePlayerData *m_gGamePlayerData;
    map<int, CPtProp*> &m_rAllProps; // 静态配置中道具信息
    map<int, int> m_vProps;          // 用户道具列表
    
private:
    map<int, CCard *>m_hashmapAllCard;
    map<int ,CSkillData *>m_vSkillInfo;
    vector< CImapact * >m_vImpactInfo;
    map<int ,CCard *>m_hashmapNpcAllCard;
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

    CStructShopInfo  *m_gameShop;
    
public:
    void updatePlayerDataWithExp();


protected:
    void sendLeveleUpRequest();
    void receiveLevelUpRequestMsg(CCObject *pObject);
    void parseLevelUpInfoByDict(CCDictionary *inDict);
    void changePlayerInfoWithLevelUp(CCDictionary *inDict);
    void levelUpData(int level);

};
typedef Singleton<CGamePlayer> SinglePlayer;


#endif
