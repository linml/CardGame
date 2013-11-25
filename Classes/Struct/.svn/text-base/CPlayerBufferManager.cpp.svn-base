//
//  CPlayerBufferManager.cpp
//  91.cube
//
//  Created by phileas on 13-10-25.
//
//

#include "CPlayerBufferManager.h"
#include "gameTools.h"

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

    m_cAltarBuffercontainer.reserve(10); //最多10个，为了节省空间
    m_nLastAddEffectId = -1;
    m_nLastAddPropEffectId = -1;
}

CPlayerBufferManager::~CPlayerBufferManager()
{
    
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
        CGameTimerManager::getInstance()->startTimer();
        m_nLastAddEffectId = inEffectId;
        m_cAltarBuffercontainer.push_back(AltarBuffer(inEffectId, inBufferType, inKeepTime));
    }

    
}
void CPlayerBufferManager::clearAltarBufferById(int inEffectId, bool bRemove /*= true*/)
{
    for (vector<AltarBuffer>::iterator it = m_cAltarBuffercontainer.begin(); it != m_cAltarBuffercontainer.end(); it++)
    {
        if (it->getSkillEffectId()== inEffectId)
        {
            if (bRemove)
            {
                m_cAltarBuffercontainer.erase(it);
            }
            else
            {
                it->setKeep(0);
            }
                
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

void CPlayerBufferManager::setPropBufferZero()
{
    map<int, PropBuffer>::iterator it = m_cBufferContainer.begin();
    for (; it != m_cBufferContainer.end(); it++)
    {
        it->second.setKeep(0);
    }
}

void CPlayerBufferManager::resetPropBufferByDict(CCDictionary *inPropBuffes)
{
    setPropBufferZero();
    if (inPropBuffes)
    {
        CCDictionary *tmpDict = NULL;
        CCDictElement *element = NULL;
        CCDICT_FOREACH(inPropBuffes, element)
        {
            tmpDict = (CCDictionary*) element->getObject();
            if (tmpDict)
            {
                int objectId = GameTools::intForKey("object_id", tmpDict);
                int numTyep = GameTools::intForKey("num_type", tmpDict);
                int num  = GameTools::intForKey("num", tmpDict);
                addPropBufferById(objectId, (PROPBUFFERTYPE)numTyep, num);
            }
        }
    }
}

void CPlayerBufferManager::addPropBufferById(int inEffectId, PROPBUFFERTYPE inBufferType, int inKeepTime)
{
    bool bRet = false;
    
    map<int, PropBuffer>::iterator it = m_cBufferContainer.find(inEffectId);
    if (it != m_cBufferContainer.end())
    {
        bRet = true;
        it->second.setKeep(inKeepTime);
    }
  
    if (!bRet)
    {
        CGameTimerManager::getInstance()->startTimer();
        m_nLastAddPropEffectId = inEffectId;
        m_cBufferContainer.insert(make_pair(inEffectId, PropBuffer(inEffectId, inBufferType, inKeepTime)));
    }
    
}
void CPlayerBufferManager::clearPropBufferById(int inEffectId , bool bRemove /*= true*/)
{
    map<int, PropBuffer>::iterator it = m_cBufferContainer.find(inEffectId);
    if (it != m_cBufferContainer.end())
    {
        if (bRemove)
        {
            m_cBufferContainer.erase(it);
        }
        else
        {
            it->second.setKeep(0);
        }

    }
}

int CPlayerBufferManager::subPropBufferKeepTime(int inEffectId, int inSubTime /*= 1*/, bool inRemoveZero /*= true*/)
{
    
    int nRet = -1;

    map<int, PropBuffer>::iterator it = m_cBufferContainer.find(inEffectId);
    
    if (it != m_cBufferContainer.end())
    {
        it->second.subKeepTime(inSubTime);
        nRet = it->second.getKeep();
        if (nRet <= 0)
        {
            nRet = 0;
            if(inRemoveZero)
            {
                m_cBufferContainer.erase(it);
            }
                
        }

    }
    return nRet ;
}
int CPlayerBufferManager::getPropBufferKeepTime(int inEffectId)
{
    
    return subPropBufferKeepTime(inEffectId, 0);
}


bool CPlayerBufferManager::hasPropBuffer()
{
    bool bRet = (m_cBufferContainer.size()!=0);
    return  bRet;
};

PropBuffer * CPlayerBufferManager::getLastAddPropBuffer()
{
    PropBuffer* buffer = NULL;
    if (m_nLastAddPropEffectId != -1)
    {
        map<int, PropBuffer>::iterator  it = m_cBufferContainer.find(m_nLastAddPropEffectId);
        if (it != m_cBufferContainer.end())
        {
            buffer = &(it->second);
        }
    }
    return buffer;
}
