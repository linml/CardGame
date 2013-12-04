//
//  CUtilityTableView.h
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#ifndef ___1_cube__CUtilityTableView__
#define ___1_cube__CUtilityTableView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace std;
using namespace cocos2d;
using namespace extension;
/**
 *该文件未来会在 和emrystableview 重构的时候整合在一起的。
 */
class CUtilityTableViewDelegate {
public:
    virtual void tablecellTouchNode(CCTableViewCell *cell,CCTouch *pTouch)=0;
    virtual void tableScrolBarView(CCTableView *)=0;
protected:
    CCNode *_cellNode;
};


class CUtilityTableView:public CCTableView
{
public:
    CUtilityTableView();
    ~CUtilityTableView();
    static CUtilityTableView *Create(CCTableViewDataSource *dataSource,CCSize size,CUtilityTableViewDelegate *_mydefineDeleagte);
    virtual  void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual  bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual  void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void scrollViewDidScroll(CCScrollView* view);
    void  reloadDataView();
public:
    CCTouch *m_pTouch;
    CCPoint m_beginTouchPoint;
    CUtilityTableViewDelegate *_mydefineDeleagte;
};
#endif /* defined(___1_cube__CUtilityTableView__) */
