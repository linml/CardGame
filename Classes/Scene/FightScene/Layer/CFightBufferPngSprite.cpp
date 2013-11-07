//
//  CFightBufferPngSprite.cpp
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#include "CFightBufferPngSprite.h"
#include "CSkillData.h"
#include "gamePlayer.h"
CFightBufferPngSprite::CFightBufferPngSprite()
{
    
}

CFightBufferPngSprite::~CFightBufferPngSprite()
{
    
}

void CFightBufferPngSprite::updateNumber()
{
    if (getChildByTag(2)) {
        ((CCLabelTTF *)(getChildByTag(2)))->setString(ConvertToString(m_nKeepTime).c_str());
    }
}
void CFightBufferPngSprite::resetValue(EN_LEFTTEAMORRIGHTTEAM isLeft,int nAtkIndex)
{
    if(m_enTeamPostion==isLeft &&nAtkIndex==m_nAtkIndex)
    {
        this->m_nKeepTime=m_nBackKeepTime;
        m_nEffectTime=m_nBackEffectTime;
        updateNumber();
    }
}

void CFightBufferPngSprite::subValue(EN_LEFTTEAMORRIGHTTEAM isLeft,int nAtkIndex)
{
    if(m_enTeamPostion==isLeft &&nAtkIndex==m_nAtkIndex)
    {
        m_nKeepTime--;
        updateNumber();
    }
}

void CFightBufferPngSprite::remove(EN_LEFTTEAMORRIGHTTEAM isLeft,int effectId)
{
    if(m_enTeamPostion==isLeft && m_nEffectId==effectId)
    {
        removeFromParentAndCleanup(true);
        return ;
    }
    
}

CFightBufferPngSprite *CFightBufferPngSprite::Create(int effectId, EN_LEFTTEAMORRIGHTTEAM isLeft, int nAtkIndex)
{
    CFightBufferPngSprite *sprite=new CFightBufferPngSprite();
    if (!sprite || !sprite->initCreate(effectId,isLeft,nAtkIndex)) {
        CC_SAFE_DELETE(sprite);
        return NULL;
    }
    sprite->autorelease();
    return  sprite;
}

bool CFightBufferPngSprite::initCreate(int effectId, EN_LEFTTEAMORRIGHTTEAM isLeft, int nAtkIndex)
{
    CImapact *pImapact=SinglePlayer::instance()->findByImpactId(effectId);
    if (pImapact)
    {
        vector<string>tempstr=GameTools::splitString(pImapact->m_sEffectFile.c_str(), ",");
        if(tempstr.size()>1)
        {
            m_sBufferPng=tempstr[1];
        }
        m_nEffectTime=pImapact->m_ibuff;
        m_nKeepTime=pImapact->m_ishowtime;
        m_nBackEffectTime=m_nEffectTime;
        m_nBackKeepTime=m_nKeepTime;
        m_nEffectId=pImapact->m_ieffect_id;
        m_enTeamPostion=isLeft;
        m_nAtkIndex=nAtkIndex;
    }
    initWithFile(m_sBufferPng.c_str());
    CCLabelTTF *labelTTF=CCLabelTTF::create(ConvertToString(m_nKeepTime).c_str(), "Arial", 10);
    labelTTF->setPosition(ccp(getContentSize().width*0.5,getContentSize().height *0.5));
    addChild(labelTTF,1,2);
    return true;
    
}