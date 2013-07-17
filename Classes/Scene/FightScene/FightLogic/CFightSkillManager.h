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
class CFightingCardLayerScene;

#define STATICCOSTTFUNCTION(FUNCTIONNAME) \
static bool FUNCTIONNAME(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact); 

#define STATICEFFICEFUNCTION(FUNCTIONNAME) \
static void FUNCTIONNAME(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject);



typedef void (*pFunc) (CFightCard *pCard,CGamePlayer *pGamePlayer,CSkillData *pSkill,CFightingCardLayerScene *scene);
typedef bool (*pbFunc) (CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact);
typedef void (*pbEffFunc) (CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact,EN_ATKOBJECT enAtkobject);

class CFightSkillManager
{
    typedef map<string, pbFunc> ::iterator IteratorMapPbFunc;
    typedef map<string,pFunc>::iterator    IteratorMapPfunc;
    typedef map<string,pbEffFunc>::iterator    IteratorEffMapPfunc;
public:
    //每一种逻辑ID对应的costfunc 不同。
    static void logicSkill_Putong(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact);
    static void logicSkill_1(CFightCard *pCard,CGamePlayer *pGamePlayer,CSkillData *pSkill,CFightingCardLayerScene *scene);
    static void logicSkill_2(CFightCard *pCard,CGamePlayer *pGamePlayer,CSkillData *pSkill,CFightingCardLayerScene *scene);
    static void logicSkill_3(CFightCard *pCard,CGamePlayer *pGamePlayer,CSkillData *pSkill,CFightingCardLayerScene *scene);
    static void logicSkill_4(CFightCard *pCard,CGamePlayer *pGamePlayer,CSkillData *pSkill,CFightingCardLayerScene *scene);
    
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
    
public:
    //进入程序初始化:
    void initSkill();
    //根据card的 技能找寻到逻辑id
    void CardFighting(CFightCard *pCard,CGamePlayer *pGamePlayer,EN_SEND_SKILL enskill, CFightingCardLayerScene *scene);

};

typedef Singleton<CFightSkillManager> g_FightSkillManager;


#endif /* defined(___1_cube__CFightSkillManager__) */
