//
//  CRankDataManager.cpp
//  91.cube
//
//  Created by phileas on 13-12-3.
//
//

#include "CRankDataManager.h"
#include "PtJsonUtility.h"
#include "gameConfig.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"

#include <vector>
using namespace std;
// local tool method:

static bool compareRank(CRankData * param1, CRankData *param2);

// test:
void test(vector<CRankData*> * rankdata)
{
    srand(time(NULL));
    char data[20];
    for (int i = 0; i < 100; i++)
    {
        sprintf(data, "gameplayername%d",i);
        rankdata->at(i) =new CRankData(data,i+1,(rand()%80)+1);
    }
     sort(rankdata->begin(), rankdata->end(), compareRank);

}

CRankDataManager * CRankDataManager::getInstance()
{
    if (s_pIntance == NULL)
    {
        s_pIntance = new CRankDataManager();
    }
    return s_pIntance;
}
void CRankDataManager::releaseDataManager()
{
    if (s_pIntance)
    {
        delete s_pIntance;
        s_pIntance = NULL;
    }
}

CRankDataManager * CRankDataManager::s_pIntance = NULL;


void CRankDataManager::startRankDataManger()
{
    if ((time(NULL)-m_tstartTime) > INTERVALTIME)
    {
        m_tstartTime = time(NULL);
        releaseData(false);
    }
}

vector<CRankData*> * CRankDataManager::sendRequest(int inTab, CCObject *pTarget, SEL_CallFunc pCallabck)
{
    vector<CRankData *>* tmp = getDataByType(inTab);
    if (tmp)
    {
        return tmp;
    }
    CC_SAFE_RETAIN(pTarget);
    CC_SAFE_RELEASE(m_pDataCallbackHander);
    m_pDataCallbackHander = pTarget;
    m_pDataCallback = pCallabck;

    switch (inTab)
    {
        case FIGHTRANK:
            onSendRequestGetFightRank();
            break;
        case LEVELRANK:
            onSendRequestGetLevelRank();
            break;
        case RECHARGERANK:
            onSendRequestGetRechargeRank();
            break;
        default:
            break;
    }
    return NULL;
}

vector<CRankData*> * CRankDataManager::getDataByType(int inTab)
{
    vector<CRankData *>* tmpVector  = NULL;
    bool flag = false;
    switch (inTab)
    {
        case 0:
            tmpVector = m_pFightRanks;
            flag = m_bFightReady;
            break;
        case 1:
            tmpVector = m_pLevelRanks;
            flag = m_bLevelReady;
            break;
        case 2:
            tmpVector = m_pRechargeRanks;
            flag = m_bRechargeReady;
            break;
        default:
            break;
    }
   
    return flag ? tmpVector : NULL;

}

CRankData * CRankDataManager::getRankDataByUid(int inUid, int inType , int &outRank)
{
    vector<CRankData *> *tmpVector = NULL;
    switch (inType)
    {
        case 0:
            tmpVector = m_pFightRanks;
            break;
        case 1:
            tmpVector = m_pLevelRanks;
            break;
        case 2:
            tmpVector = m_pRechargeRanks;
            break;
        default:
            break;
    }
    
    CRankData * tmpData = NULL;
    if (tmpVector)
    {
        for (int i = 0; i < tmpVector->size(); i++)
        {
            tmpData = tmpVector->at(i);
            if (tmpData && tmpData->uid == inUid)
            {
                outRank = i+1;
                break;
            }
        }
    }
    return tmpData;

}




CRankDataManager::CRankDataManager()
{
    initData();
}
CRankDataManager::~CRankDataManager()
{
    releaseData();
}

void CRankDataManager::initData()
{
    m_pDataCallback = NULL;
    m_pDataCallbackHander = NULL;
    m_pFightRanks = NULL;
    m_pLevelRanks = NULL;
    m_pRechargeRanks = NULL;
    m_tstartTime = NULL;
    m_bFightReady = false;
    m_bLevelReady = false;
    m_bRechargeReady = false;
}

void CRankDataManager::allocalVector(vector< CRankData * >* &data)
{
    if (data == NULL)
    {
        data = new vector<CRankData*>(100, (CRankData*)NULL);
    }
}
void CRankDataManager::releaseData(bool inDeleteContainer)
{
    for (int i = 0; i < 3; i++)
    {
        releaseRankData(i);
    }
    if (inDeleteContainer)
    {
        CC_SAFE_DELETE(m_pRechargeRanks);
        CC_SAFE_DELETE(m_pLevelRanks);
        CC_SAFE_DELETE(m_pFightRanks);
        m_pFightRanks = m_pLevelRanks = m_pRechargeRanks = NULL;
    }else
    {
        m_bFightReady = false;
        m_bLevelReady = false;
        m_bRechargeReady = false;
    }
    
}

void CRankDataManager::callBackFunc()
{
    if(m_pDataCallbackHander && m_pDataCallback)
    {
        (m_pDataCallbackHander->*m_pDataCallback)();
    }
}

void CRankDataManager::paraRankData(int inType,CCArray *pArray)
{
    vector<CRankData *> *tmpVector = NULL;
    switch (inType)
    {
        case 0:
            tmpVector = m_pFightRanks;
            break;
        case 1:
            tmpVector = m_pLevelRanks;
            break;
        case 2:
            tmpVector = m_pRechargeRanks;
            break;
        default:
            break;
    }
    if (tmpVector && pArray)
    {
        CCDictionary * tmpDict = NULL;
        for (int i = 0; i < pArray->count() && i < 100; i++)
        {
            tmpDict = (CCDictionary*) pArray->objectAtIndex(i);
            if (tmpVector->at(i))
            {
                tmpVector->at(i)->setData(GameTools::valueForKey("name", tmpDict), GameTools::intForKey("uid", tmpDict), GameTools::intForKey("num", tmpDict));
            }
            else
            {
                tmpVector->at(i) = new CRankData(GameTools::valueForKey("name", tmpDict), GameTools::intForKey("uid", tmpDict), GameTools::intForKey("num", tmpDict));
            }
        }
    
    }

}

void CRankDataManager::releaseRankData(int inTab)
{
    vector<CRankData *> *tmpVector = NULL;
    switch (inTab)
    {
        case 0:
            tmpVector = m_pFightRanks;
            break;
        case 1:
            tmpVector = m_pLevelRanks;
            break;
        case 2:
            tmpVector = m_pRechargeRanks;
            break;
        default:
            break;
    }
    if (tmpVector)
    {
        
        for (int i = 0; i < tmpVector->size(); i++)
        {
            if (tmpVector->at(i))
            {
                delete tmpVector->at(i);
                tmpVector->at(i) = NULL;
            }
        }
    }
}


void CRankDataManager::onSendRequestGetFightRank()
{
    char buffer[100]= {0};
    sprintf(buffer, "sig=%s", STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETFIGHTRANK(196), "CALLBACK_CRankDataManager::onSendRequestGetFightRank", "REQUEST_CRankDataManager::onSendRequestGetFightRank",buffer, callfuncO_selector(CRankDataManager::onReceiveDataFightRank));
}
void CRankDataManager::onSendRequestGetLevelRank()
{
    char buffer[100]= {0};
    sprintf(buffer, "sig=%s", STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETLEVELRANK(196), "CALLBACK_CRankDataManager::onSendRequestGetLevelRank", "REQUEST_CRankDataManager::onSendRequestGetLevelRank",buffer, callfuncO_selector(CRankDataManager::onReceiveDataLevelRank));
}
void CRankDataManager::onSendRequestGetRechargeRank()
{
    char buffer[100]= {0};
    sprintf(buffer, "sig=%s", STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETCASHRANK(196), "CALLBACK_CRankDataManager::onSendRequestGetRechargeRank", "REQUEST_CRankDataManager::onSendRequestGetRechargeRank",buffer ,callfuncO_selector(CRankDataManager::onReceiveDataRechageRank));

}

void CRankDataManager::onReceiveDataFightRank(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CRankDataManager::onSendRequestGetFightRank");
    allocalVector(m_pFightRanks);
    char *buffer = (char *)pObject;
    if (buffer)
    {
        CCLog("the buffer: %s", buffer);
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                resultDict = (CCDictionary*) resultDict->objectForKey("result");
                paraRankData(FIGHTRANK, (CCArray*) resultDict->objectForKey("charts"));
                m_bFightReady = true;
                int fp = GameTools::intForKey("user_fp", resultDict);
                SinglePlayer::instance()->setTotalFightPoint(fp);
                callBackFunc();
            }
            else
            {
                
            }
          
        }
    }
}

void CRankDataManager::onReceiveDataLevelRank(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CRankDataManager::onSendRequestGetLevelRank");
    allocalVector(m_pLevelRanks);
    char *buffer = (char *)pObject;
    if (buffer)
    {
        CCLog("the buffer: %s", buffer);
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                resultDict = (CCDictionary*) resultDict->objectForKey("result");
                paraRankData(LEVELRANK, (CCArray*) resultDict->objectForKey("charts"));
                m_bLevelReady = true;
                callBackFunc();
            }
            else
            {
                
            }
            
        }
    }
}
void CRankDataManager::onReceiveDataRechageRank(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CRankDataManager::onSendRequestGetRechargeRank");
    allocalVector(m_pRechargeRanks);
    char *buffer = (char *)pObject;
    if (buffer)
    {
        CCLog("the buffer: %s", buffer);
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                resultDict = (CCDictionary*) resultDict->objectForKey("result");
                paraRankData(RECHARGERANK, (CCArray*) resultDict->objectForKey("charts"));
                int recharge = GameTools::intForKey("user_cash", resultDict);
                SinglePlayer::instance()->setTotalRechargeValue(recharge);
                m_bRechargeReady = true;
                callBackFunc();
            }
            else
            {
                
            }
            
        }
    }
    
}


bool compareRank(CRankData * param1, CRankData *param2)
{
    int result = 0;
    if (param1 && param2)
    {
        result = param1->value-param2->value;
        if(result==0)
        {
            result = param2->uid - param1->uid;
        }
    }
    if(result > 0)
    {
       return  true;
    }
    else
    {
        return false;
    }
}
