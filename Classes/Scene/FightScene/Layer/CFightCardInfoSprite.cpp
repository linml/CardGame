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
    setContentSize(CCSizeMake(200, 400));
    CCSprite *pBackGroud=CCSprite::createWithSpriteFrameName("xiaonuqibackground.png");
    pBackGroud->setPosition(ccp(0,260));//下面的配置参加testcpp的样例代码
    pBackGroud->setAnchorPoint(ccp(0,0));  //默认是中间，要设置左下角
    addChild(pBackGroud,1,1);
    //加入血条背景
    CCProgressTimer* progressTimeHP = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("xiaonuqifront.png"));
    progressTimeHP->setPosition(ccp(0,260));//下面的配置参加testcpp的样例代码
    progressTimeHP->setAnchorPoint(ccp(0,0));  //默认是中间，要设置左下角
    progressTimeHP->setPercentage(100);  //初始的百分比
    progressTimeHP->setMidpoint(ccp(0, 1));  //设置中点位置，默认是四周向重点变化。现在把重点设置为左边(纵向居中)，使血从右向左减少
    progressTimeHP->setBarChangeRate(ccp(1, 0));//暂时不知道什么用
    progressTimeHP->setType(kCCProgressTimerTypeBar);//暂时不知道什么用
    addChild(progressTimeHP, 1, TAG_NUQITIAO);
    //通过sprite来获取一个头像。
//    CCSprite *sprite=CCSprite::create("");
//    addChild(sprite,1,0);
//    sprite->setPosition(ccp(100,200));
    CCSize  spriteSize=CCSizeMake(60,60)    ;//sprite->getContentSize();
    CCPoint spritePoint=ccp(0,200);//sprite->getPosition();
    string str;
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
    CCLabelTTF *labelTTF=CCLabelTTF ::create(str.c_str(), "Arial", 15);
    labelTTF->setPosition(ccp(spritePoint.x,spritePoint.y-spriteSize.height/2-20));
    if (nIndex!=5) {
        labelTTF->setColor(g_custom_color[24]);
    }
    else{
        labelTTF->setColor(g_custom_color[25]);
    }
    labelTTF->setAnchorPoint(CCPointZero);
    addChild(labelTTF,1,3);
    return true;
}


void CFightCardInfoSprite::setNowAnger(int value)
{
    if (getChildByTag(TAG_NUQITIAO)) {
        ((CCProgressTimer *)(getChildByTag(TAG_NUQITIAO)))->setPercentage(value);
    }
}
