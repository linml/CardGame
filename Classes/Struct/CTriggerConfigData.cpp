//
//  CTriggerConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-9-10.
//
//

#include "CTriggerConfigData.h"
#include "gameConfig.h"

// implement class of CPtTrigger

CPtTrigger* CPtTrigger::create()
{
    CPtTrigger* trigger = new CPtTrigger();
    trigger->autorelease();
    return trigger;
}

CPtTrigger::CPtTrigger()
{
  
}

CPtTrigger::~CPtTrigger()
{
    
}

CPtTriggerConfigData::CPtTriggerConfigData()
{
    initialTriggerConfigData();
}

CPtTriggerConfigData::~CPtTriggerConfigData()
{
    CC_SAFE_RELEASE(m_pCurrentTrigger);
}

CPtTrigger* CPtTriggerConfigData::getTriggerById(int inId)
{
    if (inId < 0)
    {
        return NULL;
    }
   
    if (inId != m_nCurrentTriggerId)
    {
        
        CCDictionary * tmpDict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "trigger.plist"));
        if (tmpDict)
        {
            char buffer[16]={0};
            sprintf(buffer, "%d", inId);
            tmpDict = (CCDictionary*) tmpDict->objectForKey(buffer);
            if (tmpDict)
            {
                /*
                 <dict>
                 <key>trigger_id</key>
                 <string>1</string>
                 <key>trigger_point</key>
                 <string>2</string>
                 <key>story_id</key>
                 <string>30001</string>
                 <key>special_event_id</key>
                 <string>0</string>
                 </dict>
                 */
                CPtTrigger *tmp = CPtTrigger::create();
                tmp->setTriggerId(GameTools::intForKey("trigger_id", tmpDict));
                tmp->setTriggerStep(GameTools::intForKey("trigger_point", tmpDict));
                tmp->setStoryId(GameTools::intForKey("story_id", tmpDict));
                tmp->setSpecialEventId(GameTools::intForKey("special_event_id", tmpDict));
                CC_SAFE_RELEASE(m_pCurrentTrigger);
                m_pCurrentTrigger = tmp;
                m_pCurrentTrigger->retain();
            }else
            {
                return NULL;
            }
           
        }else
        {
            return NULL;
        }
    }
    
    return m_pCurrentTrigger;

}



void CPtTriggerConfigData::initialTriggerConfigData()
{
    m_nCurrentTriggerId = -1;
    m_pCurrentTrigger = NULL;
    
}

