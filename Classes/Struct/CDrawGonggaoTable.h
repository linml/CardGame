//
//  CDrawGonggaoTable.h
//  91.cube
//
//  Created by linminglu on 12/5/13.
//
//

#ifndef ___1_cube__CDrawGonggaoTable__
#define ___1_cube__CDrawGonggaoTable__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CDrawGonggaoTable :public CCObject
{
public:
    CDrawGonggaoTable()
    {
        cardId=0;
    }
    int cardId;
    string cardName;
    string stringName;
};
#endif /* defined(___1_cube__CDrawGonggaoTable__) */
