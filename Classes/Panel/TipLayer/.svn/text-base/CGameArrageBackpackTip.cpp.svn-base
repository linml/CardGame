//
//  CGameArrageBackpackTip.cpp
//  91.cube
//
//  Created by phileas on 13-10-15.
//
//

#include "CGameArrageBackpackTip.h"
#include "CBackpackContainerLayer.h"

CGameArrageBackpackTip::CGameArrageBackpackTip()
{
    
}

CGameArrageBackpackTip::~CGameArrageBackpackTip()
{
    
}

bool CGameArrageBackpackTip::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CPtDialog::init());
        initCGameArrageBackpackTip();
        bRet = true;
    } while (0);
    return bRet;
}

void CGameArrageBackpackTip::initCGameArrageBackpackTip()
{
    setDialog("背包满了！快去收拾收腾个地吧",this,this,callfuncO_selector(CGameArrageBackpackTip::onArrageCallBack), callfuncO_selector(CGameArrageBackpackTip::onCancelCallBack),NULL, NULL);
    setButtonText("整理", "取消");
}

void CGameArrageBackpackTip::onArrageCallBack(CCObject *pObject)
{
    // goto the backpack layer:
    showBackPack();
}
void CGameArrageBackpackTip::onCancelCallBack(CCObject *pObject)
{
}


void CGameArrageBackpackTip::showBackPack()
{
    // show backpack layer:
    CCLayer * layer = CBackpackContainerLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 20000);
    
}