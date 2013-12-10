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
    {
//        //for test
//        appendData("aaaaaaaaaaaa1", EN_ANNOUNCEMENT_CONTEXTTYPE_GAMEACTION);
//        appendData("aaaaaaaaaaaa0", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);
//                appendData("aaaaaaaaaaaa2", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);
//                appendData("aaaaaaaaaaaa3", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);
//                appendData("aaaaaaaaaaaa4", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);        appendData("aaaaaaaaaaaa5", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);        appendData("aaaaaaaaaaaa6", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);        appendData("aaaaaaaaaaaa7", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);
//                appendData("aaaaaaaaaaaa8", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);
//                appendData("aaaaaaaaaaaa9", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);
//                appendData("aaaaaaaaaaaa10", EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD);
        
    }
    reBegin=true;
}

CAnnouncementDataManager::~CAnnouncementDataManager()
{
    
    DELETE_POINT_LIST(m_vListContext,list<CAnnouncementInfo *>);
}

void CAnnouncementDataManager::removeAll(EN_ANNOUNCEMENT_CONTEXTTYPE type)
{
    for (list<CAnnouncementInfo *>::iterator it=m_vListContext.begin(); it!=m_vListContext.end(); it++) {
        if((*it)->m_enType==type)
        {
            CAnnouncementInfo *temp=(*it);
            CC_SAFE_DELETE(temp);
            if (it==m_itListIterator) {
                m_itListIterator=it=m_vListContext.erase(it);
            }
            else{
                it=m_vListContext.erase(it);
            }
           
        }
    }
}

void CAnnouncementDataManager::appendData(string & str,EN_ANNOUNCEMENT_CONTEXTTYPE type)
{
    CCLOG("AAAAAAAAAAAAAAAPPPPPPPPPPPPPPPP%s",str.c_str());
    if (type==EN_ANNOUNCEMENT_CONTEXTTYPE_OWNDRAWCARD)
    {
        m_vListContext.insert(m_vListContext.begin(),new CAnnouncementInfo(str,type));
        reBegin=true;
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
    if (m_itListIterator==m_vListContext.end()) {
        reBegin=true;
    }
    return resultValue;
}