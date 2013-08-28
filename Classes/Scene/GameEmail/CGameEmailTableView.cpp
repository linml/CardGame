//
//  CGameEmailTableView.cpp
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#include "CGameEmailTableView.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "Utility.h"
CGameEmailTableView::CGameEmailTableView()
{
    
}
CGameEmailTableView::~CGameEmailTableView(){
   
}
CGameEmailTableView*CGameEmailTableView::creat(CCPoint p , CCSize s ,int cellNum , CCSize cellSize , CCSize tableCellSize)
{
    CGameEmailTableView* layerView = new CGameEmailTableView;
    if (layerView) {
        layerView->initView( p ,  s , cellNum ,  cellSize ,  tableCellSize);
        layerView->autorelease();
        return layerView;
    }
    CC_SAFE_DELETE(layerView);
    return NULL;
}

CGameEmailTableView *CGameEmailTableView::creat(CCPoint p , CCSize s ,int cellNum , CCSprite*cellImage , int cellgap){
    CGameEmailTableView* layerView = new CGameEmailTableView;
    if (layerView) {
        layerView->initView(  p ,  s , cellNum , cellImage ,  cellgap);
        layerView->autorelease();
        return layerView;
    }
    CC_SAFE_DELETE(layerView);
    return NULL;
}
bool CGameEmailTableView::init()
{
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    return true;
}

bool CGameEmailTableView::initView(CCPoint p , CCSize s ,int cellNum , CCSize cellSize , CCSize tableCellSize){
    if ( !CCLayer::init() )
    {
        return false;
    }
    m_tableCount = cellNum;
    m_cellSize = cellSize;
    m_tableCellSize = tableCellSize;
    m_tableViewSize = s;
    m_tableViewPoint = p;
    
    
    CCTableView * tableView = CCTableView::create(this, s);
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(p);
    tableView->setDelegate(this);
    tableView->setTouchPriority(-8);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView);
    return true;
}
bool CGameEmailTableView::initView(CCPoint p , CCSize s ,int cellNum , CCSprite*cellImage , int cellgap){
    if ( !CCLayer::init() )
    {
        return false;
    }
    m_pCell = cellImage;
    m_tableCount = cellNum;
    m_cellSize = cellImage->getContentSize();
    m_cellScaleX = cellImage->getScaleX();
    m_cellScaleY = cellImage->getScaleY();
    m_cellSize = CCSize(m_cellSize.width*m_cellScaleX, m_cellSize.height*m_cellScaleY);
    
    m_tableCellSize = CCSize(s.width , m_cellSize.height+cellgap*2);
    m_tableViewSize = s;
    m_tableViewPoint = p;
    
    m_pTexture = cellImage->getTexture();
    m_TextureRect = cellImage->getTextureRect();
    
    CCTableView * tableView = CCTableView::create(this, m_tableViewSize);
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(p);
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    
    this->addChild(tableView);
    
    return true;
}
#pragma mark - CCTableViewDataSource

unsigned int CGameEmailTableView::numberOfCellsInTableView(CCTableView *table) {
    return m_tableCount;
}

void CGameEmailTableView::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
        if(cell)
        {
            LayoutLayer *tempLayerout=LayoutLayer::create();
            tempLayerout->initWithFile(cell, CSTR_FILEPTAH(plistPath, "mail_cell.plist"));
            CCSprite *lingquButton=(CCSprite *)Utility::getNodeByTag(cell, "0,32");
            if(lingquButton)
            {
                string word = Utility::getWordWithFile("word.plist", "lingqujiangpin");
                CGameButtonControl *gameLingqu=CGameButtonControl::createButton(TEXTMID, word.c_str(), "anniu2_Normal.png", "anniu2_Normal.png");
                gameLingqu->setPosition(lingquButton->getPosition());
                gameLingqu->setTag(lingquButton->getTag());
                gameLingqu->setAnchorPoint(lingquButton->getAnchorPoint());
                cell->addChild(gameLingqu);
                cell ->reorderChild(gameLingqu,lingquButton->getZOrder());
                cell->removeChild(lingquButton, 0);
            }
        }
}

CCTableViewCell* CGameEmailTableView::tableCellAtIndex(CCTableView *table, unsigned int idx) {
    
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        this->initCellItem(cell, idx);
    }
    else
    {
        scrollBar(table);
    }
    return cell;
}


CCSize CGameEmailTableView::cellSizeForTable(CCTableView *table) {
    return m_cellSize;
}

CCSize CGameEmailTableView::tableCellSizeForIndex(CCTableView *table, unsigned int idx){
    return m_tableCellSize;
}

#pragma mark - CCTableViewDelegate
void CGameEmailTableView::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    CCLog("you touch cell at %d", cell->getIdx());
    //得到现在的时间，对比活动开始时间，结束时间，判断活动状态，调入相应的界面显示
    
    
}

void CGameEmailTableView::scrollViewDidScroll(CCScrollView* view)
{
    scrollBar((CCTableView*)view);
}

void CGameEmailTableView::scrollViewDidZoom(CCScrollView* view) {
    
}


//add处理滚动:

void CGameEmailTableView::scrollBar(CCTableView* table)
{
    CCSize conSize = table->getContentSize();
    CCSize viewSize = table->getViewSize();
    
    if (conSize.height < viewSize.height) {
        return;
    }
    
//    if (!m_huaTiao) {
//        m_huaTiao = CCSprite::create("scrollBar.png");
//        if(m_huaTiao != NULL)
//        {
//            CCSize sizeBar = m_huaTiao->getContentSize();
//            this->addChild(m_huaTiao, 1);
//            m_huaTiao->setScaleY(viewSize.height/sizeBar.height);
//            float scaley = 0;
//            scaley = m_huaTiao->getScaleY();
//            float per = viewSize.height/conSize.height;
//            scaley = scaley*per;
//            m_huaTiao->setScaleY(scaley);
//            
//            m_huaTiao->setAnchorPoint(ccp(0,0.5));
//        }
//    }
//	if(m_huaTiao == NULL)
//	{
//		return;
//	}
//    m_huaTiao->setOpacity(255);
//    
//    CCPoint scrollViewPoint = table->getPosition();
//    
//    CCSize sizeBar = m_huaTiao->getContentSize();
//	// tableSize.height == cell个数*cell的height
//	CCSize tableSize = table->getContentSize();
//	// CCTableView
//	CCSize tableViewSize = table->getViewSize();
//    //    tableViewSize = CCSize(tableViewSize.width, tableViewSize.height-sizeBar.height);
//	// 每次拖动的偏移量？(负值)
//	CCPoint contOffsetPos = table->getContentOffset();
//    
//	// 总的偏移量
//	float maxOff = tableViewSize.height - tableSize.height;
//	// 拖动的偏移量
//	float curOff = contOffsetPos.y - maxOff;
//	// 计算百分百
//	float percentage = fabs(curOff)/fabs(maxOff);
//    
//    //	printf("contOffsetPos:%f ,curOff:%f, maxOff:%f, per:%f\n", contOffsetPos.y, curOff, maxOff, percentage);
//	
//	// 拖拉到最顶端或最低端后继续拖动(弹回)会出现percentage值小于0.1和大于1.0的情况，我们分别把percentage重置为0和1.0f
//	if(curOff < 0)
//	{
//		percentage = 0;
//	}
//	if(percentage > 1.0f)
//	{
//		percentage = 1.0f;
//	}
//    
//	// bar移动到最顶端的position.y
//	float barTopPosY = scrollViewPoint.y+tableViewSize.height -(sizeBar.height*m_huaTiao->getScaleY())/2 ;
//	// bar移动到最低端的position.y
//	float barLowPosY = scrollViewPoint.y+(sizeBar.height*m_huaTiao->getScaleY())/2;
//	// ....
//	float h = barTopPosY - percentage*(barTopPosY- barLowPosY);;
//    
//	m_huaTiao->setPosition(ccp(tableViewSize.width*1.05 , h));
}
