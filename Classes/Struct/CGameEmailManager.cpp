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
//str="{"1000":19,"1001":19}"

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
            pGameEmailData->setGameEmailTime(GameTools::intForKey("time", emailDirector));
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
            CCLog("m_gameEmail====");
            m_gameEmail[pGameEmailData->getGameEmailMsgId()]=pGameEmailData;
        }
    }

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
            pGameEmailData->setGameEmailStatus(GameTools::intForKey("status",emailDirector));
            pGameEmailData->setGameEmailTime(GameTools::intForKey("time", emailDirector));
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
            m_gameEmail[pGameEmailData->getGameEmailMsgId()]=pGameEmailData;
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
    string connectData="sig=2ac2b1e302c46976beaab20a68ef95";
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
    for (map<int , CGameEmailData *>::iterator it=m_gameEmail.begin(); it!=m_gameEmail.end(); it++) {
        if(it->second)
        {
            delete  it->second;
            it->second=NULL;
        }
    }
    m_gameEmail.erase(m_gameEmail.begin(),m_gameEmail.end());
}

void CGameEmailManager::deleteEmailByEmailId(int msgID)
{
    map < int, CGameEmailData * >::iterator iter;
    iter = m_gameEmail.find(msgID);
    if (iter != m_gameEmail.end())
    {
       delete  iter->second;
       iter->second=NULL;
       m_gameEmail.erase(msgID);
    } 
}

void CGameEmailManager::loadLocalEmail()
{
        //读取本地保存的文件 放到
    
}
void CGameEmailManager::writeToFile()
{
    string str=getJsonData();
    CFileReadWrite::saveFile(str.c_str(), "emailsavefile.db");
    
}
void CGameEmailManager::cleareMyDictionaryList()
{
    for (list<CMyDictionary *>::iterator mydeleteIt=m_ldataManagerMydict.begin();mydeleteIt!=m_ldataManagerMydict.end() ;mydeleteIt++   )
    {
        if(*mydeleteIt)
        {
            CMyDictionary *tempDict=(*mydeleteIt);
            delete tempDict;
            tempDict=NULL;
        }
    }
    m_ldataManagerMydict.erase(m_ldataManagerMydict.begin(), m_ldataManagerMydict.end());

}

CMyDictionary *CGameEmailManager::createMydict()
{
    CMyDictionary *mydict=new CMyDictionary;
    this->m_ldataManagerMydict.push_back(mydict);
    return mydict;
}

string CGameEmailManager::getJsonData()
{
    string resultStr;
    list<CMyDictionary *>dataManagerMydict;
    if (m_gameEmail.size()>0) {
        CMyDictionary *mydata=createMydict();
        
        for (map<int , CGameEmailData*>::iterator it=m_gameEmail.begin(); it!=m_gameEmail.end(); it++)
        {
            if(it->second)
            {
                CGameEmailData *gameEmailData=(it->second);
                CMyDictionary *gameEmailContextDict=createMydict();
                gameEmailContextDict->InsertItem("msg_id", gameEmailData->getGameEmailMsgId());
                gameEmailContextDict->InsertItem("from_uid", gameEmailData->getGameEmailFromUid());
                gameEmailContextDict->InsertItem("type", gameEmailData->getGameEmailType());
                gameEmailContextDict->InsertItem("status", gameEmailData->getGameEmailStatus());
                gameEmailContextDict->InsertItem("time", gameEmailData->getGameEmailTime());
                gameEmailContextDict->InsertItem("title", gameEmailData->getGameEmailTitle().c_str());
                gameEmailContextDict->InsertItem("content", gameEmailData->getGameEmailContent().c_str());
                if ( gameEmailData->getGameEmailCoins()>0)
                {
                    gameEmailContextDict->InsertItem("coins", gameEmailData->getGameEmailCoins());
                }
                if(gameEmailData->getGameEmailExp()>0)
                {
                    gameEmailContextDict->InsertItem("exp", gameEmailData->getGameEmailExp());
                }
                if (gameEmailData->m_mapDataProp.size()>0)
                {
                    CMyDictionary *mypropdata=createMydict();
                    for (map<int , int>::iterator propIt=gameEmailData->m_mapDataProp.begin(); propIt!=gameEmailData->m_mapDataProp.end()   ; propIt++) {
                        char itemID[20];
                        sprintf(itemID, "%d", propIt->first);
                        mypropdata->InsertItem(itemID,propIt->second);
                    }
                    gameEmailContextDict->InsertSubItem("item", mypropdata);
                }
                char itemSubItemID[20];
                sprintf(itemSubItemID, "%d",it->first);
                mydata->InsertSubItem(itemSubItemID, gameEmailContextDict);
            }
        }
        resultStr=mydata->GetDescriptionString();
        cleareMyDictionaryList();
    }
    return  resultStr;
}

int CGameEmailManager::getMailCount()
{
    return m_gameEmail.size();
}

void CGameEmailManager::testData()
{
    CGameEmailData *gamedata=new CGameEmailData;
    gamedata->setGameEmailMsgId(1);
    m_gameEmail[2]=gamedata;
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


int  CGameEmailManager::getCurrentTotalEmail()
{
    return  CCUserDefault::sharedUserDefault()->getIntegerForKey("CurrentTotalEmail",0);
}
int  CGameEmailManager::getUpdateTotalEmail()
{
    return  CCUserDefault::sharedUserDefault()->getIntegerForKey("UpdateTotalEmail",0);    
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
   map<int, CGameEmailData *>::iterator loc = max_element(m_gameEmail.begin(), m_gameEmail.end(), second_less<int ,const CGameEmailData *>());
    if(loc!=m_gameEmail.end())
    {
        return loc->first;
    }
    return 0;
}

template<class Key, class Value,class UStatus>
class totalUnread : public binary_function<typename std::map<Key, Value>::value_type , UStatus, bool>
{
public:
    bool operator()(const typename map<Key, Value>::value_type iter, const UStatus& value) const
    {
        if (iter.second->getGameEmailStatus() == value)
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
    return count_if(m_gameEmail.begin(), m_gameEmail.end(), bind2nd(totalUnread<int ,const CGameEmailData *,int>(),0));
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
