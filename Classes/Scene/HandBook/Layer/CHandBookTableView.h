//
//  CHandBookTableView.h
//  91.cube
//
//  Created by linminglu on 11/11/13.
//
//

#ifndef ___1_cube__CHandBookTableView__
#define ___1_cube__CHandBookTableView__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <iostream>
using namespace std;
using namespace cocos2d;
using namespace extension;

class CHandBookTableView :public CCTableView
{
public:
    CHandBookTableView();
    ~CHandBookTableView();
    static  CHandBookTableView *create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
     CC_SYNTHESIZE(CCTouch*,m_pTouch,TouchSet);
    
};

#endif /* defined(___1_cube__CHandBookTableView__) */
