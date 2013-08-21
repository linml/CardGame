//
//  CEffectInterface.cpp
//  91.cube
//
//  Created by linminglu on 13-7-23.
//
//

#include "CEffectInterface.h"
#include "CCard.h"
#include "CSkillData.h"
#include "gamePlayer.h"
#include "CCardBufferStatus.h"
CEffectInterface::CEffectInterface()
{
    iChangeShanghaiHp=0;
    iChangeatk=0;
    iChangedef=0;
    iChangeengry=0;
}
CEffectInterface::~CEffectInterface()
{
    
}
void CEffectInterface::dealWithBufferAndImmediately(CFightCard *pCard, CFightCard *pMonster, CImapact *pImapact,std::string bufferfile)
{
    if(pImapact->m_irandom)
    {
        CGamePlayer *gamePlayer= SinglePlayer::instance();
        gamePlayer->logicRandValue(iChangeShanghaiHp);
        gamePlayer->logicRandValue(iChangeatk);
        gamePlayer->logicRandValue(iChangedef);
        gamePlayer->logicRandValue(iChangeengry);
    }
    if(pImapact->m_ifdelay==0)
    {
        bool needDealWithHpAtkDefImmideala=true;
        if (pImapact->m_ishowtime>1)
        {
            CCardBufferStatusRefactor *bufferRefactor=new CCardBufferStatusRefactor(iChangeShanghaiHp,iChangeatk,iChangedef,iChangeengry,pImapact->m_ieffect_id,bufferfile);
            if(!pMonster->appendBufferData(bufferRefactor))
            {
                needDealWithHpAtkDefImmideala=false;
            }
        }
        if (needDealWithHpAtkDefImmideala)
        {
            pMonster->appendHp(iChangeShanghaiHp);
            pMonster->m_attack += -iChangeatk;
            pMonster->m_defend += -iChangedef;
            pMonster->appendEngry(iChangeengry);
        }
    }
    else
    {
        CCardBufferStatusRefactor *bufferRefactor=new CCardBufferStatusRefactor(iChangeShanghaiHp,iChangeatk,iChangedef,iChangeengry,pImapact->m_ieffect_id,bufferfile);
        pMonster->appendBufferData(bufferRefactor);
    }
       
}