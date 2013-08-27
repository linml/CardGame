//
//  CBackpackPageLayer.cpp
//  91.cube
//
//  Created by phileas on 13-8-21.
//
//

#include "CBackpackPageLayer.h"
#include "CPtTool.h"


CBackpackPageLayer * CBackpackPageLayer::create(int inOpenNumber)
{
    CBackpackPageLayer *layer = new CBackpackPageLayer();
    if (layer && layer->init(inOpenNumber))
    {
        layer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(layer);
        layer = NULL;
    }
   
    
    return layer;
}

CBackpackPageLayer::CBackpackPageLayer()
{
    m_bTouchLock = false;
    m_cMaps = NULL;
    m_pItemNums = NULL;
    m_pMasks = NULL;
    m_nTouchTag = -1;
    m_nOpenGridNumber = 0;
    m_nUserGridNumber = 0;
    
    for (int i = 0;  i < 9; i++)
    {
        m_pDeleteButtons[i] = NULL;
        m_pUseButton[i] = NULL;
    }
    
}

CBackpackPageLayer::~CBackpackPageLayer()
{
    //relase resouce:
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    CC_SAFE_RELEASE(m_pItemNums);
    CC_SAFE_RELEASE(m_pMasks);
}

bool CBackpackPageLayer::init(int inOpenNumber)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initCBackpackPageLayer(inOpenNumber);
        bRet = true;
    } while (0);
    
    return bRet;
}
bool CBackpackPageLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    // note: if touch lock sprite: set m_bTouchLock = true & return false
    m_bTouchLock = false;
    m_nTouchTag = -1;
    CCLog("CBackpackPageLayer::touchBegin");
    bool bRect = false;
    
    // detect lock buttons:
    int lockCount = m_pMasks->count();
    CCSprite * sprite = NULL;
    for (int i = 0;  i < lockCount; i++)
    {
        sprite = (CCSprite*) m_pMasks->objectAtIndex(i);
        if (CPtTool::isInNode(sprite, pTouch))
        {
            m_bTouchLock = true;
            return bRect;
        }
    }
    CCSprite * tmpSprite = NULL;
    
    // detect use and delete button:
    for (int i = 0; i < m_nOpenGridNumber; i++)
    {
        
        // test:
        // test end:
        if (CPtTool::isInNode(m_pDeleteButtons[i], pTouch))
        {
            m_nTouchTag =(int) m_pDeleteButtons[i]->getUserData();
            tmpSprite = m_pDeleteButtons[i];
            
            CCLog("delete begin: %d, %d ", m_nTouchTag, i);
            bRect = true;
            break;
        }
        if (CPtTool::isInNode(m_pUseButton[i], pTouch))
        {
            m_nTouchTag = (int) m_pUseButton[i]->getUserData();
            tmpSprite = m_pUseButton[i];
            CCLog("use begin: %d", m_nTouchTag);
            bRect = true;
            break;
        }
    }
    if (m_nTouchTag == -1)
    {
        bRect = false;
    }else
    {
        tmpSprite->setTextureRect(m_aButtonRect[1]);
        
    }
    
    return bRect;
    
}
void CBackpackPageLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CBackpackPageLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    // two click event:
    // 1. use and delete:
    // 2. unlock grid:
    
    if (m_bTouchLock)
    {
        // do open the grid:
        int lockCount = m_pMasks->count();
        CCSprite * sprite = NULL;
        for (int i = 0;  i < lockCount; i++)
        {
            sprite = (CCSprite*) m_pMasks->objectAtIndex(i);
            if (CPtTool::isInNode(sprite, pTouch))
            {
                CCLog("open the grid:");
                openGrid();
                break;
            }
        }

       
        return;
    }
    
    
    // reset button state:
    if (m_nTouchTag!= -1)
    {
        if (m_nTouchTag >= UseTag)
        {
            m_pUseButton[m_nTouchTag-UseTag]->setTextureRect(m_aButtonRect[0]);
        }else
        {
            m_pDeleteButtons[m_nTouchTag-DeleteTag]->setTextureRect(m_aButtonRect[0]);
        }
        
    }
    int tag = -1;
    for (int i = 0; i < m_nOpenGridNumber; i++)
    {
        if (CPtTool::isInNode(m_pDeleteButtons[i], pTouch))
        {
            tag =(int) m_pDeleteButtons[i]->getUserData();
            break;
        }
        if (CPtTool::isInNode(m_pUseButton[i], pTouch))
        {
            tag =(int) m_pUseButton[i]->getUserData();
            break;
        }
    }
    if (tag != -1)
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        if (m_nTouchTag == tag)
        {
            // delete event:
            CCLog("delete end: %d", m_nTouchTag);
            
        }else if(m_nTouchTag == tag)
        {
            // use event:
             CCLog("use: end %d", m_nTouchTag);
        }
        
    }

    
}
void CBackpackPageLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CBackpackPageLayer::initCBackpackPageLayer(int inOpenNumber)
{
    
    
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "daoju.plist"));
    
    m_pMasks = CCArray::createWithCapacity(9);
    m_pMasks->retain();
    
    m_pItemNums = CCArray::createWithCapacity(9);
    m_pItemNums->retain();
    
    initPanel(true);
    // test:
    m_nOpenGridNumber =inOpenNumber;
    addMask(AllGridInPageNumber-m_nOpenGridNumber);
    
    if (inOpenNumber != 0)
    {
        // use grid:
        addItems(6);
    }
  
}

void CBackpackPageLayer::handlerTouch()
{
    
}

void CBackpackPageLayer::addItems(int inUseNum)
{
    inUseNum = inUseNum > AllGridInPageNumber ? AllGridInPageNumber : inUseNum;

    CCNode * node = NULL;
    int array[]={1,0};
    for (int i = 0 ; i < inUseNum; i++)
    {
        array[1] = i;
        node =  m_cMaps->getElementByTags(array,2);
        
        // test:
        CPtProp *prop = SinglePropConfigData::instance()->getPropById(200001+i);
        if (prop)
        {
            setItem(node, prop);
        }
        
    }
}
void CBackpackPageLayer::addMask(int inCloseNum)
{
    int array[]={1,0,0};
    inCloseNum  = inCloseNum >= AllGridInPageNumber ? AllGridInPageNumber : inCloseNum;
    inCloseNum = inCloseNum /3;
    CCNode * node = NULL;
    CCSprite * sprite = NULL;

    if (inCloseNum > 0)
    {
        for (int i = (3-inCloseNum)*3; i < AllGridInPageNumber; i++)
        {
            array[1]= i;
            node =  m_cMaps->getElementByTags(array, 2);
            sprite = CCSprite::createWithSpriteFrameName("lock.png");
            node->addChild(sprite, 1000);
            node = m_cMaps->getElementByTags(array, 3);
            sprite->setAnchorPoint(node->getAnchorPoint());
            sprite->setPosition(node->getPosition());
            
            m_pMasks->addObject(sprite);
        }
    }
    
}

void CBackpackPageLayer::initPanel(bool inResetTexture)
{
    CCTexture2D * texture = NULL;
    if (inResetTexture)
    {
       
        CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("normal.png");
        texture = frame->getTexture();
        m_aButtonRect[0] = frame->getRect();
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("pressed.png");
        m_aButtonRect[1] = frame->getRect();
        
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("disabled.png");
        m_aButtonRect[2] = frame->getRect();

    }
      
    CCNode *mapLayer = NULL;
    CCLabelTTF * label = NULL;
    int array[]={1,0,0};
    for (int i = 0; i < AllGridInPageNumber; i++)
    {
        array[1] = i;
        mapLayer = m_cMaps->getElementByTags(array, 2);
    
        // set use button:
        CCSprite * tmp = (CCSprite *) mapLayer->getChildByTag(19) ;
        tmp->setUserData((void*)(-1));
        if (inResetTexture)
        {
             tmp->setTexture(texture);
        }
       
        label = CCLabelTTF::create("Use", "Arial", 18);
        label->setPosition(ccp(60,18));
        tmp->addChild(label,2, 1);
        if (i < m_nUserGridNumber)
        {
            label->setColor(ccGREEN);
            tmp->setTextureRect(m_aButtonRect[0]);

        }else
        {
            label->setColor(ccGREEN);
            tmp->setTextureRect(m_aButtonRect[2]);
            tmp->setVisible(false);
        }
        m_pUseButton[i] = tmp;
        
        // set delete button:
        tmp = (CCSprite *) mapLayer->getChildByTag(20) ;
        tmp->setUserData((void*)(-1));
        if (inResetTexture)
        {
            tmp->setTexture(texture);
        }
        label = CCLabelTTF::create("Delete", "Arial", 18);
        label->setPosition(ccp(60,18));
        tmp->addChild(label,2, 1);
        if (i < m_nUserGridNumber)
        {
            label->setColor(ccRED);
            tmp->setTextureRect(m_aButtonRect[0]);
            
        }else
        {
            label->setColor(ccRED);
            tmp->setTextureRect(m_aButtonRect[0]);
            tmp->setVisible(false);
        }
        m_pDeleteButtons[i] = tmp;

    }
    
}

void CBackpackPageLayer::clearItem(CCNode *node)
{
    
    CCNode * parent;
    if (node)
    {
        for (int i = 16;  i<21 ; i++)
        {
            parent = node->getChildByTag(i);
            if (i == 19 || i == 20)
            {
                ((CCLabelTTF *)parent->getChildByTag(1))->setColor(ccGRAY);
                ((CCSprite *)parent)->setTextureRect(m_aButtonRect[2]);
            }else
            {
                parent->removeAllChildrenWithCleanup(true);
            }
        }
        
        int tag = node->getTag();
        CCLog("clearItem: %d", tag);
        parent = m_pUseButton[tag];
        node = m_pDeleteButtons[tag];
        if (parent)
        {
            parent->setUserData((void*)-1);
        }if (node)
        {
            node->setUserData((void*)-1);
        }
        
    }
    
}
void CBackpackPageLayer::resetPanel()
{
    CCNode *mapLayer = NULL;

    int array[]={1,0};
    for (int i = 0; i < AllGridInPageNumber; i++)
    {
        array[1] = i;
        mapLayer = m_cMaps->getElementByTags(array, 2);
        if (mapLayer)
        {
            for (int j = 16; j < 21; j++)
            {
                mapLayer = mapLayer->getChildByTag(j);
                if (mapLayer)
                {
                    mapLayer->removeAllChildrenWithCleanup(true);
                }
            }
            
        }
        
        //reset button tag:
        m_pDeleteButtons[i]->setUserData((void*)-1);
        m_pUseButton[i]->setUserData((void*)-1);
        
    }
    if (m_pItemNums)
    {
        m_pItemNums->removeAllObjects();
    }
  

}
/*
 * 1.可使用，可删除
 * 2.可使用不可删除
 * 3.不可使用可删除
 * 4.不可使用不可删除
 */
void CBackpackPageLayer::setItem(CCNode *node, CPtProp *inData)
{
    if (node && inData)
    {
        int tag = node->getTag();
        // tag: 19-->use, 20-->delete:
        CCSprite *useButton =(CCSprite *) node->getChildByTag(19);
        CCSprite *deleteButton =(CCSprite*)node->getChildByTag(20);
       
        CCNode * parent = NULL;
        CCLabelTTF *label = NULL;
        CCRect tmpRect[2] = {m_aButtonRect[0], m_aButtonRect[0]};
        ccColor3B tmpColor[2] = {ccGREEN , ccRED};
        bool tmpFlag[2] = {true,true};
        int useDelete = inData->getUseDelete();
        
        switch (useDelete)
        {
            case 1:
                deleteButton->setUserData((void *)(tag+DeleteTag));
                useButton->setUserData((void*)(tag+UseTag));
                break;
            case 3:
                deleteButton->setUserData((void *)(tag+DeleteTag));
                tmpRect[0] = m_aButtonRect[2];
                tmpColor[0] = ccGRAY;
                tmpFlag[0] = false;
                break;
            case 2:
                useButton->setUserData((void*)(tag+UseTag));
                tmpColor[1] = ccGRAY;
                tmpRect[1] = m_aButtonRect[0];
                break;
            case 4:
                
                tmpRect[0] = m_aButtonRect[2];
                tmpColor[0] = ccGRAY;
                tmpColor[1] = ccGRAY;
                tmpRect[1] = m_aButtonRect[0];
                tmpFlag[0] = false;
                break;
            default:
                break;
        }
        
        deleteButton->setVisible(tmpFlag[1]);
        useButton->setVisible(tmpFlag[0]);
        deleteButton->setTextureRect(tmpRect[1]);
        useButton->setTextureRect(tmpRect[0]);
        
        label = (CCLabelTTF*)deleteButton->getChildByTag(1);
        label->setColor(tmpColor[1]);
        label = (CCLabelTTF*)useButton->getChildByTag(1);
        label->setColor(tmpColor[0]);
        CCSize size;
        
        
        // set name:
        parent = node->getChildByTag(16);
        label = CCLabelTTF::create(inData->getPropName().c_str(), "arial", 13);
        size = parent->getContentSize();
        label->setPosition(ccp(size.width/2, size.height/2));
        parent->addChild(label, 1);
        
        // set tips:
        parent = node->getChildByTag(17);
        label = CCLabelTTF::create(inData->getTips().c_str(), "arial", 13);
        label->setPosition(ccp(116, 90));
        label->setAnchorPoint(ccp(0,1));
        label->setDimensions(CCSizeMake(139, 0));
        parent->addChild(label, 1);
        CCSprite *icon = CCSprite::create(CSTR_FILEPTAH(g_propImagesPath, inData->getIconName().c_str()));
        icon->setPosition(ccp(60,60));
        parent->addChild(icon);
        
        int tmpTag = (int)deleteButton->getUserData();
        CCLog("delet Tag: %d", tmpTag);
        tmpTag = (int) useButton->getUserData();
         CCLog("use Tag: %d", tmpTag);

    }

}


void CBackpackPageLayer::openGrid()
{
    
    // handler event:
    // updateData:
    m_nOpenGridNumber += 3;
    // more todo :
    
    // update ui
    openGridUI();
    
    // play sound effect:
    PtSoundTool::playSysSoundEffect("UI_click.wav");
   
}
void CBackpackPageLayer::openGridUI()
{
    if (m_pMasks->count() < 3)
    {
        CCLog("errror:CBackpackPageLayer::openGridUI()");
        return;
    }
    CCSprite* sprite = NULL;
    for (int i = 0; i < 3; i++)
    {
        sprite = (CCSprite*)m_pMasks->objectAtIndex(0);
        sprite->removeFromParentAndCleanup(true);
        m_pMasks->removeObjectAtIndex(0);
    }
}