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
#include "gameConfig.h"
using namespace std;
class CCardBufferStatus
{
public:
    CCardBufferStatus(){};
    ~CCardBufferStatus(){};
    CCardBufferStatus(int buff_effectTimes,
                      int buff_showTimes,
                      bool isDeal,
                      int mutex,
                      int mutexlevel,
                      float value,EN_BUFF_FIELD_TYPE enBuffer_Field);
public:
    bool m_bIsBeDeal;                    
    EN_BUFF_FIELD_TYPE m_enBuffer_Field; //  buffer类型，
    int    m_iBuff_effectTimes;   //效果的累计次数
    int    m_iBuff_showTimes;   //显示的次数
    float  m_iValue;       //设置浮点数字  百分比 或者是整数的的模式。
    int    m_mutex;
    int    m_mutexlevel;
    int    m_oldValue;
    EN_BUFF_TARGET m_enBuffer_target;  //buffer目标;  //待定 看是否添加在最后里面
};

#endif /* defined(___1_cube__CCardBufferStatus__) */
