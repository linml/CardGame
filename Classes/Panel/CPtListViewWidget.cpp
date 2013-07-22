#include "CPtListViewWidget.h"
#include "CPtTableItem.h"
USING_NS_CC;
USING_NS_CC_EXT;


static bool m_bBegan = false;
static bool m_bEnable = false;
static bool m_bMove;

static bool isMove(CCTouch *pTouch);

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



bool TableView::ccTouchDelayBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bBegan = true;
    m_bEnable = false;
    m_bMove = false;
    m_bTouchDragSelect = false;
   
    
    if (!this->isVisible())
    {
        return false;
    }
    CCRect frame;
    CCPoint frameOriginal = this->getParent()->convertToWorldSpace(this->getPosition());
    frame = CCRectMake(frameOriginal.x, frameOriginal.y, m_tViewSize.width, m_tViewSize.height);
    if (frame.containsPoint(pTouch->getLocation()))
    {
        m_bDelay = true;
        scheduleOnce(schedule_selector(TableView::timer),0.030);
        return  true;
    }
    return  false;
    
}
void TableView::ccTouchDelayMove(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint delta = pTouch->getDelta();
    
    if (!m_bMove)
    {
        
        
        m_bMove = isMove(pTouch);
        if (m_bMove)
        {
            if (m_bDelay == false)
            {
                
                m_bTouchDragSelect = false;
                m_pSelectItem = getCell(pTouch);
                if (m_pSelectItem)
                {
                    CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem) ;
                    if (touchDelegate)
                    {
                        m_bTouchDragSelect = touchDelegate->ccTouchBegan(pTouch, pEvent);
                    }
                    
                }

            }
            
        }else
        {
            return;
        }
    }
    
    
    if (m_bDelay == false)
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
    }
    
    
    
    if (!m_bTouchDragSelect &&m_bBegan)
    {
        
        m_bBegan = false;
        CCLog("1");
        m_bEnable =  CCTableView::ccTouchBegan(pTouch, pEvent);
    }
    if (m_bEnable)
    {
        CCLog("2");
        CCScrollView::ccTouchMoved(pTouch, pEvent);
    }
    
}
void TableView::ccTouchDelayEnd(CCTouch *pTouch, CCEvent *pEvent)
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
      //  return;
        
    }
    
    
    if (m_bEnable)
    {
        CCLog("3");
        CCTableView::ccTouchEnded(pTouch, pEvent);
    }
    


    
}
void TableView::ccTouchDelayCancel(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


bool TableView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_bDelayMode)
    {
      return  ccTouchDelayBegan(pTouch, pEvent);
    }
    
    
    CCPoint location = pTouch->getLocation();
    location = this->getParent()->convertToNodeSpace(location);
    
    if (!m_pContainer->boundingBox().containsPoint(location))
    {
        return false;
    }
    
    if (getDirection() == kCCScrollViewDirectionVertical)
    {
        m_bTouchDragSelect = false;
        m_pSelectItem = getCell(pTouch);
        if (m_pSelectItem)
        {
            CCTouchDelegate *touchDelegate = dynamic_cast<CCTouchDelegate*>(m_pSelectItem) ;
            if (touchDelegate)
            {
                m_bTouchDragSelect = touchDelegate->ccTouchBegan(pTouch, pEvent);
            }
            
        }
        if (m_bTouchDragSelect)
        {
            return m_bTouchDragSelect;
        }

    }
    return CCTableView::ccTouchBegan(pTouch, pEvent);
}
void TableView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
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
                }
            }
            
            return;
        }
    }
    
    if (getViewSize().height > m_pContainer->getContentSize().height)
    {
       // return;
    }
    CCScrollView::ccTouchMoved(pTouch, pEvent);
}


void TableView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
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
            
            return;
        }

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
            
            return;
        }
        
    }
    CCTableView::ccTouchCancelled(pTouch, pEvent);
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
                  
                    CPtTableItem * point = (CPtTableItem *)(pSprite->getUserData());
                
                 //  CCLog("the point: %d" ,(int)point);
                    return point;
                    //return pSprite;
                    
                    break;
                }
            }
        }
        
    }
    
    
    return NULL;

}

void TableView::addBackground(cocos2d::CCNode *node, int zorder /* = 0*/ , int tag /*= -1*/)
{
    CCLayer::addChild(node, -100, tag);
    
}

void TableView::timer(float dt)
{
//    CCLog("the timer: %f", dt);
//    static float delay =0.0f;
//    delay += dt;
//    if (delay > 0.2)
//    {
//         delay= 0.0f;
        m_bDelay = false;
//        unschedule(schedule_selector(TableView::timer));
        CCLog("timer");
//    }
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
   
    return CCSizeMake(m_fContainerHeight, (m_cItemSize.height + m_cPaddingSize.height*2));
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
            return NULL;
            
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
            index = i+idx*m_nColumcount;
            if (index >= m_cItems->count())
            {
                break;
            }
            
            CPtTableItem *tmp = dynamic_cast<CPtTableItem *>(m_cItems->objectAtIndex(i+idx*m_nColumcount));
            
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
              //  CCLog("the item: %f, %f", pSprite->getPositionX(), pSprite->getPositionY());
                pCell->addChild(pSprite, 0, i);
               //  CCLog("the point: %d" ,(int)tmp);
                pSprite->setUserData((void *) (tmp));
            }
        }
        
        return pCell;
    }
    
   
}

unsigned int CPtListViewWidget::numberOfCellsInTableView(CCTableView *table)
{
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
