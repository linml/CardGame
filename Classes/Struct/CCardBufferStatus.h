//
//  CCardBufferStatus.h
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#ifndef ___1_cube__CCardBufferStatus__
#define ___1_cube__CCardBufferStatus__
/**
 * 定义一些战斗卡牌处于的buffer 表格 
 * 
 *
 */
 
#include <iostream>

#include "cocos2d.h"
#include "CCard.h"
using namespace  cocos2d;
using namespace std;
/* 
 buff_field'=>'attach',
'buff_time'=>0,
'buff_num'=>50,
'buff_action'=>'add',
'buff_mark'=>'+',
'buff_target'=>'0'//0  0
*/
//添加还是减少
enum EN_BUFF_ACTION
{
    EN_BUFF_ACTION_NONE,
    EN_BUFF_ACTION_ADD,
    EN_BUFF_ACTION_SUB
};
//乘以还是add
enum EN_BUFF_MARK
{
    EN_BUFF_MARK_NONE,
    EN_BUFF_MARK_MULTIPY,
    EN_BUFF_MARK_ADD
};
//处理的类型
enum EN_BUFF_FIELD
{
    EN_BUFF_FIELD_NONE,
    EN_BUFF_FIELD_ATTACK,   //影响攻击力
    EN_BUFF_FIELD_DEFEND,   //影响防御力
    EN_BUFF_FIELD_HP,       //影响HP，
    EN_BUFF_FIELD_BINGFENG, //被冰封
    EN_BUFF_FIELD_XUANYUN,  //眩晕
    EN_BUFF_FIELD_DUYAO,    //毒药

};
enum EN_BUFF_TARGET {
    EN_BUFF_TARGET_NONE,
    EN_BUFF_TARGET_OWNZERO,
    EN_BUFF_TARGET_OWNALL,
    EN_BUFF_TARGET_MONSTERZERO,
    EN_BUFF_TARGET_MONSTERALL
    };


class CCardBufferStatus :public CCObject
{
public:
    CCardBufferStatus();
    EN_BUFF_FIELD m_enBuffer_Field; //  buffer类型，
    int    m_iBuff_effectTimes;   //效果的累计次数
    int    m_iBuff_showTimes;   //显示的次数
    int    m_iDataType;     //查看buffer  是用  整数加值还是百分比加10
    float  m_iValue;       //设置浮点数字  百分比 或者是整数的的模式。
    EN_BUFF_TARGET m_enBuffer_target;  //buffer目标;  //待定 看是否添加在最后里面
};

#endif /* defined(___1_cube__CCardBufferStatus__) */
