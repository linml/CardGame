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
    m_iBuff_times=0;
    m_enBuffer_action=EN_BUFF_ACTION_NONE;
    m_enBuffer_Field=EN_BUFF_FIELD_NONE;
    m_enBuffer_mark=EN_BUFF_MARK_NONE;
    m_enBuffer_target=EN_BUFF_TARGET_NONE;
    m_iValue=0;
}