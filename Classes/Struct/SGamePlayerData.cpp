//
//  SGamePlayerData.cpp
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#include "SGamePlayerData.h"
#include "CGamePlayerStruct.h"
#include "gameTools.h"
#include "gameStruct.h"

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



CGamePlayerData::CGamePlayerData()
{
    m_nMaxPVPCount = 5;   //最大挑战次数
    m_nAddPVPCashPer = 10;
    m_nPVPCount = 0;
    m_nKaHun = 0;
    m_nTotalFightPoint = 0;
    m_nTotalRechargeValue = 0;
    m_icoin=0;
    m_icash=0;
    m_iexp=0;
    m_ilevel=0;
    m_iAp=0; //精力
    m_iVipScore=0;
    m_iAp_update_time=0;
    m_iupdate_time=0;
    m_create_time=0;
    m_guide_step=0;
    m_sign=0;
    m_friend_count=0;
    m_sLevelPlayer=NULL;
    m_irvc=0;
    m_iGp=0;
    m_nOpenGridCount = 9;
    m_nFriendly=0;
   
}
void CGamePlayerData::clearPlayerData()
{
    map<int ,SLevelPlayer *>::iterator it;
    for(it=m_gvPlayerLevel.begin();it!=m_gvPlayerLevel.end();++it)
    {
        if(it->second)
        {
            delete it->second;
            it->second=NULL;
        }
    }
    m_gvPlayerLevel.erase(m_gvPlayerLevel.begin(),m_gvPlayerLevel.end());
}
int CGamePlayerData::getGpMax()
{
    return m_sLevelPlayer->m_iGp_max;
}
int CGamePlayerData::getApMax()
{
    return m_sLevelPlayer->m_iAp_max;
}

int CGamePlayerData::getExpMax()
{
    return m_sLevelPlayer->m_iExp_max;
}

int CGamePlayerData::getFriendMax()
{
    return m_sLevelPlayer->m_iFriend_max;
}


int CGamePlayerData::getCardBagAppend()
{
    return m_nCardAppend;
}

void CGamePlayerData::appendCardBagAppend()
{
    m_nCardAppend += 10;
    m_nCardAppend = m_nCardAppend >= EXTENTCARDBAG_MAX ? EXTENTCARDBAG_MAX : m_nCardAppend;
}

void CGamePlayerData::settestInit(int level)
{
    m_sLevelPlayer=m_gvPlayerLevel[level];
}

void CGamePlayerData::logicValue(int &OutValue,int inValue, char logicChar)
{
    switch (logicChar) {
        case '+':
            OutValue+=inValue;
            break;
            case '-':
            OutValue-=inValue;
            break;
            case '*':
            OutValue *=inValue;
            break;
            case '/':
            CCAssert(inValue!=0, "FUCK  U input zero Value");
            OutValue-=inValue;
            break;
        default:
            break;
    }
    
}




int CGamePlayerData::appendGp(int iValue)
{
    m_iGp += iValue;
    if(m_iGp<=0)
    {
        m_iGp=0;
    }
    else if(m_iGp>m_sLevelPlayer->m_iGp_max)
    {
        m_iGp=m_sLevelPlayer->m_iGp_max;

    }
    return m_iGp;
}


int CGamePlayerData::appendAp(int  iValue)
{
     m_iAp +=iValue;
    if (m_iAp<0) {
        m_iAp=0;
    }
    else if(m_iAp>m_sLevelPlayer->m_iAp_max){
        m_iAp=m_sLevelPlayer->m_iAp_max;
    }
    return m_iAp;
}


void CGamePlayerData::decodeDictnory(CCDictionary *dict)
{
    m_icoin=GameTools::intForKey("coin", dict);
    m_icash=GameTools::intForKey("cash", dict);
    m_iexp=GameTools::intForKey("exp", dict);
    m_ilevel=GameTools::intForKey("level", dict);
    m_nFriendly=GameTools::intForKey("friendly", dict);    
    this->m_sLevelPlayer=m_gvPlayerLevel[m_ilevel];
    
    CCAssert(this->m_sLevelPlayer!=NULL, "FUCH FUWUDUAN 传递一个配置表里面没有的等级");
    
    m_iAp=GameTools::intForKey("ap", dict); //体力
    m_iAp_update_time=GameTools::intForKey("ap_update_time", dict);
    m_iVipScore=GameTools::intForKey("vip_score", dict);
    m_irvc= this->m_sLevelPlayer->m_iLeader_max; //GameTools::intForKey("leader", dict);
    m_iupdate_time=GameTools::intForKey("update_time",dict);
    m_create_time=GameTools::intForKey("create_time", dict);
    m_guide_step=GameTools::intForKey("guide_step", dict);
    m_sign=GameTools::intForKey("sign",dict);
    m_friend_count=GameTools::intForKey("friend_max",dict);
    m_susername=GameTools::valueForKey("username", dict);
    m_nOpenGridCount = GameTools::intForKey("max_bag", dict);
    m_nCardAppend = GameTools::intForKey("card_max_buy", dict);
    m_iGp=GameTools::intForKey("gp", dict);
    
    // 卡魂：
    m_nKaHun = GameTools::intForKey("card_soul", dict);
    m_nPVPCount = GameTools::intForKey("pvp_num", dict);
    
    //是否首充
    m_bFirstRecharge=GameTools::intForKey("recharge", dict);
    
    //获取性别 wood modify
    m_nGender = GameTools::intForKey("sex", dict);
    
    CCUserDefault::sharedUserDefault()->setStringForKey("name",m_susername);

}


CGamePlayerData::~CGamePlayerData()
{
    
}