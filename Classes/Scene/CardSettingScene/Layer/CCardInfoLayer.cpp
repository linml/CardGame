//
//  CCardInfoLayer.cpp
//  91.cube
//
//  Created by phileas on 13-8-14.
//
//

#include "CCardInfoLayer.h"
#include "gameConfig.h"
#include "gamePlayer.h"
#include "CSkillData.h"

CCardInfoLayer* CCardInfoLayer::create(CFightCard *card)
{
    CCardInfoLayer * layer = new CCardInfoLayer();
    if (layer->init(card))
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}

CCardInfoLayer::CCardInfoLayer()
{
    m_nTouchTag = -1;
    m_pCard = NULL;
}

CCardInfoLayer::~CCardInfoLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}

bool CCardInfoLayer::init(CFightCard * card)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initCCardInfo(card);
        bRet = true;
    } while (0);
    return bRet;
}
bool CCardInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(point, m_cTouches);
    return true;
}
void CCardInfoLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
}
void CCardInfoLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();

    if (m_nTouchTag == TouchRect::SearchTouchTag(point, m_cTouches) )
    {
        if (m_nTouchTag != -1)
        {
            handlerTouch();
        }
        
    }
}

void CCardInfoLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CCardInfoLayer::initCCardInfo(CFightCard * card)
{
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "kapaixinxijiemian.plist"));
    
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-20000);
    m_cMaps->getTouchRects(m_cTouches);

    m_pCard = CPtDisPlayCard::Create(card);
    m_pCard->setAnchorPoint(CCPointZero);
    m_pCard->setScale(3.0f);
    m_pCard->setPosition(ccp(80,90));
    m_cMaps->getElementByTags("0,0")->addChild(m_pCard);
    
    CGamePlayer * player = SinglePlayer::instance();
    // skill name:
    vector<CSkillData *> &skillData = player->m_vSkillInfo;
    int size = skillData.size();
    int skillId = card->m_pCard->m_iskillBuff;
    int array[3] = {0,1,4};
    int j = 0;
    if (skillId != 0)
    {
        for (int i = 0;  i < size; i++)
        {
            if (skillData[i]->skill_id == skillId)
            {
                CCLabelTTF * label = CCLabelTTF::create(skillData[i]->m_skillName.c_str(), "Arial", 20);
                label->setAnchorPoint(CCPointZero);
                label->setPosition(ccp(80,40));
                label->setColor(ccGREEN);
                m_cMaps->getElementByTags(array,3)->addChild(label);
                
                label = CCLabelTTF::create(skillData[i]->m_skillTrip.c_str(), "Arial", 15);
                label->setAnchorPoint(ccp(0,1));
                label->setHorizontalAlignment(kCCTextAlignmentLeft);
                label->setPosition(ccp(80,40));
                label->setDimensions(CCSizeMake(320, 0));
                m_cMaps->getElementByTags(array,3)->addChild(label);
                j++;
                break;
            }
        }
    }
    
    skillId = card->m_pCard->m_iskillDead;
    if (skillId != 0)
    {
        for (int i = 0;  i < size; i++)
        {
            if (skillData[i]->skill_id == skillId)
            {
                array[2] = 4+j;
                j++;
                CCLabelTTF * label = CCLabelTTF::create(skillData[i]->m_skillName.c_str(), "Arial", 20);
                label->setAnchorPoint(CCPointZero);
                label->setPosition(ccp(80,40));
                label->setColor(ccGREEN);
                m_cMaps->getElementByTags(array,3)->addChild(label);
                
                label = CCLabelTTF::create(skillData[i]->m_skillTrip.c_str(), "Arial", 15);
                label->setAnchorPoint(ccp(0,1));
                label->setHorizontalAlignment(kCCTextAlignmentLeft);
                label->setPosition(ccp(80,40));
                label->setDimensions(CCSizeMake(320, 0));
                m_cMaps->getElementByTags(array,3)->addChild(label);
                break;
            }
        }
    }

    skillId = card->m_pCard->m_iskillHelp;

    if (skillId != 0)
    {
        for (int i = 0;  i < size; i++)
        {
            if (skillData[i]->skill_id == skillId)
            {
               
                array[2] = 4+j;
                j++;
                CCLabelTTF * label = CCLabelTTF::create(skillData[i]->m_skillName.c_str(), "Arial", 20);
                label->setAnchorPoint(CCPointZero);
                label->setPosition(ccp(80,40));
                label->setColor(ccGREEN);
                m_cMaps->getElementByTags(array,3)->addChild(label);
                
                label = CCLabelTTF::create(skillData[i]->m_skillTrip.c_str(), "Arial", 15);
                label->setAnchorPoint(ccp(0,1));
                label->setHorizontalAlignment(kCCTextAlignmentLeft);
                label->setPosition(ccp(80,40));
                label->setDimensions(CCSizeMake(320, 0));
                m_cMaps->getElementByTags(array,3)->addChild(label);
                break;
            }
        }
    }

    // 怒气
    skillId = card->m_pCard->m_iskillLine;
    
    if (skillId != 0)
    {
        for (int i = 0;  i < size; i++)
        {
            if (skillData[i]->skill_id == skillId)
            {
                
                if (j == 3)
                {
                    array[1] = 0;
                    array[2] = 1;
                }else
                {
                    array[2] = 4+j;
                }
                
                CCLabelTTF * label = CCLabelTTF::create(skillData[i]->m_skillName.c_str(), "Arial", 20);
                label->setAnchorPoint(CCPointZero);
                label->setPosition(ccp(80,40));
                label->setColor(ccGREEN);
                m_cMaps->getElementByTags(array,3)->addChild(label);
                
                label = CCLabelTTF::create(skillData[i]->m_skillTrip.c_str(), "Arial", 15);
                label->setAnchorPoint(ccp(0,1));
                label->setHorizontalAlignment(kCCTextAlignmentLeft);
                label->setPosition(ccp(80,40));
                label->setDimensions(CCSizeMake(320, 0));
                m_cMaps->getElementByTags(array,3)->addChild(label);
                break;
            }
        }
    }

    
    
}
void CCardInfoLayer:: handlerTouch()
{
    
    switch (m_nTouchTag)
    {
        case 3001:
            removeFromParentAndCleanup(true);
            break;
            
        default:
            break;
    }
    
}