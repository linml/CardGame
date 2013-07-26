//
//  CEffectInterfaceEleven.cpp
//  91.cube
//
//  Created by linminglu on 13-7-25.
//
//

#include "CEffectInterfaceEleven.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gameConfig.h"
CEffectInterfaceEleven::CEffectInterfaceEleven()
{
    
}
CEffectInterfaceEleven::~CEffectInterfaceEleven()
{
    
}


void CEffectInterfaceEleven::logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)
{
    int iShanghaiHp=0;
    if (pImapact->m_iParameter_1!=0 ||pImapact->m_iParameter_2!=0 || pImapact->m_iParameter_3!=0)
    {
        
        //  恢复血量值的计算公式 parameter_1+自身卡牌攻击力*（parameter_2/100)+目标总血量*parameter_3/100
        iShanghaiHp=pImapact->m_iParameter_1 +
        pCard->m_attack*pImapact->m_iParameter_2/100+pMonster->m_iHp*pImapact->m_iParameter_3/100;
        if (pImapact->m_ibuff > 0)
        {
            //当前算一次
            pMonster->m_iCurrHp += iShanghaiHp;
            CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff-1,pImapact->m_ishowtime-1,false,pImapact->m_iMutex,pImapact->m_iMutex_level,iShanghaiHp,pImapact->m_ieffect_id,EN_BUFF_FIELD_TYPE_HP);
            pMonster->appendBuffer(buffer);
        }
        else
        {
            pMonster->m_iCurrHp += iShanghaiHp;
        }
        
    }
    int atk=0;
    if(pImapact->m_iParameter_8!=0||pImapact->m_iParameter_9!=0)
    {
        atk= +pImapact->m_iParameter_8 + pMonster->m_attack*pImapact->m_iParameter_9/100;
        if (atk!=0)
        {
            CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff-1,pImapact->m_ishowtime,false,pImapact->m_iMutex,pImapact->m_iMutex_level,atk,pImapact->m_ieffect_id,EN_BUFF_FIELD_TYPE_ATTACK);
            pMonster->appendBuffer(buffer);
            pMonster->m_attack += atk;
        }
    }
    int def=0;
    if(pImapact->m_iParameter_4 || pImapact->m_iParameter_5)
    {
        def =  pImapact->m_iParameter_4  + pMonster->m_defend*pImapact->m_iParameter_5/100;
        CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff-1,pImapact->m_ishowtime,false,pImapact->m_iMutex,pImapact->m_iMutex_level,-def,pImapact->m_ieffect_id,EN_BUFF_FIELD_TYPE_DEFEND);
        pMonster->appendBuffer(buffer);
        pMonster->m_defend += def;
        CCLog("def + %d",def);
    }
    int engry=0;
    if(pImapact->m_iParameter_6!=0 ||pImapact->m_iParameter_7!=0)
    {
        engry= +pImapact->m_iParameter_6 +
        pMonster->m_iEngryMax  * pImapact->m_iParameter_7/100;
        if (engry) {
            pMonster->m_iCurrEngry += engry;
            CCardBufferStatus *buffer=new CCardBufferStatus(pImapact->m_ibuff-1,pImapact->m_ishowtime-1,false,pImapact->m_iMutex,pImapact->m_iMutex_level,+engry,pImapact->m_ieffect_id,EN_BUFF_FIELD_TYPE_ANGRY);
            pMonster->appendBuffer(buffer);
        }
    }
}