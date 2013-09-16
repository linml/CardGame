//
//  CPtExpandableListView.cpp
//  91.cube
//
//  Created by phileas on 13-9-10.
//
//

#include "CPtExpandableListView.h"


// implement class of CPtExpandableListViewItem

CPtExpandableListViewItem::CPtExpandableListViewItem()
{
    initData();
}

CPtExpandableListViewItem::~CPtExpandableListViewItem()
{
    CC_SAFE_RELEASE(m_pChildrenItems);
}

int CPtExpandableListViewItem::getChildrenCount()
{
    if (m_bOpen && m_pChildrenItems)
    {
        return m_pChildrenItems->count();
    }
    else
    {
        return 0;
    }
}

CCArray* CPtExpandableListViewItem::getChildren()
{
    if (m_pTargetHandler && m_pGetChildrenSelector && m_bLoadChildren == false)
    {
        m_bLoadChildren = true;
        (m_pTargetHandler->*m_pGetChildrenSelector)(this);
    }
    return m_pChildrenItems;
}

void CPtExpandableListViewItem::setHandler(cocos2d::CCObject *inTarget, SEL_CallFuncO inNormalSelector, SEL_CallFuncO inSelectedSelector, SEL_CallFuncO inGetChildrenSelector)
{
    m_pTargetHandler = inTarget;
    m_pNormalSelector = inNormalSelector;
    m_pSelectedSelector = inSelectedSelector;
    m_pGetChildrenSelector = inGetChildrenSelector;
}

void CPtExpandableListViewItem::setChildrenItem(cocos2d::CCArray *inChildrenItems)
{
    if (inChildrenItems)
    {
        m_bLoadChildren = true;
        inChildrenItems->retain();
        CC_SAFE_FREE(m_pChildrenItems);
        m_pChildrenItems = inChildrenItems;
    }
}

void CPtExpandableListViewItem::setNormal()
{
    if (m_pTargetHandler && m_pNormalSelector)
    {
        (m_pTargetHandler->*m_pNormalSelector)(this);
    }
}

void CPtExpandableListViewItem::setSelected()
{
    if (m_pTargetHandler && m_pSelectedSelector)
    {
        (m_pTargetHandler->*m_pSelectedSelector)(this);
    }
}

// protected method:
void CPtExpandableListViewItem::initData()
{
    m_bOpen = false;
    m_bLoadChildren = false;
    
    m_eItemType = PARENT_TYPE;
    m_pChildrenItems = NULL;
    m_pTargetHandler = NULL;
    m_pNormalSelector = NULL;
    m_pSelectedSelector = NULL;
    m_pGetChildrenSelector = NULL;
    m_pDisplayView = NULL;
    m_nIndex = -1;
}

void CPtExpandableListViewItem::setDisplayView(cocos2d::CCNode *var)
{
    CC_SAFE_RELEASE(m_pDisplayView);
    m_pDisplayView = var;
    m_pDisplayView->retain();
}

CCNode* CPtExpandableListViewItem::getDisplayView()
{
    return m_pDisplayView;
}

// implement class of CPtExpandableListViewItemList


CPtExpandableListViewItemList::CPtExpandableListViewItemList()
{
    m_pExpListViewItems = NULL;
    m_nWhichOneOpen = -1;
}

CPtExpandableListViewItemList::~CPtExpandableListViewItemList()
{
    CC_SAFE_RELEASE(m_pExpListViewItems);
}

CPtExpandableListViewItem* CPtExpandableListViewItemList::getItemByIndex(int inIndex)
{
    if (m_pExpListViewItems && inIndex >= 0)
    {
        if (inIndex <= m_nWhichOneOpen)
        {
            return (CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(inIndex) ;
        }else
        {
            CCArray * chilren =((CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen))->getChildren();
            int count = 0;
            if (chilren)
            {
                count = chilren->count();
                int tmpIndex = inIndex - m_nWhichOneOpen;
                if (tmpIndex <= count)
                {
                    return (CPtExpandableListViewItem*) chilren->objectAtIndex(tmpIndex-1) ;
                }else
                {
                    return (CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(inIndex-count) ;
                }
            }
            
            
        }
    }
    
    return NULL;
}

CCArray * CPtExpandableListViewItemList::getListViewItems()
{
    return m_pExpListViewItems;
}
void CPtExpandableListViewItemList::setListViewItems(CCArray *inItems)
{
    if(inItems)
    {
        inItems->retain();
        CC_SAFE_RELEASE(m_pExpListViewItems);
        m_pExpListViewItems = inItems;
        m_nWhichOneOpen = -1;
    }
}

int CPtExpandableListViewItemList::getAllItemCount()
{
    int count = 0;
    if (m_pExpListViewItems)
    {
        count = m_pExpListViewItems->count();
        if (m_nWhichOneOpen != -1)
        {
            count += ((CPtExpandableListViewItem *)(m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen)))->getChildren()->count();
            
        }
    }
    CCLog("the count: %d", count);
    return count;
}

bool CPtExpandableListViewItemList::setOpenItemByIndex(int inIndex)
{
    if (m_pExpListViewItems && inIndex >= 0)
    {
        if (inIndex <= m_nWhichOneOpen)
        {
            CPtExpandableListViewItem* item=  ((CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen));
            item->setOpen(false) ;
             m_nWhichOneOpen = inIndex;
            item = ((CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen));
            item->getChildren();
            item->setOpen(true);
           
            
        }else
        {
            CCArray * chilren =((CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen))->getChildren();
            int count = 0;
            if (chilren)
            {
                count = chilren->count();
                int tmpIndex = inIndex - m_nWhichOneOpen;
                if (tmpIndex <= count)
                {
                    return false;
                }else
                {
                    CPtExpandableListViewItem* item=  ((CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen));
                    item->setOpen(false) ;
                    m_nWhichOneOpen = inIndex-count;
                    item = ((CPtExpandableListViewItem*) m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen));
                    item->getChildren();
                    item->setOpen(true);
                }
            }
            
            
        }
    }
    
    return true ;
    
}
void CPtExpandableListViewItemList::setWhichOneOpen(int var)
{
    if (m_pExpListViewItems && m_pExpListViewItems->count() > var)
    {
        if (m_nWhichOneOpen != -1)
        {
            ((CPtExpandableListViewItem*)m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen))->setOpen(false);
        }
        m_nWhichOneOpen = var;
        ((CPtExpandableListViewItem*)m_pExpListViewItems->objectAtIndex(m_nWhichOneOpen))->setOpen(true);
    }
    
}

int CPtExpandableListViewItemList::getWhichOneOpen()
{
    return m_nWhichOneOpen;
}

// implement class of CPTExpandableListview


CPtExpandableListView * CPtExpandableListView::create(CPtExpandableListViewItemList * items, CCSize containerSize, CCSize paddingSize)
{
    CPtExpandableListView * layer = new CPtExpandableListView();
    if (layer)
    {
        layer->autorelease();
        if(!layer->initialCPtExpandableListView(items, containerSize, paddingSize))
        {
            delete layer;
            layer = NULL;
        }
    }
    return  layer;
}

CPtExpandableListView::CPtExpandableListView()
{
    m_cItems = NULL;
}

CPtExpandableListView::~CPtExpandableListView()
{
    CC_SAFE_RELEASE(m_cItems);
}

bool CPtExpandableListView::initialCPtExpandableListView(CPtExpandableListViewItemList * items, CCSize containerSize, CCSize paddingSize)
{
    bool bRet = false;
	do
	{
		CC_BREAK_IF( !CCLayer::init() );
        initData(items, containerSize, paddingSize);
        m_pBackground = CCLayer::create();
		m_pTableView = CCTableView::create(this, CCSizeMake(containerSize.width, containerSize.height));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);
		m_pTableView->setDelegate(this);
        this->setContentSize(containerSize);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		this->addChild(m_pTableView);
		m_pTableView->reloadData();
        
		bRet = true;
	}while(0);
    
	return bRet;
    
    
}

CCSize CPtExpandableListView::cellSizeForTable(CCTableView *table)
{
    return  CCSizeMake(m_cItemSize.width+m_cPaddingSize.width, m_cItemSize.height+m_cPaddingSize.height);
}
CCTableViewCell* CPtExpandableListView::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *pCell = table->dequeueCell();
    if (!pCell) {
        pCell = new CCTableViewCell();
        pCell->autorelease();
        
        
    }
    else
    {
        pCell->removeAllChildrenWithCleanup(true);
    }
    
    
    CPtExpandableListViewItem *tmp = m_cItems->getItemByIndex(idx);
    
    if (tmp)
    {
        
        CCNode *pSprite =tmp->getDisplayView();
        
        
        CCPoint point = CCPointMake(m_cItemSize.width/2, m_cItemSize.height/2);
        if(pSprite->getParent())
        {
            pSprite->setParent(NULL);
        }
        
        pSprite->setPosition(point);
        pCell->addChild(pSprite, 0);
        
    }
    
    
    return pCell;
    
    
}
unsigned int CPtExpandableListView:: CPtExpandableListView::numberOfCellsInTableView(CCTableView *table)
{
    return  m_cItems->getAllItemCount();
}
void  CPtExpandableListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}


void CPtExpandableListView::initData(CPtExpandableListViewItemList *items, CCSize containerSize, CCSize paddingSize)
{
    m_cItems = items;
    m_cItems->retain();

    m_cItems = items;
    m_cItems->retain();
    
    
    m_fContainerHeight = containerSize.height;
    m_fContainerWidth = containerSize.width;
    m_cItemSize   = CCSizeZero;
    m_cPaddingSize = paddingSize;
    
    
    
    if (items && items->getAllItemCount()!= 0)
    {
        CCNode * node = ((CPtExpandableListViewItem *)items->getListViewItems()->objectAtIndex(0))->getDisplayView();
        m_cItemSize = node->getContentSize();
    }
    
}




void CPtExpandableListView::setItems(CPtExpandableListViewItemList *var)
{
    CC_SAFE_RELEASE(m_cItems);
    m_cItems = var;
    m_cItems->retain();
}

CPtExpandableListViewItemList* CPtExpandableListView::getItems()
{
    return m_cItems;
}

void CPtExpandableListView::setOpenItemByIndex(int inIndex)
{
    bool bRet = m_cItems->setOpenItemByIndex(inIndex);
    if(bRet)
    {
    
        m_pTableView->reloadData();
        CCSize contentSize =  m_pTableView->getContentSize();
        CCSize containerSize = m_pTableView->getViewSize();
        float y = containerSize.height - contentSize.height;
        m_pTableView->setContentOffset(ccp(0, y));
        
    }
}