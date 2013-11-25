//
//  CGameSelectDialog.cpp
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#include "CGameSelectDialog.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CPtTool.h"

CGameSelectDialog* CGameSelectDialog::create(std::string inTipContent)
{
    CGameSelectDialog *dialog = new CGameSelectDialog();
    if (dialog && dialog->init())
    {
        dialog->autorelease();
        dialog->setContentTip(inTipContent.c_str());
    }
    else
    {
        CC_SAFE_DELETE(dialog);
        dialog = NULL;
    }
    return  dialog;
}

CGameSelectDialog::CGameSelectDialog()
{
    initData();
}
CGameSelectDialog::~CGameSelectDialog()
{
//    for (int i = 0; i <2 ; i++)
//    {
//        CC_SAFE_RELEASE(m_pFrame[i]);
//    }
}

void CGameSelectDialog::setLeftHandler(CCObject *inLeftTarget,SEL_CallFuncO  inLeftSelector,  CCObject *inLeftParam /*= NULL*/)
{
    m_pLeftHandler = inLeftTarget;
    m_pLeftSelector = inLeftSelector;
    m_pLeftParam = inLeftParam;
}
void CGameSelectDialog::setRightHandler(CCObject *inRightTarget,SEL_CallFuncO  inRightSelector,CCObject *inRightParam /*= NULL*/)
{
    m_pRightHandler = inRightTarget;
    m_pRightSelector =inRightSelector;
    m_pRightParam = inRightParam;
}
void CGameSelectDialog::setMiddleHanlder(CCObject *inMiddleTarget,SEL_CallFuncO  inMiddleSelector,  CCObject *inMiddleParam /*= NULL*/)
{
    m_pMiddleHandler = inMiddleTarget;
    m_pMiddleSelector = inMiddleSelector;
    m_pMiddleParam = inMiddleParam;
}

void CGameSelectDialog::setCloseCallBack(CCObject *pObject, SEL_CallFunc closeCallback)
{
    m_pCloseHandler = pObject;
    m_pCloseSelector = closeCallback;
}

/*
 * @breif: 如果不想改变按钮的文字，则直接设置NULL
 */
void CGameSelectDialog::setButtonText(const char *inLeftText,const char *inMiddleText, const char *inRightText)
{
    if (inLeftText && m_pLeft)
    {
        m_pLeft->setString(inLeftText);
    }
    
    if (inMiddleText && m_pMiddle)
    {
        m_pMiddle->setString(inMiddleText);
    }
    
    if (inRightText && m_pRight)
    {
        m_pRight->setString(inRightText);
    }
}

void CGameSelectDialog::setContentTip(const char *inContentTip)
{
    if (m_pTip && inContentTip)
    {
        m_pTip->setString(inContentTip);
    }
}
bool CGameSelectDialog::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        initCGameSelectDialog();
        bRet = true;
    } while (0);
    return bRet;
}

bool CGameSelectDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

    for (int i = 0; i < 3; i++)
    {
        if (CPtTool::isInNode(m_pBtn[i], pTouch))
        {
            m_pBtn[i]->setDisplayFrame(m_pFrame[1]);
            m_nTouchTag = i+2001;
            break;
        }
    }
    
    return true;

}
void CGameSelectDialog::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameSelectDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        if (m_nTouchTag >= SELECT_LEFT_TAG && m_nTouchTag <= SELECT_RIGHT_TAG)
        {
            m_pBtn[m_nTouchTag-2001]->setDisplayFrame(m_pFrame[0]);
        }
        
        for (int i = 0; i < 3; i++)
        {
            if (CPtTool::isInNode(m_pBtn[i], pTouch))
            {
      
                if(m_nTouchTag == (i+2001))
                {
                    handlerTouch();
                    PtSoundTool::playSysSoundEffect("UI_click.wav");
                    break;
                }
            }
        }

    }

    
}
void CGameSelectDialog::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CGameSelectDialog::initData()
{
    m_nTouchTag = -1;
    m_pTip = NULL;
    m_pLeft = NULL;
    m_pRight = NULL;
    m_pMiddle = NULL;
    
    m_pLeftHandler = NULL;
    m_pRightHandler = NULL;
    m_pMiddleHandler = NULL;
    
    m_pRightSelector = NULL;
    m_pLeftSelector = NULL;
    m_pMiddleSelector = NULL;
    
    m_pRightParam = NULL;
    m_pLeftParam = NULL;
    m_pMiddleParam = NULL;
    
    m_pCloseHandler = NULL;
    m_pCloseSelector = NULL;
    
    for (int i = 0; i < 2; i++)
    {
        m_pFrame[i] = NULL;
        m_pBtn[i] = NULL;
    }
    m_pBtn[2] = NULL;
    
    
    
}
void CGameSelectDialog::initCGameSelectDialog()
{
    loadResource();
    
    //加载map
    
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(this, CSTR_FILEPTAH(plistPath, "cuowutishi.plist"));

    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-30000);
    
    
    // add tip label to dialog();
    m_pTip = CCLabelTTF::create("", "Arial", 18);
    int array[3] ={1,2,0};
    m_pTip->setPosition(ccp(195,80));
    m_pTip->setColor(ccc3(0, 0, 0));
    m_pTip->setDimensions(CCSizeMake(350, 0));
    m_pTip->setHorizontalAlignment(kCCTextAlignmentLeft);
    tempLayerout->getElementByTags(array, 3)->addChild(m_pTip, 1000);
    
    // reset btn:
    CCPoint point;
    CCSprite *node = NULL;
    CCNode *parent = NULL;
    
    node =(CCSprite*) tempLayerout->getElementByTags(array, 3);
    parent = node->getParent();

    m_pLeft = CCLabelTTF::create("取消", "Arial", 18);
    point = node->getPosition();
    node->setPositionX(point.x-30);
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
    
    // add middle button:
    
    node =(CCSprite*)CCSprite::createWithSpriteFrame(frame);
    node->initWithSpriteFrame(frame);
    node->setAnchorPoint(ccp(0,1));
    m_pMiddle = CCLabelTTF::create("选择", "Arial", 18);
    node->setPosition(ccp(point.x+size.width, point.y));
    node->addChild(m_pMiddle);
    

    node->setAnchorPoint(ccp(0,1));
    size =  node->boundingBox().size;
    m_pMiddle->setPosition(ccp(size.width/2, size.height/2));
    m_pMiddle->setColor(ccc3(126, 60, 30));
    parent->addChild(node);
    m_pBtn[1] = node;
    // add end:
    
    array[2] = 1;
    node =(CCSprite*) tempLayerout->getElementByTags(array, 3);
    point = node->getPosition();
    m_pRight = CCLabelTTF::create("确定", "Arial", 18);
    
    node->addChild(m_pRight);
    node->initWithSpriteFrame(frame);
    node->setAnchorPoint(ccp(0,1));
    size =  node->boundingBox().size;
    m_pRight->setPosition(ccp(size.width/2, size.height/2));
    m_pRight->setColor(ccc3(126, 60, 30));
    node->setPositionX(point.x+ 60);
    frame  = cach->spriteFrameByName("Use_Pressed.png");
    m_pFrame[1] = frame;
    CC_SAFE_RETAIN(m_pFrame[1]);
    m_pBtn[2] = node;
    
    
}
void CGameSelectDialog::handlerTouch()
{
    if (m_nTouchTag == SELECT_RIGHT_TAG)
    {
        if (m_pRightHandler && m_pRightSelector)
        {
            (m_pRightHandler->*m_pRightSelector)(m_pRightParam);
            
        }
    }else if(m_nTouchTag == SELECT_LEFT_TAG)
    {
        if (m_pLeftHandler && m_pLeftSelector)
        {
            (m_pLeftHandler->*m_pLeftSelector)(m_pLeftParam);
            
        }
        
    }else if(m_nTouchTag == SELECT_MIDDLE_TAG)
    {
        if (m_pMiddleHandler && m_pMiddleSelector)
        {
            (m_pMiddleHandler->*m_pMiddleSelector)(m_pMiddleParam);
            
        }
    }

    
}
void CGameSelectDialog::onClose()
{
    removeFromParentAndCleanup(true);
    if (m_pCloseHandler && m_pCloseSelector)
    {
        (m_pCloseHandler->*m_pCloseSelector)();
    }
}
void CGameSelectDialog::loadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "different_state.plist"), CSTR_FILEPTAH(g_mapImagesPath, "different_state.png"));
}
