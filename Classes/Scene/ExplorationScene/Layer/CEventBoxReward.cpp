//
//  CEventBoxReward.cpp
//  91.cube
//
//  Created by phileas on 13-9-13.
//
//

#include "CEventBoxReward.h"
#include "gameConfig.h"
#include "CPtTool.h"


CEventBoxRewordLayer * CEventBoxRewordLayer::create(CEventBoxData *inEventBoxData,int inType)
{
    CEventBoxRewordLayer *pRet = new CEventBoxRewordLayer();
    if (pRet && pRet->init(inEventBoxData, inType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }

    
}


CEventBoxRewordLayer::CEventBoxRewordLayer()
{
    m_cMaps = NULL;
    m_pTarget = NULL;
    m_pConfirmSelector = NULL;
    m_pState[0] = m_pState[1] = NULL;
}

CEventBoxRewordLayer::~CEventBoxRewordLayer()
{
    CC_SAFE_RELEASE(m_cMaps);
    
}

void CEventBoxRewordLayer::setHanlder(cocos2d::CCObject *inTarget, SEL_CallFuncO inCanfirmSelector)
{
    m_pTarget = inTarget;
    m_pConfirmSelector = inCanfirmSelector;
}

bool CEventBoxRewordLayer::init(CEventBoxData *inEventBoxData,int inType)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initEventBoxRewordLayer(inEventBoxData,inType);
        bRet = true;
    } while (0);
    return bRet;
}



bool CEventBoxRewordLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    if (CPtTool::isInNode(m_pBtn, pTouch))
    {
        m_nTouchTag = 0;
      //  m_pBtn->setTextureRect(m_cTouchSpriteFrameRect[1]);
        m_pBtn->setDisplayFrame(m_pState[1]);
    }
       

    return true;
}
void CEventBoxRewordLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CEventBoxRewordLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag == 0 )
    {
      //  m_pBtn->setTextureRect(m_cTouchSpriteFrameRect[0]);
        m_pBtn->setDisplayFrame(m_pState[0]);
    }

    if (CPtTool::isInNode(m_pBtn, pTouch))
    {
        if (m_nTouchTag == 0)
        {
            handlerTouch();
        }
        
    }

    
}
void CEventBoxRewordLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

/*
 * @param inType 0-1:
 * 0---> 领奖
 * 1---> 不可忽略
 */

void CEventBoxRewordLayer::initEventBoxRewordLayer(CEventBoxData *inEventBoxData,int inType)
{
    this->setUserData((void*) inType);
    loadResource();
    
    m_nType = inType;
    if (inType == GET_BOX)
    {
        createConfirmDialog();
    }else if(inType == IGNORE_BOX)
    {
        createReWordDialog(inEventBoxData);
    }
    else
    {
        createEmptyDialog();
    }


    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-40000);
}

void CEventBoxRewordLayer:: handlerTouch()
{
    if (m_nTouchTag == 0)
    {
        // continue exploration
        if(m_pTarget && m_pConfirmSelector)
        {
            
            (m_pTarget->*m_pConfirmSelector)(this);
        }

    }
    
}

void CEventBoxRewordLayer::loadResource()
{
      CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "different_state.plist"), CSTR_FILEPTAH(g_mapImagesPath, "different_state.png"));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "dialog_bg.plist"), CSTR_FILEPTAH(g_mapImagesPath, "dialog_bg.png"));
}

void CEventBoxRewordLayer:: createReWordDialog(CEventBoxData *inEventBoxData)
{
    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "dialog_bg.png"));
    CCSpriteBatchNode * bathNode = CCSpriteBatchNode::createWithTexture(texture);
    
    CCSprite * top = CCSprite::createWithSpriteFrameName("dialog_top.png");
    CCSprite * mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
    CCSprite * bottom = CCSprite::createWithSpriteFrameName("dialog_bottom.png");
    bottom->setPosition(ccp(280, 250));
    bottom->setAnchorPoint(CCPointZero);
    top->setAnchorPoint(CCPointZero);
    mid->setAnchorPoint(CCPointZero);
    CCSize size1 = bottom->boundingBox().size;
    mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height-1));
    bathNode->addChild(mid);
    CCSize size2 = mid->boundingBox().size;
    int count = 2;
    for (int i = 1; i < 4; i++)
    {
        mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
        mid->setAnchorPoint(CCPointZero);
        mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height+i*size2.height-i-1));
        bathNode->addChild(mid);
    }
    
    top->setPosition(ccp(mid->getPositionX(), mid->getPositionY()+size2.height-count));
    bathNode->addChild(top);
    bathNode->addChild(bottom);
    this->addChild(bathNode);
    

//    char * title = "基础事件描述";
//    char * describle = "捡到一个小袋子，被下了禁制，不知道里面藏了什么好东西！需要注入15点神力才能开启";
    const char * title = inEventBoxData->getTitle().c_str();
    const char * describle = Utility::getWordWithFile("dictionary.plist", inEventBoxData->getDictionaryKey().c_str()).c_str(); // "今天早上你从家里走出来的时候，踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！";
    CCLog("the describle: %s", describle);
    CCPoint point = top->getPosition();
    CCLabelTTF * label = CCLabelTTF::create(title, "Arial", 18);
    label->setColor(ccc3(0, 240, 255));
    label->setAnchorPoint(ccp(0, 1));
    label->setPosition(ccp(point.x+ 180, point.y+ 70));
    addChild(label);

    label =  CCLabelTTF::create(Utility::getWordWithFile("dictionary.plist", inEventBoxData->getDictionaryKey().c_str()).c_str(), "Arial", 15, CCSizeMake(300, 0), kCCTextAlignmentLeft);
    label->setColor(ccc3(126, 60, 30));
    label->setAnchorPoint(ccp(0,1));
    label->setPosition(ccp(point.x+150, point.y));
    addChild(label);
    
  const  char *icon1 = "baoxiang_2.png";
  const   char *icon2 = "baoxiang_3.png";
    
    texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_eventBoxPath, icon1));
    CCSprite * icon = CCSprite::createWithTexture(texture);
    icon->setAnchorPoint(CCPointZero);
    icon->setPosition(ccp(point.x+50, point.y- 80));
    addChild(icon);
    
    point = bottom->getPosition();
    
    texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_eventBoxPath, icon2));
    
    icon = CCSprite::createWithTexture(texture);
    icon->setAnchorPoint(CCPointZero);
    icon->setPosition(ccp(point.x+200, point.y+80));
    addChild(icon);
    
    // btn:
    
    CCSpriteFrameCache* cach = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame * frame  = cach->spriteFrameByName("Use_Normal.png");
//    m_cTouchSpriteFrameRect[0] = frame->getRect();
    m_pState[0] = frame;
    
    const char * name="确定";
    CCSprite *node = CCSprite::createWithSpriteFrame(frame);
    node->setPosition(ccp(point.x+ 180, point.y + 25));
    node->setAnchorPoint(CCPointZero);
    label = CCLabelTTF::create(name, "Arial", 18);
    CCSize size =  node->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setColor(ccc3(126, 60, 30));
    node->addChild(label);
   
    m_pBtn = node;
    
    frame  = cach->spriteFrameByName("Use_Pressed.png");
//    m_cTouchSpriteFrameRect[1] = frame->getRect();
    m_pState[1] = frame;

    addChild(node);
}

void CEventBoxRewordLayer::createConfirmDialog()
{
    m_nTouchTag = -1;
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "deleteprop.plist"));
 

    int number = 1;
    const char *title = "获得了";
    const char *iconString = "icon_001.png";
    char buffer[30] ={0};
    // set title
    int array[4]={1,6,0,0};
    CCSprite *parent = (CCSprite*) m_cMaps->getElementByTags(array, 3);
    CCLabelTTF * label = CCLabelTTF::create(title, "Arial", 18);
    label->setColor(ccc3(0, 240, 255));
    
    CCSize size =  parent->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    parent->addChild(label);
    
    // set icon
    array[1] = 4;
    array[2] = 0;
    parent = (CCSprite*) m_cMaps->getElementByTags(array, 3);
    parent->setPositionX(parent->getPositionX()+160);
    
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_propImagesPath, iconString));
    if(texture)
    {
        CCSprite * icon = CCSprite::createWithTexture(texture);
        icon->setAnchorPoint(CCPointZero);
        icon->setPosition(ccp(8,8));
        parent->addChild(icon);
    }
    
    
    // set number:
    sprintf(buffer, "x %d", number);
    label = CCLabelTTF::create(buffer, "Arial", 18);
    label->setColor(ccc3(0, 0, 0));
    parent->addChild(label);
    label->setAnchorPoint(ccp(0,1));
    label->setPosition(ccp(90, 20));
    // touch event:
    
    const char * name="确定";
    CCSprite *node = NULL;
    array[1]= 2, array[2] = 2;
    node =(CCSprite*) m_cMaps->getElementByTags(array, 3);
    label = CCLabelTTF::create(name, "Arial", 18);
    size =  node->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setColor(ccc3(126, 60, 30));
    node->addChild(label);
    CCSpriteFrameCache* cach = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame * frame  = cach->spriteFrameByName("Use_Normal.png");
  //  m_cTouchSpriteFrameRect[0] = frame->getRect();
    m_pState[0] = frame;
    node->setDisplayFrame(frame);
    m_pBtn = node;
    
    node->setPositionX(node->getPositionX()+80);
    
    array[2] = 3;
    node =(CCSprite*) m_cMaps->getElementByTags(array, 3);
    node->removeFromParentAndCleanup(true);
    
    frame  = cach->spriteFrameByName("Use_Pressed.png");
//    m_cTouchSpriteFrameRect[1] = frame->getRect();
    m_pState[1] = frame;
    
    array[1]= 5, array[2] = 8;
    m_cMaps->getElementByTags(array, 2)->removeAllChildrenWithCleanup(true);
    
}

void CEventBoxRewordLayer::createEmptyDialog()
{
    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "dialog_bg.png"));
    CCSpriteBatchNode * bathNode = CCSpriteBatchNode::createWithTexture(texture, 5);
    
    CCSprite * top = CCSprite::createWithSpriteFrameName("dialog_top.png");
    CCSprite * mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
    CCSprite * bottom = CCSprite::createWithSpriteFrameName("dialog_bottom.png");
    bottom->setPosition(ccp(280, 250));
    bottom->setAnchorPoint(CCPointZero);
    top->setAnchorPoint(CCPointZero);
    mid->setAnchorPoint(CCPointZero);
    CCSize size1 = bottom->boundingBox().size;
    mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height-1));
    bathNode->addChild(mid);
    CCSize size2 = mid->boundingBox().size;
    int count = 2;
    for (int i = 1; i < 2; i++)
    {
        mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
        mid->setAnchorPoint(CCPointZero);
        mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height+i*size2.height-i-1));
        bathNode->addChild(mid);
    }
    
    top->setPosition(ccp(mid->getPositionX(), mid->getPositionY()+size2.height-count));
    bathNode->addChild(top);
    bathNode->addChild(bottom);
    this->addChild(bathNode);
    
    
   const char * title = "空事件";
   const char * describle = "快去洗洗手，啥都没捞到了！";
   
    CCLog("the describle: %s", describle);
    CCPoint point = top->getPosition();
    CCLabelTTF * label = CCLabelTTF::create(title, "Arial", 18);
    label->setColor(ccc3(0, 240, 255));
    label->setAnchorPoint(ccp(0, 1));
    label->setPosition(ccp(point.x+ 200, point.y+70));
    addChild(label);
    
    label =  CCLabelTTF::create(describle, "Arial", 15, CCSizeMake(300, 0), kCCTextAlignmentLeft);
    label->setColor(ccc3(126, 60, 30));
    label->setAnchorPoint(ccp(0,1));
    label->setPosition(ccp(point.x+150, point.y));
    addChild(label);
    
    // btn:
    
    CCSpriteFrameCache* cach = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame * frame  = cach->spriteFrameByName("Use_Normal.png");
  //  m_cTouchSpriteFrameRect[0] = frame->getRect();
    m_pState[0] = frame;
    point = bottom->getPosition();
    const char * name="确定";
    CCSprite *node = CCSprite::createWithSpriteFrame(frame);
    node->setPosition(ccp(point.x+ 180, point.y + 25));
    node->setAnchorPoint(CCPointZero);
    label = CCLabelTTF::create(name, "Arial", 18);
    CCSize size =  node->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setColor(ccc3(126, 60, 30));
    node->addChild(label);
    
    m_pBtn = node;
    
    frame  = cach->spriteFrameByName("Use_Pressed.png");
 //   m_cTouchSpriteFrameRect[1] = frame->getRect();
    m_pState[1] = frame;
    
    addChild(node);

}