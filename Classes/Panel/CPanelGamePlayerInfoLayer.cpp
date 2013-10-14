//
//  CPanelGamePlayerInfoLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-11.
//
//

#include "CPanelGamePlayerInfoLayer.h"
#include "gameplayer.h"

CPanelGamePlayerInfoLayer::CPanelGamePlayerInfoLayer()
{
    
}

CPanelGamePlayerInfoLayer::~CPanelGamePlayerInfoLayer()
{
    
}
void CPanelGamePlayerInfoLayer::updateShowData(float t)
{
    updateInfo();
}
void CPanelGamePlayerInfoLayer::updateInfo()
{
    CCSize wndSize=CCDirector::sharedDirector()->getWinSize();
    if (!getChildByTag(1000001))
    {
        CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 20);
        addChild(labelttf,0,1000001);
        labelttf->setPosition(ccp(350, 140));
        labelttf->setString(CCUserDefault::sharedUserDefault()->getStringForKey("name").c_str());
    }
    char data[30];
    // usercoin 金币
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000002);
        if (!getChildByTag(1000002))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000002);
            labelttf->setPosition(ccp(450, 140));
        }
        sprintf(data, "金币:%d",SinglePlayer::instance()->getCoin());
        labelttf->setString(data);
        
    }
    //user exp;现金币
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000003);
        if (!getChildByTag(1000003))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000003);
            labelttf->setPosition(ccp(350, 180));
        }
        sprintf(data, "现金:%d",SinglePlayer::instance()->getPlayerCash());
        labelttf->setString(data);
    }
    
    //user 体力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000004);
        if (!getChildByTag(1000004))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000004);
            labelttf->setPosition(ccp(500, 180));
        }
        sprintf(data, "体力:%d",SinglePlayer::instance()->getPlayerAp());
        labelttf->setString(data);
    }
    
    //user 领导力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000005);
        if (!getChildByTag(1000005))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000005);
            labelttf->setPosition(ccp(650, 180));
        }
        sprintf(data, "领导力:%d",SinglePlayer::instance()->getRVC());
        labelttf->setString(data);
    }
    // 等级
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000006);
        if (!getChildByTag(1000006))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000006);
            labelttf->setPosition(ccp(550, 140));
        }
        sprintf(data, "等级:%d",SinglePlayer::instance()->getPlayerLevel());
        labelttf->setString(data);
    }
    
    // 体力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000007);
        if (!getChildByTag(1000007))
        {
            labelttf=CCLabelTTF::create("", "Arial", 20);
            addChild(labelttf,0,1000007);
            labelttf->setPosition(ccp(650, 140));
        }
        sprintf(data, "神力:%d",SinglePlayer::instance()->getPlayerGp());
        labelttf->setString(data);
    }

}

bool CPanelGamePlayerInfoLayer::init()
{
    if (cocos2d::CCLayer::init())
    {
        schedule(schedule_selector(CPanelGamePlayerInfoLayer::updateShowData), 2.0);
        return true;
    }
    return false;
}