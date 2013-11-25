//
//  CCardBagFullDialog.cpp
//  91.cube
//
//  Created by phileas on 13-11-13.
//
//

#include "CCardBagFullDialog.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CPtTool.h"
#include "SceneManager.h"
#include "CGameUserPropDialogLayer.h"

CCardBagFullDialog* CCardBagFullDialog::create()
{
    CCardBagFullDialog *dialog = new CCardBagFullDialog();
    if (dialog && dialog->init())
    {
        dialog->autorelease();
    
    }
    else
    {
        CC_SAFE_DELETE(dialog);
        dialog = NULL;
    }
    return  dialog;
}

CCardBagFullDialog::CCardBagFullDialog()
{
    initData();
}
CCardBagFullDialog::~CCardBagFullDialog()
{
    for (int i = 0; i <2 ; i++)
    {
        CC_SAFE_RELEASE(m_pFrame[i]);
    }
}


bool CCardBagFullDialog::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CGameSelectDialog::init());
        initCCardBagFullDialog();
        bRet = true;
    } while (0);
    return bRet;
}

bool CCardBagFullDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CGameSelectDialog::ccTouchBegan(pTouch, pEvent);
    if (CPtTool::isInNode(m_pCloseBtn, pTouch))
    {
        m_nTouchTag = CARDBAGCLOSE_TAG;
        m_pCloseBtn->setDisplayFrame(m_pCloseFrame[1]);
    }
    return true;
}
void CCardBagFullDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CGameSelectDialog::ccTouchMoved(pTouch, pEvent);
}
void CCardBagFullDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CGameSelectDialog::ccTouchEnded(pTouch, pEvent);
    if (m_nTouchTag == CARDBAGCLOSE_TAG)
    {
         m_pCloseBtn->setDisplayFrame(m_pCloseFrame[0]);
    }
    if (CPtTool::isInNode(m_pCloseBtn, pTouch)&&  m_nTouchTag == CARDBAGCLOSE_TAG)
    {
         onClickCloseBtn();
         PtSoundTool::playSysSoundEffect("UI_click.wav");
    }

    
}
void CCardBagFullDialog::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CCardBagFullDialog::initCCardBagFullDialog()
{
    const char * tip = "卡包已满，你可以选择强化，出售开牌或扩展卡包容量再进行此操作";
    setButtonText("强化", "出售", "扩展");
    setContentTip(tip);
    setLeftHandler(this, callfuncO_selector(CCardBagFullDialog::onClickGoEnhance));
    setMiddleHanlder(this, callfuncO_selector(CCardBagFullDialog::onClickGoSell));
    setRightHandler(this, callfuncO_selector(CCardBagFullDialog::onClickExtentCardBag));
    
    // add close btn:
    m_pCloseFrame[0] = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("QuitButton_Normal.png");
    m_pCloseFrame[1] = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("QuitButton_Actived.png");
    m_pCloseBtn = CCSprite::createWithSpriteFrame(m_pCloseFrame[0]);
    CCNode *parent = m_pBtn[1]->getParent();
    CCPoint point = m_pBtn[1]->getPosition();
    m_pCloseBtn->setPosition(ccp(point.x+300, point.y+100));
    parent->addChild(m_pCloseBtn);
}


void CCardBagFullDialog::onClickGoEnhance(CCObject *pObject)
{
    SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_CARDSETTINGSCENE); //runTargetScene(EN_CURRSCENE_CARDSETTINGSCENE);
}
void CCardBagFullDialog::onClickGoSell(CCObject *pObject)
{
    SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_CARDSETTINGSCENE);
}
void CCardBagFullDialog::onClickExtentCardBag(CCObject *pObject)
{
    CGameUserPropDialogLayer *layer = CGameUserPropDialogLayer::create(EXTENDCARDPROPID);
    layer->setSucessHandler(this, callfunc_selector(CCardBagFullDialog::onClose));
    addChild(layer);
}
void CCardBagFullDialog::onClickCloseBtn()
{
    onClose();
}
