//
//  CScreenHandBookLayer.h
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#ifndef ___1_cube__CScreenHandBookLayer__
#define ___1_cube__CScreenHandBookLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class CHandBookLayerCell:public CCLayer
{
    
};



class CScreenHandBookLayer :public CCLayer
{
public:
    CScreenHandBookLayer();
    ~CScreenHandBookLayer();
    CREATE_FUNC(CScreenHandBookLayer);
    bool init();
    void createBackGroud();
public:
};

#endif /* defined(___1_cube__CScreenHandBookLayer__) */
