
//
//  CCardBufferStatus.cpp
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#include "CCardBufferStatus.h"

CCardBufferStatus::CCardBufferStatus(int buff_effectTimes,
                  int buff_showTimes,
                  bool isDeal,
                  int mutex,
                  int mutexlevel,
                  float value,int effectid,EN_BUFF_FIELD_TYPE enBuffer_Field)
{
    m_enBuffer_Field=enBuffer_Field;
    m_bIsBeDeal=isDeal;
    m_iBuff_effectTimes=buff_effectTimes;
    m_iBuff_showTimes=buff_showTimes;
    m_iValue=value;
    m_mutex=mutex;
    m_mutexlevel=mutexlevel;
    m_ieffectid=effectid;
}