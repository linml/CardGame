//
//  CGameEmailData.cpp
//  91.cube
//
//  Created by linminglu on 13-8-26.
//
//

#include "CGameEmailData.h"
#include "CPtTool.h"
#include <time.h>
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
        m_iGameEmailEndTime=0;
        m_iGameEmailStartTime=0;
    
}

CGameEmailData::~CGameEmailData()
{
    
}

void CGameEmailData::getEmailCreateTime(string &str)
{
    //double diffValue=CPtTool::getDateIntermissionRebackMSC(time(NULL),(time_t)m_iGameEmailStartTime);
    CPtTool::getDataShijianChai((time_t)m_iGameEmailStartTime,time(NULL),str);
    
}

bool CGameEmailData::isOutActiveTime()
{
    //读取当前的时间 判断两个事件差  是否大于
    double diffValue=CPtTool::getDateIntermissionRebackMSC(time(NULL),(time_t)m_iGameEmailEndTime);
    if((int)(diffValue/86400.0) >= 0)
    {
        return true;
    }
    return false;
}


void CGameEmailData::decodeJsonData(std::string str)
{
    
}
#include "CPtPropConfigData.h"
string CGameEmailData::getFirstItemsPng()
{
    map<int, int>::iterator it=m_mapDataProp.begin();
    if(it==m_mapDataProp.end())
    {
        return "";
    }
    return SinglePropConfigData::instance()->getIconName(it->first);
}


