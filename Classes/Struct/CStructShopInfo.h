//
//  CStructShopInfo.h
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#ifndef ___1_cube__CStructShopInfo__
#define ___1_cube__CStructShopInfo__

#include <iostream>
#include <string.h>
#include <vector>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;

class CStructShopSellItem ;
typedef  vector<CStructShopSellItem *> SHOPSELLITEMMAP;


class CStructShopInfo
{
public:
    CStructShopInfo();
    ~CStructShopInfo();
    int getShopItemCount();
    void clearShopItemData();
    void logicShopItemNumber(int itemId,int nNowHaveNumber);
public:
    CC_SYNTHESIZE(std::string , m_shop_name, ShopName);
    CC_SYNTHESIZE(int ,m_shop_id,ShopId);
    CC_SYNTHESIZE(int ,m_shop_type,ShopType);
    CC_SYNTHESIZE(bool ,m_shop_is_rondom,ShopIsRondom);
    CC_SYNTHESIZE(int,m_shop_reloadTime,ShopReloadTime);
    CC_SYNTHESIZE(int,m_rondom_num,ShopRondomNumber);
public:
    SHOPSELLITEMMAP mapShopItem;
    
};
#endif /* defined(___1_cube__CStructShopInfo__) */
