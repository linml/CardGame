//
//  CPVPStructMissionTaskReword.cpp
//  91.cube
//
//  Created by linminglu on 14-1-7.
//
//

#include "CPVPStructMissionTaskReword.h"
#include "gameConfig.h"
#include "gameStruct.h"
#include "gameTools.h"
#include "gamePlayer.h"
#include "CCard.h"
#include "CPtPropConfigData.h"



#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,__TYPECLASSNAME__) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
__TYPECLASSNAME__ *temp=*it; \
delete temp; \
temp=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
CEmrysClearVectorMemory< __TYPECLASSNAME__ *> tempClear(VECTORARRAY) ; \
tempClear.clearVector(); \
}


CPVPStructMissionTaskReword::CPVPStructMissionTaskReword()
{
    m_bMissionTaskLingQu =false;
    m_bMissionTaskKeYiLingQu=false;
 
}

CPVPStructMissionTaskReword::~CPVPStructMissionTaskReword()
{
    
}

int CPVPStructMissionTaskRewordManager::m_nTiaoZhanCishu=0;
int CPVPStructMissionTaskRewordManager::m_nTiaoZhanJifen=0;

CPVPStructMissionTaskRewordManager::CPVPStructMissionTaskRewordManager()
{
    m_vtaskRewordManager.clear();
    initLoadConfigFile();
}

CPVPStructMissionTaskRewordManager::~CPVPStructMissionTaskRewordManager()
{
    DELETE_POINT_VECTOR(m_vtaskRewordManager,vector<CPVPStructMissionTaskReword *>,CPVPStructMissionTaskReword);
}

void CPVPStructMissionTaskRewordManager::changeLingQuStatuas(CCArray *array)
{
    if(array->count()!=0)
    {
        for (int i=0; i<array->count(); i++)
        {
            int id=(int)array->objectAtIndex(i);
            for (int j=0; j<m_vtaskRewordManager.size(); j++) {
                if (m_vtaskRewordManager[j]->getMissionTaskID()==id) {
                    m_vtaskRewordManager[j]->setMissionTaskLingQu(true);
                    break;
                }
            }
            
        }
    }
    
}

void CPVPStructMissionTaskRewordManager::changeKeyiLingqu(int nTiaoZhanCiShu,int nTiaoZhanJifen)
{
    m_nTiaoZhanCishu=nTiaoZhanCiShu;
    m_nTiaoZhanJifen=nTiaoZhanJifen;
    for ( int i=0; i<m_vtaskRewordManager.size(); i++) {
        switch (m_vtaskRewordManager[i]->getMissionTaskType()) {
            case 1:
                if (!m_vtaskRewordManager[i]->getMissionTaskLingQu() && m_vtaskRewordManager[i]->getMissionTaskNum()<=nTiaoZhanCiShu) {
                    m_vtaskRewordManager[i]->setMissionTaskKeYiLingQu(true);
                }
                break;
            case 2:
                if (!m_vtaskRewordManager[i]->getMissionTaskLingQu() && m_vtaskRewordManager[i]->getMissionTaskNum()<=nTiaoZhanJifen) {
                    m_vtaskRewordManager[i]->setMissionTaskKeYiLingQu(true);
                }
                break;
            default:
                
                CCLOG("存在 PVP任务奖励不在规定内的");
                break;
        }
    }
}

string CPVPStructMissionTaskRewordManager::getDictionStr(CPVPStructMissionTaskReword *pvp)
{
    string resultValue;
    resultValue="PVP";
    switch (pvp->getMissionTaskType()) {
        case 1:
            resultValue+="挑战次数达到"+ConvertToString(pvp->getMissionTaskNum());
            break;
        case 2:
            resultValue+="挑战积分达到"+ConvertToString(pvp->getMissionTaskNum());
            break;
        default:
            break;
    }
    return resultValue;

    
    return resultValue;
}

string CPVPStructMissionTaskRewordManager::getJinDu(CPVPStructMissionTaskReword *pvp)
{
    string tempResualt;
    switch (pvp->getMissionTaskType()) {
        case 1:
            tempResualt=string("进度:")+ConvertToString(m_nTiaoZhanCishu)+"/"+ConvertToString(pvp->getMissionTaskNum());
            break;
        case 2:
            tempResualt=string("进度:")+ConvertToString(m_nTiaoZhanJifen)+"/"+ConvertToString(pvp->getMissionTaskNum());
        break;
        default:
            break;
    }
    
    return tempResualt;
    
}

string CPVPStructMissionTaskRewordManager::getItemIcon(CPVPStructMissionTaskReword *pvp)
{
        string tmpPath;
            char buffer[20]={0};
    int index=1;
    if (pvp->getMissionTaskItem()) {
        index=1;
     }
     else if(pvp->getMissionTaskCoin())
     {
         index=2;
     }else if(pvp->getMissionTaskSoul())
     {
         index=3;
     }else if(pvp->getMissionTaskCash())
     {
         index=4;
     }else if(pvp->getMissionTaskCardId())
     {
         index=5;
     }
    snprintf(buffer, 20, "%d_1.png", index);
    tmpPath = g_loginRewardIconPath + buffer;
    return tmpPath;

}


string CPVPStructMissionTaskRewordManager::getContextStr(CPVPStructMissionTaskReword *pvp)
{
    string resultContext;
    resultContext +="可以获得:";
    if (pvp->getMissionTaskCash()) {
        resultContext+="钻石:"+GameTools::ConvertToString(pvp->getMissionTaskCash())+"枚";
    }
    if (pvp->getMissionTaskCoin()) {
        resultContext+="金币:"+GameTools::ConvertToString(pvp->getMissionTaskCoin())+"金";
    }
    if (pvp->getMissionTaskSoul()) {
        resultContext+="卡魂:"+GameTools::ConvertToString(pvp->getMissionTaskSoul())+"块";
    }
    if (pvp->getMissionTaskItem())
    {
        CPtProp *m_itemProp=SinglePropConfigData::instance()->getPropById(pvp->getMissionTaskItem());
        resultContext+="道具:"+m_itemProp->getPropName();
    }
    if (pvp->getMissionTaskCardId())
    {
        CCard * card=SinglePlayer::instance()->getCardByCardId(pvp->getMissionTaskCardId());
        resultContext+="卡牌:"+card->m_scard_name;
    }
    return resultContext;
}

void CPVPStructMissionTaskRewordManager::initLoadConfigFile()
{
    CCDictionary *directory = CCDictionary::createWithContentsOfFile((resRootPath+"mission_config.plist").c_str());
    if(directory)
    {
        CCArray *vKeyArray=directory->allKeys();
        if(vKeyArray->count()!=0)
        {
            for (int i=0; i<vKeyArray->count(); i++)
            {
                CPVPStructMissionTaskReword *missionItem=new CPVPStructMissionTaskReword;
                CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
                CCDictionary *missionInfoDict=(CCDictionary*)(directory->objectForKey(key->m_sString));
                missionItem->setMissionTaskID(key->intValue());
                missionItem->setMissionTaskType(GameTools::intForKey("type", missionInfoDict));
                missionItem->setMissionTaskNum(GameTools::intForKey("num", missionInfoDict));
                missionItem->setMissionTaskSoul(GameTools::intForKey("soul", missionInfoDict));
                missionItem->setMissionTaskCoin(GameTools::intForKey("coin", missionInfoDict));
                missionItem->setMissionTaskCash(GameTools::intForKey("cash",missionInfoDict));
                missionItem->setMissionTaskItem(GameTools::intForKey("item", missionInfoDict));
                missionItem->setMissionTaskCardId(GameTools::intForKey("card", missionInfoDict));
                this->m_vtaskRewordManager.push_back(missionItem);
            }
        }
    }
}