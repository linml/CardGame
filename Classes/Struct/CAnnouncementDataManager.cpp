//
//  CAnnouncementDataManager.cpp
//  91.cube
//
//  Created by linminglu on 12/4/13.
//
//

#include "CAnnouncementDataManager.h"
#include "cocos2d.h"
using namespace cocos2d;

#define DELETE_POINT_LIST(VECTORARRAY,VECTORITETYPE) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
delete *it; \
*it=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
}

CAnnouncementDataManager::CAnnouncementDataManager()
{
    m_vListContext.clear();
}

CAnnouncementDataManager::~CAnnouncementDataManager()
{
    
    DELETE_POINT_LIST(m_vListContext,list<CAnnouncementInfo *>);
}

void CAnnouncementDataManager::appendData(string str,EN_ANNOUNCEMENT_CONTEXTTYPE type)
{
    if (type==EN_ANNOUNCEMENT_CONTEXTTYPE_GAMEACTION)
    {
        m_vListContext.insert(m_vListContext.begin(),new CAnnouncementInfo(str,type));
    }
    else {
        m_vListContext.push_back(new CAnnouncementInfo(str,type));
    }
}

int CAnnouncementDataManager::getListSize()
{
    return m_vListContext.size();
}

string CAnnouncementDataManager::getShowData()
{
    string resultValue;
    if (reBegin)
    {
        m_itListIterator=m_vListContext.begin();
        reBegin=false;
    }
    if (m_itListIterator!=m_vListContext.end() && m_vListContext.size()!=0)
    {
        resultValue =(*m_itListIterator)->m_sContext;
        CAnnouncementInfo *temp=(*m_itListIterator);
        CC_SAFE_DELETE(temp);
        m_itListIterator=m_vListContext.erase(m_itListIterator);
    }
    return resultValue;
}