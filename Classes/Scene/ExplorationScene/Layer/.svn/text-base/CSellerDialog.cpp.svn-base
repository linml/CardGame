//
//  CSellerDialog.cpp
//  91.cube
//
//  Created by phileas on 13-10-17.
//
//

#include "CSellerDialog.h"
#include "CPtSellerConfigData.h"
#include "gameConfig.h"
#include "CPtTool.h"
#include "CSellerShopDialog.h"



CSellerDialog* CSellerDialog::create(SELLER_DATA inSellData)
{
    CSellerDialog * sellerDialog = new CSellerDialog();
    if (sellerDialog && sellerDialog->initCSellerDialog(inSellData))
    {
        sellerDialog->autorelease();
    }else
    {
        CC_SAFE_RELEASE_NULL(sellerDialog);
    }
    return sellerDialog;
}

CSellerDialog::CSellerDialog()
{
    m_nTouchTag = -1;
    m_nSellerType = -1;
    m_nShopId = -1;
    m_pCancel = NULL;
    m_pGoshop = NULL;
    m_pCloseSelector = NULL;
    m_pCloseHandler = NULL;
}

CSellerDialog::~CSellerDialog()
{
    
}


bool CSellerDialog::initCSellerDialog(SELLER_DATA inSellData)
{
    loadResource();
    m_sSellerData = inSellData;
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        initSellerDialogUI();
        bRet = true;
    } while (0);
    return bRet;
}
void CSellerDialog::setCloseHandler(cocos2d::CCObject *inTarget, SEL_CallFuncO inSelector)
{
    m_pCloseHandler = inTarget;
    m_pCloseSelector = inSelector;
}

bool CSellerDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    if (CPtTool::isInNode(m_pGoshop, pTouch))
    {
        m_nTouchTag = 2001;
        m_pGoshop->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("goShopPressed.png"));
    }else if(CPtTool::isInNode(m_pCancel, pTouch))
    {
        m_nTouchTag = 2002;
        m_pCancel->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeSelected.png"));
    }
    return true;
}
void CSellerDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CSellerDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        handlerTouch(pTouch);
    }
    
}
void CSellerDialog::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CSellerDialog::initSellerDialogUI()
{
    // test:
    
    CCSprite * bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "shangren_bg.png"));
    bg->setPosition(ccp(512, 388));
    CCSize bgSize = bg->getContentSize();
    addChild(bg);
    
    //add seller picture
    CCSprite *sellerPic = CCSprite::create(CSTR_FILEPTAH(g_sellerPath, "shangren_01.png"));
    sellerPic->setPosition(ccp(bgSize.width*(0.125), 0));
    sellerPic->setAnchorPoint(ccp(0.5, 0));
    bg->addChild(sellerPic);
    
    CCSprite * goShopBtn = CCSprite::createWithSpriteFrameName("goShopNormal.png");
    goShopBtn->setPosition(ccp(bgSize.width*(0.6), 30));
    bg->addChild(goShopBtn);
    
    CCSprite * cancelBtn = CCSprite::createWithSpriteFrameName("closeNormal.png");
    cancelBtn->setAnchorPoint(ccp(1,1));
    cancelBtn->setPosition(ccp(bgSize.width-10, bgSize.height-10));
    bg->addChild(cancelBtn);
    
    CCLabelTTF * go = CCLabelTTF::create("看看都有啥", "Arial", 13);
    go->setPosition(ccp(55, 20));
    go->setAnchorPoint(ccp(0.5,0.5));
    goShopBtn->addChild(go);
    
   
    
    m_pGoshop = goShopBtn;
    m_pCancel = cancelBtn;
    
    CPtSellerConfigData * sellerData = SingleSellerConfigData::instance();
    if (sellerData)
    {
        CPtSeller * seller = sellerData->getSellerById(m_sSellerData.sellerId);
       
        if (seller)
        {
            char buffer[40]={0};
            sprintf(buffer, "%s",seller->getSellerName().c_str());
            m_sSellerName = buffer;
            m_nShopId = seller->getShopId();
            CCLabelTTF* title = CCLabelTTF::create(buffer, "Arial", 25);
            title->setPosition(ccp(bgSize.width*(0.6), bgSize.height-35));
            bg->addChild(title);
            m_nSellerType = seller->getSellerType();
            sprintf(buffer, "%d", seller->getSellerDecriptionId());
            // set description:
            CCLabelTTF * content = CCLabelTTF::create(Utility::getWordWithFile("dictionary.plist",buffer).c_str(), "Arial", 15);
            content->setDimensions(CCSizeMake(340, 0));
            content->setHorizontalAlignment(kCCTextAlignmentLeft);
            content->setAnchorPoint(ccp(0, 1));
            content->setPosition(ccp(bgSize.width*0.35, bgSize.height*0.65));
            bg->addChild(content);
        }
       
    }
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(CSELLERDIALOG_TOUCH_PRORITY);
    
}

void CSellerDialog::handlerTouch(CCTouch *pTouch)
{
    if (m_nTouchTag == 2001)
    {

        m_pGoshop->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("goShopNormal.png"));
    }
    else if(m_nTouchTag == 2002)
    {
         m_pCancel->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("closeNormal.png"));
    }
    
    if (CPtTool::isInNode(m_pGoshop, pTouch) && m_nTouchTag == 2001)
    {
        onClickGoSellerShop();
       
    }else if(CPtTool::isInNode(m_pCancel, pTouch) && m_nTouchTag == 2002)
    {
        onClickCancel();
    }

    
}

void CSellerDialog::onClickGoSellerShop()
{
    CSellerShopDialog * shopDialog = CSellerShopDialog::create(m_sSellerData, m_nSellerType, m_nShopId,m_sSellerName);
    CCDirector::sharedDirector()->getRunningScene()->addChild(shopDialog, 20000);
    shopDialog->setCloseHandler(m_pCloseHandler, m_pCloseSelector);
    
    removeFromParentAndCleanup(true);
    
}

void CSellerDialog::onClickCancel()
{
    if (m_pCloseSelector && m_pCloseHandler)
    {
        (m_pCloseHandler->*m_pCloseSelector)(NULL);
    }
    removeFromParentAndCleanup(true);
}


void CSellerDialog::loadResource()
{

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "seller.plist"), CSTR_FILEPTAH(g_mapImagesPath, "seller.png"));
}

