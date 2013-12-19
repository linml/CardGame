//
//  CStructShopInfo.cpp
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#include "CStructShopInfo.h"
#include "CStructShopSellItem.h"
#include "gameStruct.h"
#include "CPtPropConfigData.h"


#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,__TYPECLASSNAME__) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
__TYPECLASSNAME__ *temp=*it; \
delete temp; \
temp=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
CEmrysClearVectorMemory< __TYPECLASSNAME__ *> tempClear(VECTORARRAY) ; \
tempClear.clearVector(); \
}


CStructShopInfo::CStructShopInfo()
{
    m_nShuaXinShiJian=3602;
    m_bNeedShuaxin=false;
}

CStructShopInfo::~CStructShopInfo()
{
    
}

void CStructShopInfo::clearShopItemData()
{
    DELETE_POINT_VECTOR(mapShopItem, vector<CStructShopSellItem *> ,CStructShopSellItem);
}

void CStructShopInfo::setRebackMaxNumber()
{
    vector<CStructShopSellItem *>::iterator iter;
    for (iter=mapShopItem.begin(); iter!=mapShopItem.end(); iter++)
    {
        (*iter)->setReback();
    }
    
}

class ShopItem_eq
{
public:
    ShopItem_eq(const int& ss):s(ss){}
    bool operator() (const CStructShopSellItem * c) const
    {
        return c->getItemID() == s;
    }
private:
    int s;
};

void CStructShopInfo::logicShopItemNumber(int itemId,int nNowHaveNumber)
{
     vector<CStructShopSellItem *>::iterator iter;
    iter=find_if(mapShopItem.begin(), mapShopItem.end(), ShopItem_eq(itemId));
    if(iter!=mapShopItem.end())
    {
        (*iter)->setItemSellMaxNum((*iter)->getItemSellMaxNum()-nNowHaveNumber);
    }else
    {
        CCMessageBox("FUCK后台,传递前台商店表里面没有的物品", "FUCK服务端");
    }
}

int CStructShopInfo::getShopItemCount()
{
    return mapShopItem.size();
}


CStructShopInfoManager::CStructShopInfoManager()
{
    
}
CStructShopInfoManager::~CStructShopInfoManager()
{
    
}