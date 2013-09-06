//
//  CGameEmailManager.cpp
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#include "CGameEmailManager.h"
#include "Utility.h"
#include "PtJsonUtility.h"
#include "CSingleton.h"
#include "PtHttpURL.h"
#include "gameConfig.h"
#include "PtHttpClient.h"
#include "CMyDictionary.h"
#include "CGameEmailData.h"
#include "CFileReadWrite.h"
#include "CPtTool.h"
//str="{"1000":19,"1001":19}"

bool compare_index(const CGameEmailData* lhs,const CGameEmailData* rhs)
{
        return lhs->getGameEmailStartTime() < rhs->getGameEmailStartTime();
};


template<class GAMEEMAILDATA,class UStatus>
class findFuncByEmailID : public binary_function<typename std::list<GAMEEMAILDATA>::value_type , UStatus, bool>
{
public:
    bool operator()(const typename list<GAMEEMAILDATA>::value_type iter, const UStatus& value) const
    {
        if (iter->getGameEmailMsgId() == value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

CGameEmailManager::CGameEmailManager()
{
    
}
CGameEmailManager::~CGameEmailManager()
{
    for (list<CGameEmailData *>::iterator it=m_listGameEamil.begin(); it!=m_listGameEamil.end(); it++) {
        if(*it)
        {
            CGameEmailData *data=*it;
            delete data;
            data=NULL;
        }
    }
    m_listGameEamil.erase(m_listGameEamil.begin(),m_listGameEamil.end());
}
class nodejiangxu
{
public:
    bool operator()(const CGameEmailData * t1,const CGameEmailData* t2){
        return t1->getGameEmailStartTime()>t2->getGameEmailStartTime();    //会产生升序排序,若改为>,则变为降序
    }
};
void CGameEmailManager::sortList()
{
//    std::greater<CGameEmailData *> gt;
    m_listGameEamil.sort(nodejiangxu());
//    for (list<CGameEmailData *>::iterator it=m_listGameEamil.begin(); it!=m_listGameEamil.end(); it++) {
//        cout<<CPtTool::timetodate((*it)->getGameEmailStartTime())<<" "<<(*it)->getGameEmailStartTime()<<endl;
//    }
}

void CGameEmailManager::decodeEmap(CCDictionary *dict)
{
    CCArray *vKeyArray=dict->allKeys();
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *emailDirector=(CCDictionary*)(dict->objectForKey(key->m_sString));
        if(emailDirector)
        {
            CGameEmailData *pGameEmailData=new CGameEmailData;
            pGameEmailData->setGameEmailMsgId(key->intValue());
            pGameEmailData->setGameEmailExp(GameTools::intForKey("exp", emailDirector));
            pGameEmailData->setGameEmailCoins(GameTools::intForKey("coins", emailDirector));
            pGameEmailData->setGameEmailFromUid(GameTools::intForKey("from_uid", emailDirector));
            pGameEmailData->setGameEmailType(GameTools::intForKey("type", emailDirector));
            pGameEmailData->setGameEmailStatus(GameTools::intForKey("status",emailDirector));
            pGameEmailData->setGameEmailStartTime(GameTools::intForKey("start_time", emailDirector));
            pGameEmailData->setGameEmailEndTime(GameTools::intForKey("expire_time", emailDirector));
            pGameEmailData->setGameEmailTitle(GameTools::valueForKey("title", emailDirector));
            pGameEmailData->setGameEmailContent(GameTools::valueForKey("content", emailDirector));
            CCDictionary *emailItemDirector=(CCDictionary *)(emailDirector->objectForKey("item"));
            if(emailItemDirector)
            {
                CCDictElement* pElement = NULL;
                CCDICT_FOREACH(emailItemDirector, pElement)
                {
                    const char* pchKey = pElement->getStrKey();
                    pGameEmailData->m_mapDataProp[atoi(pchKey)]=GameTools::intForKey(pchKey, emailItemDirector);
                }
            }
            m_listGameEamil.push_back(pGameEmailData);
        }
    }
    sortList();
}

void CGameEmailManager::decodeEmap(std::string str)
{
    /*
     {"1":{"from_uid":"20","type":"0","msg_type":"103","status":"0","item":{"200005":2},"time":"0","title":"\u4f3d\u5229\u7565","content":"\u590f\u6d1b\u514b"},"2":{"from_uid":"20","type":"0","msg_type":"1001","status":"0","item":{"200001":2,"200004":3},"time":"0","title":"\u4efb\u610f","content":"\u5929\u5929"},"3":{"from_uid":"20","type":"0","msg_type":"1002","status":"0","item":{"200002":2,"200003":3,"200004":5},"time":"0","title":"\u4efb\u610f","content":"\u82b1\u975e\u82b1"}
     */
    CCLOG("%s",str.c_str());
    CCDictionary* dictInfo = PtJsonUtility::JsonStringParse(str.c_str());
    CCArray *vKeyArray=dictInfo->allKeys();
    for (int i=0; i<vKeyArray->count(); i++) {
        CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
        CCDictionary *emailDirector=(CCDictionary*)(dictInfo->objectForKey(key->m_sString));
        if(emailDirector)
        {
            CGameEmailData *pGameEmailData=new CGameEmailData;
            pGameEmailData->setGameEmailMsgId(key->intValue());
            pGameEmailData->setGameEmailExp(GameTools::intForKey("exp", emailDirector));
            pGameEmailData->setGameEmailCoins(GameTools::intForKey("coins", emailDirector));
            pGameEmailData->setGameEmailFromUid(GameTools::intForKey("from_uid", emailDirector));
            pGameEmailData->setGameEmailType(GameTools::intForKey("type", emailDirector));
            //pGameEmailData->setGameEmailStatus(GameTools::intForKey("status",emailDirector));
            pGameEmailData->setGameEmailStatus(0);
            pGameEmailData->setGameEmailStartTime(GameTools::intForKey("start_time", emailDirector));
            pGameEmailData->setGameEmailEndTime(GameTools::intForKey("expire_time", emailDirector));
            pGameEmailData->setGameEmailTitle(GameTools::valueForKey("title", emailDirector));
            pGameEmailData->setGameEmailContent(GameTools::valueForKey("content", emailDirector));
            CCDictionary *emailItemDirector=(CCDictionary *)(emailDirector->objectForKey("item"));
            if(emailItemDirector)
            {
                CCDictElement* pElement = NULL;
                CCDICT_FOREACH(emailItemDirector, pElement)
                {
                    const char* pchKey = pElement->getStrKey();
                    pGameEmailData->m_mapDataProp[atoi(pchKey)]=GameTools::intForKey(pchKey, emailItemDirector);
                }
            }
            m_listGameEamil.push_back(pGameEmailData);
        }
    }
}


void CGameEmailManager::postHttpRequest()
{
    m_iGetHttpStatus=0;
#ifndef AAAAFOROSMACHINE
    char data[50];
    sprintf(data, "%d",getCurrentTotalEmail());
    string str=string("info={\"total\":")+ data;
    sprintf(data, "%d",getUpdateTotalEmail());
    str +=string(",\"update\":")+data+"}";
//xianbei modify    string connectData="sig=2ac2b1e302c46976beaab20a68ef95";
    string connectData="sig=";
    connectData += STR_USER_SIG;
    connectData+="&"+str;
    //http://cube.games.com/api.php?m=Fight&a=getTeamInfo&uid=194&sig=2ac2b1e302c46976beaab20a68ef95
    ADDHTTPREQUESTPOSTDATA(STR_URL_EMAIL(connectData), "GetEmail", "merlinaskplayerinfo1",connectData.c_str(),callfuncO_selector(CGameEmailManager::getHttpReponse));
#else
    char *data=new char[5];
    getHttpReponse((CCObject *)data);
#endif
}

void CGameEmailManager::deleteAllEmail()
{
    for (list<CGameEmailData *>::iterator it=m_listGameEamil.begin(); it!=m_listGameEamil.end(); it++)
    {
        if (*it) {
            CGameEmailData *data= *it;
            delete data;
            data=NULL;
        }
    }
    m_listGameEamil.erase(m_listGameEamil.begin(),m_listGameEamil.end());
}
class vector_finder
{
public:
    vector_finder(const int a):m_i_a(a){}
   bool operator ()(const std::vector<int>::value_type &value)
    {
       return value == m_i_a;
      }
    private:
 int m_i_a;
};
void CGameEmailManager::deleteEmailData(vector<int >emilIdList)
{
    if(emilIdList.size()==0)
    {
        deleteAllEmail(); //如果后台返回的数据全是空得 那么删除本地的文件 同时需要保存 新的文件名称 但是内容是clear
    }
    else
    {
        for (list<CGameEmailData *>::iterator it=m_listGameEamil.begin(); it!=m_listGameEamil.end();it++)
        {
            vector<int>::iterator emialIDiterator=find_if(emilIdList.begin(),emilIdList.end(),vector_finder((*it)->getGameEmailMsgId()));
            if(emialIDiterator==emilIdList.end())
            {
                CGameEmailData *data=*it;
                delete data;
                data=NULL;
                m_listGameEamil.erase(it);
            }
        }
    }
}

void CGameEmailManager::deleteEmailByEmailId(int msgID)
{
    list<CGameEmailData *>::iterator it=find_if(m_listGameEamil.begin(), m_listGameEamil.end(), bind2nd(findFuncByEmailID<const CGameEmailData *,int>(),msgID));
    if(it!=m_listGameEamil.end())
    {
        CGameEmailData *data=*it;
        delete data;
        data=NULL;
        m_listGameEamil.erase(it);
    }
}

void CGameEmailManager::loadLocalEmail()
{
        //读取本地保存的文件 放到
    
}

#include <fstream>
void CGameEmailManager::writeToFile()
{
    string path = CCFileUtils::sharedFileUtils()->getWriteablePath() + "emailsavefile.db";
    CCLOG("wanna save file path = %s",path.c_str());
    ofstream outFile(path.c_str());
    list <CGameEmailData *>::const_iterator list_it = m_listGameEamil.begin();
    while(list_it!= m_listGameEamil.end())
    {
        if(*list_it)
        {
         CGameEmailData *gameEmailData=(*list_it);
         outFile <<gameEmailData->getGameEmailMsgId()<<"|";
         outFile <<gameEmailData->getGameEmailFromUid()<<"|";
         outFile <<gameEmailData->getGameEmailType()<<"|";
         outFile <<gameEmailData->getGameEmailStatus()<<"|";
         outFile <<gameEmailData->getGameEmailStartTime()<<"|";
         outFile <<gameEmailData->getGameEmailEndTime()<<"|";
         outFile <<gameEmailData->getGameEmailTitle().c_str()<<"|";
         outFile <<gameEmailData->getGameEmailContent().c_str()<<"|";
         outFile <<gameEmailData->getGameEmailCoins()<<"|";
         outFile <<gameEmailData->getGameEmailExp()<<"|";
          for (map<int , int>::iterator propIt=gameEmailData->m_mapDataProp.begin(); propIt!=gameEmailData->m_mapDataProp.end()   ; propIt++)
          {
                outFile <<propIt->first<<" ";
              outFile <<  propIt->second<<" ";
          }
         outFile <<endl;
        }

        list_it++;
    }
    //CFileReadWrite::saveFile(str.c_str(), "emailsavefile.db");
    
}

CGameEmailData *CGameEmailManager::getGameDataByIndex(int index)
{
    if(index>m_listGameEamil.size())
    {
        return NULL;
    }
    list<CGameEmailData *>::iterator i = m_listGameEamil.begin();
    advance( i , index);
    if(i!=m_listGameEamil.end())
    {
        return *i;
    }
    return NULL;
}

int CGameEmailManager::getMailCount()
{
    return m_listGameEamil.size();
}

void CGameEmailManager::testData()
{
//    CGameEmailData *gamedata=new CGameEmailData;
//    gamedata->setGameEmailMsgId(1);
//    m_gameEmail[2]=gamedata;
}

void CGameEmailManager::getHttpReponse(CCObject *object)
{
    if(!object)
    {
        this->m_iGetHttpStatus=ERROR_MSG_CONNECTSERVERERROR;
        return;
    }
    char *data=(char *)object;
    //判断code问题；
    // CCDictionary *dict=PtJsonUtility::JsonStringParse(data);
    delete data;
    data=NULL;
}
struct funcChangeRead{
    void operator ()(CGameEmailData *data)
    {
        data->setGameEmailStatus(1);
    }
};
void CGameEmailManager::changeEmailStatus()
{
    for_each(m_listGameEamil.begin(), m_listGameEamil.end(), funcChangeRead());
}


int  CGameEmailManager::getCurrentTotalEmail()
{
    return  CCUserDefault::sharedUserDefault()->getIntegerForKey("CurrentTotalEmail",0);
}
int  CGameEmailManager::getUpdateTotalEmail()
{
    return  CCUserDefault::sharedUserDefault()->getIntegerForKey("UpdateTotalEmail",0);    
}


void CGameEmailManager::removeGameEmailData(list<int> vlistEmailList)
{
    for ( list<int>::iterator itList=vlistEmailList.begin() ; itList!=vlistEmailList.end(); itList++) {
        list<CGameEmailData *>::iterator it=find_if(m_listGameEamil.begin(), m_listGameEamil.end(),bind2nd(findFuncByEmailID<const CGameEmailData *,int>(),*itList));
        if(it!=m_listGameEamil.end())
        {
            CGameEmailData *tempData=*it;
            delete tempData;
            tempData=NULL;
            m_listGameEamil.erase(it);
        }
    }
}

void CGameEmailManager::copyDataTovectory(vector<EMAIL_DATA>&vEmaildata,int data)
{
    if(data==-1)
    {
            EMAIL_DATA tamep;
            for (list<CGameEmailData *>::iterator it=m_listGameEamil.begin(); it!=m_listGameEamil.end(); it++)
            {
                    tamep.emailId=(*it)->getGameEmailMsgId();
                    tamep.props=(*it)->m_mapDataProp;
                    vEmaildata.push_back(tamep);
            }
    }
    else
    {
        EMAIL_DATA tamep;
        list<CGameEmailData *>::iterator it=m_listGameEamil.begin();
        advance( it , data);
        if(it!=m_listGameEamil.end())
        {
            tamep.emailId=(*it)->getGameEmailMsgId();
           tamep.props=(*it)->m_mapDataProp;
            vEmaildata.push_back(tamep);
          return;
        }
    }
}

template <class fT, class sT>
struct second_less : public binary_function <fT , sT , bool>
{
    bool operator()(
                    const pair<fT, sT>& _1,
                    const pair<fT, sT>& _2
                    ) const
    {
        return _1.first < _2.first;
    }
};


int  CGameEmailManager::getCurrentEmailMapMaxMsgId()
{
    list<CGameEmailData *>::reverse_iterator it =m_listGameEamil.rbegin();
    if (it!=m_listGameEamil.rend()) {
        return (*it)->getGameEmailMsgId();
    }
    return 0;
}

template<class GAMEEMAILDATA,class UStatus>
class totalUnread : public binary_function<typename std::list<GAMEEMAILDATA>::value_type , UStatus, bool>
{
public:
    bool operator()(const typename list<GAMEEMAILDATA>::value_type iter, const UStatus& value) const
    {
        if (iter->getGameEmailStatus() == value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
int  CGameEmailManager::getCurrentEmailUnreadCount()
{
    return count_if(m_listGameEamil.begin(), m_listGameEamil.end(), bind2nd(totalUnread<const CGameEmailData *,int>(),0));
    ;
}
void CGameEmailManager::setCurrentTotalEmail(int value)
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("CurrentTotalEmail", value);
}
void CGameEmailManager::setUpdateTotalEmail(int value)
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("UpdateTotalEmail", value);
}
