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
    CCLog("asdfasdfasdfasdfasdfa,%d",pImapact->m_ieffect_id);
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
        if ((pImapact->m_ishowtime==1 && pImapact->m_process)||(pImapact->m_ishowtime>1))
        {
            CCardBufferStatusRefactor *bufferRefactor=new CCardBufferStatusRefactor(iChangeShanghaiHp,iChangeatk,iChangedef,iChangeengry,pImapact->m_ieffect_id,bufferfile);
            if(!pMonster->appendBufferData(bufferRefactor))
            {
                needDealWithHpAtkDefImmideala=false;
            }
             m_sNeadAppendBufferFile=SinglePlayer::instance()->getBufferPlistByEffectId(pImapact->m_ieffect_id);
        }
        if (needDealWithHpAtkDefImmideala)
        {
            m_sNeadAppendBufferFile="";
            pMonster->appendHp(iChangeShanghaiHp);
            pMonster->subAtk(-iChangeatk);
            pMonster->subDef(-iChangedef);
            CCLog("iChangeengry:%d,%d",pMonster->m_iCurrEngry,iChangeengry);
            pMonster->appendEngry(iChangeengry);
        }
    }
    else
    {
        
        CCardBufferStatusRefactor *bufferRefactor=new CCardBufferStatusRefactor(iChangeShanghaiHp,iChangeatk,iChangedef,iChangeengry,pImapact->m_ieffect_id,bufferfile);
        bool isNeedAdd=true;
        CCLog("pImapact->m_ieffect_id:%d",pImapact->m_ieffect_id);
        if(pMonster->isHaveBufferRefactor(pImapact->m_ieffect_id)) //如果身上有这个特效的ID的话。
        {
            isNeedAdd=false; //就不必要 添加这个文件
        }
        if(!pMonster->appendBufferData(bufferRefactor)) //能不能添加这个buffer 如果不能的话 也没必要添加这个文件
        {
            isNeedAdd=false;
        }
        if (isNeedAdd)
        {
            m_sNeadAppendBufferFile=SinglePlayer::instance()->getBufferPlistByEffectId(pImapact->m_ieffect_id);//需要播放的特效
        }
        CCLog("m_sNeadAppendBufferFile :%s",m_sNeadAppendBufferFile.c_str());
    }
       
}