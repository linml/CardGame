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
 
    m_tempCardFactory=CGameCardFactory::getInstance();
}
CFightHeadLayer::~CFightHeadLayer()
{
    m_tempCardFactory=NULL;
}
CFightHeadLayer *CFightHeadLayer::create(CFightCard *card,int TotalHp,int TotalEngry,bool isLeft)
{
    CFightHeadLayer *pCard=new CFightHeadLayer();
    if (!pCard||!pCard->init(card,TotalHp,TotalEngry,isLeft)) {
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
void CFightHeadLayer::FilpXSprite(int Width,bool isLeft,CCSprite *sprite)
{
    if (sprite) {
        if (!isLeft) {
            sprite->setFlipX(true);
            sprite->setPosition(ccp(Width-sprite->getPosition().x,sprite->getPosition().y));
            sprite->setAnchorPoint(CCPoint(1, 0));
        }
    }
}


bool CFightHeadLayer::setNewInit(CFightCard *card)
{
    if (card) {
        ((CCSprite *)getChildByTag(FIGHTHEAD_INFO_HEAD))->setTexture(m_tempCardFactory->createCardHead(card)->getTexture());
        char tempdata[20];
        sprintf(tempdata, "LV%d",card->m_iCurrLevel);
        ((CCLabelTTF *)getChildByTag(FIGHTHEAD_INFO_LEVEL))->setString(tempdata);
        ((CCLabelTTF *)getChildByTag(FIGHTHEAD_INFO_NAME))->setString(card->m_pCard->m_scard_name.c_str());
    }
    return true;
    
}

bool CFightHeadLayer::init(CFightCard *card,int totalHp,int totalEngry,bool isLeft)
{
    //更具card 添加一个头像。
    if (card &&m_tempCardFactory) {
        int theLayerWidth=440;
        setContentSize(CCSizeMake(theLayerWidth, 70));
        CCSprite *pSprite=m_tempCardFactory->createCardHead(card);
        addChild(pSprite,0,FIGHTHEAD_INFO_HEAD);
        if (isLeft) {

        }
        pSprite->setAnchorPoint(CCPoint(0, 0));
        pSprite->setPosition(ccp(0,0));
        FilpXSprite(theLayerWidth,isLeft,pSprite);
        //创建等级
        char tempdata[20];
        sprintf(tempdata, "LV%d",card->m_iCurrLevel);
        CCLabelTTF *pLabel=CCLabelTTF::create(tempdata, "Arial", 25);
        addChild(pLabel,0,FIGHTHEAD_INFO_LEVEL);
        pLabel->setColor(ccc3(0, 255, 0));
        pLabel->setAnchorPoint(CCPointZero);
        pLabel->setPosition(ccp(100,53));
        //创建名字
        pLabel=CCLabelTTF::create(card->m_pCard->m_scard_name.c_str(), "Arial", 25);
        addChild(pLabel,0,FIGHTHEAD_INFO_NAME);
        pLabel->setAnchorPoint(CCPointZero);
        pLabel->setPosition(ccp(160,53));
        //创建血条
    pSprite=CCSprite::createWithSpriteFrameName("xuetiaobackgroud.png");
        addChild(pSprite,0);
        pSprite->setPosition(ccp(132,30));
        pSprite->setAnchorPoint(CCPointZero);
        FilpXSprite(theLayerWidth,isLeft,pSprite);
    pSprite=CCSprite::createWithSpriteFrameName("xuetiaofront.png");
        CCProgressTimer *left;
        if (isLeft) {
            left= CCProgressTimer::create(pSprite);
            left->setPosition(ccp(132,30));
            left->setAnchorPoint(CCPointZero);
            left->setMidpoint(CCPoint(0, 0));

        }
        else{
            pSprite->setFlipX(true);
            left= CCProgressTimer::create(pSprite);
            left->setPosition(ccp(308,30));
            left->setAnchorPoint(CCPoint(1, 0));
            left->setMidpoint(CCPoint(1, 0));
        }
        left->setBarChangeRate(ccp(1,0));//
        addChild(left,1,FIGHTHEAD_INFO_HP);
        left->setType(kCCProgressTimerTypeBar);
        left->setPercentage(100.0);
        
        //创建怒气条
        pSprite=CCSprite::createWithSpriteFrameName("danuqibackgroud.png");
        addChild(pSprite,0);
        pSprite->setPosition(ccp(110,20));
        pSprite->setAnchorPoint(CCPointZero);
        FilpXSprite(theLayerWidth,isLeft,pSprite);
        m_nMaxValueEngry=card->m_iEngryMax;
        pSprite=CCSprite::createWithSpriteFrameName("danuqifront.png");
        if (isLeft) {
            left= CCProgressTimer::create(pSprite);
            left->setPosition(ccp(110,20));
            left->setAnchorPoint(CCPointZero);
            left->setMidpoint(CCPoint(0, 0));
        }
        else{
            pSprite->setFlipX(true);
            left= CCProgressTimer::create(pSprite);
            left->setPosition(ccp(330,20));
            left->setAnchorPoint(CCPoint(1, 0));
            left->setMidpoint(CCPoint(1, 0));
            
        }
        left->setBarChangeRate(ccp(1,0));//
        addChild(left,1,FIGHTHEAD_INFO_ENGRY);
        left->setType(kCCProgressTimerTypeBar);
        left->setPercentage(0);
    }
    return true;
    
}

void CFightHeadLayer::setCurrEngry(int CurrEngry, int currEngryMax)
{
    int value=(int)((CCProgressTimer *)(getChildByTag(FIGHTHEAD_INFO_ENGRY)))->getPercentage() ;
    CCLog("value =%d,%d",value,(int)CurrEngry*100/m_nMaxValueEngry);
    
    if (value!= (int)(CurrEngry*100/m_nMaxValueEngry)) {
        CCLog("CurrHp*100/CurrTtotalhp: %d",(int)CurrEngry*100/m_nMaxValueEngry);
        CCProgressFromTo* progress3 = CCProgressFromTo::create(0.2,value,CurrEngry*100/m_nMaxValueEngry);
        ((CCProgressTimer *)(getChildByTag(FIGHTHEAD_INFO_ENGRY)))->runAction(progress3);
    }
    
}
void CFightHeadLayer::setCurrHP(int CurrHp, int CurrTtotalhp)
{
    int value=(int)((CCProgressTimer *)(getChildByTag(FIGHTHEAD_INFO_HP)))->getPercentage() ;
    CCLog("value =%d,%d",value,(int)CurrHp*100/CurrTtotalhp);
    
    if (value!= (int)(CurrHp*100/CurrTtotalhp)) {
        CCLog("CurrHp*100/CurrTtotalhp: %d",(int)CurrHp*100/CurrTtotalhp);
        CCProgressFromTo* progress3 = CCProgressFromTo::create(0.2,value,CurrHp*100/CurrTtotalhp);
        ((CCProgressTimer *)(getChildByTag(FIGHTHEAD_INFO_HP)))->runAction(progress3);
    }
   
}
void CFightHeadLayer::FilpXChildSprite(cocos2d::CCSprite *sprite)
{
    if (sprite) {
        sprite->setFlipX(true);
        sprite->setAnchorPoint(ccp(1.0, 0));
        sprite->setPosition(ccp(sprite->getPosition().x+sprite->getContentSize().width,sprite->getPosition().y));
    }
}

void CFightHeadLayer::setValue(CFightCard *card,int totalHp,int totalEngry)
{
    
}
