#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


using namespace std;

USING_NS_CC;
USING_NS_CC_EXT;

class TableViewDelegate
{
public:
    virtual void TouchEnd(CCNode *parent, CCNode *current, CCTouch *pTouch) = 0;
};

class TableView : public CCTableView, public TableViewDelegate
{
    
public:
    static TableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
    static TableView*  create(CCTableViewDataSource* dataSource, CCSize size);
    
public:
    void setControlTouchDelegate(TableViewDelegate *inTouchEnd);
public:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
  //  virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void TouchEnd(CCNode *parent, CCNode *current, CCTouch *pTouch);
    
    virtual void addBackground(CCNode *node, int zorder = 0 , int tag = -1);
    virtual void visit();
public:
    CCNode *getCell(CCTouch *pTouch);
protected:
    TableViewDelegate *m_pTouchEnd;
protected:
    void beforeDraw();
    void afterDraw();
};


class CPtListViewWidget : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate, public TableViewDelegate
{
    
public:
    static CPtListViewWidget *create(CCArray *items, CCSize containerSize, CCSize itemSize, int inColumCount);
    
public:
    CPtListViewWidget();
    virtual ~CPtListViewWidget();
    void setBackGround(CCLayer *layer, int zorder = -100, int tag = 1);
    void setPaddingWidth(float paddWidth);
    void setPaddingHeight(float paddingHeight);
public:
    virtual bool init(CCArray *items, CCSize containerSize, CCSize PaddingSize = CCSizeMake(5, 2.5), int inColumCount = 1);
    
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);

    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);

    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);

    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);

    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);

    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

    virtual void TouchEnd(CCNode *parent, CCNode *current, CCTouch *pTouch);
    
    
protected:
    void initData(CCArray *items, CCSize containerSize, CCSize itemSize, int inColumCount);
protected:
    
    int m_nRows;

    CCTableView * m_pTableView;
    CCLayer *m_pBackground;
    CCSize m_cItemSize;
    CC_PROPERTY(CCSize, m_cPaddingSize, PaddingSize);
    CC_PROPERTY(int, m_nColumcount, ColumCount);
    CC_PROPERTY(float, m_fContainerWidth, ContainerWidth);
    CC_PROPERTY(float, m_fContainerHeight, ContainerHeight);
    CC_PROPERTY(CCArray*, m_cItems, Items);
};

#endif 