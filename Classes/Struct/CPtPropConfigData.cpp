//
//  CPtPropConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-8-12.
//
//

#include "CPtPropConfigData.h"
#include "gameConfig.h"

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
        m_nPrice =  prop->getPrice(); 
        m_nSkillId = prop->getSkillId(); 
        m_nSellCoin = prop->getSellCoin(); 
        m_nCoin = prop->getCoin(); 
        m_strIconName = prop->getIconName(); 
        m_nLimitNum = prop->getLimitNum();
        m_nIsOnly = prop->getIsOnlyNum(); 
        m_strTips = prop->getTips(); 
        
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
        prop = m_pAllProps.at(inPropId);
        
    }

    return prop;
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
            prop= new CPtProp();
            tmpValue = (CCDictionary *)element->getObject();
            prop->setPropName(GameTools::valueForKey("name", tmpValue));
            prop->setPropType(GameTools::intForKey("type", tmpValue));
            prop->setUnLockLevel(GameTools::intForKey("unlock_level", tmpValue));
            prop->setUseDelete(GameTools::intForKey("use_delete", tmpValue));
            prop->setPrice(GameTools::intForKey("price", tmpValue));
            prop->setSkillId(GameTools::intForKey("skill_id", tmpValue));
            prop->setSellCoin(GameTools::intForKey("sell_coin", tmpValue));
            prop->setCoin(GameTools::intForKey("coin",tmpValue));
            prop->setIconName(GameTools::valueForKey("icon", tmpValue));
            prop->setLimitNum(GameTools::intForKey("limit_num", tmpValue));
            prop->setIsOnlyNum(GameTools::intForKey("is_only", tmpValue));
            prop->setTips(GameTools::valueForKey("tips", tmpValue));
            m_pAllProps.insert(map<int, CPtProp*>::value_type (keyId, prop));
        }
    }
}
