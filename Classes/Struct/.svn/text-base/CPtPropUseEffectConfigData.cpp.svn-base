//
//  CPtPropUseEffectConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-10-30.
//
//

#include "CPtPropUseEffectConfigData.h"
#include "gameConfig.h"

//implement class of PtPropUseEffectData



PtPropUseEffectData::PtPropUseEffectData():m_nItemEffectId(0), m_sIcon(""), m_sSpecialEffect(""), m_nItemEffectTipId(0)
{
    
}


//implement class of CPtPropUseEffectConfigData

CPtPropUseEffectConfigData * CPtPropUseEffectConfigData::s_pInstance = NULL;
CPtPropUseEffectConfigData* CPtPropUseEffectConfigData::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new CPtPropUseEffectConfigData();
    }
    return s_pInstance;
      
}
void CPtPropUseEffectConfigData::releaseData()
{
    CC_SAFE_DELETE(s_pInstance);
    s_pInstance = NULL;
}


CPtPropUseEffectConfigData::CPtPropUseEffectConfigData()
{
    loadDataFromFile();
}

CPtPropUseEffectConfigData::~CPtPropUseEffectConfigData()
{
    map< int, PtPropUseEffectData* >::iterator it = m_cPropUseEffectContainer.begin();
    for (; it != m_cPropUseEffectContainer.end(); it++)
    {
       CC_SAFE_DELETE(it->second);
    }
    m_cPropUseEffectContainer.clear();
   
}

PtPropUseEffectData* CPtPropUseEffectConfigData::getPropUseDataById(int inItemEffectId)
{
    PtPropUseEffectData * data = NULL;
    map<int, PtPropUseEffectData*>::iterator it = m_cPropUseEffectContainer.find(inItemEffectId);
    if (it != m_cPropUseEffectContainer.end())
    {
        data = it->second;
    }
    return data;
    
}
void CPtPropUseEffectConfigData::loadDataFromFile()
{
    CCDictionary * tmp = CCDictionary::createWithContentsOfFileThreadSafe(CSTR_FILEPTAH(resRootPath, "item_effect_config.plist"));
    if (tmp)
    {
        CCDictionary *elementDict = NULL;
        CCDictElement *element = NULL;
        /*
         <key>item_effect_id</key>
         <string>907000</string>
         <key>plist</key>
         <string>0</string>
         <key>tips</key>
         <string>907000</string>
         */
        CCDICT_FOREACH(tmp, element)
        {
            elementDict = (CCDictionary*) element->getObject();
            if (elementDict)
            {
                PtPropUseEffectData *data = new PtPropUseEffectData();
                int itemEffectId = GameTools::intForKey("item_effect_id", elementDict);
                data->setItemEffectId(itemEffectId);
                data->setSpecialEffect(GameTools::valueForKey("plist", elementDict));
                data->setIcon(GameTools::valueForKey("pic", elementDict));
                data->setItemEffectTipId(GameTools::intForKey("tips", elementDict));
                data->setItemType(GameTools::intForKey("type", elementDict));
                data->setItemTarget(GameTools::intForKey("target", elementDict));
                m_cPropUseEffectContainer.insert(make_pair(itemEffectId, data));
                
            }
        }
    }
}
