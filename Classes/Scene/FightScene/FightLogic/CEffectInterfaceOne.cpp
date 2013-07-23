//
//  CEffectInterfaceOne.cpp
//  91.cube
//
//  Created by linminglu on 13-7-23.
//
//
#include <list>
using namespace std;
#include "CEffectInterfaceOne.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
CEffectInterfaceOne::CEffectInterfaceOne()
{
    
}
CEffectInterfaceOne::~CEffectInterfaceOne()
{
    
}
void CEffectInterfaceOne::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    int iShanghaiHp=0;
    if (pImapact->m_iParameter_1!=0 ||pImapact->m_iParameter_2!=0 || pImapact->m_iParameter_3!=0)
    {
        int tempdata=pImapact->m_iParameter_1 +
        pCard->m_attack*pImapact->m_iParameter_2/100+pMonster->m_iHp*pImapact->m_iParameter_3/100;
        iShanghaiHp=(tempdata-pMonster->m_defend >=0?tempdata-pMonster->m_defend :tempdata*0.2) ;
        iShanghaiHp*=-1;
        if (pImapact->m_ibuff > 0)
        {
            //当前算一次
            pMonster->m_iCurrHp += iShanghaiHp;
            CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff-1,pImapact->m_ishowtime-1,false,pImapact->m_iMutex,pImapact->m_iMutex_level,iShanghaiHp,EN_BUFF_FIELD_TYPE_HP);
            pMonster->appendBuffer(buffer);
        }
        else{
            pMonster->m_iCurrHp += iShanghaiHp;
        }
        
    }
    int atk=0;
    if(pImapact->m_iParameter_8!=0||pImapact->m_iParameter_9!=0)
    {
         atk=pMonster->m_attack-pImapact->m_iParameter_8 -pMonster->m_attack*pImapact->m_iParameter_9/100;
         if (atk!=0)
         {
             CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff,pImapact->m_ishowtime,false,pImapact->m_iMutex,pImapact->m_iMutex_level,-atk,EN_BUFF_FIELD_TYPE_ATTACK);
             pMonster->appendBuffer(buffer);
         }
    }
    int def=0;
    if(pImapact->m_iParameter_4 ||pImapact->m_iParameter_5)
    {
        def = (pMonster->m_defend -pImapact->m_iParameter_4)-pMonster->m_defend*pImapact->m_iParameter_5/100;
        CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff,pImapact->m_ishowtime,false,pImapact->m_iMutex,pImapact->m_iMutex_level,-def,EN_BUFF_FIELD_TYPE_DEFEND);
        pMonster->appendBuffer(buffer); 
    }
    int engry=0;
    if(pImapact->m_iParameter_6!=0 ||pImapact->m_iParameter_7!=0)
    {
     engry= (pMonster->m_iEngryMax)-pImapact->m_iParameter_6 -
        pMonster->m_iEngryMax  * pImapact->m_iParameter_7/100;
        if (engry) {
            CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff,pImapact->m_ishowtime,false,pImapact->m_iMutex,pImapact->m_iMutex_level,engry,EN_BUFF_FIELD_TYPE_ANGRY);
            pMonster->appendBuffer(buffer); 
        }
    }
  
}