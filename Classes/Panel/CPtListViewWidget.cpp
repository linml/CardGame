#include "CPtListViewWidget.h"
#include "CPtTableItem.h"
USING_NS_CC;
USING_NS_CC_EXT;


static bool isMove(CCTouch *pTouch);

TableView::TableView()
{
    m_bItemHanlerTouch = true;
    m_bBegan = false;
    m_bDecide = false;
    m_bScroll = false;
    m_bCanScroll = true;
}
TableView::~TableView()
{
    
}

TableView* TableView::create(CCTableViewDataSource* dataSource, CCSize size)
{
    return TableView::create(dataSource, size, NULL);
}

TableView* TableView::create(cocos2d::extension::CCTableViewDataSource *dataSource, cocos2d::CCSize size ,cocos2d::CCNode *container)
{
    TableView *table = new TableView();
    table->autorelease();
    table->initWithViewSize(size, container);
    table->setDataSource(dataSource);
    table->_updateContentSize();
    return table;
}

void TableView::setInitState()
{
    m_bTouchDragSelect = false;
    m_bDecide = false;
    m_bBegan = false;
}

bool TableView::ccTouchDelayBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    unscheduleAllSelectors();
    m_cPointBegin = pTouch->getLocation();
    if (!this->isVisible())
    {
        return false;
    }
    CCRect frame;
    CCPoint frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
    frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);
    
    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (m_pTouches->count() > 2 ||
        m_bTouchMoved          ||
        !frame.containsPoint(m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(pTouch))))
    {
        return false;
    }
    

    setInitState();
    return true;
  

    
}
void TableView::ccTouchDelayMove(CCTouch *pTouch, CCEvent *pEvent)
{    
    if (m_bDecide)
    {
        if (m_bScroll)
        {
            if (m_bBegan)
            {
                if (m_bCanScroll)
                {
                     CCTableView::ccTouchMoved(pTouch, pEvent);
                }
               
            }
            else
            {
                CCTableView::ccTouchBegan(pTouch, pEvent);
                m_bBegan = true;
            }
            
        }else
        {
            if (m_bBegan)
            {
                if (m_bTouchDragSelect)
                {
                    if (m_pSelectItem)
                    {
                        CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem);
                        if (touchDelegate)
                        {
                            touchDelegate->ccTouchMoved(pTouch, pEvent);
                        }
                    }
                    
                }

                return;
            }
            
            m_bTouchDragSelect = false;
            m_pSelectItem = getCell(pTouch);
            if (m_pSelectItem)
            {
                CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem) ;
                if (touchDelegate)
                {
                    m_bTouchDragSelect = touchDelegate->ccTouchBegan(pTouch, pEvent);
                  
                }
                if (m_bTouchDragSelect == false)
                {
                    m_bScroll = true;
                    CCTableView::ccTouchBegan(pTouch, pEvent);
                }
                
                m_bBegan = true;
                
            }else
            {
                m_bScroll = true;
                CCTableView::ccTouchBegan(pTouch, pEvent);
                m_bBegan = true;
            }

        }
    }else
    {
        CCPoint pointEnd = pTouch->getLocation();
        CCPoint p = pTouch->getDelta();
        if (ccpDistance(m_cPointBegin, pointEnd) < 2 )
        {
            return;
        }else
        {
            m_bDecide = true;
            float x = fabs(m_cPointBegin.x-pointEnd.x);
            float y = fabs(m_cPointBegin.y-pointEnd.y);
            if (y*0.70 > x)
            {
                m_bScroll = true;
            }else
            {
                m_bScroll = false;
            }
            CCLog(" x, y: %f, %f, %d", x, y,m_bScroll);
           
        }
        CCLog("p: x, y: %f, %f", p.x, p.y);
        
    }
       
}
void TableView::ccTouchDelayEnd(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_bBegan && m_bDecide)
    {
        if (m_bScroll)
        {
            CCTableView::ccTouchEnded(pTouch, pEvent);
        }else
        {
            if (m_bTouchDragSelect)
            {
                if (m_pSelectItem)
                {
                    CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem);
                    if (touchDelegate)
                    {
                        touchDelegate->ccTouchEnded(pTouch, pEvent);
                    }
                }
                
            }

        }
        
    }else
    {
       // CCLog("click...");
        
        m_pSelectItem = getCell(pTouch);
        if (m_pSelectItem)
        {
            CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem) ;
            if (touchDelegate)
            {
                m_bTouchDragSelect = touchDelegate->ccTouchBegan(pTouch, pEvent);
                if (m_bTouchDragSelect)
                {
                    touchDelegate->ccTouchEnded(pTouch, pEvent);
                }
                
            }
           
            
            
        }

    }

    
}
void TableView::ccTouchDelayCancel(CCTouch *pTouch, CCEvent *pEvent)
{
    setInitState();
    ccTouchEnded(pTouch, pEvent);
}


bool TableView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
 
   // m_bCanScroll
    CCSize size=  getViewSize();
    CCSize size2 = getContentSize();
   
    m_bCanScroll = size2.height > size.height ;
    
    if (m_bItemHanlerTouch == false)
    {
        return CCTableView::ccTouchBegan(pTouch, pEvent);
    }
    
    if (m_bDelayMode)
    {
      return  ccTouchDelayBegan(pTouch, pEvent);
    }
    
    
    CCPoint location = pTouch->getLocation();
    location = this->getParent()->convertToNodeSpace(location);
    
    CCRect frame;
    CCPoint frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
    frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);
    
    //dispatcher does not know about clipping. reject touches outside visible bounds.
    if (m_pTouches->count() > 2 ||
        m_bTouchMoved          ||
        !frame.containsPoint(m_pContainer->convertToWorldSpace(m_pContainer->convertTouchToNodeSpace(pTouch))))
    {
        return false;
    }
    
    
    if (getDirection() == kCCScrollViewDirectionVertical)
    {
        bool flag = false;
        m_bTouchDragSelect = false;
        m_pSelectItem = getCell(pTouch);
        if (m_pSelectItem)
        {
            CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem) ;
            if (touchDelegate != NULL)
            {
                m_bTouchDragSelect = touchDelegate->ccTouchBegan(pTouch, pEvent);
            }
            
        }
        
        flag = CCTableView::ccTouchBegan(pTouch, pEvent);
        flag = flag | m_bTouchDragSelect ;
        return flag;
    }
    return CCTableView::ccTouchBegan(pTouch, pEvent);
}
void TableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bItemHanlerTouch == false)
    {
        return CCTableView::ccTouchMoved(pTouch, pEvent);
    }
    
    if (m_bDelayMode)
    {
        return  ccTouchDelayMove(pTouch, pEvent);
    }

    
    if (getDirection() == kCCScrollViewDirectionVertical)
    {
    
        if (m_bTouchDragSelect)
        {
            if (m_pSelectItem)
            {
                CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem);
                if (touchDelegate)
                {
                    touchDelegate->ccTouchMoved(pTouch, pEvent);
                    return;
                }
            }
            
        }
        if (m_bCanScroll)
        {
            CCScrollView::ccTouchMoved(pTouch, pEvent);
        }
        
        return;
    }
    
    if (getViewSize().height > m_pContainer->getContentSize().height)
    {
       // return;
    }
    CCScrollView::ccTouchMoved(pTouch, pEvent);
}


void TableView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

    if (m_bItemHanlerTouch == false)
    {
      return   CCTableView::ccTouchEnded(pTouch, pEvent);
    }
    
    if (m_bDelayMode)
    {
        return  ccTouchDelayEnd(pTouch, pEvent);
    }

    
    if (getDirection() == kCCScrollViewDirectionVertical)
    {
        if (m_bTouchDragSelect)
        {
            if (m_pSelectItem)
            {
                CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem) ;
                if (touchDelegate)
                {
                    touchDelegate->ccTouchEnded(pTouch, pEvent);
                }
            }
        }
        CCTableView::ccTouchEnded(pTouch, pEvent);
        return;
        
    }
    CCTableView::ccTouchEnded(pTouch, pEvent);
}


void TableView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
 
    if (getDirection() == kCCScrollViewDirectionVertical)
    {
        if (m_bTouchDragSelect)
        {
            if (m_pSelectItem)
            {
                CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem) ;
                if (touchDelegate)
                {
                    touchDelegate->ccTouchCancelled(pTouch, pEvent);
                }
            }
        }
        
    }

    CCTableView::ccTouchCancelled(pTouch, pEvent);
    setInitState();
}


/**
 * clip this view so that outside of the visible bounds can be hidden.
 */
void TableView::beforeDraw()
{
    if (m_bClippingToBounds)
    {
		// TODO: This scrollview should respect parents' positions
		CCPoint screenPos = this->getParent()->convertToWorldSpace(this->getPosition());
        glEnable(GL_SCISSOR_TEST);
        float s = this->getScale();
        CCEGLView::sharedOpenGLView()->setScissorInPoints(screenPos.x*s, screenPos.y*s, m_tViewSize.width*s, m_tViewSize.height*s);
    }
}

/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void TableView::afterDraw()
{
    if (m_bClippingToBounds)
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

void TableView::visit()
{
	// quick return if not visible
	if (!isVisible())
    {
		return;
    }
    
	kmGLPushMatrix();
	
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }
    
	this->transform();
    this->beforeDraw();
    
	if(m_pChildren)
    {
		ccArray *arrayData = m_pChildren->data;
		unsigned int i=0;
       
        sortAllChildren();
     
		for( ; i < arrayData->num; i++ )
        {
			CCNode *child =  (CCNode*)arrayData->arr[i];
			if ( child->getZOrder() < 0 )
            {
				child->visit();
			}
            else
            {
				break;
            }
		}
		
		this->draw();
		
		// draw children zOrder >= 0
		for( ; i < arrayData->num; i++ )
        {
			CCNode* child = (CCNode*)arrayData->arr[i];
			child->visit();
		}
        
	}
    else
    {
		this->draw();
    }
    
    this->afterDraw();
	if ( m_pGrid && m_pGrid->isActive())
    {
		m_pGrid->afterDraw(this);
    }
    
	kmGLPopMatrix();
}



CCNode * TableView::getCell(CCTouch *pTouch)
{
    unsigned int        index;
    CCTableViewCell   *cell=NULL;
    CCPoint           point;
    
    point = this->getContainer()->convertTouchToNodeSpace(pTouch);
    
    if (m_eVordering == kCCTableViewFillTopDown)
    {
        CCSize cellSize = m_pDataSource->cellSizeForTable(this);
        point.y -= cellSize.height;
    }
    index = this->_indexFromOffset(point);
    cell  = this->_cellWithIndex(index);
    
    if (cell)
    {
        CCPoint location = pTouch->getLocation();
        location =  cell->convertToNodeSpace(location);
        
        
        CCArray *children = cell->getChildren();
        
        if (children == NULL)
        {
            return NULL;
        }
        for (int i = 0; i < children->count(); i++)
        {
            
            
            
            CCNode *pSprite =(CCNode *) children->objectAtIndex(i);
            if (pSprite)
            {
                if (pSprite->getParent() == NULL)
                {
                    pSprite->setParent(cell);
                }
                if(pSprite->boundingBox().containsPoint(location))
                {
                  
                    CPtTableItem * item = (CPtTableItem *)(pSprite->getUserData());
                    
                
                    return item; 
                    break;
                }
            }
        }
        
    }
    
    
    return NULL;

}

void TableView::reload()
{
    reloadData();
    CCLog("CCScrollView1--- %f, %f",m_tViewSize.width - m_pContainer->getContentSize().width*m_pContainer->getScaleX(),
          m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY());
    CCLog("CCScrollView2--- %f, %f",m_tViewSize.width ,
          m_tViewSize.height );
    float y = (m_tViewSize.height - m_pContainer->getContentSize().height*m_pContainer->getScaleY()) ;
    if (y >= 0)
    {
         setContentOffset(ccp(0,y));
    }else
    {
         setContentOffset(ccp(0,y));
    }
   
    
    
   
}
void TableView::addBackground(cocos2d::CCNode *node, int zorder /* = 0*/ , int tag /*= -1*/)
{
    CCLayer::addChild(node, -100, tag);
    
}



// implement class CPtListViewWidget
CPtListViewWidget * CPtListViewWidget::create(CCArray*items, CCSize containerSize,  CCScrollViewDirection direction, CCSize paddingSize, int inColumCount)
{
    
    CPtListViewWidget * layer = new CPtListViewWidget();
    if (layer)
    {
        layer->autorelease();
        if(!layer->init(items, containerSize, direction, paddingSize, inColumCount))
        {
            delete layer;
            layer = NULL;
        }
    }
    return  layer;
}

CPtListViewWidget::CPtListViewWidget()
{
    m_nStartIndex = 0;
    m_nItemCount = 0;
    m_bReverse = false;
}

CPtListViewWidget::~CPtListViewWidget()
{
    if (m_cItems)
    {
        m_cItems->release();
    }
  
}


void CPtListViewWidget::setBackGround(cocos2d::CCLayer *layer, int zorder, int tag)
{
    if (m_pBackground)
    {
        if (m_pBackground->getChildByTag(tag))
        {
            m_pBackground->removeChildByTag(tag, true);
        }
        m_pBackground->addChild(layer, zorder, tag);
    }
}

void CPtListViewWidget::initData(CCArray *items, CCSize containerSize, CCScrollViewDirection direction, CCSize paddingSize, int inColumCount)
{
    m_pTableItemDelegate = NULL;
    m_cItems = items;
    m_cItems->retain();
   
    m_nColumcount = inColumCount;
    m_nRows = (m_cItems == NULL)? 1 : m_cItems->count() / m_nColumcount;
    m_nRows += (m_cItems->count() % m_nColumcount) == 0 ? 0 : 1;
    m_fContainerHeight = containerSize.height;
    m_fContainerWidth = containerSize.width;
    m_cItemSize   = CCSizeZero;
    m_cPaddingSize = paddingSize;
    m_cDirection = direction;
    
    
    if (items && items->count() > 0)
    {
        m_nStartIndex = 0;
        m_nItemCount = items->count();
        CCNode * node = ((CPtTableItem *)items->objectAtIndex(0))->getDisplayView();
        m_cItemSize = node->getContentSize();
    }
}

bool CPtListViewWidget::init(CCArray *items, CCSize containerSize, CCScrollViewDirection direction, CCSize itemSize, int inColumCount)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF( !CCLayer::init() );
        initData(items, containerSize, direction, itemSize, inColumCount);
        m_pBackground = CCLayer::create();
		m_pTableView = TableView::create(this, CCSizeMake(containerSize.width, containerSize.height));
        ((TableView*)m_pTableView)->addBackground(m_pBackground);
        m_pTableView->setDirection(direction);
		m_pTableView->setDelegate(this);
        this->setContentSize(containerSize);
        if (direction == kCCScrollViewDirectionVertical)
        {
            m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        }
		
		this->addChild(m_pTableView);
		m_pTableView->reloadData();

		bRet = true;
	}while(0);

	return bRet;
}



void CPtListViewWidget::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    if (m_pTableItemDelegate)
    {
        m_pTableItemDelegate->tableCellTouched(table, cell);
    }
}

CCSize CPtListViewWidget::cellSizeForTable(CCTableView *table)
{
    if (m_cDirection == kCCScrollViewDirectionHorizontal)
    {
       return  CCSizeMake(m_cItemSize.width+m_cPaddingSize.width, m_cItemSize.height+m_cPaddingSize.height*10);
    }
   
    return CCSizeMake(m_fContainerWidth, (m_cItemSize.height + m_cPaddingSize.height*2));
}

CCTableViewCell* CPtListViewWidget::tableCellAtIndex(CCTableView *table, unsigned int idx)
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

    if (m_cDirection == kCCScrollViewDirectionHorizontal)
    {
        CPtTableItem * item = ((CPtTableItem *) m_cItems->objectAtIndex(idx));
        if (item == NULL)
        {
            return pCell;
            
        }
        CCNode *pSprite = item->getDisplayView();
        if (pSprite == NULL)
        {
            CCLog("item is null");
            return  NULL;
        }
        pSprite->setParent(NULL);
        pSprite->setAnchorPoint(CCPointZero);
        pSprite->setPosition(ccp(m_cPaddingSize.width, m_cPaddingSize.height*2));
        pCell->addChild(pSprite);
        
       
        pSprite->setUserData((void *) item);
    
        
        return pCell;
    }
    else
    {
        int index = 0;
        for (int i = 0; i < m_nColumcount; i++)
        {
            index = i+idx*m_nColumcount+m_nStartIndex;
            if (index >= (m_nStartIndex+m_nItemCount) )//m_cItems->count())
            {
                break;
            }
            
            CPtTableItem *tmp = NULL;
            if (m_bReverse)
            {
                index =  (m_nItemCount+m_nStartIndex-1-index); // m_cItems->count() -1 -index;
            }
            tmp= dynamic_cast<CPtTableItem *>(m_cItems->objectAtIndex(index));
           
            
            if (tmp)
            {
            
                CCNode *pSprite =tmp->getDisplayView();
        
                CCPoint point = CCPointMake(pSprite->getContentSize().width, 0);
                if(pSprite->getParent())
                {
                    pSprite->setParent(NULL);
                }
                
                pSprite->setAnchorPoint(CCPointZero);
                pSprite->setPosition(ccp(point.x *i + m_cPaddingSize.width*(i+1) , 0));
         
                pCell->addChild(pSprite, 0, i);
                pSprite->setUserData((void *) (tmp));
                tmp->setUserData((void *)(index));
            }
        }
        
        return pCell;
    }
    
   
}

unsigned int CPtListViewWidget::numberOfCellsInTableView(CCTableView *table)
{
  //  return 1;
    return m_nRows;
}




// getter && setter:

/*
 * @brief:  the listView  retain the list '
 
 */
void CPtListViewWidget::setItems(CCArray *var)
{
    if (m_cItems && m_cItems != var)
    {
        var->retain();
        m_cItems->release();
        m_cItems = NULL;
        m_cItems = var;
        m_pTableView->reloadData();
    }

}

CCArray* CPtListViewWidget::getItems()
{
    return m_cItems;
}


void CPtListViewWidget::setPaddingSize(cocos2d::CCSize var)
{
    m_cPaddingSize = var;
    m_pTableView->reloadData();
}

CCSize CPtListViewWidget::getPaddingSize()
{
    return m_cPaddingSize;
}

void CPtListViewWidget::setColumCount(int var)
{
    m_nColumcount = var;
    m_pTableView->reloadData();
}

int CPtListViewWidget::getColumCount()
{
    return  m_nColumcount;
}

void CPtListViewWidget::setContainerHeight(float var)
{
    m_fContainerHeight = var;
    m_pTableView->reloadData();
}

float CPtListViewWidget::getContainerHeight()
{
    return m_fContainerHeight;
}


void CPtListViewWidget::setContainerWidth(float var)
{
    m_fContainerWidth = var;
    m_pTableView ->reloadData();
}

float CPtListViewWidget::getContainerWidth()
{
    return  m_fContainerWidth;
}

void CPtListViewWidget::setPaddingWidth(float paddWidth)
{
    m_cPaddingSize.width = paddWidth;
    m_pTableView->reloadData();
}
void CPtListViewWidget::setPaddingHeight(float paddingHeight)
{
    m_cPaddingSize.height = paddingHeight;
     m_pTableView->reloadData();
}

void CPtListViewWidget::reload()
{
    if (m_cItems == NULL)
    {
        m_nRows = 0;
    }
    m_nStartIndex =  (m_nStartIndex < m_cItems->count())? m_nStartIndex: 0;
    m_nItemCount = m_nItemCount+m_nStartIndex < m_cItems->count() ? m_nItemCount : m_cItems->count()-m_nStartIndex;
    m_nRows = (m_cItems == NULL)? 1 : m_nItemCount / m_nColumcount;
    m_nRows += (m_nItemCount % m_nColumcount) == 0 ? 0 : 1;
    //    m_nRows = (m_cItems == NULL)? 1 : m_cItems->count() / m_nColumcount;
    //    m_nRows += (m_cItems->count() % m_nColumcount) == 0 ? 0 : 1;

    ((TableView*)(m_pTableView))->reload();
    
}

// tool function:
bool isMove(CCTouch *pTouch)
{
    CCPoint deltaPoint = pTouch->getDelta();
    if(fabs(deltaPoint.x)>1||fabs(deltaPoint.y)>1)
    {
        return true;
    }
    return false;
}
