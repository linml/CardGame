//
//  CCardBufferStatus.h
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#ifndef ___1_cube__CCardBufferStatus__
#define ___1_cube__CCardBufferStatus__
/**
 * 定义一些战斗卡牌处于的buffer 表格 
 * 
 *
 */
 
#include <iostream>

#include "cocos2d.h"
#include "CCard.h"
using namespace  cocos2d;
using namespace std;
class CCardBufferStastus :public CCObject
{
public:
    CFightCard *m_fightCardID;
    int m_Huihe;
    int m_SKillId;
};


#endif /* defined(___1_cube__CCardBufferStatus__) */
