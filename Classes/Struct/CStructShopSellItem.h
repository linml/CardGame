//
//  CStructShopSellItem.h
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#ifndef ___1_cube__CStructShopSellItem__
#define ___1_cube__CStructShopSellItem__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CPtProp;
class CStructShopSellItem
{
public:
    CStructShopSellItem(int itemId);
    ~CStructShopSellItem();
    string getTip();
    string getName();
    string getItemPng();
    bool isXiangLiangItem();
    bool isDazheItem();
private:
    CC_SYNTHESIZE(CPtProp *,m_itemProp,ShopSellItemPropData);
    CC_SYNTHESIZE(unsigned int , m_iMaxNum, ItemSellMaxNum);
    CC_SYNTHESIZE(int , m_iGroupNum, GroupNum);
    CC_SYNTHESIZE(int , m_iValueType, ValueType);
    CC_SYNTHESIZE(int , m_iValue, Value);
    CC_SYNTHESIZE(int , m_iOldValue, OldValue);
};

#endif /* defined(___1_cube__CStructShopSellItem__) */
