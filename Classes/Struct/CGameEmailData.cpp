//
//  CGameEmailData.cpp
//  91.cube
//
//  Created by linminglu on 13-8-26.
//
//

#include "CGameEmailData.h"
#include "CPtTool.h"
CGameEmailData::CGameEmailData()
{
    //mktime(<#struct tm *#>)
    m_iGameEmailCoins=0;
    m_iGameEmailExp=0;
    m_mapDataProp.clear();
    m_iGameEmailItemid=0;
        m_iGameEmailStatus=0;
        m_iGameEmailMsgId=0;
        m_iGameEmailUid=0;
        m_iGameEmailFromUid=0;
        m_iGameEmailType=0;
        m_iGameEmailTime=0;
    
}
CGameEmailData::~CGameEmailData()
{
    
}

bool CGameEmailData::isOutActiveTime()
{
    //读取当前的时间 判断两个事件差  是否大于
    double diffValue=CPtTool::getDateIntermissionRebackMSC(time(NULL),(time_t)m_iGameEmailTime);
    if((int)(diffValue/86400.0) >= 0)
    {
        return true;
    }
    return false;
}


void CGameEmailData::decodeJsonData(std::string str)
{
    
}




