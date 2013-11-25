//
//  CLackAPDialog.cpp
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#include "CLackAPDialog.h"
#include "CGameUserPropDialogLayer.h"


CLackAPDialog::CLackAPDialog()
{
    
}
CLackAPDialog::~CLackAPDialog()
{
    
}
    

bool CLackAPDialog::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CLackGPDialog::init());
        initCLackAPDialog();
        bRet = true;
    } while (0);
    return bRet;
    
}

void CLackAPDialog::initCLackAPDialog()
{
    const char * tip = "体力为空, 探索失败!";
    setButtonText("恢复", "存档", "取消");
    setContentTip(tip);
    
    setLeftHandler(this, callfuncO_selector(CLackAPDialog::onClickRevive));
    setRightHandler(this, callfuncO_selector(CLackAPDialog::onClickCancle));
}
void CLackAPDialog::onClickRevive(CCObject *pObject)
{
    CGameUserPropDialogLayer *layer = CGameUserPropDialogLayer::create(HUIAP);
    layer->setSucessHandler(this, callfunc_selector(CLackAPDialog::onClose));
    addChild(layer);

}

void CLackAPDialog::onClickCancle(cocos2d::CCObject *pObject)
{
    onClose();
}
    
