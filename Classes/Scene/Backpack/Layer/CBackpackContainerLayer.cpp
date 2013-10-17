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
    m_pCloseButton = NULL;
    
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
    SinglePlayer::instance()->updateProps();
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
        int value =clock();
        CC_BREAK_IF(!CCLayer::init());
        initCBackpackContainerLayer(inOpenNumber);
        CCLog("aaaaa====%f",(clock()-value)*1.0/CLOCKS_PER_SEC);
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
        if (m_nTouchTag == 8)
        {
            // pressed close button:
            
            return  true;
        }
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
                if (m_nTouchTag == 8)
                {
                    removeFromParentAndCleanup(true);
                    return;
                }
                handlerTouch();
            }
        
        }else
        {
            if (m_nTouchTag == 8)
            {
                // close Button event:
                return;
            }
            
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

static void test(CCNode *pSender)
{
    CCLog("the begin %d", time(NULL), clock());
    LayoutLayer* map = LayoutLayer::create();
    map->initWithFile(pSender, CSTR_FILEPTAH(plistPath, "beibao.plist"));
    for (int i = 0; i < 5; i++)
    {
        CCLayer *layer = CCLayer::create();
        map = LayoutLayer::create();
        map->initWithFile(layer, CSTR_FILEPTAH(plistPath, "daoju.plist"));
        pSender->addChild(layer);
    }
     CCLog("the end %d", time(NULL), clock());
}

void CBackpackContainerLayer::initCBackpackContainerLayer(int inOpenNumber)
{
    inOpenNumber = SinglePlayer::instance()->getOpenGridCount();
    initGrids();
    int tmpPageCount = inOpenNumber/9+ 1 ;
    m_nPageCount = m_nPageCount >= tmpPageCount ? tmpPageCount: m_nPageCount;
    
    CCLog("the size %d", m_cNumInGrid.size());
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
    multimap<int, int>::iterator mapIterator = m_cNumInGrid.begin();
    multimap<int, int>::iterator mapIteratorEnd = mapIterator;
   
    for (int i = 0; i < m_nMaxPageCount; i++)
    {
        CBackpackPageLayer * item  = NULL;
        if (inOpenNumber-9 >= 0)
        {
            int j =0;
            do {
                if (mapIteratorEnd == m_cNumInGrid.end())
                {
                    break;
                }
                j++;
                mapIteratorEnd++;
            } while (j < 9);
            inOpenNumber -= 9;
            CCLog("the begin-- %d", clock());
            item = CBackpackPageLayer::create(&m_cNumInGrid, mapIterator, mapIteratorEnd, 9);
            mapIterator = mapIteratorEnd;
            CCLog("the end-- %d", clock());
        }
        else
        {
             item = CBackpackPageLayer::create(&m_cNumInGrid, mapIterator, m_cNumInGrid.end(), inOpenNumber);
            CCLog("%d", inOpenNumber);
            inOpenNumber = 0;
          
        }
        m_pItems->addObject(item);
        item->setCurrentPageTag(i);
        item->setContainerHandler(this);
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
  
    CCLog("the size: %d", m_cNumInGrid.size());
    
    
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
    array[0] = 1, array[1] = 1, array[2] = 8;
    m_pCloseButton = (CCSprite*) m_cMaps->getElementByTags(array, 3);
//    Utility::addTouchRect(8, m_pCloseButton, m_cTouches);
    CCRect rect = Utility::getNodeTouchRect(m_pCloseButton) ;
    rect.size.width *= 1.6;
    rect.size.height *= 1.6;
    Utility::addTouchRect(rect, 8, m_pCloseButton, m_cTouches);
  
    updatePageContent(m_nPageCount);
    updateIndicators(m_nPageCount);
}

void CBackpackContainerLayer::initGrids()
{
    map<int, int> allProps = SinglePlayer::instance()->getPlayerProps();
    CPtProp * tmp = NULL;
    CPtPropConfigData * propData = SinglePropConfigData::instance();
    int limitNum = 0;
    int lastNum = 0;
    int itemNum = 0;
    for (map<int, int>::iterator i = allProps.begin(); i != allProps.end();  i++)
    {
        tmp = propData->getPropById(i->first);
        CCAssert(tmp != NULL, "no has this prop");
        if (tmp->getIsOnlyNum() == 1)
        {
            m_cNumInGrid.insert(map<int, int>::value_type(i->first, 1));
            
        }else
        {
            itemNum = i->second;
            limitNum = tmp->getLimitNum();
            if (itemNum <= limitNum)
            {
                  m_cNumInGrid.insert(map<int, int>::value_type(i->first, itemNum));
            }else
            {
                for (int j = 0; j < itemNum/limitNum; j++)
                {
                    m_cNumInGrid.insert(map<int, int>::value_type(i->first, limitNum));
                }
                lastNum = itemNum%limitNum;
                if (lastNum != 0)
                {
                   m_cNumInGrid.insert(map<int, int>::value_type(i->first, lastNum));
                }
            }
            
        }
    }
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
    
    nAdjustPage=MIN(nAdjustPage,m_nPageCount-1);
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

void CBackpackContainerLayer::updatePageContent(const int &inCount,bool inFirstPage /*= true*/)
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
    if (inFirstPage)
    {
        updateToFirstPage();
    }
    
}

void CBackpackContainerLayer::updateUI(const int &inCount, bool inFirstPage /*= true*/)
{
    updateIndicators(inCount);
    updatePageContent(inCount,inFirstPage);
    updateCurrentPage();
}

void CBackpackContainerLayer::updateCurrentPage()
{
    m_pCurrentPage = (CBackpackPageLayer*)m_pItems->objectAtIndex(m_nCurPage);
}

void CBackpackContainerLayer::updateToFirstPage()
{
    m_pContainer->setContentOffset(CCPointZero);
    updateIndicator(m_nCurPage, 0);
    m_nCurPage = 0;
}

void CBackpackContainerLayer::updateTabContent(vector<multimap<int, int>::iterator> & inVector, int inFromPage /*= 0*/, bool inFirstPage /*= true*/)
{
    
    vector<multimap<int, int>::iterator> &tmpVector  = inVector;
    int size = tmpVector.size();
    if (size == 0)
    {
        // ...
        
    }
    int pageCount = size/9 + (size%9 == 0 ? 0 : 1);
    if (inFromPage != 0 && inFromPage >= pageCount)
    {
        return;
    }
    CBackpackPageLayer *itemPage = NULL;
    updateUI(pageCount, inFirstPage);
    vector<multimap<int, int>::iterator>::iterator beginIterator = tmpVector.begin();
    for (int i  = inFromPage; i <pageCount; i++)
    {
        itemPage = (CBackpackPageLayer*) m_pItems->objectAtIndex(i);
        if (size < 9)
        {
            itemPage->updatePageContent( vector<multimap<int, int>::iterator>(beginIterator, tmpVector.end()));
            
        }else
        {
            itemPage->updatePageContent( vector<multimap<int, int>::iterator>(beginIterator, beginIterator+9));
            beginIterator = beginIterator+9;
            size -= 9;
            
        }
        
        
        
    }

}
// action:

void CBackpackContainerLayer::goAllTab()
{
    
    int  inOpenNumber = SinglePlayer::instance()->getOpenGridCount();
    CCLog("the openCountNumber: %d", inOpenNumber);
    int tmpPageCount = inOpenNumber/9+ 1 ;
    CBackpackPageLayer * item  = NULL;

    // add backpackpage to container:
    multimap<int, int>::iterator mapIterator = m_cNumInGrid.begin();
    multimap<int, int>::iterator mapIteratorEnd = mapIterator;
    CCLog("the openCountNumber: %d, pageCount: %d", inOpenNumber, tmpPageCount);
    for (int i = 0; i < tmpPageCount; i++)
    {
        item = (CBackpackPageLayer *)m_pItems->objectAtIndex(i);
        
        if (inOpenNumber-9 >= 0)
        {
            int j =0;
            do {
                if (mapIteratorEnd == m_cNumInGrid.end())
                {
                    break;
                }
                j++;
                mapIteratorEnd++;
            } while (j < 9);
            
            inOpenNumber -= 9;
            item->updatePageContent(mapIterator, mapIteratorEnd, 9);
            mapIterator = mapIteratorEnd;
        }
        else if(inOpenNumber == 0)
        {
            CCLog("the openNumber %d", inOpenNumber);
            item->updatePageContent(mapIterator, m_cNumInGrid.end(), 0);
        }else
        {
            item->updatePageContent(mapIterator, m_cNumInGrid.end(), inOpenNumber);
            inOpenNumber = 0;
        }
        
    }
    updateUI(tmpPageCount);
        
}
void CBackpackContainerLayer::goPropTab()
{
    vector<multimap<int, int>::iterator> tmpVector  = getFilterPointer(1);
    updateTabContent(tmpVector);
}
void CBackpackContainerLayer::goMaterialTab()
{
    vector<map<int, int>::iterator> tmpVector  = getFilterPointer(3);
    updateTabContent(tmpVector);
    
}
void CBackpackContainerLayer::goTaskTab()
{
    vector<map<int, int>::iterator> tmpVector  = getFilterPointer(2);
    updateTabContent(tmpVector);
}

void CBackpackContainerLayer:: closeBackPack()
{
    
    removeFromParentAndCleanup(true);
}

/*
 * @breif :获取过滤后的grid数据指针队列
 * @param: inType: 1--> props; 3-->Material; 2-->task
 */
vector<multimap<int, int>::iterator> CBackpackContainerLayer::getFilterPointer(int inType)
{
    vector<map<int, int>::iterator> tmpVector ;
    CPtPropConfigData * propData = SinglePropConfigData::instance();
    for (multimap<int, int>::iterator i = m_cNumInGrid.begin(); i != m_cNumInGrid.end(); i++)
    {
        if (propData->getPropById(i->first)->getPropType() == inType)
        {
            tmpVector.push_back(i);
        }
    }
    return tmpVector;
}

/* @note：调用之前要先grid中value为0的格子
 * @breif: 重派分页：
 * @param: inFromPag (0-5)下一页：
 */
void CBackpackContainerLayer::reLoadPage(int inFromPage)
{
        
    vector<multimap<int, int>::iterator> dataIterators;
    CBackpackPageLayer *pageLayer = NULL;
    for (int i = inFromPage; i < m_nPageCount; i++)
    {
        pageLayer =(CBackpackPageLayer*) m_pItems->objectAtIndex(i);
        if (pageLayer)
        {
             const  vector<multimap<int, int>::iterator> &tmpIterator =  pageLayer->getCurrentGridDataIterators();
            if (tmpIterator.size() == 0)
            {
                break;
            }else
            {
                dataIterators.insert(dataIterators.end(), tmpIterator.begin(), tmpIterator.end());
            }
        }
    }
    
    if (m_nCurrentTab == 0)
    {
        
        int  inOpenNumber = SinglePlayer::instance()->getOpenGridCount();
        CCLog("the openCountNumber: %d", inOpenNumber);
        int tmpPageCount = inOpenNumber/9+ 1 ;
        CBackpackPageLayer * item  = NULL;
        
        // add backpackpage to container:
        multimap<int, int>::iterator mapIterator  = m_cNumInGrid.begin();

        multimap<int, int>::iterator mapIteratorEnd = mapIterator;
    
        int count = inFromPage*9;

        for (; count > 0; )
        {
            mapIterator++;
            count--;

        }
        mapIteratorEnd = mapIterator;

        inOpenNumber -= inFromPage * 9;
        CCLog("the has data grid count : %d, %d", m_cNumInGrid.size(), dataIterators.size());
        for (int i = inFromPage; i < tmpPageCount; i++)
        {
            CCLog("the openGrid: %d", inOpenNumber);
            item = (CBackpackPageLayer *)m_pItems->objectAtIndex(i);
            
            if (inOpenNumber-9 >= 0)
            {
                int j =0;
                do {
                    if (mapIteratorEnd == m_cNumInGrid.end())
                    {
                        break;
                    }
                    j++;
                    mapIteratorEnd++;
                } while (j < 9);
                
                inOpenNumber -= 9;
                item->updatePageContent(mapIterator, mapIteratorEnd, 9);
                mapIterator = mapIteratorEnd;
            }
            else if(inOpenNumber == 0)
            {
                CCLog("the openNumber %d", inOpenNumber);
                item->updatePageContent(mapIterator, m_cNumInGrid.end(), 0);
            }else
            {
                item->updatePageContent(mapIterator, m_cNumInGrid.end(), inOpenNumber);
                mapIterator = m_cNumInGrid.end();
                inOpenNumber = 0;
            }

            
//            if (inOpenNumber-9 >= 0)
//            {
////                if (dataIterators.size() > 9)
////                {
////                    mapIteratorEnd = dataIterators.at(9);
////                }else
////                {
////                    mapIteratorEnd = m_cNumInGrid.end();
////                }
//                int j = 0;
//                mapIteratorEnd = mapIterator;
//                do {
//                    if (mapIteratorEnd == m_cNumInGrid.end())
//                    {
//                        break;
//                    }
//                    j++;
//                    mapIteratorEnd++;
//                } while (j < 9);
//                inOpenNumber -= 9;
//                item->updatePageContent(mapIterator, mapIteratorEnd, 9);
//                mapIterator = mapIteratorEnd;
//            }
//            else if(inOpenNumber == 0)
//            {
//                CCLog("the openNumber %d", inOpenNumber);
//                item->updatePageContent(mapIterator, m_cNumInGrid.end(), 0);
//            }else
//            {
//                item->updatePageContent(mapIterator, m_cNumInGrid.end(), inOpenNumber);
//                inOpenNumber = 0;
//            }
            
        }
        updateUI(tmpPageCount,false);
        return;
    }

    
  //  if (dataIterators.size()!= 0)
    {
        updateTabContent(dataIterators, inFromPage,false);
    }
    
}

void CBackpackContainerLayer::appendPage()
{
    if (m_nPageCount < m_nMaxPageCount)
    {
        m_nPageCount ++;
        updateIndicators(m_nPageCount);
     
        CBackpackPageLayer *item = NULL;

        item = (CBackpackPageLayer*) m_pItems->objectAtIndex(m_nPageCount-1);
        item->initGridPage();
        m_pContainer->addChild(item);
        m_pContainer->setContentSize(CCSizeMake(m_cContainerSize.width*m_nPageCount, m_cContainerSize.height));

    }
    
}