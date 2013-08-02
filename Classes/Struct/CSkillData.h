//
//  CSkillData.h
//  91.cube
//
//  Created by linminglu on 13-7-12.
//
//

#ifndef ___1_cube__CSkillData__
#define ___1_cube__CSkillData__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CSkillData
{
public:
    CSkillData();
    int skill_id;        //技能ID
    int skill_type;      //技能类型
    int skill_logic;
    //技能逻辑ID 根据策划定义 该ID 可以指定 以下的parameter_1 ,parameter_2,parameter_3,parameter_4代表的是否是standarimpact.plist里面的ID;
    
    int cost_func_1;       //条件和消耗逻辑
    int cost_parameter_1; //条件或消耗参数值
    int cost_parameter_p_1; //条件或消耗参数值
    
    int cost_func_2;        //条件和消耗逻辑
    int cost_parameter_2;  //条件或消耗参数值
    int cost_parameter_p_2;  //条件或消耗参数值
    
    int parameter_1;        //参数值 || 效果ID
    int parameter_2;        //参数值 || 效果ID
    int parameter_3;        //参数值 || 效果ID
    int parameter_4;        //参数值 || 效果ID
    int parameter_5;
    int parameter_6;
    string effect_plist;  //特效文件地址

};

class CImapact
{
public:
    int m_ieffect_id;        //技能ID
    int m_ieffect_logic;      //技能类型
    int m_ibuff;      
    
    int  m_ishowtime;       //条件和消耗逻辑
    int  m_iMutex; //条件或消耗参数值
    int  m_iMutex_level; //条件或消耗参数值
    
    int m_iParameter_1;        //条件和消耗逻辑
    int m_iParameter_2;  //条件或消耗参数值
    int m_iParameter_3;  //条件或消耗参数值
    
    int m_iParameter_4;        //参数值 || 效果ID
    int m_iParameter_5;        //参数值 || 效果ID
    int m_iParameter_6;        //参数值 || 效果ID
    int m_iParameter_7;        //参数值 || 效果ID
    int m_iParameter_8;  //特效文件地址
    int m_iParameter_9;
    int m_iParameter_10;
    string m_sEffectFile;
    
};
//class CFightCard;
//class CGamePlayer;
//class CCubeSkillManager
//{
//    static void logicSkill_Putong(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact);
//    static void logicSkill_Two(CFightCard *pCard,CGamePlayer *pGamePlayer,CFightCard *pMonterCard,CSkillData *pSkill,CImapact *pCimapact);
//};

#endif /* defined(___1_cube__CSkillData__) */
