//
//  CLackGPDialog.cpp
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#include "CLackGPDialog.h"
#include "CGameUserPropDialogLayer.h"

CLackGPDialog::CLackGPDialog()
{
    m_pGoHallHandler = NULL;
    m_pGoHallCallBack = NULL;
}
CLackGPDialog::~CLackGPDialog()
{
    
}


bool CLackGPDialog::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CGameSelectDialog::init());
        initCLackGPDialog();
        bRet = true;
    } while (0);
    return bRet;
}

void CLackGPDialog::setGoHallBackCallBack(CCObject *pObject, SEL_CallFunc pGoHallCallBack)
{
    m_pGoHallHandler = pObject;
    m_pGoHallCallBack = pGoHallCallBack;
}

void CLackGPDialog::initCLackGPDialog()
{
    const char * tip = "神力为空, 探索失败!";
    setButtonText("复活", "存档", "放弃");
    setContentTip(tip);
    setLeftHandler(this, callfuncO_selector(CLackGPDialog::onClickRevive));
    setMiddleHanlder(this, callfuncO_selector(CLackGPDialog::onClickFile));
}
// detail event handler:
void CLackGPDialog::onClickRevive(CCObject *pObject)
{
    CGameUserPropDialogLayer *layer = CGameUserPropDialogLayer::create(HUIGP);
    layer->setSucessHandler(this, callfunc_selector(CLackGPDialog::onClose));
    addChild(layer);
}
void CLackGPDialog::onClickFile(CCObject *pObject)
{
//    CGameUserPropDialogLayer *layer = CGameUserPropDialogLayer::create(HUICHENG);
//    layer->setSucessHandler(this, callfunc_selector(CLackGPDialog::onClose));
//    addChild(layer);
    CSpecialPropUserDialog *layer = CSpecialPropUserDialog::create(HUICHENG, m_uSpecialPropData);
    layer->setSucessHandler(m_pGoHallHandler, m_pGoHallCallBack);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
}
