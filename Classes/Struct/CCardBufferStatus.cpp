//
//  CCardBufferStatus.cpp
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#include "CCardBufferStatus.h"
CCardBufferStatus::CCardBufferStatus()
{
    m_enBuffer_Field=EN_BUFF_FIELD_NONE; //  buffer类型，
    m_iBuff_effectTimes=0;   //效果的累计次数
    m_iBuff_showTimes=0;   //显示的次数
    m_iDataType=0;     //查看buffer  是用  整数加值还是百分比加10
    m_iValue=0;       //设置浮点数字  百分比 或者是整数的的模式。
    m_enBuffer_target=EN_BUFF_TARGET_NONE;  //buffer目标;  //待定 看是否添加在最后里面

}