//
//  CHandBookTableView.cpp
//  91.cube
//
//  Created by linminglu on 11/11/13.
//
//

#include "CCustomerTableView.h"

CCustomerTableView::CCustomerTableView():CCTableView()
{
    
}

CCustomerTableView::~CCustomerTableView()
{
    
}
CCustomerTableView *CCustomerTableView::create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
{
    CCustomerTableView *pTableView=new CCustomerTableView();
    pTableView->initWithViewSize(size,container);
    pTableView->autorelease();
    pTableView->setDataSource(dataSource);
    pTableView->_updateContentSize();
    return pTableView;
}


bool CCustomerTableView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_pTouch=NULL;
    bool resualt=CCTableView::ccTouchBegan(pTouch, pEvent);
    return resualt;
}
void CCustomerTableView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!this->isVisible()) {
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
            if (m_eVordering == kCCTableViewFillTopDown) {
                CCSize cellSize = m_pDataSource->cellSizeForTable(this);
                point.y -= cellSize.height;
            }
            index = this->_indexFromOffset(point);
            cell  = this->_cellWithIndex(index);
            
            if (cell) {
                m_pTouch=pTouch;
                m_pTableViewDelegate->tableCellTouched(this, cell);
               
            }
        }
        CCScrollView::ccTouchEnded(pTouch, pEvent);

}