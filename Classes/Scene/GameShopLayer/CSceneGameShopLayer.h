//
//  CSceneGameShopLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#ifndef ___1_cube__CSceneGameShopLayer__
#define ___1_cube__CSceneGameShopLayer__

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class CSceneGameShopLayer : public cocos2d::CCLayer
{
    typedef  cocos2d::CCLayer Super;
public:
    CSceneGameShopLayer();
    ~CSceneGameShopLayer();
    bool  init();
    CREATE_FUNC(CSceneGameShopLayer);
};

#endif /* defined(___1_cube__CSceneGameShopLayer__) */
