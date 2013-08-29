//
//  CGameErrorConfig.cpp
//  91.cube
//
//  Created by linminglu on 13-8-15.
//
//

#include "CGameDialogLayer.h"

#include "gameConfig.h"
#include "LayoutLayer.h"
#include "PtMapUtility.h"

CPtDialog* CPtDialog::create(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inConfirmSelector, SEL_CallFuncO  inCancelSelector, CCObject *inConfirmParam, CCObject *inCancleParam)
{
    CPtDialog * dialog = create();
    if (dialog)
    {
        dialog->setDialog(inTipContent, inTarget, inConfirmSelector, inCancelSelector,inConfirmParam, inCancleParam);
    }

    return dialog;
}

CPtDialog::CPtDialog()
{
    m_nTouchTag = -1;
    m_pHandler = NULL;
    m_pConfirmSelector = NULL;
    m_pCancelSelector = NULL;
    m_pConfirmParam = NULL;
    m_pCancleParam = NULL;
}
CPtDialog::~CPtDialog()
{
    
}

void CPtDialog::setDialog(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inConfirmSelector, SEL_CallFuncO  inCancelSelector, CCObject *inConfirmParam, CCObject *inCancelParam)
{
    if (m_pTip)
    {
        m_pTip->setString(inTipContent.c_str());
    }
    m_pHandler = inTarget;
    m_pConfirmSelector = inConfirmSelector;
    m_pCancelSelector = inCancelSelector;
    m_pConfirmParam = inConfirmParam;
    m_pCancleParam = inCancelParam;
}

bool CPtDialog::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initDialog();
        bRet = true;
    } while (0);
    return bRet;
}
bool CPtDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = TouchRect::SearchTouchTag(pTouch->getLocation(), m_cTouches);
    
    return true;
}
void CPtDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPtDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        if(m_nTouchTag == TouchRect::SearchTouchTag(pTouch->getLocation(), m_cTouches))
        {
            handlerTouch();
            PtSoundTool::playSysSoundEffect("UI_click.wav");
        }else
        {
            
        }
    }
    
    
}
void CPtDialog::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CPtDialog::initDialog()
{
   
    //加载map
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(this, CSTR_FILEPTAH(plistPath, "cuowutishi.plist"));
    tempLayerout->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-30000);
    
    tempLayerout->getTouchRects(m_cTouches);
    
    // add tip label to dialog();
    m_pTip = CCLabelTTF::create("你确定要使用XXXXXX道具吗?", "Arial", 18);
    int array[3] ={1,2,0};
    m_pTip->setPosition(ccp(195,80));
    tempLayerout->getElementByTags(array, 3)->addChild(m_pTip, 1000);
    
}

void CPtDialog::handlerTouch()
{
  
    if (m_nTouchTag == CANCEL_TAG)
    {
        if (m_pHandler && m_pCancelSelector)
        {

            (m_pHandler->*m_pCancelSelector)(m_pCancleParam);
        }
        removeFromParentAndCleanup(true);
    }else if(m_nTouchTag == CONFIRM_TAG)
    {
        if (m_pHandler && m_pConfirmSelector)
        {
            (m_pHandler->*m_pConfirmSelector)(m_pConfirmParam);
        }
        
    }else
    {
        
    }
}