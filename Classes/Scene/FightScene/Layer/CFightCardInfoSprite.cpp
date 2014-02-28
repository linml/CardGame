//
//  CFightCardInfoSprite.cpp
//  91.cube
//
//  Created by linminglu on 14-1-2.
//
//

#include "CFightCardInfoSprite.h"
#include "CCard.h"
#include "gameConfig.h"
#include "CGamesCard.h"
#define TAG_NUQITIAO 2
CFightCardInfoSprite::CFightCardInfoSprite()
{
    
}

CFightCardInfoSprite::~CFightCardInfoSprite()
{
    
}

CFightCardInfoSprite * CFightCardInfoSprite::CreateSprite(CFightCard *pCard,int nIndex)
{
    CFightCardInfoSprite *pSprite=new CFightCardInfoSprite;
    if (!pSprite || !pSprite->initSprite(pCard,nIndex)) {
        CC_SAFE_DELETE(pSprite);
        return NULL;
    }
    pSprite->autorelease();
    return pSprite;
}
bool  CFightCardInfoSprite::initSprite(CFightCard *pCard, int nIndex)
{
    CGamesCard::initCGamesCard(pCard, CGameCardFactory::getInstance());
    en_gryMax=pCard->m_iEngryMax;
    CCSprite *pBackGroud=CCSprite::createWithSpriteFrameName("xiaonuqibackgroud.png");
    pBackGroud->setPosition(ccp(0,-10));//下面的配置参加testcpp的样例代码
    pBackGroud->setAnchorPoint(ccp(0,0));  //默认是中间，要设置左下角
    addChild(pBackGroud,1,1);
    //加入血条背景
    CCProgressTimer* progressTimeEngry = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("xiaonuqifront.png"));
    progressTimeEngry->setPosition(ccp(0,-10));//下面的配置参加testcpp的样例代码
    progressTimeEngry->setAnchorPoint(ccp(0,0));  //默认是中间，要设置左下角
    progressTimeEngry->setPercentage(0);  //初始的百分比
    progressTimeEngry->setMidpoint(ccp(0, 0));  //设置中点位置，默认是四周向重点变化。现在把重点设置为左边(纵向居中)，使血从右向左减少
    progressTimeEngry->setBarChangeRate(ccp(1, 0));//横向
    progressTimeEngry->setType(kCCProgressTimerTypeBar);//暂时不知道什么用
    addChild(progressTimeEngry, 1, TAG_NUQITIAO);
    CCPoint spritePoint=ccp(0,200);//sprite->getPosition();
    string str;
    m_nIndex=nIndex;
    switch (nIndex)
    {
        case 0:
            str=Utility::getWordWithFile("word.plist", "diyizhujiang");
            break;
        case 1:
            str=Utility::getWordWithFile("word.plist", "dierzhujiang");
            break;
        case 2:
            str=Utility::getWordWithFile("word.plist", "disanzhujiang");
            break;
        case 3:
            str=Utility::getWordWithFile("word.plist", "disizhujiang");
            break;
        case 4:
            str=Utility::getWordWithFile("word.plist", "diwuzhujiang");
            break;
        default:
            break;
    }
    CCLog("str=%s",str.c_str());
    CCLabelTTF *labelTTF=CCLabelTTF ::create(str.c_str(), "Arial", 15);
    labelTTF->setPosition(ccp(0,100));
    if (nIndex!=4) {
        labelTTF->setColor(g_custom_color[24]);
    }
    else{
        labelTTF->setColor(g_custom_color[25]);
    }
    labelTTF->setAnchorPoint(CCPointZero);
    addChild(labelTTF,2,3);
    return true;
}
void CFightCardInfoSprite::setDead()
{
    CGamesCard::setDead();
    ((CCLabelTTF *)getChildByTag(3))->setString("死亡");
}
void CFightCardInfoSprite::setLive()
{
    CGamesCard::setLive();
    string str;
    switch (m_nIndex)
    {
        case 0:
            str=Utility::getWordWithFile("word.plist", "diyizhujiang");
            break;
        case 1:
            str=Utility::getWordWithFile("word.plist", "dierzhujiang");
            break;
        case 2:
            str=Utility::getWordWithFile("word.plist", "disanzhujiang");
            break;
        case 3:
            str=Utility::getWordWithFile("word.plist", "disizhujiang");
            break;
        case 4:
            str=Utility::getWordWithFile("word.plist", "diwuzhujiang");
            break;
        default:
            break;
    }

    ((CCLabelTTF *)getChildByTag(3))->setString(str.c_str());
}

void CFightCardInfoSprite::setNowAnger(int value)
{
    if (getChildByTag(TAG_NUQITIAO)) {
        int nCurrValue=((CCProgressTimer *)(getChildByTag(TAG_NUQITIAO)))->getPercentage();
        CCLog("value*100/en_gryMax:%d",value*100/en_gryMax);
        if (nCurrValue!=value*100/en_gryMax) {
            CCProgressFromTo *pFrame=CCProgressFromTo::create(0.2, nCurrValue, value*100/en_gryMax);
            ((CCProgressTimer *)(getChildByTag(TAG_NUQITIAO)))->runAction(pFrame);
        }
    }
}
