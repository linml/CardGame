//
//  CFightSkillManager.h
//  91.cube
//
//  Created by linminglu on 13-7-16.
//
//

#ifndef ___1_cube__CFightSkillManager__
#define ___1_cube__CFightSkillManager__

#include <iostream>
#include <map>

#include "CSingleton.h"
#include "gameConfig.h"

using namespace std;
class CFightCard;
class CGamePlayer;
class CSkillData;
class CImapact;
class CAnimationSpriteGameFight;
class CCardBufferStatus;
class CFightCardBufferDataEveryFight;

#define STATICCOSTTFUNCTION(FUNCTIONNAME) \
static int FUNCTIONNAME(CFightCard *pCard,CFightCard *pMonster,CSkillData *pData);

#define STATICEFFICEFUNCTION(FUNCTIONNAME) \
static void FUNCTIONNAME(CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject);



typedef void (*pFunc) (CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk);
typedef int (*pbFunc) (CFightCard *pCard,CFightCard *pMonster,CSkillData *pData);
typedef void (*pbEffFunc) (CFightCard *pCard,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject);

class CFightSkillManager
{
    typedef map<string, pbFunc> ::iterator IteratorMapPbFunc;
    typedef map<string,pFunc>::iterator    IteratorMapPfunc;
    typedef map<string,pbEffFunc>::iterator    IteratorEffMapPfunc;
public:
    //每一种逻辑ID对应的costfunc 不同。
    static void logicSkill_Putong(CFightCard *pCard,vector< CFightCard *>pFightCard,vector< CFightCard *>pMonterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill);
    static void logicSkillFight(CFightCard *pCard,vector< CFightCard *>pFightCard,vector< CFightCard *>pMonterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,int paramid);
    static void logicSkill_1(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk);
    
    static void logicSkill_2(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk);
    
    static void logicSkill_3(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk);
    
    static void logicSkill_4(CFightCard *pCard,vector<CFightCard *>FightCard,vector<CFightCard *>MonsterCard,int FightIndex,int MonsterIndex,CSkillData *pSkill,EN_ATKFIGHT_INDEX enatk);
    
    //定义一个costfunction 的函数
    //每一种的costfunction的ID  对应的需求不一样
    STATICCOSTTFUNCTION(costFunc_0);
    STATICCOSTTFUNCTION(costFunc_1);
    STATICCOSTTFUNCTION(costFunc_2);
    STATICCOSTTFUNCTION(costFunc_3);
    STATICCOSTTFUNCTION(costFunc_4);
    STATICCOSTTFUNCTION(costFunc_5);
    STATICCOSTTFUNCTION(costFunc_6);
    STATICCOSTTFUNCTION(costFunc_7);
    STATICCOSTTFUNCTION(costFunc_8);
    STATICCOSTTFUNCTION(costFunc_9);
    

    
    
    //每一种的效果id 对应的参数值得又不一样
    STATICEFFICEFUNCTION(effect_0);
    STATICEFFICEFUNCTION(effect_1);
    STATICEFFICEFUNCTION(effect_2);
    STATICEFFICEFUNCTION(effect_3);
    STATICEFFICEFUNCTION(effect_4);
    STATICEFFICEFUNCTION(effect_5);
    STATICEFFICEFUNCTION(effect_6);
    STATICEFFICEFUNCTION(effect_7);
    STATICEFFICEFUNCTION(effect_8);
    STATICEFFICEFUNCTION(effect_9);
    STATICEFFICEFUNCTION(effect_10);
    STATICEFFICEFUNCTION(effect_11);
    
public:
  static  void appendAnimation(int AtkIndex,int DefIndex,int AddHp,int SubHp,int skillid,int AddEngry,int subAngry,EN_ANIMATIONTYPE enAnimationType,EN_ATKFIGHT_INDEX enatkindex);
    static  void appendBuffIcon(CFightCard *fightCard,CFightCard *monster,EN_ATKFIGHT_INDEX enAtkFightIndex);
    static  void addSkillBuffer(CFightCard *pMonster,CCardBufferStatus *buffer);
    static  void dealWithBuffer(CFightCard *pFightCard,int AtkIndex, int DefIndex,EN_ATKFIGHT_INDEX enatkindex );//处理自身的buffer
    
    //进入程序初始化，加载函数map表格
    void initSkill();
    //根据card的 技能找寻到逻辑id
    void CardFighting(CFightCard *pCard,vector<CFightCard *>fightCard,vector<CFightCard *>monsterCard,int FightIndex,int MonstIndex,EN_SEND_SKILL enskill,EN_ATKFIGHT_INDEX enAtkFightIndex);
    //当前是否可以发动怒气技能
    bool isCanSpendAngrySkill(CFightCard *pFight);
    //当前是否可以攻击
    bool isCanSpendAtkToMonster(CFightCard *pFight);
    //当前添加buffer的属性
    void addOrSubBuffer(CFightCard *pFight);
    //物理免疫的buffer;
    static bool isHavaPhysicHarmMagic(CFightCard *pMonstFight);
    //怒气免疫的buffer;
    static bool isHaveMagicHarm(CFightCard *pMonstFight);
    //普通伤害计算
    void basicAtk(CFightCard *pFightCard,CFightCard *pMonstFight);
    void clearAnimationList();
public:
   static vector<CAnimationSpriteGameFight *>m_animationVector;
   static vector<CFightCardBufferDataEveryFight *>m_animationBufferVector;
};

typedef Singleton<CFightSkillManager> G_FightSkillManager;


#endif /* defined(___1_cube__CFightSkillManager__) */
