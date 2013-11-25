//
//  CAnimationSpriteGameFight.cpp
//  91.cube
//
//  Created by linminglu on 13-7-17.
//
//

#include "CAnimationSpriteGameFight.h"
CAnimationSpriteGameFight::CAnimationSpriteGameFight()
{
    
}

CAnimationSpriteGameFight::CAnimationSpriteGameFight(
                          EN_ANIMATIONTYPE enanimationtype,
                          EN_ATKFIGHT_INDEX enAtkFight,
                          int iAtkindex,
                          int iDefIndex,
                          int iAddHp,
                          int iSubHp,
                          int iAddEngry,
                          int iSubEngry,
                          int skillid,
                          int ispritetag,string plistfile)
{
    this->m_iAddHp=iAddHp;
    this->m_iATKindex=iAtkindex;
    this->m_iDefIndex=iDefIndex;
    this->m_iSKillId=skillid;
    this->m_iSubHp=iSubHp;
    this->m_iAddEngry=iAddEngry;
    this->m_iSubEngry=iSubEngry;
    this->m_enAnimationType=enanimationtype;
    this->m_enAtkFightIndex=enAtkFight;
    this->spritetag=ispritetag;
    m_sBufferPlist=plistfile;
}

CAnimationSctrip::CAnimationSctrip()
{
     m_enAnimationType=EN_ANIMATIONTYPEREFACTOR_NONE;
     m_enAtkFightIndex=EN_ATKFIGHT_INDEX_NONE;
     m_nSubHp=0;
     m_nAddHp=0;
     m_nATKindex=0;
     m_nDEFindex=0;
     m_nSpritetag=0;
     m_nSKillId=0;
}

CAnimationSctrip::CAnimationSctrip(EN_ANIMATIONTYPEREFACTOR entype,EN_ATKFIGHT_INDEX enAtkIndex,
                 int skillId,
                 int nATKindex,
                 int nDefIndex,
                 int nAddHp,int nSubHp,string sRunActionFile)
{
    this->m_enAnimationType=entype;
    this->m_enAtkFightIndex=enAtkIndex;
    this->m_nSKillId=skillId;
    this->m_nAddHp=nAddHp;
    this->m_nSubHp=nSubHp;
    this->m_sRunActionFile=sRunActionFile;
    this->m_nATKindex=nATKindex;
    this->m_nDEFindex=nDefIndex;
    if(!sRunActionFile.empty())
    {
        m_nSpritetag=m_nSKillId;
    }
}

CAnimationSctrip::~CAnimationSctrip()
{
    
}
