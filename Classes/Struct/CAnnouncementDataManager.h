//
//  CAnnouncementDataManager.h
//  91.cube
//
//  Created by linminglu on 12/4/13.
//
//

#ifndef ___1_cube__CAnnouncementDataManager__
#define ___1_cube__CAnnouncementDataManager__

#include <iostream>
#include <list>
using namespace std;
enum EN_ANNOUNCEMENT_CONTEXTTYPE {
    EN_ANNOUNCEMENT_CONTEXTTYPE_GAMEACTION,
    EN_ANNOUNCEMENT_CONTEXTTYPE_OWNDRAWCARD,
    EN_ANNOUNCEMENT_CONTEXTTYPE_OTHERDRAWCARD
    };
struct CAnnouncementInfo {
public:
    CAnnouncementInfo(){m_enType=EN_ANNOUNCEMENT_CONTEXTTYPE_OWNDRAWCARD;}
    CAnnouncementInfo(string str,EN_ANNOUNCEMENT_CONTEXTTYPE type)
    {
        this->m_sContext=str;
        this->m_enType=type;
    }
    string m_sContext;
    EN_ANNOUNCEMENT_CONTEXTTYPE m_enType;
};

class CAnnouncementDataManager
{
public:
    CAnnouncementDataManager();
    ~CAnnouncementDataManager();
    void appendData(string str,EN_ANNOUNCEMENT_CONTEXTTYPE type);
    int getListSize();
    string getShowData();
private:
    list <CAnnouncementInfo *>m_vListContext; //应该50条左右外加一个当前抽卡的内容。
    list<CAnnouncementInfo *>::iterator m_itListIterator;
     bool reBegin;
};

#endif /* defined(___1_cube__CAnnouncementDataManager__) */
