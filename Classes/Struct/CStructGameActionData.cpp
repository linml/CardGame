//
//  CStructGameActionData.cpp
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#include "CStructGameActionData.h"
#include "gameTools.h"
CStructGameActionData::CStructGameActionData()
{
    
}

CStructGameActionData::~CStructGameActionData()
{
    
}
void CStructGameActionData::setDataValue(CCDictionary *dict)
{
    if (dict) {
        m_nBoard_id=GameTools::intForKey("board_id", dict);
        m_sName=GameTools::valueForKey("name", dict);
        m_nShow_type=GameTools::intForKey("show_type", dict);
        m_nPard_id=GameTools::intForKey("pard_id", dict);
        m_nPic=GameTools::valueForKey("pic", dict);
        m_sShow_time=GameTools::valueForKey("show_time", dict);
        m_nItem_1=GameTools::intForKey("item_1", dict);
        m_sItem_tips_1=GameTools::valueForKey("item_tips_2", dict);
        m_nItem_2=GameTools::intForKey("item_2", dict);
        m_sItem_tips_2=GameTools::valueForKey("item_tips_2", dict);
        m_nItem_3=GameTools::intForKey("item_3", dict);
        m_sItem_tips_2=GameTools::valueForKey("item_tips_3", dict);
        m_sContent=GameTools::valueForKey("content", dict);
    }
    
}