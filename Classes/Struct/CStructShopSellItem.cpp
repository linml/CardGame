//
//  CStructShopSellItem.cpp
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#include "CStructShopSellItem.h"
#include "CPtPropConfigData.h"
#include "CPanelGameLog.h"
CStructShopSellItem::CStructShopSellItem(int itemId)
{
    m_itemProp=SinglePropConfigData::instance()->getPropById(itemId);
    if (!m_itemProp) {
        CCLog("fuck服务端 传递本地没有数据的商城物品");
        appendFileLog("fuck服务端 传递本地没有数据的商城物品");
    }
    m_iMaxNum=UINT_MAX;
    m_iValue=0;
    m_iOldValue=0;
}

CStructShopSellItem::~CStructShopSellItem()
{
    
}

string CStructShopSellItem::getTip()
{
    if (m_itemProp) {
        return m_itemProp->getTips();
    }
    return "NULL";
}

string CStructShopSellItem::getName()
{
    if (m_itemProp) {
        return m_itemProp->getPropName();
    }
    return "NULL";
}

string CStructShopSellItem::getItemPng()
{
    if (m_itemProp) {
        return m_itemProp->getIconName();
    }
    return "";
}

bool CStructShopSellItem::isDazheItem()
{
    return m_iOldValue!=m_iValue;
}

bool  CStructShopSellItem::isXiangLiangItem()
{
    return m_iMaxNum!=UINT_MAX;
}