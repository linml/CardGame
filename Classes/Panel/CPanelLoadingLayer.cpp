//
//  CPanelLoadingLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-22.
//
//

#include "CPanelLoadingLayer.h"


CPanelLoadingLayer::CPanelLoadingLayer()
{
    
}
CPanelLoadingLayer::~CPanelLoadingLayer()
{
    
}

bool CPanelLoadingLayer::init()
{
    CCLabelTTF *labelttf=CCLabelTTF::create("等待服务器返回数据", "Arial", 50);
    addChild(labelttf,1,200);
    //CCSize size=CCDirector::sharedDirector()->getWinSize();
    //labelttf->setPosition(ccp(size.width *0.5, size.height *0.5));
    setTouchPriority(-999);
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    return true;
}

bool CPanelLoadingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void CPanelLoadingLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, getTouchPriority());

}
void CPanelLoadingLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    SUPER::onExit();
}