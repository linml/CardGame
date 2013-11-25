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
    m_pAllPresent = NULL;
}

CStructGameActionData::~CStructGameActionData()
{
    realeaseData();
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
        m_sContent=GameTools::valueForKey("content", dict);
        //
        if (m_nShow_type == 1)
        {
            if (m_pAllPresent)
            {
                realeaseData();
            }else
            {
                m_pAllPresent = new CCArray(7);
            }
            char buffer[50]={0};
            for (int i = 1; i < 8; i++)
            {
                sprintf(buffer, "item_%d",i);
                int itemId = GameTools::intForKey(buffer, dict);
               
                if (itemId != 0)
                {
                    sprintf(buffer, "item_tips_%d",i);
                    Present* present = new Present(itemId, GameTools::valueForKey(buffer, dict),false);
                    m_pAllPresent->addObject(present);
                }
                
            }
            
        }
        //        m_nItem_1=GameTools::intForKey("item_1", dict);
//        m_sItem_tips_1=GameTools::valueForKey("item_tips_2", dict);
//        m_nItem_2=GameTools::intForKey("item_2", dict);
//        m_sItem_tips_2=GameTools::valueForKey("item_tips_2", dict);
//        m_nItem_3=GameTools::intForKey("item_3", dict);
//        m_sItem_tips_2=GameTools::valueForKey("item_tips_3", dict);
    
    }
    
}

void CStructGameActionData::setItemStatue(CCArray *inItemStatus)
{
    if(inItemStatus)
    {
        CCObject *pObject = NULL;
        CCARRAY_FOREACH(inItemStatus, pObject)
        {
            int itemId = ((CCString*)pObject)->intValue();
            for (int i = 0; i < m_pAllPresent->count(); i++)
            {
                Present *tmpPresent = (Present*) m_pAllPresent->objectAtIndex(i);
                if (tmpPresent && itemId == tmpPresent->getItemId())
                {
                    tmpPresent->setHasGet(true);
                    break;
                }
            }
        }
    }
}

void CStructGameActionData::realeaseData()
{
    if (m_pAllPresent)
    {
        CCObject *pObject =NULL;
        CCARRAY_FOREACH(m_pAllPresent, pObject)
        {
            delete pObject;
        }
        delete m_pAllPresent;
    }
    m_pAllPresent = NULL;
}