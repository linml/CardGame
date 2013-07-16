//
//  CSkillData.cpp
//  91.cube
//
//  Created by linminglu on 13-7-12.
//
//

#include "CSkillData.h"
CSkillData::CSkillData()
{
    skill_id=0;        //技能ID
    skill_type=0;      //技能类型
    stskill_id=0;
   
    cost_1_func=0;       //条件和消耗逻辑
    cost_1_parameter1=0; //条件或消耗参数值
    cost_1_parameter2=0; //条件或消耗参数值
    
     cost_2_func=0;        //条件和消耗逻辑
     cost_2_parameter1=0;  //条件或消耗参数值
     cost_2_parameter2=0;  //条件或消耗参数值
    
     parameter_1=0;        //参数值 || 效果ID
     parameter_2=0;        //参数值 || 效果ID
     parameter_3=0;        //参数值 || 效果ID
     parameter_4=0;        //参数值 || 效果ID
     effect_plist.clear();  //特效文件地址
}