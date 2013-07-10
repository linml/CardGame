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
    EN_BUFF_FIELD_ATTACK,
    EN_BUFF_FIELD_DEFEND,
    EN_BUFF_FIELD_HP,
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
    int m_iBuff_times;
    EN_BUFF_FIELD m_enBuffer_Field;
    EN_BUFF_TARGET m_enBuffer_target;
    EN_BUFF_ACTION m_enBuffer_action;
    EN_BUFF_MARK  m_enBuffer_mark;
    int m_iValue;
};

#endif /* defined(___1_cube__CCardBufferStatus__) */
