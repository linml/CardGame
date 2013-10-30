//
//  CPlayerBufferManager.cpp
//  91.cube
//
//  Created by phileas on 13-10-25.
//
//

#include "CPlayerBufferManager.h"



AltarBuffer::AltarBuffer()
{
    m_eAlarBufferType = KEEPTIME;
    m_nSkillEffectId = -1;
    m_nKeep = 0;
}
AltarBuffer::AltarBuffer(int inSkillEffectId, ALTARBUFFERTYPE inBufferType, int inKeepTime) : m_nSkillEffectId(inSkillEffectId), m_eAlarBufferType(inBufferType), m_nKeep(inKeepTime)
{
    
}

AltarBuffer::~AltarBuffer()
{
    
}

void AltarBuffer::subKeepTime(int subTime /*=1*/)
{
    m_nKeep -= subTime;
}

CPlayerBufferManager* CPlayerBufferManager::s_pBufferManger = NULL;

CPlayerBufferManager* CPlayerBufferManager::getInstance()
{
    if (!s_pBufferManger)
    {
        s_pBufferManger = new CPlayerBufferManager();
    }
     return s_pBufferManger;
}

void CPlayerBufferManager::releaseBufferManager()
{
    if (s_pBufferManger)
    {
        delete s_pBufferManger;
        s_pBufferManger = NULL;
    }
}


CPlayerBufferManager::CPlayerBufferManager()
{
    m_cBufferContainer.reserve(10);
    m_cAltarBuffercontainer.reserve(10); //最多10个，为了节省空间
    m_nLastAddEffectId = -1;
}

CPlayerBufferManager::~CPlayerBufferManager()
{
    
}

void CPlayerBufferManager::clearPlayerBuffer()
{
    m_cBufferContainer.clear();
}

void CPlayerBufferManager::clearAllAltarBufferes()
{
    m_cAltarBuffercontainer.clear();
}


void CPlayerBufferManager::addAltarBufferById(int inEffectId, ALTARBUFFERTYPE inBufferType, int inKeepTime)
{
    bool bRet = false;
    for (int i = 0; i < m_cAltarBuffercontainer.size(); i++)
    {
        if (m_cAltarBuffercontainer.at(i).getSkillEffectId() == inEffectId)
        {
            bRet = true;
            m_cAltarBuffercontainer.at(i).setKeep(inKeepTime);
            break;
        }
    }
    if (!bRet)
    {
        m_nLastAddEffectId = inEffectId;
        m_cAltarBuffercontainer.push_back(AltarBuffer(inEffectId, inBufferType, inKeepTime));
    }

    
}
void CPlayerBufferManager::clearAltarBufferById(int inEffectId)
{
    for (vector<AltarBuffer>::iterator it = m_cAltarBuffercontainer.begin(); it != m_cAltarBuffercontainer.end(); it++)
    {
        if (it->getSkillEffectId()== inEffectId)
        {
            m_cAltarBuffercontainer.erase(it);
            break;
        }
    }

}

int CPlayerBufferManager::subAltarBufferKeepTime(int inEffectId, int inSubTime /*=1*/, bool inRemoveZero /* = true*/)
{
    int nRet = -1;
    vector<AltarBuffer>::iterator it = m_cAltarBuffercontainer.begin();
    for (; it != m_cAltarBuffercontainer.end(); it++)
    {
        if (it->getSkillEffectId() == inEffectId)
        {
            it->subKeepTime(inSubTime);
            nRet = it->getKeep();
            if (nRet <= 0)
            {
                nRet = 0;
                if(inRemoveZero)
                {
                    m_cAltarBuffercontainer.erase(it);
                }
                
            }
            break;
        }
    }
    return nRet ;
}
int CPlayerBufferManager::getAltarBufferKeepTime(int inEffectId)
{
    return subAltarBufferKeepTime(inEffectId, 0);
}

ALTARBUFFERTYPE CPlayerBufferManager::getAltarBufferTypeById(int inEffectId)
{
    ALTARBUFFERTYPE eRet = KEEPNONE;

    for (int i = 0; i < m_cAltarBuffercontainer.size(); i++)
    {
        if (m_cAltarBuffercontainer.at(i).getSkillEffectId() == inEffectId)
        {
           
           eRet = m_cAltarBuffercontainer.at(i).getAltarBufferType();
           break;
        }
    }

    return eRet;
}

void CPlayerBufferManager::addBufferById(int inEffectId, int inKeepTime)
{
    bool bRet = false;
    for (int i = 0; i < m_cBufferContainer.size(); i++)
    {
        if (m_cBufferContainer.at(i).effectId == inEffectId)
        {
            bRet = true;
            m_cBufferContainer.at(i).keepTime = inKeepTime;
            break;
        }
    }
    if (!bRet)
    {
        m_cBufferContainer.push_back(PLAYERBUFFERDATA(inEffectId, inKeepTime));
    }
}
void CPlayerBufferManager::subBufferById(int inEffectId)
{
    for (vector<PLAYERBUFFERDATA>::iterator it = m_cBufferContainer.begin(); it != m_cBufferContainer.end(); it++)
    {
        if (it->effectId == inEffectId)
        {
            m_cBufferContainer.erase(it);
            break;
        }
    }
}



/*
 * @breif: 倒计时buffer的keep time， 当没有或者减到负值或零时，自动删除该buffer
 * @return nRet: 3种情况：1) >0 剩余的秒数 ; 2) =0减到零了，且自动删除了该buffer ; 3) =-1 没有该buffer
 */

int CPlayerBufferManager::subBufferKeepTime(int inEffectId, int inSubTime)
{
    int nRet = -1;
    
//    map<int, int>::iterator it = m_cBufferContainer.find(inEffectId);
//    if (it != m_cBufferContainer.end())
//    {
//        it->second = -inSubTime;
//        nRet = it->second;
//        if (nRet<= 0)
//        {
//            nRet = 0;
//            m_cBufferContainer.erase(it);
//        }
//    }
//
    vector<PLAYERBUFFERDATA>::iterator it = m_cBufferContainer.begin();
    for (; it != m_cBufferContainer.end(); it++)
    {
        if (it->effectId == inEffectId)
        {
            it->keepTime -= inSubTime;
            nRet = it->keepTime;
            if (nRet <= 0)
            {
                nRet = 0;
                m_cBufferContainer.erase(it);
            }
            break;
        }
    }
    return nRet ;
}

int CPlayerBufferManager::getBufferKeepTimeById(int inEffectId)
{
    return subBufferKeepTime(inEffectId, 0);
}

