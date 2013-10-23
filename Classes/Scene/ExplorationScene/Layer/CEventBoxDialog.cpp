//
//  CEventBoxDialog.cpp
//  91.cube
//
//  Created by phileas on 13-9-12.
//
//

#include "CEventBoxDialog.h"
#include "gameConfig.h"
#include "CPtTool.h"
#include "gamePlayer.h"



CEventBoxLayer* CEventBoxLayer::create(int inEventBoxId)
{
    CEventBoxData * eventData = SingleEventBoxes::instance()->getEventBoxDataById(inEventBoxId);
    return create(eventData);
}

CEventBoxLayer* CEventBoxLayer::create(CEventBoxData* inEventData)
{
    if (inEventData == NULL)
    {
        return NULL;
    }
    CEventBoxLayer* layer = new CEventBoxLayer();
    if (layer && layer->init(inEventData))
    {
        layer->autorelease();
    }else
    {
        CC_SAFE_DELETE(layer);
    }
    return  layer;
    
}


CEventBoxLayer::CEventBoxLayer()
{
    m_pSkipSelector = NULL;
    m_pOpenSelector = NULL;
    m_pTarget = NULL;
    m_bGPEnough = true;

    tips = NULL;
}
CEventBoxLayer::~CEventBoxLayer()
{
    CC_SAFE_RELEASE(m_cMaps);
}
void CEventBoxLayer::setHanlder(CCObject *inTarget, SEL_CallFuncO inOpenSelector, SEL_CallFuncO inSkipSelector)
{
    m_pTarget = inTarget;
    m_pOpenSelector = inOpenSelector;
    m_pSkipSelector = inSkipSelector;
}


bool CEventBoxLayer::init(CEventBoxData *inEventBoxData)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initEventBoxLayer(inEventBoxData);
        bRet = true;
    } while (0);
    return bRet;
}
bool CEventBoxLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    for (int i = 0; i < 2; i++)
    {
        if (CPtTool::isInNode(m_pBtn[i], pTouch))
        {
            m_nTouchTag = i;
           // m_pBtn[i]->setTextureRect(m_cTouchSpriteFrameRect[1]);
            m_pBtn[i]->setDisplayFrame(m_pState[1]);
            break;
        }
    }
    return true;
}
void CEventBoxLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CEventBoxLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    for (int i = 0; i < 2; i++)
    {
        if (CPtTool::isInNode(m_pBtn[i], pTouch))
        {
            if (m_nTouchTag == i)
            {
              //  m_pBtn[m_nTouchTag]->setTextureRect(m_cTouchSpriteFrameRect[0]);
                m_pBtn[m_nTouchTag]->setDisplayFrame(m_pState[0]);
                handlerTouch();
                return;
            }
        }
    }
    if (m_nTouchTag != -1)
    {
        //m_pBtn[m_nTouchTag]->setTextureRect(m_cTouchSpriteFrameRect[0]);
         m_pBtn[m_nTouchTag]->setDisplayFrame(m_pState[0]);
    }
    
    // reset:

}
void CEventBoxLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
       

}

void CEventBoxLayer::initEventBoxLayer(CEventBoxData *inEventBoxData)
{
    loadResource();
    
    if (inEventBoxData && inEventBoxData->getBoxType() == 1)
    {
        m_bGPEnough = SinglePlayer::instance()->getPlayerGp() >= inEventBoxData->getGP() ? true : false;
    }
    
    const char * title = inEventBoxData->getTitle().c_str();
    const char * describle = "今天早上你从家里走出来的时候，踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！踩到狗屎了！";
    const char * iconString ="baoxiang_1.png";
    CCLog("the describle: %s", describle);


    m_nTouchTag = -1;
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "deleteprop.plist"));
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-30000);
    m_cMaps->getTouchRects(m_cTouches);
    
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
    CCPoint point = parent->getPosition();
    parent->removeFromParentAndCleanup(true);
    parent = NULL;
    
    parent = (CCSprite*) m_cMaps->getElementByTags(array, 2);
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_eventBoxPath, iconString));
    if(texture)
    {
        CCSprite * icon = CCSprite::createWithTexture(texture);
        icon->setAnchorPoint(ccp(0,1));
        icon->setPosition(point);
        parent->addChild(icon);
    }
    
    // set describle:
    label = CCLabelTTF::create(Utility::getWordWithFile("dictionary.plist", inEventBoxData->getDictionaryKey().c_str()).c_str(), "Arial", 15, CCSizeMake(300, 0), kCCTextAlignmentLeft);
    label->setColor(ccc3(0, 0, 0));
    label->setAnchorPoint(ccp(0, 1));
    label->setPosition(ccp(point.x+ 110, point.y-10));
    parent->addChild(label);
    
    tips = label;
    // touch event:
    
    const char * name="继续探险";
    CCSprite *node = NULL;
    array[1]= 2, array[2] = 2;
    node =(CCSprite*) m_cMaps->getElementByTags(array, 3);
    Utility::addTouchRect(node->getTag(), node, m_cTouches);
    label = CCLabelTTF::create(name, "Arial", 18);
    size =  node->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setColor(ccc3(126, 60, 30));
    node->addChild(label);
    CCSpriteFrameCache* cach = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame * frame  = cach->spriteFrameByName("Use_Normal.png");
//    m_cTouchSpriteFrameRect[0] = frame->getRect();
    m_pState[0]=frame;
    node->setDisplayFrame(frame);
    m_pBtn[0] = node;
    
    array[2] = 3;
    name = "打开";
    node =(CCSprite*) m_cMaps->getElementByTags(array, 3);
    Utility::addTouchRect(node->getTag(), node, m_cTouches);
    label = CCLabelTTF::create(name, "Arial", 18);
    size =  node->boundingBox().size;
    label->setPosition(ccp(size.width/2, size.height/2));
    label->setColor(ccc3(126, 60, 30));
    node->addChild(label);
    node->setDisplayFrame(frame);
    frame  = cach->spriteFrameByName("Use_Pressed.png");
//    m_cTouchSpriteFrameRect[1] = frame->getRect();
    m_pState[1] = frame;
    m_pBtn[1] = node;
    
    array[1]= 5, array[2] = 8;
    m_cMaps->getElementByTags(array, 2)->removeAllChildrenWithCleanup(true);

    
}
void CEventBoxLayer::handlerTouch()
{
    if (m_nTouchTag == 0)
    {
        // continue exploration
        if(m_pTarget && m_pSkipSelector)
        {
            (m_pTarget->*m_pSkipSelector)(this);
        }
    }else
    {
        // open
        if (m_pTarget && m_pOpenSelector)
        {
            testUpdateString();
            (m_pTarget->*m_pOpenSelector)(this);
        }
    }
 //   removeFromParentAndCleanup(true);
}


void CEventBoxLayer::loadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "different_state.plist"), CSTR_FILEPTAH(g_mapImagesPath, "different_state.png"));
}

void CEventBoxLayer::testUpdateString()
{
    if (!m_bGPEnough &&tips)
    {
        tips->setString("神力不够");
    }
    
}