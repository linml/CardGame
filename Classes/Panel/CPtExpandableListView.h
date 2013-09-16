//
//  CPtExpandableListView.h
//  91.cube
//
//  Created by phileas on 13-9-10.
//
//

#ifndef ___1_cube__CPtExpandableListView__
#define ___1_cube__CPtExpandableListView__

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

enum ITEM_TYPE
{
    PARENT_TYPE = 0,
    CHILDRENT_TYPE = 1
};

class  CPtExpandableListViewItem : public CCObject
{
public:
    CREATE_FUNC(CPtExpandableListViewItem);
    CPtExpandableListViewItem();
    virtual ~CPtExpandableListViewItem();
    
    virtual bool init(){return true;};
    // DATA:
    int getChildrenCount();
    CCArray *getChildren();
    ITEM_TYPE getItemType(){return m_eItemType;};
    void setItemType(ITEM_TYPE inItemType){m_eItemType = inItemType;};
    void setHandler(CCObject *inTarget, SEL_CallFuncO inNormalSelector, SEL_CallFuncO inSelectedSelector,SEL_CallFuncO inGetChildrenSelector);
    void setChildrenItem(CCArray *inChildrenItems);
    // UI:
    void setNormal();
    void setSelected();
    
    
protected:
    void initData();
protected:
    
    bool m_bLoadChildren;

    ITEM_TYPE m_eItemType;
    CCArray *m_pChildrenItems;
    
    CCObject *m_pTargetHandler;
    SEL_CallFuncO m_pNormalSelector;
    SEL_CallFuncO m_pSelectedSelector;
    SEL_CallFuncO m_pGetChildrenSelector;
    
    CC_SYNTHESIZE(bool, m_bOpen, Open);
    CC_PROPERTY(CCNode*, m_pDisplayView, DisplayView);
    CC_SYNTHESIZE(int, m_nIndex, Index);
    
};

class CPtExpandableListViewItemList : public CCObject
{
public:
    CREATE_FUNC(CPtExpandableListViewItemList);
public:
    
    CPtExpandableListViewItemList();
    virtual ~CPtExpandableListViewItemList();
    virtual bool init(){return true;};
    CPtExpandableListViewItem *getItemByIndex(int inIndex);
    CCArray *getListViewItems();
    void setListViewItems(CCArray *inItems);
    int  getAllItemCount();
    bool setOpenItemByIndex(int inIndex);
    
protected:
    CCArray *m_pExpListViewItems;
    
    CC_PROPERTY(int, m_nWhichOneOpen, WhichOneOpen)
    
};




class CPtExpandableListView : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
    
public:
    static CPtExpandableListView *create(CPtExpandableListViewItemList * items, CCSize containerSize, CCSize paddingSize);
    
public:
    
    CPtExpandableListView();
    virtual ~CPtExpandableListView();
    bool initialCPtExpandableListView(CPtExpandableListViewItemList * items, CCSize containerSize, CCSize paddingSize);
    
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    CCTableView *getTabelView(){return m_pTableView;};
    void setOpenItemByIndex(int inIndex);
    
protected:
    void initData(CPtExpandableListViewItemList *items, CCSize containerSize, CCSize itemSize);
protected:
    
    float m_fContainerHeight;
    float m_fContainerWidth;
    
    CCSize m_cPaddingSize;
    CCTableView * m_pTableView;
    CCLayer *m_pBackground;
    CC_PROPERTY(CPtExpandableListViewItemList*, m_cItems, Items);
    CC_SYNTHESIZE(CCSize, m_cItemSize, ItemSize);
    
    
};

#endif /* defined(___1_cube__CPtExpandableListView__) */
