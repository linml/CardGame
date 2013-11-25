//
//  CEventBoxConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-9-11.
//
//

#include "CEventBoxConfigData.h"
#include "gameConfig.h"

// implement class of CEventBoxData
bool CEventBoxData::init()
{
    m_nBoxId = 0;
    m_nType = -1; //1：可略过， 满足消耗条件则可获得奖励， 2：不可略过，无条件奖励， 3: 不可略过，直接消耗并获得奖励， 4:不可略过
    return true;
}

// implement class of CEventBoxConfigData

CEventBoxConfigData::CEventBoxConfigData()
{
    loadDataFromFile();
}

CEventBoxConfigData::~CEventBoxConfigData()
{
    for ( map<int, CEventBoxData*>::iterator i = m_cEventBoxs.begin(); i != m_cEventBoxs.end(); i++)
    {
        CC_SAFE_RELEASE(i->second);
    }
    m_cEventBoxs.clear();
}

CEventBoxData* CEventBoxConfigData::getEventBoxDataById(int inId)
{
    map<int, CEventBoxData*>::iterator i = m_cEventBoxs.find(inId);
    if (i!= m_cEventBoxs.end())
    {
        return i->second;
    }
    return NULL;
}

void CEventBoxConfigData::loadDataFromFile()
{
    CCDictionary * tmp = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "event_box_config.plist"));
    /*
     *<key>box_id</key>
     <string>980001</string>
     <key>name</key>
     <string>神力宝箱</string>
     <key>dictionary_id</key>
     <string>940001</string>
     <key>type</key>
     <string>1</string>
     <key>gp</key>
     <string>25</string>
     <key>coin</key>
     <string>0</string>
     <key>cash</key>
     <string>0</string>
     <key>pic</key>
     <string>b.png</string>
     <key>plist</key>
     <string>plist</string>
     */
    
    if (tmp)
    {
        CCDictElement * element = NULL;
        CCDictionary * elementValue = NULL;
        CEventBoxData * eventData = NULL;
        CCDICT_FOREACH(tmp, element)
        {
            eventData = CEventBoxData::create();
            elementValue = (CCDictionary*) element->getObject();
            eventData->setBoxId(GameTools::intForKey("box_id", elementValue)); //宝箱ID
            eventData->setDictionaryKey(GameTools::valueForKey("dictionary_id", elementValue));//宝箱描述字典ID
            eventData->setBoxType(GameTools::intForKey("type", elementValue));//宝箱类类型 1: 可忽略， 2－4: 不可忽略
            eventData->setGP(GameTools::intForKey("gp", elementValue));
            eventData->setTitle(GameTools::valueForKey("name", elementValue));//宝箱名字
            eventData->setIconPic(GameTools::valueForKey("pic", elementValue));//宝箱的logo图标或特效的作用主体
            eventData->setPlist(GameTools::valueForKey("plist", elementValue));//开启改宝箱的特效
            eventData->retain();
            m_cEventBoxs.insert(make_pair(eventData->getBoxId(), eventData));
            
        }
    }
}