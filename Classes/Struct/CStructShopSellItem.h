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
    int propID();
    void setReback();
private:
    CC_SYNTHESIZE(CPtProp *,m_itemProp,ShopSellItemPropData); //对应物品表里面的物品的类
    CC_SYNTHESIZE(unsigned int , m_iMaxNum, ItemSellMaxNum); //最大能够购买的组目
    CC_SYNTHESIZE(int , m_iGroupNum, GroupNum);    // 一个商店该商品如果购买一次 （点击一次）就是一下子购买 该商品1*该number
    CC_SYNTHESIZE(int , m_iValueType, ValueType); //预留在这边
    CC_SYNTHESIZE(int , m_iValue, Value);         //组单价 打折后的价格
    CC_SYNTHESIZE(int , m_iOldValue, OldValue);  //打折前的价格
    CC_SYNTHESIZE(int , m_nItemId, ItemID);
    CC_SYNTHESIZE(int ,m_iMaxNumberBack,MaxNumberBack);
    CC_SYNTHESIZE(int, m_nPriceType, PriceType); // 1--> cash 2-->coin
};

#endif /* defined(___1_cube__CStructShopSellItem__) */
