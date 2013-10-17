//
//  CGameRechargeTip.cpp
//  91.cube
//
//  Created by phileas on 13-10-14.
//
//

#include "CGameRechargeTip.h"

#include "CPtRecharge.h"


CGameRechargeTip::CGameRechargeTip()
{
    
}

CGameRechargeTip::~CGameRechargeTip()
{
    
}

bool CGameRechargeTip::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CPtDialog::init());
        initCGameRechargeTip();
        
        bRet = true;
    } while (0);
    return bRet;
}


void CGameRechargeTip::initCGameRechargeTip()
{
    setDialog("没钱了！快来坑爹去吧",this,callfuncO_selector(CGameRechargeTip::onRechargeCallBack), callfuncO_selector(CGameRechargeTip::onCancelCallBack),NULL, NULL);
    setButtonText("充值", "取消");
}

void CGameRechargeTip::onRechargeCallBack(CCObject *pObject)
{
    // go to the recharge and remove this layer:
    CCLog("go to recharge");
    CPtRecharge * layer = CPtRecharge::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000000);
}
void CGameRechargeTip::onCancelCallBack(CCObject *pObject)
{
}