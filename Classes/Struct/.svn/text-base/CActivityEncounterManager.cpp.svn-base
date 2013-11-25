//
//  CActivityEncounterManager.cpp
//  91.cube
//
//  Created by phileas on 13-11-11.
//
//

#include "CActivityEncounterManager.h"
#include <string>
using namespace std;
CActivityEncounterManager::CActivityEncounterManager()
{
    m_pSectionManager = SingleSectionManager::instance();
}

CActivityEncounterManager::~CActivityEncounterManager()
{
    
}

/*
 * @param inDict  == activity_config
 */
void CActivityEncounterManager::parseActivityByDic(CCDictionary * inDict)
{
    m_vAllActivites.clear();
    if (inDict)
    {
        string typeName = typeid(*inDict).name();
        if (typeName.find("CCDictionary") != string::npos)
        {
            CCDictElement *element = NULL;
            CCDICT_FOREACH(inDict, element)
            {
                CCDictionary *activty = (CCDictionary*) element->getObject();
                if (activty)
                {
                    CCDICT_FOREACH(activty, element)
                    {
                      int sectionId = strtol(element->getStrKey(), NULL, 10);
                      int num = ((CCString*)element->getObject())->intValue();
                      m_vAllActivites.insert(make_pair(sectionId, num));
                    }
                }
            }
        }
    }
}

/*
 * @param inDict == activity_part
 */

CCArray * CActivityEncounterManager::getCurrentActivityByDict(CCDictionary * inDict)
{
    CCArray * array = CCArray::create();
   
    if (inDict)
    {
        string typeName = typeid(*inDict).name();
        if (typeName.find("CCDictionary") != string::npos)
        {
            CCDictElement *element = NULL;
            CCDICT_FOREACH(inDict, element)
            {
                CCDictionary *activty = (CCDictionary*) element->getObject();
                if (activty)
                {
                    CCDICT_FOREACH(activty, element)
                    {
                        int sectionId = strtol(element->getStrKey(), NULL, 10);
                        int num = ((CCString*)element->getObject())->intValue();
                        CActivityEncounter* encounter = getActivityEncounter(sectionId, num);
                        array->addObject(encounter);
                    }
                }
            }
        }

    }
    
    return array;
}

CActivityEncounter* CActivityEncounterManager::getActivityEncounter(int sectionId, int num)
{
    int allNum =num;
    CPtSectionConfigData * allActivity = m_pSectionManager->getActivitySections();
    map<int, int>::iterator it = m_vAllActivites.find(sectionId);
    if (it == m_vAllActivites.end())
    {
        m_vAllActivites.insert(make_pair(sectionId, num));
    }else
    {
        allNum = it->second;
    }

    CActivityEncounter* encounter  = CActivityEncounter::create();
    CPtSection *section = allActivity->getSectionById(sectionId);
    encounter->setSectionInfo(section);
    encounter->setAllTimes(allNum);
    encounter->setCurrentTime(num);
    return encounter;
}

bool CActivityEncounterManager::haveEncounterBySectionId(int inSectionId)
{
    bool bRet = false;
    map<int, int>::iterator it = m_vAllActivites.find(inSectionId);
    if (it != m_vAllActivites.end())
    {
        bRet = true;
    }
    return bRet;
}