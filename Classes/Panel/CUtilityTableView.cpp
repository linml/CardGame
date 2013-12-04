//
//  CUtilityTableView.cpp
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#include "CUtilityTableView.h"

CUtilityTableView::CUtilityTableView()
{
    _mydefineDeleagte=NULL;
    m_beginTouchPoint=CCPointZero;
}

CUtilityTableView::~CUtilityTableView()
{
    
}

CUtilityTableView *CUtilityTableView::Create(CCTableViewDataSource *dataSource,CCSize size,CUtilityTableViewDelegate *_mydefineDeleagte)
{
    CUtilityTableView*tableView=new CUtilityTableView();
    tableView->initWithViewSize(size, NULL);
    tableView->autorelease();
    tableView->setDataSource(dataSource);
    tableView->_updateContentSize();
    tableView->_mydefineDeleagte=_mydefineDeleagte;
    return tableView;;
}

void CUtilityTableView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!this->isVisible())
    {
        return;
    }
    CCPoint   newPoint= this->convertTouchToNodeSpace(pTouch);
    
    
#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
    if (m_pTouches->count() == 1 &&  (abs((int)(newPoint.x-m_oldPoint.x))<=20 && abs((int)(newPoint.y-m_oldPoint.y))<=20))
    {
        CCLOG("adfasdf");
#else
        if (m_pTouches->count() == 1 && !this->isTouchMoved())
        {
#endif
            unsigned int        index;
            CCTableViewCell   *cell;
            CCPoint           point;
            
            point = this->getContainer()->convertTouchToNodeSpace(pTouch);
            if (m_eVordering == kCCTableViewFillTopDown)
            {
                CCSize cellSize = m_pDataSource->cellSizeForTable(this);
                point.y -= cellSize.height;
            }
            
            index = this->_indexFromOffset(point);
            CCLog("index=%d",index);
            cell  = this->_cellWithIndex(index);
            
            
            if (cell)
            {
                CCLog("%f,%f",cell->getContentSize().width,cell->getContentSize().height);
                m_pTouch=pTouch;
                m_pTableViewDelegate->tableCellTouched(this, cell);
                
            }
        }
        CCScrollView::ccTouchEnded(pTouch, pEvent);
#if(CC_PLATFORM_ANDROID==CC_TARGET_PLATFORM)
    }
#endif

}

bool CUtilityTableView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //让  父类 去计算  中间点。
    bool flag=CCTableView::ccTouchBegan(pTouch, pEvent);
    if(flag)
    {
        CCPoint  point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        m_beginTouchPoint=pTouch->getLocation();
        int index = this->_indexFromOffset(point);
        CCTableViewCell   *cell;
        cell=this->_cellWithIndex(index);
        if(_mydefineDeleagte && cell)
        {
            _mydefineDeleagte->tablecellTouchNode(cell, pTouch);
        }
    }
    return flag;
}

void CUtilityTableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(abs((int)(pTouch->getLocation().y-m_beginTouchPoint.y))<20.0)
    {
        return;
    }
    CCTableView::ccTouchMoved(pTouch, pEvent);
    if (_mydefineDeleagte) {
        CCPoint  point = this->getContainer()->convertTouchToNodeSpace(pTouch);
        if (m_eVordering == kCCTableViewFillTopDown) {
            CCSize cellSize = m_pDataSource->cellSizeForTable(this);
            point.y -= cellSize.height;
        }
        int index = this->_indexFromOffset(point);
        CCTableViewCell   *cell;
        cell=this->_cellWithIndex(index);
        if(cell)
        {
            _mydefineDeleagte->tablecellTouchNode(cell, pTouch);
        }
    }

}

void CUtilityTableView::scrollViewDidScroll(CCScrollView* view)
{
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }
    
    unsigned int startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    CCPoint offset = ccpMult(this->getContentOffset(), -1);
    maxIdx = MAX(uCountOfItems-1, 0);
    const CCSize cellSize = m_pDataSource->cellSizeForTable(this);
    
    if (m_eVordering == kCCTableViewFillTopDown)
    {
        offset.y = offset.y + m_tViewSize.height/this->getContainer()->getScaleY() - cellSize.height;
    }
    startIdx = this->_indexFromOffset(offset);
    
    if (m_eVordering == kCCTableViewFillTopDown)
    {
        offset.y -= m_tViewSize.height/this->getContainer()->getScaleY();
    }
    else
    {
        offset.y += m_tViewSize.height/this->getContainer()->getScaleY();
    }
    offset.x += m_tViewSize.width/this->getContainer()->getScaleX();
    //  CCLog("offset=%f,offset=%f",offset.x,offset.y);
    endIdx   = this->_indexFromOffset(offset);
    
#if 0 // For Testing.
    CCObject* pObj;
    int i = 0;
    CCARRAY_FOREACH(m_pCellsUsed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLog("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLog("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(m_pCellsFreed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLog("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLog("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
    
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell* cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
        
        idx = cell->getIdx();
        while(idx <startIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell *cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
        idx = cell->getIdx();
        
        while(idx <= maxIdx && idx > endIdx)
        {
            this->_moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
                idx = cell->getIdx();
                
            }
            else
            {
                break;
            }
        }
    }
    
    for (unsigned int i=startIdx; i <= endIdx; i++)
    {
        //if ([m_pIndices containsIndex:i])
        if (m_pIndices->find(i) != m_pIndices->end())
        {
            continue;
        }
        this->updateCellAtIndex(i);
    }
    if(_mydefineDeleagte)
    {
        _mydefineDeleagte->tableScrolBarView(this);
    }

}

void  CUtilityTableView::reloadDataView()
{
    CCTableView::reloadData();
    float y = (m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY()) ;
    if (y >= 0)
    {
        setContentOffset(ccp(0,y));
    }else
    {
        setContentOffset(ccp(0,y));
    }
}
