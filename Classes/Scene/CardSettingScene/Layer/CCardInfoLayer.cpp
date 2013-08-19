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
#include "CPtTool.h"

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
    CSkillData * tmp = NULL;
    int skillId = card->m_pCard->m_iskillBuff;
    int array[3] = {0,1,4};
    int j = 0;
    if (skillId != 0)
    {
        tmp = player->getSkillBySkillId(skillId);
        
        if (tmp)
        {
            CCLabelTTF * label = CCLabelTTF::create(tmp->m_skillName.c_str(), "Arial", 20);
            label->setAnchorPoint(CCPointZero);
            label->setPosition(ccp(80,40));
            label->setColor(ccGREEN);
            m_cMaps->getElementByTags(array,3)->addChild(label);
            
            label = CCLabelTTF::create(tmp->m_skillTrip.c_str(), "Arial", 15);
            label->setAnchorPoint(ccp(0,1));
            label->setHorizontalAlignment(kCCTextAlignmentLeft);
            label->setPosition(ccp(80,40));
            label->setDimensions(CCSizeMake(320, 0));
            m_cMaps->getElementByTags(array,3)->addChild(label);
            j++;
            
        }
        
    }
    
    skillId = card->m_pCard->m_iskillDead;
    if (skillId != 0)
    {
        
        tmp = player->getSkillBySkillId(skillId);
        
        if (tmp)
        {
            array[2] = 4+j;
            CCLabelTTF * label = CCLabelTTF::create(tmp->m_skillName.c_str(), "Arial", 20);
            label->setAnchorPoint(CCPointZero);
            label->setPosition(ccp(80,40));
            label->setColor(ccGREEN);
            m_cMaps->getElementByTags(array,3)->addChild(label);
            
            label = CCLabelTTF::create(tmp->m_skillTrip.c_str(), "Arial", 15);
            label->setAnchorPoint(ccp(0,1));
            label->setHorizontalAlignment(kCCTextAlignmentLeft);
            label->setPosition(ccp(80,40));
            label->setDimensions(CCSizeMake(320, 0));
            m_cMaps->getElementByTags(array,3)->addChild(label);
            j++;
            
        }
    }

    skillId = card->m_pCard->m_iskillHelp;

    if (skillId != 0)
    {
        tmp = player->getSkillBySkillId(skillId);
        
        if (tmp)
        {
            array[2] = 4+j;
            CCLabelTTF * label = CCLabelTTF::create(tmp->m_skillName.c_str(), "Arial", 20);
            label->setAnchorPoint(CCPointZero);
            label->setPosition(ccp(80,40));
            label->setColor(ccGREEN);
            m_cMaps->getElementByTags(array,3)->addChild(label);
            
            label = CCLabelTTF::create(tmp->m_skillTrip.c_str(), "Arial", 15);
            label->setAnchorPoint(ccp(0,1));
            label->setHorizontalAlignment(kCCTextAlignmentLeft);
            label->setPosition(ccp(80,40));
            label->setDimensions(CCSizeMake(320, 0));
            m_cMaps->getElementByTags(array,3)->addChild(label);
            j++;
            
        }
    }

    // 怒气
    skillId = card->m_pCard->m_iskillLine;
    
    if (skillId != 0)
    {
        tmp = player->getSkillBySkillId(skillId);
        if (tmp)
        {
            array[2] = 1;
            array[1] = 0;
            CCLabelTTF * label = CCLabelTTF::create(tmp->m_skillName.c_str(), "Arial", 20);
            label->setAnchorPoint(CCPointZero);
            label->setPosition(ccp(80,40));
            label->setColor(ccGREEN);
            m_cMaps->getElementByTags(array,3)->addChild(label);
            
            label = CCLabelTTF::create(tmp->m_skillTrip.c_str(), "Arial", 15);
            label->setAnchorPoint(ccp(0,1));
            label->setHorizontalAlignment(kCCTextAlignmentLeft);
            label->setPosition(ccp(80,40));
            label->setDimensions(CCSizeMake(320, 0));
            m_cMaps->getElementByTags(array,3)->addChild(label);
            j++;
            
        }
    }
    
    //card info:
    const char * text = "哈伦比出生于遥远的东方，少年时偷偷爬上商船出海，中途遇到海难，抓住一块木板得以生存，一直漂流到姆伦特港。到达姆伦特港后奄奄一息的哈伦比被一对老夫妇收养，度过了一段还算快乐的时光。一天夜里老夫妇被强盗袭击双双死亡，哈伦比流落街头，靠小偷小摸为生。十一岁那年，哈伦比在平民窟偷到一个钱包，却引来一个庞大盗贼团的袭击，最终死不屈服的哈伦比被盗贼团首领看中，收其为徒，传授各种本领。在之后的盗贼团叛乱中哈伦比被杀，享年17岁--哈伦比出生于遥远的东方，少年时偷偷爬上商船出海，中途遇到海难，抓住一块木板得以生存，一直漂流到姆伦特港。到达姆伦特港后奄奄一息的哈伦比被一对老夫妇收养，度过了一段还算快乐的时光。一天夜里老夫妇被强盗袭击双双死亡，哈伦比流落街头，靠小偷小摸为生。十一岁那年，哈伦比在平民窟偷到一个钱包，却引来一个庞大盗贼团的袭击，最终死不屈服的哈伦比被盗贼团首领看中，收其为徒，传授各种本领。在之后的盗贼团叛乱中哈伦比被杀，享年17岁";
    CCScrollView * scrollword = CPtTool::getScrollWord(text, CCSizeMake(400, 250), ccc3(125, 0, 0), "arial", 15);

   // scrollword->ignoreAnchorPointForPosition(false);
  //  scrollword->setAnchorPoint(ccp(0,1));
    scrollword->setPosition(ccp(530,80));
   // m_cMaps->getElementByTags("0,1,2")->addChild(scrollword);
    addChild(scrollword);
    scrollword->setTouchPriority(-20001);
    
    
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