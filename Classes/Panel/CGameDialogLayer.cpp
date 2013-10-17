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

CPtDialog* CPtDialog::create(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inLeftSelector, SEL_CallFuncO  inRightSelector, CCObject *inLeftParam, CCObject *inRightParam)
{
    CPtDialog * dialog = create();
    if (dialog)
    {
        dialog->setDialog(inTipContent, inTarget, inLeftSelector, inRightSelector,inLeftParam, inRightParam);
    }

    return dialog;
}

CPtDialog::CPtDialog()
{
    m_nTouchTag = -1;
    m_pHandler = NULL;
    m_pLeftSelector = NULL;
    m_pRightSelector = NULL;
    m_pLeftParam = NULL;
    m_pRightParam = NULL;
    m_pLeft = NULL;
    m_pRight = NULL;
    for (int i = 0; i < 2; i++)
    {
        m_pBtn[i] = NULL;
        m_pFrame[i] = NULL;
    
    }
}
CPtDialog::~CPtDialog()
{
    for (int i = 0; i <2 ; i++)
    {
         CC_SAFE_RELEASE(m_pFrame[i]);
    }
   
}

void CPtDialog::setDialog(std::string inTipContent, CCObject *inTarget, SEL_CallFuncO  inLeftSelector, SEL_CallFuncO  inRightSelector, CCObject *inLeftParam, CCObject *inRightParam)
{
    if (m_pTip)
    {
        m_pTip->setString(inTipContent.c_str());
    }
    m_pHandler = inTarget;
    m_pLeftSelector = inLeftSelector;
    m_pRightSelector =inRightSelector;
    m_pLeftParam = inLeftParam;
    m_pRightParam = inRightParam;
}

void CPtDialog::setButtonText(std::string inLeftText, std::string inRightText)
{
    if (m_pLeft)
    {
        m_pLeft->setString(inLeftText.c_str());
    }
    
    if (m_pRight)
    {
        m_pRight->setString(inRightText.c_str());
    }
    
}

bool CPtDialog::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        initDialog();
        bRet = true;
    } while (0);
    return bRet;
}
bool CPtDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = TouchRect::SearchTouchTag(pTouch->getLocation(), m_cTouches);
    if (m_nTouchTag == RIGHT_TAG)
    {
        m_pBtn[1]->setDisplayFrame(m_pFrame[1]);
        
    }else if(m_nTouchTag == LEFT_TAG)
    {
        m_pBtn[0]->setDisplayFrame(m_pFrame[1]);
    }

    
    return true;
}
void CPtDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPtDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        if (m_nTouchTag == RIGHT_TAG)
        {
            m_pBtn[1]->setDisplayFrame(m_pFrame[0]);
            
        }else if(m_nTouchTag == LEFT_TAG)
        {
            m_pBtn[0]->setDisplayFrame(m_pFrame[0]);
        }

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
    loadResource();
   
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
    m_pTip = CCLabelTTF::create("", "Arial", 18);
    int array[3] ={1,2,0};
    m_pTip->setPosition(ccp(195,80));
    m_pTip->setColor(ccc3(0, 0, 0));
    tempLayerout->getElementByTags(array, 3)->addChild(m_pTip, 1000);
    
    // reset btn:
    CCSprite *node = NULL;
    node =(CCSprite*) tempLayerout->getElementByTags(array, 3);
    Utility::addTouchRect(LEFT_TAG, node, m_cTouches);
    m_pLeft = CCLabelTTF::create("取消", "Arial", 18);
   
    CCSpriteFrameCache* cach = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame * frame  = cach->spriteFrameByName("Use_Normal.png");
    m_pFrame[0] = frame;
    CC_SAFE_RETAIN(m_pFrame[0]);
    node->initWithSpriteFrame(frame);
    node->setAnchorPoint(ccp(0,1));
    
    CCSize size =  node->boundingBox().size;
    m_pLeft->setPosition(ccp(size.width/2, size.height/2));
    m_pLeft->setColor(ccc3(126, 60, 30));
    node->addChild(m_pLeft);
    m_pBtn[0] = node;
    
    array[2] = 1;
    node =(CCSprite*) tempLayerout->getElementByTags(array, 3);
    Utility::addTouchRect(RIGHT_TAG, node, m_cTouches);
    m_pRight = CCLabelTTF::create("确定", "Arial", 18);
   
    node->addChild(m_pRight);
    node->initWithSpriteFrame(frame);
    node->setAnchorPoint(ccp(0,1));
    size =  node->boundingBox().size;
    m_pRight->setPosition(ccp(size.width/2, size.height/2));
    m_pRight->setColor(ccc3(126, 60, 30));
    
    frame  = cach->spriteFrameByName("Use_Pressed.png");
    m_pFrame[1] = frame;
    CC_SAFE_RETAIN(m_pFrame[1]);
    m_pBtn[1] = node;

    
    
}

void CPtDialog::handlerTouch()
{
  
    if (m_nTouchTag == RIGHT_TAG)
    {
        if (m_pHandler && m_pRightSelector)
        {

            (m_pHandler->*m_pRightSelector)(m_pRightParam);
           
        }
        removeFromParentAndCleanup(true);
    }else if(m_nTouchTag == LEFT_TAG)
    {
        if (m_pHandler && m_pLeftSelector)
        {
            (m_pHandler->*m_pLeftSelector)(m_pLeftParam);
            
        }
        removeFromParentAndCleanup(true);
        
    }else
    {
        
    }
}

void CPtDialog::loadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "different_state.plist"), CSTR_FILEPTAH(g_mapImagesPath, "different_state.png"));
}