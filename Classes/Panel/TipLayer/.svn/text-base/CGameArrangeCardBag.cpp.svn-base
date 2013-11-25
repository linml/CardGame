//
//  CGameArrangeCardBag.cpp
//  91.cube
//
//  Created by linminglu on 13-10-23.
//
//

#include "CGameArrangeCardBag.h"
#include "SceneManager.h"

CGameArrangeCardBag::CGameArrangeCardBag()
{
    
}

CGameArrangeCardBag::~CGameArrangeCardBag()
{
    
}

bool CGameArrangeCardBag::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CPtDialog::init());
        initCGameArrageBackpackTip();
        bRet = true;
    } while (0);
    return bRet;
}

void CGameArrangeCardBag::initCGameArrageBackpackTip()
{
    setDialog("卡包满了 快去整理下卡包吧",this,this,callfuncO_selector(CGameArrangeCardBag::onArrageCallBack), callfuncO_selector(CGameArrangeCardBag::onCancelCallBack),NULL, NULL);
    setButtonText("整理", "取消");
}

void CGameArrangeCardBag::onArrageCallBack(CCObject *pObject)
{
    // goto the backpack layer:
    showBackPack();
}
void CGameArrangeCardBag::onCancelCallBack(CCObject *pObject)
{
    
}


void CGameArrangeCardBag::showBackPack()
{
    SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_CARDSETTINGSCENE);
}