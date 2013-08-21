
//
//  CCardBufferStatus.cpp
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#include "CCardBufferStatus.h"
#include "gamePlayer.h"
#include "CSkillData.h"

CCardBufferStatusRefactor::CCardBufferStatusRefactor(int iHp,int iAtk,int iDef,int iEngry,int EffectId,string skillBufferFile)
{
    this->m_iHp=iHp;
    this->m_iEffectid=EffectId;
    this->m_iEngry=iEngry;
    this->m_iDef=iDef;
    this->m_iAtk=iAtk;
    this->m_sbfferFile=skillBufferFile;
    this->m_iMutex=-1;
    this->m_iMutexLevel=-1;
    this->m_iNeedAddBack=0;
    this->m_iKeepTime=0;
    this->m_iEffect_time=0;
    CImapact *pImapactEffect =SinglePlayer::instance()->getEffectTableByEffectId(EffectId);
    if (pImapactEffect) {
        this->m_iMutex=pImapactEffect->m_iMutex;
        this->m_iMutexLevel=pImapactEffect->m_iMutex_level;
        this->m_iNeedAddBack=pImapactEffect->m_process;
        this->m_iKeepTime=pImapactEffect->m_ishowtime;
        this->m_iEffect_time=pImapactEffect->m_ibuff;
        if (pImapactEffect->m_process) {
            this->m_iKeepTime -=1;
            this->m_iEffect_time -=1;
        }

    }    
}