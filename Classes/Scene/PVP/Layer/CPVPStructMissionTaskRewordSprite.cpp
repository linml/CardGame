//
//  CPVPStructMissionTaskRewordSprite.cpp
//  91.cube
//
//  Created by linminglu on 14-1-8.
//
//

#include "CPVPStructMissionTaskRewordSprite.h"
#include "CPVPStructMissionTaskReword.h"
CPVPStructMissionTaskRewordSprite::CPVPStructMissionTaskRewordSprite()
{
    
}
CPVPStructMissionTaskRewordSprite::~CPVPStructMissionTaskRewordSprite()
{
    
}
CPVPStructMissionTaskRewordSprite *CPVPStructMissionTaskRewordSprite::CreateByPvpData(CPVPStructMissionTaskReword *pvp)
{
    CPVPStructMissionTaskRewordSprite *pSprite=new CPVPStructMissionTaskRewordSprite;
    if (!pSprite || !pSprite->init(pvp)) {
        delete pSprite;
        pSprite=NULL;
        return NULL;
    }
    pSprite->autorelease();
    return pSprite;
}
bool CPVPStructMissionTaskRewordSprite::init(CPVPStructMissionTaskReword *pvp)
{
    m_pCPVPStructMissionTaskReword=pvp;
    createBackgroud();
    createTitle();
    createContext();
    createJindu();
    createFlag();
    createItemIcon();
    setContentSize(CCSizeMake(250,100));
    return true;
}

void CPVPStructMissionTaskRewordSprite::createBackgroud()
{
    CCLayerColor *player=CCLayerColor::create(ccc4(255, 0, 0,120), 250, 100);
    addChild(player,1);
    
}

void CPVPStructMissionTaskRewordSprite::createItemIcon()
{
    CCSprite *sprite=CCSprite::create(CPVPStructMissionTaskRewordManager::getItemIcon(m_pCPVPStructMissionTaskReword).c_str());
    sprite->setPosition(ccp(40, 50));
    addChild(sprite,1);
}

void CPVPStructMissionTaskRewordSprite::createTitle()
{
    CCLabelTTF *labelTTF=CCLabelTTF::create(CPVPStructMissionTaskRewordManager::getDictionStr(m_pCPVPStructMissionTaskReword).c_str(),"Arial",15);
    labelTTF->setDimensions(CCSizeMake(200, 50));
    labelTTF->setPosition(ccp(40, 55));
    labelTTF->setAnchorPoint(CCPointZero);
    addChild(labelTTF,1);
}


void CPVPStructMissionTaskRewordSprite::createContext()
{
    CCLabelTTF *labelTTF=CCLabelTTF::create(CPVPStructMissionTaskRewordManager::getContextStr(m_pCPVPStructMissionTaskReword).c_str(),"Arial",15);
    labelTTF->setDimensions(CCSizeMake(200, 20));
    labelTTF->setPosition(ccp(40, 40));
    labelTTF->setAnchorPoint(CCPointZero);
    addChild(labelTTF,1);
}

void CPVPStructMissionTaskRewordSprite::createJindu()
{
    CCLabelTTF *labelTTF=CCLabelTTF::create(CPVPStructMissionTaskRewordManager::getJinDu(m_pCPVPStructMissionTaskReword).c_str(),"Arial",15);
    labelTTF->setDimensions(CCSizeMake(200, 20));
    labelTTF->setPosition(ccp(40, 0));
    labelTTF->setAnchorPoint(CCPointZero);
    addChild(labelTTF,1);
}
void CPVPStructMissionTaskRewordSprite::createFlag()
{
    if (m_pCPVPStructMissionTaskReword->getMissionTaskLingQu()) {
        CCLabelTTF *label=CCLabelTTF::create("已经领取", "Arial", 25);
        label->setColor(ccc3(0, 255, 0));
        label->setPosition(ccp(200,90));
        addChild(label,1);
    }
    else if(m_pCPVPStructMissionTaskReword->getMissionTaskKeYiLingQu())
    {
        CCLabelTTF *label=CCLabelTTF::create("可领取", "Arial", 25);
        label->setColor(ccc3(0, 255, 0));
        label->setPosition(ccp(200,90));
        addChild(label,1);
        
    }
}
