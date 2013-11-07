//
//  CPtPropUseManager.cpp
//  91.cube
//
//  Created by phileas on 13-10-29.
//
//

#include "CPtPropUseManager.h"
#include "gameConfig.h"
#include "gameTools.h"
CPtPropUserManager * CPtPropUserManager::s_pInstance = NULL;

CPtPropUserManager * CPtPropUserManager::getInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CPtPropUserManager();
    }
    return s_pInstance;
}

void CPtPropUserManager::releaseManager()
{
    CC_SAFE_DELETE(s_pInstance);
    s_pInstance = NULL;
}

CPtPropUserManager::CPtPropUserManager()
{
    loadDataFromFile();
}

CPtPropUserManager::~CPtPropUserManager()
{
}

vector<int>* CPtPropUserManager::getUserPropsBy(USERTYPE inUserType)
{
    map< USERTYPE, vector<int> >::iterator it  = m_cUserPropContainer.find(inUserType);
    if (it != m_cUserPropContainer.end())
    {
        return &it->second;
    }else
    {
        return NULL;
    }
}

void CPtPropUserManager::loadDataFromFile()
{
    CCDictionary * tmp = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "function.plist"));
    CCAssert(tmp, "no have the file : function.plist");
    CCDictElement *element = NULL;
    CCDictionary *elementDict = NULL;
    CCDICT_FOREACH(tmp, element)
    {
        elementDict = (CCDictionary*) element->getObject();
        USERTYPE id = (USERTYPE) GameTools::intForKey("id", elementDict);
        if (id != NONE)
        {
            int  value = 0;
            char buffer[30] = {0};
            vector<int > props;
            for (int i = 1; i < 6; i++)
            {
                sprintf(buffer, "item_id_%d", i);
                value  = GameTools::intForKey(buffer, elementDict);
                if (value != 0)
                {
                    props.push_back(value);
                }
            }
            props.reserve(props.size());
            m_cUserPropContainer.insert(make_pair(id, props));
        }
    
        
    }
}