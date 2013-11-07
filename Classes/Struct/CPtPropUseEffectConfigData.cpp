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
        char buffer[80] = {0};
        CCDICT_FOREACH(tmp, element)
        {
            elementDict = (CCDictionary*) element->getObject();
            if (elementDict)
            {
                PtPropUseEffectData *data = new PtPropUseEffectData();
                int itemEffectId = GameTools::intForKey("item_effect_id", elementDict);
                data->setItemEffectId(itemEffectId);
                CCString *tmp = (CCString*) elementDict->objectForKey("plist");
                if (tmp)
                {
                    sprintf(buffer, "%s", tmp->getCString());
                    vector<string> plist = GameTools::splitArgString(buffer, ",");
                    if (plist.size() == 2)
                    {
                        data->setSpecialEffect(plist.at(0));
                        data->setIcon(plist.at(1));
                    }else if(plist.size() == 1)
                    {
                        data->setSpecialEffect(plist.at(0));
                    }
                }
                data->setItemEffectTipId(GameTools::intForKey("tips", elementDict));
                m_cPropUseEffectContainer.insert(make_pair(itemEffectId, data));
                
            }
        }
    }
}
