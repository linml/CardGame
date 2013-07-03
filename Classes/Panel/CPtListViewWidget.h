#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;


class TableView : public CCTableView
{
    
public:
    static TableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
    static TableView*  create(CCTableViewDataSource* dataSource, CCSize size);
    
public:
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher()
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(), true);
    }
    virtual void addBackground(CCNode *node, int zorder = 0 , int tag = 100);
    virtual void visit();
public:
    CCNode *getCell(CCTouch *pTouch);
    
protected:
    CCNode * m_pSelectItem;
    bool m_bTouchDragSelect;

protected:
    void beforeDraw();
    void afterDraw();
};


class CPtListViewWidget : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
    
public:
    static CPtListViewWidget *create(CCArray *items,  CCSize containerSize,  CCScrollViewDirection direction = kCCScrollViewDirectionVertical,  CCSize PaddingSize = CCSizeMake(5, 2.5), int inColumCount = 1);
    
public:
    CPtListViewWidget();
    virtual ~CPtListViewWidget();
    void setBackGround(CCLayer *layer, int zorder = -100, int tag = 1);
    void setPaddingWidth(float paddWidth);
    void setPaddingHeight(float paddingHeight);
    void setChipEnable(const bool & inChipEnable);
    CCTableView * getTableView(){ return m_pTableView;};
public:
    virtual bool init(CCArray *items, CCSize containerSize,CCScrollViewDirection direction = kCCScrollViewDirectionVertical,  CCSize PaddingSize = CCSizeMake(5, 2.5), int inColumCount = 1);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);

    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);

    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);

    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);

    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);

    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);


    
    
protected:
    void initData(CCArray *items, CCSize containerSize, CCScrollViewDirection direction, CCSize itemSize, int inColumCount);
protected:
    
    int m_nRows;

    CCTableView * m_pTableView;
    CCLayer *m_pBackground;
    CCSize m_cItemSize;
    CCScrollViewDirection m_cDirection;
    CC_PROPERTY(CCSize, m_cPaddingSize, PaddingSize);
    CC_PROPERTY(int, m_nColumcount, ColumCount);
    CC_PROPERTY(float, m_fContainerWidth, ContainerWidth);
    CC_PROPERTY(float, m_fContainerHeight, ContainerHeight);
    CC_PROPERTY(CCArray*, m_cItems, Items);
    CC_SYNTHESIZE(CCTableViewDelegate*, m_pTableItemDelegate, TableItemDelegate);
};

#endif 