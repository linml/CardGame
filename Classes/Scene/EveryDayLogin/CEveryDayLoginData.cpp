//
//  CEveryDayLoginData.cpp
//  91.cube
//
//  Created by linminglu on 12/9/13.
//
//

#include "CEveryDayLoginData.h"
#include "gamePlayer.h"
#include "CPtTool.h"

bool CEveryDayLoginData::getIsLogin()
{
    int nToDay=CPtTool::getToDayByTimet(m_nServerTime)-1;
    if (nToDay<31&&nToDay>=0) {
        CCLog("%d,%d",nToDay,m_nQianDaoArray[nToDay]);
        return m_nQianDaoArray[nToDay];
    }
    CCLog("日期出错啦");
    return false;
}
#include "gameConfig.h"
#include "CPtPropConfigData.h"
CEveryDayLoginDataStruct::CEveryDayLoginDataStruct()
{
    initData();
}
CEveryDayLoginDataStruct::~CEveryDayLoginDataStruct()
{
    if(m_pItemList)
    {
        deleteDropData();
        delete m_pItemList;
    }
  
}

vector<CEveryDayItemValue *> *CEveryDayLoginDataStruct::getDropData()
{
    if (m_pItemList == NULL)
    {
        loadDropData();
    }
    return m_pItemList;
}


std::string CEveryDayLoginDataStruct::getIconFullPathByType(int inType)
{
    string tmpPath;
    if (m_nIconType > 0 && m_nIconType < 5)
    {
        char buffer[20]={0};
        if (inType == 0)
        {
            snprintf(buffer, 20, "%d_1.png", m_nIconType);
           
        }else
        {
            snprintf(buffer, 20, "%d_2.png", m_nIconType);
        }
        tmpPath = g_loginRewardIconPath + buffer;
    }
    return tmpPath;
}

void CEveryDayLoginDataStruct::initData()
{
    m_bIsSignIn = false;
    m_nIconType = 5;
    m_nIconType = 0;
    m_nDay = 0;
    m_pItemList = NULL;
}

void CEveryDayLoginDataStruct::loadDropData()
{
    if (m_pItemList==NULL)
    {
        
        m_pItemList = new vector<CEveryDayItemValue *>();
        CCDictionary *dropDict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "drop_reward_config.plist"));
        if (dropDict)
        {
            CPtPropConfigData *propDatas = SinglePropConfigData::instance();
            CGamePlayer *player = SinglePlayer::instance();
            CEveryDayItemValue * tmpItem = NULL;
            char buffer[20]={0};
            snprintf(buffer,sizeof(buffer) ,"%d", m_nDropId);
            dropDict = (CCDictionary*)dropDict->objectForKey(buffer);
            
            int itemId = 0;
            if (dropDict)
            {
                for (int i = 1; i < 6; i++)
                {
                    snprintf(buffer, sizeof(buffer), "item_%d", i);
                    itemId = GameTools::intForKey(buffer, dropDict);
                    if (itemId==0) {
                        continue;
                    }
                    tmpItem = new CEveryDayItemValue();
                    if (m_nIconType == 1)
                    {
                        CCard* card =  player->getCardByCardId(itemId);
                        if (card)
                        {
                           tmpItem->m_sItemValueStr = card->m_scard_name;
                        }
                        
                    }else
                    {
                       
                        tmpItem->m_sItemValueStr = propDatas->getPropName(itemId);
                    }
                   
                    snprintf(buffer, sizeof(buffer), "num_%d", i);
                    tmpItem->count = GameTools::intForKey(buffer, dropDict);
                    m_pItemList->push_back(tmpItem);
                }
            }
        }
    }
    
}
void CEveryDayLoginDataStruct::deleteDropData()
{
   if(m_pItemList)
   {
       CEveryDayItemValue * tmpItem = NULL;
       for (int i = 0; i < m_pItemList->size(); i++)
       {
           tmpItem = m_pItemList->at(i);
           if (tmpItem)
           {
               delete tmpItem;
               m_pItemList->at(i) = NULL;
           }
       }
   }
}


CEveryDayLoginRewardDataContainer::CEveryDayLoginRewardDataContainer()
{
    m_pLoginDataContainer = vector<CEveryDayLoginDataStruct*>(31, (CEveryDayLoginDataStruct*)NULL);
    loadData();
}

CEveryDayLoginRewardDataContainer::~CEveryDayLoginRewardDataContainer()
{
    relaseData();
}

void CEveryDayLoginRewardDataContainer::initWithLoginData(const CEveryDayLoginData * loginData)
{
    if (loginData)
    {
        CEveryDayLoginDataStruct * tmpData = NULL;
        for (int i = 0; i < 31; i++)
        {
            tmpData =  m_pLoginDataContainer[i];
            if (tmpData)
            {
                tmpData->setSignIn(loginData->m_nQianDaoArray[i]);
            }
        }
    }
}

int CEveryDayLoginRewardDataContainer::getDataSize()
{
    return m_pLoginDataContainer.size();
}

CEveryDayLoginDataStruct* CEveryDayLoginRewardDataContainer::getLoginDataStructByIndex(int index)
{
    if (index >= 0 && index < m_pLoginDataContainer.size())
    {
        return m_pLoginDataContainer[index];
    }
    return NULL;
}


void CEveryDayLoginRewardDataContainer::loadData()
{
    CCDictionary * tmpDict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath
                                                                                  , "login_reward_config.plist"));
    if (tmpDict)
    {
        CCDictElement *tmpElement = NULL;
        CCDictionary * tmpValue = NULL;
        int id = 0;
        int showType = 0;
        int dropId = 0;
        CEveryDayLoginDataStruct * tmpData = NULL;
        CCDICT_FOREACH(tmpDict, tmpElement)
        {
            tmpValue = (CCDictionary*) tmpElement->getObject();
            if (tmpValue)
            {
                id = GameTools::intForKey("id", tmpValue);
                showType = GameTools::intForKey("show_type", tmpValue);
                dropId = GameTools::intForKey("drop_id", tmpValue);
                if ((id >= 1 && id <= 31)&&(m_pLoginDataContainer[id-1]==NULL))
                {
                    tmpData = new CEveryDayLoginDataStruct();
                    tmpData->setDropId(dropId);
                    tmpData->setIconType(showType);
                    tmpData->setDayInMonth(id);
                    m_pLoginDataContainer[id-1] = tmpData;
                }
            }
        }
    }
}
void CEveryDayLoginRewardDataContainer::relaseData()
{
    CEveryDayLoginDataStruct* tmp = NULL;
    for (int i = 0; i < m_pLoginDataContainer.size(); i++)
    {
        tmp = m_pLoginDataContainer[i];
        if (tmp)
        {
            delete tmp;
            m_pLoginDataContainer[i] = tmp = NULL;
        }
    }
}
