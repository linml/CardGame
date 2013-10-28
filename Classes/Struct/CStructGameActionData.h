//
//  CStructGameActionData.h
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#ifndef ___1_cube__CStructGameActionData__
#define ___1_cube__CStructGameActionData__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CStructGameActionData
{
public:
    CStructGameActionData();
    ~CStructGameActionData();
public:
    CC_SYNTHESIZE(int,  m_nActionId,    ActionID);
    CC_SYNTHESIZE(int,  m_nActionName,  ActionName);
};

#endif /* defined(___1_cube__CStructGameActionData__) */
