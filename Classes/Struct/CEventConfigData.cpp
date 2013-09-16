//
//  CEventConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-9-11.
//
//

#include "CEventConfigData.h"
#include "gameConfig.h"

// implement class of CEventData

CEventData::CEventData()
{
    
}
CEventData::~CEventData()
{

    
}
bool CEventData::init()
{
    m_nEventId = 0;
    m_nEventTipId = 0;
    m_nEventType = 0;
    m_nBackStep = 0;
    m_nCostHP = 0;
    m_nParamOne = 0;
    m_nParamTwo = 0;
    m_nParamThree = 0;
    m_nParamFouth = 0;
    m_nParamFive = 0;
    
    return true;
}

// implement class of CEventDataConfig


CEventDataConfig::CEventDataConfig()
{
    loadEventConfigData();
}
CEventDataConfig::~CEventDataConfig()
{
   
    for ( map<int, CEventData* >::iterator i = m_cAllEvents.begin(); i != m_cAllEvents.end(); i++)
    {
        CC_SAFE_RELEASE(i->second);
    }
    m_cAllEvents.clear();
}


CEventData* CEventDataConfig::getEventById(int inId)
{
    map<int, CEventData*>:: iterator i = m_cAllEvents.end();
    if (inId >= 0 && inId < m_cAllEvents.size())
    {
        i = m_cAllEvents.find(inId);
    }
    if (i == m_cAllEvents.end())
    {
        return NULL;
    }else
    {
        return i->second;
    }
    
}


void CEventDataConfig::loadEventConfigData()
{
    CCDictionary * tmpDict = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "eventlist.plist"));
    
    CCDictElement *tmpElement = NULL;
    CCDictionary * tmp = NULL;
    CCDICT_FOREACH(tmpDict, tmpElement)
    {
        tmp = (CCDictionary*) tmpElement->getObject();
        
        /*
         *	
         <key>event_id</key>
         <string>2</string>
         <key>event_name</key>
         <string>ming2</string>
         <key>event_tips</key>
         <string>miao2</string>
         <key>event_type</key>
         <string>2</string>
         <key>effect</key>
         <string>baba</string>
         <key>soundeffects</key>
         <string>babab.wav</string>
         <key>back_step</key>
         <string>1</string>
         <key>hp</key>
         <string>10</string>
         <key>parameter_1</key>
         <string>1012</string>
         <key>parameter_2</key>
         <string>1012</string>
         <key>parameter_3</key>
         <string>1012</string>
         <key>parameter_4</key>
         <string>1012</string>
         <key>parameter_5</key>
         <string>1012</string>
         */
        if (tmp)
        {
            CEventData *data = CEventData::create();
            data->setEventId(GameTools::intForKey("event_id", tmp));
            data->setEventName(GameTools::valueForKey("event_name", tmp));
            data->setEventTipId(GameTools::intForKey("event_tips", tmp));
            data->setEventType(GameTools::intForKey("event_type", tmp));
            data->setEventEffect(GameTools::valueForKey("effect", tmp));
            data->setSoundEffect(GameTools::valueForKey("soundeffects", tmp));
            data->setBackStep(GameTools::intForKey("back_step", tmp));
            data->setCostHP(GameTools::intForKey("hp", tmp));
            data->setParamOne(GameTools::intForKey("parameter_1", tmp));
            data->setParamTwo(GameTools::intForKey("parameter_2", tmp));
            data->setParamThree(GameTools::intForKey("parameter_3", tmp));
            data->setParamFouth(GameTools::intForKey("parameter_4", tmp));
            data->setParamFive(GameTools::intForKey("parameter_5", tmp));
            data->retain();
            m_cAllEvents.insert(map<int, CEventData*>::value_type(data->getEventId(), data));
        }
    }

}

// implement class of CSpecialEvent

CSpecialEvent::CSpecialEvent()
{
    
}

CSpecialEvent::~CSpecialEvent()
{
    
}

bool CSpecialEvent::init()
{
    m_nSpecialEventId = 0;
    return true;
}

/*
 * @param inPosition 0-->left, 1-->center, 2-->right
 */
void CSpecialEvent::setLeftEvent(int inPosition, int inEventId, int inWordId, const char *inPic)
{
    if (inPosition >= 0 && inPosition < SPECIALEVENTBUTTONCOUNT)
    {
        m_vSpecitialButtons[inPosition].eventId = inEventId;
        m_vSpecitialButtons[inPosition].wordId = inWordId;
        m_vSpecitialButtons[inPosition].iconPic = inPic;
    }
}


// implement class of CSpecialEventConfigData

CSpecialEventConfigData::CSpecialEventConfigData()
{
    m_nCurrentSpecialEventId = -1;
    m_pCurrentSpecialEvent = NULL;
}
CSpecialEventConfigData::~CSpecialEventConfigData()
{
    CC_SAFE_RELEASE(m_pCurrentSpecialEvent);
}


CSpecialEvent * CSpecialEventConfigData::getSpecialEventById(int inId)
{
    if (inId < 0)
    {
        return NULL;
    }
    if (inId != m_nCurrentSpecialEventId)
    {
        
        CCDictionary * tmp = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "special_event_config.plist"));
        if (tmp)
        {
            char buff[15]= {0};
            sprintf(buff, "%d", inId);
            tmp =(CCDictionary*) tmp->objectForKey(buff);
            if (tmp)
            {
                CSpecialEvent * tmpEvent = CSpecialEvent::create();
                /*
                 <key>special_id</key>
                 <string>10000</string>
                 <key>name</key>
                 <string>花花</string>
                 <key>left_event_id</key>
                 <string>10011</string>
                 <key>left_word</key>
                 <string>10001</string>
                 <key>left_pic</key>
                 <string>a.png</string>
                 <key>mid_event_id</key>
                 <string>10002</string>
                 <key>mid_word</key>
                 <string>10003</string>
                 <key>mid_pic</key>
                 <string>b.png</string>
                 <key>right_event_id</key>
                 <string>100040</string>
                 <key>right_word</key>
                 <string>1140</string>
                 <key>right_pic</key>
                 <string>c.png</string>
                 */
                tmpEvent->setSpecialEventId(GameTools::intForKey("special_id", tmp));
                
                int inPosition =  0, inEventId;
                
            }
        }
        
        CC_SAFE_RELEASE(m_pCurrentSpecialEvent);
        
    }
    return m_pCurrentSpecialEvent;
}
