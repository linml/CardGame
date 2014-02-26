//
//  CFightHeadLayer.cpp
//  91.cube
//
//  Created by linminglu on 14-2-26.
//
//

#include "CFightHeadLayer.h"
#include "CCard.h"
#include "CGamesCard.h"

#define FIGHTHEAD_INFO_HEAD 1
#define FIGHTHEAD_INFO_ENGRY 2
#define FIGHTHEAD_INFO_HP   3
#define FIGHTHEAD_INFO_LEVEL 4
#define FIGHTHEAD_INFO_NAME  5

CFightHeadLayer::CFightHeadLayer()
{
    if(!CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xuetiaobackgroud.png"))
    {
      CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "zhandoujiemianziyuan.plist"));
    }
    m_tempCardFactory=CGameCardFactory::getInstance();
}
CFightHeadLayer::~CFightHeadLayer()
{
    m_tempCardFactory=NULL;
}
CFightHeadLayer *CFightHeadLayer::create(CFightCard *card,int TotalHp,int TotalEngry)
{
    CFightHeadLayer *pCard=new CFightHeadLayer();
    if (!pCard||!pCard->init(card,TotalHp,TotalEngry)) {
        CC_SAFE_DELETE(pCard);
        return pCard;
    }
    pCard->autorelease();
    return pCard;
}

void CFightHeadLayer::setHpValue(int hp)
{
    
}

void CFightHeadLayer::setEngry(int engry)
{
    
}

bool CFightHeadLayer::init(CFightCard *card,int totalHp,int totalEngry)
{
    //更具card 添加一个头像。
    if (card &&m_tempCardFactory) {
        CCSprite *pSprite=m_tempCardFactory->createCardHead(card);
        addChild(pSprite,0,FIGHTHEAD_INFO_HEAD);
        pSprite->setPosition(ccp(30,35));
        //创建等级
        char tempdata[20];
        sprintf(tempdata, "LV%d",card->m_iCurrLevel);
        CCLabelTTF *pLabel=CCLabelTTF::create(tempdata, "Arial", 25);
        addChild(pLabel,0,FIGHTHEAD_INFO_LEVEL);
        pLabel->setColor(ccc3(0, 255, 0));
        pLabel->setAnchorPoint(CCPointZero);
        pLabel->setPosition(ccp(60,53));
        //创建名字
        pLabel=CCLabelTTF::create(card->m_pCard->m_scard_name.c_str(), "Arial", 25);
        addChild(pLabel,0,FIGHTHEAD_INFO_NAME);
        pLabel->setAnchorPoint(CCPointZero);
        pLabel->setPosition(ccp(120,53));
        //创建血条
    pSprite=CCSprite::createWithSpriteFrameName("xuetiaobackgroud.png");
        addChild(pSprite,0);
        pSprite->setPosition(ccp(102,30));
        pSprite->setAnchorPoint(CCPointZero);
        CCProgressTimer *left = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("xuetiaofront.png"));
        left->setType(kCCProgressTimerTypeBar);
        addChild(left,1);
        left->setPosition(ccp(102,30));
        left->setAnchorPoint(CCPointZero);
        left->setPercentage(100.0);
        //创建怒气条
    pSprite=CCSprite::createWithSpriteFrameName("xuetiaobackgroud.png");
        addChild(pSprite,0);
        pSprite->setPosition(ccp(110,15));
        pSprite->setAnchorPoint(CCPointZero);
        left = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("xuetiaofront.png"));
        left->setType(kCCProgressTimerTypeBar);
        addChild(left,1);
        left->setPosition(ccp(110,15));
        left->setAnchorPoint(CCPointZero);
    }
    
    return true;
    
}

void CFightHeadLayer::setValue(CFightCard *card,int totalHp,int totalEngry)
{
    
}
