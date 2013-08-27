//
//  CBackpackContainerLayer.cpp
//  91.cube
//
//  Created by phileas on 13-8-21.
//
//

#include "CBackpackContainerLayer.h"
#include "gameConfig.h"
#include "Utility.h"

CBackpackContainerLayer*  CBackpackContainerLayer::create(int inOpenNumber)
{
    CBackpackContainerLayer *layer = new CBackpackContainerLayer();
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

CBackpackContainerLayer::CBackpackContainerLayer()
{
    
    m_cMaps = NULL;
    m_pItems = NULL;
    m_pContainer = NULL;
    m_pIndicators = NULL;
    m_pTabs = NULL;
    m_pCurrentPage = NULL;
    
    m_bPageTouchEnable = false;
    m_nOpenGridCount = 9;
    m_nPageCount = 5;
    m_nMaxPageCount = 5;
    m_nTouchTag = -1;
    m_nCurrentTab  = 0;
    m_cContainerSize = CCSizeMake(863, 623);
    m_cContainerOffset = ccp(80,50);
}

CBackpackContainerLayer::~CBackpackContainerLayer()
{
    //relase resouce:
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    if (m_pItems)
    {
        m_pItems->release();
    }
    CC_SAFE_RELEASE(m_pIndicators);
    CC_SAFE_RELEASE(m_pTabs);
}

bool CBackpackContainerLayer::init(int inOpenNumber)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initCBackpackContainerLayer(inOpenNumber);
        bRet = true;
    } while (0);
    
    return bRet;
}
bool CBackpackContainerLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bScrollEnable = false;
    m_bMoveEnable = false;
    m_bPageTouchEnable = false;
    m_nTouchTag =  TouchRect::SearchTouchTag(pTouch->getLocation(), m_cTouches);
    if (m_nTouchTag != -1)
    {
        if ( m_nCurrentTab == (m_nTouchTag-3000))
        {
            return true ;
        }else
        {
            setStateTab(m_nTouchTag, 2);
        }
    }else
    {
        m_bPageTouchEnable = m_pCurrentPage->ccTouchBegan(pTouch, pEvent);
        if (m_bPageTouchEnable)
        {
            return  m_bPageTouchEnable;
        }
        
        m_cTouchPoint = m_pContainer->getContentOffset();
        m_bScrollEnable = m_pContainer->ccTouchBegan(pTouch, pEvent);
    }
   
    return true;
    
}
void CBackpackContainerLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("the distance: %f", ccpDistance(CCPointZero, pTouch->getDelta()));
    if (m_bMoveEnable == false && ccpDistance(CCPointZero, pTouch->getDelta())>5)
    {
         m_bMoveEnable = true;
    }
   
    
    if (m_bPageTouchEnable)
    {
        m_pCurrentPage->ccTouchMoved(pTouch, pEvent);
        return;
    }
    
    CCLog("CBackpackContainerLayer::ccTouchMove");
    if (m_bScrollEnable)
    {
        CCLog("scroll");
        m_pContainer->ccTouchMoved(pTouch, pEvent);
    }
}
void CBackpackContainerLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
  
    if (m_nTouchTag != -1)
    {
        if ( m_nTouchTag == TouchRect::SearchTouchTag(pTouch->getLocation(), m_cTouches))
        {
            if (m_nCurrentTab == m_nTouchTag-3000)
            {
                return;
            }else
            {
                PtSoundTool::playSysSoundEffect("UI_click.wav");
                handlerTouch();
            }
        
        }else
        {
            if (m_nCurrentTab == m_nTouchTag-3000)
            {
                return;
            }
            // reset sprite normal state:
            setStateTab(m_nTouchTag, 1);
        }
        
    }else
    {
        if (m_bPageTouchEnable)
        {
            m_pCurrentPage->ccTouchEnded(pTouch, pEvent);
            return;
        }
        if (m_pCurrentPage->getTouchLock() && m_bMoveEnable == false)
        {
            m_pCurrentPage->ccTouchEnded(pTouch, pEvent);
            m_pContainer->ccTouchEnded(pTouch, pEvent);
            return;
        }
        if (m_bScrollEnable)
        {
            m_pContainer->ccTouchEnded(pTouch, pEvent);
            CCPoint endPoint = m_pContainer->getContentOffset();
            float distance = endPoint.x-m_cTouchPoint.x;
            adjustScrollView(distance);
            
        }
    }
    
   

}
void CBackpackContainerLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    ccTouchEnded(pTouch, pEvent);
}


void CBackpackContainerLayer::initCBackpackContainerLayer(int inOpenNumber)
{
    int tmpPageCount = inOpenNumber/9+ 1 ;
    m_nPageCount = m_nPageCount >= tmpPageCount ? tmpPageCount: m_nPageCount;
    
    loadResource();
    
    
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "beibao.plist"));
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-10000);
    
    m_pItems = CCArray::createWithCapacity(5);
    m_pItems->retain();
   
   
    
    // init scrollview:
    m_pContainer = CCScrollView::create(m_cContainerSize);
    m_pContainer->setContentOffset(CCPointZero);
    m_pContainer->setTouchEnabled(false);
    m_pContainer->setDirection(kCCScrollViewDirectionHorizontal);
    m_pContainer->setPosition(m_cContainerOffset);
    addChild(m_pContainer, 100);
    
    m_nCurPage = 0;
    // add backpackpage to container:
    for (int i = 0; i < m_nMaxPageCount; i++)
    {
        CBackpackPageLayer * item  = NULL;
        if (inOpenNumber-9 >= 0)
        {
            inOpenNumber -= 9;
            item = CBackpackPageLayer::create(9);
        }
        else if(inOpenNumber> 0)
        {
             item = CBackpackPageLayer::create(inOpenNumber);
        }else
        {
              item = CBackpackPageLayer::create();
        }
      
        m_pItems->addObject(item);
        item->setPosition(ccp(-m_cContainerOffset.x+i*m_cContainerSize.width, -m_cContainerOffset.y));
         m_pContainer->addChild(item);
        if (i == m_nCurPage)
        {
            m_pCurrentPage = item;
        }
        if (i == 0)
        {
            
        }
    }
    m_pContainer->setContentSize(CCSizeMake(m_cContainerSize.width*m_nPageCount, m_cContainerSize.height));
    
    // init indicator:
    int array[] = {1,3,0,0};
    m_pIndicators = CCArray::create();
    m_pIndicators->retain();
    CCNode * node = NULL;
    for (int i = 0; i < m_nMaxPageCount ; i++)
    {
        array[2] = i;
        node = m_cMaps->getElementByTags(array, 3);
        if (node)
        {
            m_pIndicators->addObject(node);
        }
    }
    m_cSelectedIndicateRect =((CCSprite*)m_pIndicators->objectAtIndex(0))->getTextureRect();
    m_cUnSelectedIndicateRect =((CCSprite*) m_pIndicators->objectAtIndex(1))->getTextureRect();

    array[1] = 1;
    CCSprite * sprite = NULL;
    CCLabelTTF *label = NULL;
    
   
    m_cPressedTabRect = CCSprite::createWithSpriteFrameName("fenyebutton_pressed.png")->getTextureRect();
   
    // init tabs:
    m_pTabs = CCArray::create();
    m_pTabs->retain();
    
    array[0] = array[1] = 1;
    CCNode *mapParent = m_cMaps->getElementByTags(array, 2);
    array[0] = 57;
    array[1] = 194;
    array[2] = 331;
    array[3] = 468;
    const char * str[4]={"backpack_all", "backpack_prop", "backpack_material", "backpack_task"};
    for (int i = 0; i < 4; i++)
    {
        label = CCLabelTTF::create("", "Arial", 20);
        label->setPosition(ccp(70,20));
        if (i == 0)
        {
            sprite = CCSprite::createWithSpriteFrameName("fenyebutton_selected.png");
            m_cSelectedTabRect = sprite->getTextureRect();

        }else
        {
            sprite = CCSprite::createWithSpriteFrameName("fenyebutton_normal.png");
             m_cNoramlTabRect = sprite->getTextureRect();
        }
        if(sprite)
        {
            sprite->setPosition(ccp(array[i],703));
            sprite->setAnchorPoint(ccp(0, 1));
            m_pTabs->addObject(sprite);
            mapParent->addChild(sprite, 100, 3000+i);
            Utility::addTouchRect(3000+i, sprite, m_cTouches);
            sprite->addChild(label);
            label->setString(Utility::getWordWithFile("word.plist", str[i]).c_str());
        }

        
    }
  
    goAllTab();
}

void CBackpackContainerLayer::handlerTouch()
{
    setStateTab(m_nCurrentTab+3000, 1);
    setStateTab(m_nTouchTag, 3);
    switch (m_nTouchTag)
    {
        case 3000:
            m_nCurrentTab = 0;
            goAllTab();
            break;
        case 3001:
            m_nCurrentTab = 1;
            goPropTab();
            break;
        case 3002:
            m_nCurrentTab = 2;
            goMaterialTab();
            break;
        case 3003:        
            m_nCurrentTab = 3;
            goTaskTab();
            break;
        default:
            break;
    }
    
    
}


void CBackpackContainerLayer::adjustScrollView(float offset)
{
    int nPage = m_nCurPage;
    int nAdjustPage = 0;
  
    
    if ( offset < -m_pContainer->getViewSize().width/3)
    {
        nAdjustPage=nPage+1;
    }
    else if (offset >m_pContainer->getViewSize().width/3)
    {
        nAdjustPage=nPage-1;
    }
    else
    {
        nAdjustPage=nPage;
    }
    
    nAdjustPage=MIN(nAdjustPage,m_nPageCount);
    nAdjustPage=MAX(nAdjustPage,0);
    
    //change indicator:
    updateIndicator(m_nCurPage, nAdjustPage);
    
    scrollToPage(nAdjustPage);
    CCLog("the page: %d, %d", nAdjustPage, nPage);
    updateCurrentPage();
}

void CBackpackContainerLayer::updateIndicator(const int &srcIndex, const int &desrIndex)
{
    if (srcIndex == desrIndex)
    {
        return;
    }
    CCSprite * sprite = ((CCSprite*)m_pIndicators->objectAtIndex(srcIndex));
    if (sprite)
    {
        sprite->setTextureRect(m_cUnSelectedIndicateRect);
    }
    sprite = ((CCSprite*)m_pIndicators->objectAtIndex(desrIndex));
    if (sprite)
    {
        sprite->setTextureRect(m_cSelectedIndicateRect);
    }
}
void CBackpackContainerLayer::scrollToPage(int inPage)
{
   
    inPage = inPage > m_nPageCount ? m_nPageCount : (inPage < 0) ? 0 : inPage;
    m_nCurPage = inPage;
    // 关闭CCScrollView中的自调整
    m_pContainer->unscheduleAllSelectors();
    CCPoint adjustPos=ccp(-inPage*m_cContainerSize.width,0);
    // 调整位置
    m_pContainer->setContentOffsetInDuration(adjustPos, 0.2f);
}

void CBackpackContainerLayer::loadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "backpack.plist"), CSTR_FILEPTAH(g_mapImagesPath, "backpack.png"));
}
void CBackpackContainerLayer::releaseResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
}

/*
 * @param inState: 1->normal; 2->pressed; 3->selected:
 */
void CBackpackContainerLayer::setStateTab(int inTag, int inState)
{
    CCRect rect;
    if (inState == 1)
    {
        rect = m_cNoramlTabRect;
    }else if(inState == 2)
    {
        rect = m_cPressedTabRect;
    }else if(inState == 3)
    {
        rect = m_cSelectedTabRect;
    }else
    {
        // error:
        return;
    }
    
    CCSprite *sprite = NULL;
    int i = 0;
    switch (inTag)
    {
        case 3000:
        case 3001:
        case 3002:
        case 3003:
            i =  inTag -3000;
            break;
        default:
            return;
    }
    sprite = (CCSprite*)m_pTabs->objectAtIndex(i);
    sprite->setTextureRect(rect);

}

void CBackpackContainerLayer::updateIndicators(const int & inCount)
{
    CCNode * item = NULL;
    m_nPageCount = inCount;
    for (int i = 0; i < m_nMaxPageCount; i++)
    {
         item = (CCNode *)m_pIndicators->objectAtIndex(i);
        if (i < inCount)
        {
            item->setVisible(true);
        }else
        {
            item->setVisible(false);
        }
    }
    
}

void CBackpackContainerLayer::updatePageContent(const int &inCount)
{
    m_pContainer->getContainer()->removeAllChildrenWithCleanup(true);
    CCNode *item = NULL;
    // add backpackpage to container:
    for (int i = 0; i < inCount; i++)
    {
        item = (CCNode*) m_pItems->objectAtIndex(i);
        m_pContainer->addChild(item);
    }
    m_pContainer->setContentSize(CCSizeMake(m_cContainerSize.width*inCount, m_cContainerSize.height));
    m_pContainer->setContentOffset(CCPointZero);
    updateIndicator(m_nCurPage, 0);
    m_nCurPage = 0;
}

void CBackpackContainerLayer::updateUI(const int &inCount)
{
    updateIndicators(inCount);
    updatePageContent(inCount);
    updateCurrentPage();
}

void CBackpackContainerLayer::updateCurrentPage()
{
    m_pCurrentPage = (CBackpackPageLayer*)m_pItems->objectAtIndex(m_nCurPage);
}

// action:

void CBackpackContainerLayer::goAllTab()
{
    updateUI(2);
    
    
}
void CBackpackContainerLayer::goPropTab()
{
    updateUI(1);
}
void CBackpackContainerLayer::goMaterialTab()
{
    updateUI(1);
    
}
void CBackpackContainerLayer::goTaskTab()
{
    updateUI(1);
}

void CBackpackContainerLayer:: closeBackPack()
{
    
    removeFromParentAndCleanup(true);
}