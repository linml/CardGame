//
//  CGamePropBufferTipLayer.cpp
//  91.cube
//
//  Created by phileas on 13-10-31.
//
//

#include "CGamePropBufferTipLayer.h"
#include "CGameBufferTip.h"
#include "CPtPropUseEffectConfigData.h"
#include "gameConfig.h"

//implement class of CPropBufferLogo:


CPropBufferLogo* CPropBufferLogo::create(PropBuffer &inBuffer)
{
    CPropBufferLogo *logo = new CPropBufferLogo(inBuffer);
    if (logo && logo->initCPropBufferLogo())
    {
        logo->autorelease();
    }else
    {
        CC_SAFE_DELETE(logo);
        logo = NULL;
    }
    return logo;
}

CPropBufferLogo::CPropBufferLogo(PropBuffer &inBuffer):CBufferLogo(inBuffer)
{
    
}
CPropBufferLogo::~CPropBufferLogo()
{
    
}
bool CPropBufferLogo::initCPropBufferLogo()
{
    bool bRet = false;
    PtPropUseEffectData * data = SinglePropUseEffectConfig::instance()->getPropUseDataById(m_rBuffer.getSkillEffectId());
    if (data)
    {
        std::string icon = g_bufferIconPath+data->getIcon();
      bRet = initCBufferLog(icon);
      updateTime();
    }
    return bRet;
}


// implement class of CGamePropBufferTipLayer
CGamePropBufferTipLayer::CGamePropBufferTipLayer()
{
    m_pIconNodeContainer = NULL;
    m_pIconContainer = new CCDictionary();
    m_pPlayerBufferManager= CPlayerBufferManager::getInstance();
}

CGamePropBufferTipLayer::~CGamePropBufferTipLayer()
{
    CC_SAFE_DELETE(m_pIconContainer);
    m_pIconContainer = NULL;
}

bool CGamePropBufferTipLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCGamePropBufferTipLayer();
        bRet = true;
    } while (0);
    return bRet;
}

void CGamePropBufferTipLayer::initCGamePropBufferTipLayer()
{
    CCPoint point = ccp(80, 700);
    m_pIconNodeContainer = CCNode::create();
    m_pIconNodeContainer->setPosition(point);
    addChild(m_pIconNodeContainer);

    map<int, PropBuffer>& propBuffer = m_pPlayerBufferManager->getAllPropBuffer();
    map<int, PropBuffer>::iterator it = propBuffer.begin();
    int i = 0;
    for (; it != propBuffer.end(); it++)
    {
        
        CPropBufferLogo *logo = CPropBufferLogo::create(it->second);
        if (logo)
        {
            logo->setPosition(ccp(i*80, 0));
            m_pIconNodeContainer->addChild(logo);
            m_pIconContainer->setObject(logo, it->first);
            i++;
        }
    }
    this->schedule(schedule_selector(CGamePropBufferTipLayer::updateUI));
}

void CGamePropBufferTipLayer::updateUI(float dt)
{
    CCDictElement *element = NULL;
    CPropBufferLogo *tmpLogo = NULL;
    int lastAddId = m_pPlayerBufferManager->getLastAddPropEffectId();
    bool bHave = false;
    int i = 0;
    CCDICT_FOREACH(m_pIconContainer, element)
    {
        tmpLogo = (CPropBufferLogo*)element->getObject();
        if (tmpLogo)
        {
            int skilleffectId = tmpLogo->getBufferData().getSkillEffectId();
            if (skilleffectId == lastAddId)
            {
                bHave = true;
            }
            if(tmpLogo->getBufferData().getAltarBufferType() == KEEPTIME)
            {
                
                int result =  m_pPlayerBufferManager->getPropBufferKeepTime(skilleffectId);
                if (result == 0)
                {
                    m_pIconContainer->removeObjectForKey(skilleffectId);
                    tmpLogo->removeFromParentAndCleanup(true);
                    m_pPlayerBufferManager->clearPropBufferById(tmpLogo->getBufferData().getSkillEffectId());
                }else
                {
                    tmpLogo->setPositionX(i*80);
                    i++;
                    tmpLogo->updateTime();
                }
            }else if(tmpLogo->getBufferData().getAltarBufferType() == KEEPTIMES)
            {
                if (tmpLogo->getBufferData().getKeep() == 0)
                {
                    m_pIconContainer->removeObjectForKey(skilleffectId);
                    tmpLogo->removeFromParentAndCleanup(true);
                    m_pPlayerBufferManager->clearPropBufferById(tmpLogo->getBufferData().getSkillEffectId());
                }else
                {
                    tmpLogo->setPositionX(i*80);
                    i++;
                }
                
            }
        }
    }
    if (lastAddId != -1 && !bHave)
    {
        PropBuffer *buffer = m_pPlayerBufferManager->getLastAddPropBuffer();
        if (buffer)
        {
            tmpLogo = CPropBufferLogo::create(*buffer);
            tmpLogo->setPosition(ccp(i*80,0));
            m_pIconContainer->setObject(tmpLogo, lastAddId);
            m_pIconNodeContainer->addChild(tmpLogo);
        }
       

    }
          
}

