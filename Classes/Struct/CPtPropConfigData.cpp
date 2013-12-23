//
//  CPtPropConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-8-12.
//
//

#include "CPtPropConfigData.h"
#include "gameConfig.h"
#include "Utility.h"

std::string CPtProp::getTips()
{
  return  Utility::getWordWithFile("dictionary.plist", getTipKey().c_str());
}


CPtPropConfigData::CPtPropConfigData()
{
   CCDictionary* m_pConfigData = CCDictionary::createWithContentsOfFile(g_str_prop_config.c_str());
    if (m_pConfigData)
    {
        CCAssert(m_pConfigData!=NULL, "openfilefailure");
        loadPropToMap(m_pConfigData);
    }
   
    m_nPropId = -1;

}

CPtPropConfigData::~CPtPropConfigData()
{
    
    for (std::map<int, CPtProp*>::iterator i = m_pAllProps.begin(); i != m_pAllProps.end(); i++)
    {
        CC_SAFE_RELEASE(i->second);
        i->second = NULL;
    }
    m_pAllProps.clear();
}

bool CPtPropConfigData::getPropDataById(const int &inPropId)
{
    if(inPropId < 0)
    {
        return false;
    }
    if (m_nPropId == inPropId)
    {
        CCLog("don't update propconfig file");
        return true;
    }
   
        CPtProp *prop = m_pAllProps.at(inPropId);
        if (prop == NULL)
        {
            return false;
            
        }
        // getData:
        m_strPropName = prop->getPropName();    
        m_nPropType  = prop->getPropType();    
        m_nUnLockLevel = prop->getUnLockLevel();
        m_nUseDelete = prop->getUseDelete(); 
        m_nUseId = prop->getUseId();
        m_strIconName = prop->getIconName(); 
        m_nLimitNum = prop->getLimitNum();
        m_nIsOnly = prop->getIsOnlyNum(); 
        m_sTipKey = prop->getTipKey();
        
        m_nPropId = inPropId;
        return true;
}

CPtProp * CPtPropConfigData::getPropById(const int &inPropId)
{
    bool bRet = false;
    CPtProp * prop = NULL;
    
    if(inPropId < 0)
    {
        bRet = false;
    }else
    {
        prop = m_pAllProps[inPropId];
        
    }

    return prop;
}

std::string CPtPropConfigData::getIconName(const int &inPropId)
{
    CPtProp * prop = NULL;
    
    if(inPropId < 0)
    {
        return "";
    }else if(inPropId == EXPID)
    {
        return m_sExpIcon;
    }else if(inPropId == COINSID)
    {
        return m_sCoinIcon;
    }
    else
    {
        prop = m_pAllProps.at(inPropId);
        return prop->getIconName();
    }
    
    return "";
}

std::string CPtPropConfigData::getPropName(const int inPropId)
{
    std::string propName;
    if(inPropId > 0)
    {
        if(inPropId == EXPID)
        {
            propName = m_sExpName;
        }else if(inPropId == COINSID)
        {
            propName= m_sCoinName;
        }
        else
        {
            CPtProp* prop = m_pAllProps.at(inPropId);
            propName = prop->getPropName();
        }
    }     

    return propName;
}


void CPtPropConfigData::loadPropToMap(CCDictionary* inConfigData)
{
    if (inConfigData)
    {
        CPtProp *prop = NULL;
        CCDictElement *element = NULL;
        CCDictionary * tmpValue = NULL;
        int keyId = 0;
        CCDICT_FOREACH(inConfigData, element)
        {
            keyId = atoi(element->getStrKey());
            tmpValue = (CCDictionary *)element->getObject();
            if (keyId == COINSID)
            {
                //经验和金币
                m_sCoinIcon = GameTools::valueForKey("icon", tmpValue);
                m_sCoinName = GameTools::valueForKey("name", tmpValue);
                continue;
            }else if(keyId == EXPID)
            {
                m_sExpIcon = GameTools::valueForKey("icon", tmpValue);
                m_sExpName = GameTools::valueForKey("name", tmpValue);
                continue;
                
            }

            /*
             *
             <key>item_id</key>
             <string>100001</string>
             <key>name</key>
             <string>体力药水</string>
             <key>type</key>
             <string>1</string>
             <key>unlock_level</key>
             <string>1</string>
             <key>use_delete</key>
             <string>1</string>
             <key>item_effect_id</key>
             <string>100001</string>
             <key>use_scene</key>
             <string>0</string>
             <key>icon</key>
             <string>icon_007.png</string>
             <key>limit_num</key>
             <string>20</string>
             <key>is_only</key>
             <string>0</string>
             <key>tips</key>
             <string>999991</string>

             */
            tmpValue = (CCDictionary *)element->getObject();
            if (keyId == COINSID)
            {
                //经验和金币
                m_sCoinIcon = GameTools::valueForKey("icon", tmpValue);
                m_sCoinName = GameTools::valueForKey("name", tmpValue);
                CCLog(m_sCoinName.c_str());
                continue;
            }else if(keyId == EXPID)
            {
                m_sExpIcon = GameTools::valueForKey("icon", tmpValue);
                m_sExpName = GameTools::valueForKey("name", tmpValue);
                CCLog(m_sExpName.c_str());
                continue;
                
            }
            prop= new CPtProp();
            prop->setPropId(keyId);
            prop->setPropName(GameTools::valueForKey("name", tmpValue));
            prop->setPropType(GameTools::intForKey("type", tmpValue));
            prop->setUnLockLevel(GameTools::intForKey("unlock_level", tmpValue));
            prop->setUseDelete(GameTools::intForKey("use_delete", tmpValue));
            prop->setUseId(GameTools::intForKey("item_effect_id", tmpValue));
            prop->setIconName(GameTools::valueForKey("icon", tmpValue));
            prop->setLimitNum(GameTools::intForKey("limit_num", tmpValue));
            prop->setIsOnlyNum(GameTools::intForKey("is_only", tmpValue));
            prop->setTipKey(GameTools::valueForKey("tips", tmpValue));
            m_pAllProps.insert(map<int, CPtProp*>::value_type (keyId, prop));
        }
    }
}
