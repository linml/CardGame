//
//  CTaskConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-9-9.
//
//

#include "CTaskConfigData.h"
#include "gameConfig.h"
#include "PtJsonUtility.h"
CPtTask *CPtTask::create()
{
    CPtTask * task = new CPtTask();
    task->autorelease();
    return task;
}


CPtTask::CPtTask():m_cRewordCardIDs(3), m_cRewordPropItems(3)
{
 
    
}
CPtTask::~CPtTask()
{
    
}

void CPtTask::setTaskReword(int inExp, int inCoin, int inRmb)
{
    m_sTaskReword.exp = inExp;
    m_sTaskReword.coin = inCoin;
    m_sTaskReword.rmb = inRmb;
}

STaskReword CPtTask::getTaskReword()
{
    return m_sTaskReword;
}

void CPtTask::setCardIds(const char* inJsonString)
{
    CCDictionary *tmp = NULL;
    if (inJsonString)
    {
      tmp =  PtJsonUtility::JsonStringParse(inJsonString);
      CCDictElement *element = NULL;
      CCDICT_FOREACH(tmp, element)
      {
          int id = atoi(element->getStrKey());
          int number = ((CCString *)element->getObject())->intValue();
          CardItem *item = new CardItem();
          item->autorelease();
          item->cardId = id;
          item->cardCount = number;
          m_cRewordCardIDs.addObject(item);
      }
    }
    
}
void CPtTask::setPropItems(const char* inJsonString)
{
    CCDictionary *tmp = NULL;
    if (inJsonString)
    {
        tmp =  PtJsonUtility::JsonStringParse(inJsonString);
        CCDictElement *element = NULL;
        CCDICT_FOREACH(tmp, element)
        {
            int id = atoi(element->getStrKey());
            int number = ((CCString *)element->getObject())->intValue();
            PropItem * item= new PropItem();
            item->autorelease();
            item->propCount = number;
            item->propId = id;
            m_cRewordPropItems.addObject(item);
            
        }
    }

    
}

// implement class of CTaskConfigData

CTaskConfigData::CTaskConfigData()
{
    m_pTaskConfigData = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "task_config.plist"));
    m_nCurrentTaskId = -1;
    m_pCurrentTask = NULL;
}

CTaskConfigData::~CTaskConfigData()
{
    CC_SAFE_RELEASE(m_pTaskConfigData);
    CC_SAFE_RELEASE(m_pCurrentTask);
}

CPtTask * CTaskConfigData::getTaskById(int inId)
{
    if (inId <  0)
    {
        return  NULL;
        
    }
    
    if (inId !=m_nCurrentTaskId)
    {
   
        char buff[10] = {0};
        sprintf(buff, "%d", inId);
        CCDictionary * tmpDic = (CCDictionary*) m_pTaskConfigData->objectForKey(buff);
        if (tmpDic)
        {
            CPtTask *tmpTask = CPtTask::create();
            /*
             *<dict>
             <key>task_id</key>
             <string>300001</string>
             <key>name</key>
             <string>通关序章第一节</string>
             <key>type</key>
             <string>1</string>
             <key>target_id</key>
             <string>0</string>
             <key>num</key>
             <string>1</string>
             <key>pre_task</key>
             <string>0</string>
             <key>exp</key>
             <string>100</string>
             <key>coin</key>
             <string>40</string>
             <key>rmb</key>
             <string>0</string>
             <key>items</key>
             <string>{&quot;300001&quot;:&quot;1&quot;}</string>
             <key>card_ids</key>
             <string>0</string>
             <key>chapter_id</key>
             <string>1</string>
             <key>pard_id</key>
             <string>1</string>
             <key>task_tags</key>
             <string>1</string>
             */
            tmpTask->setTaskId(GameTools::intForKey("task_id", m_pTaskConfigData ));
            tmpTask->setTaskType(GameTools::intForKey("type", m_pTaskConfigData));
            tmpTask->setTargetId(GameTools::intForKey("target_id", m_pTaskConfigData));
            tmpTask->setTargetNum(GameTools::intForKey("num", m_pTaskConfigData));
            tmpTask->setPreTaskId(GameTools::intForKey("pre_task", m_pTaskConfigData));
            tmpTask->setChapterId(GameTools::intForKey("chapter", m_pTaskConfigData));
            tmpTask->setSectionId(GameTools::intForKey("pard_id", m_pTaskConfigData));
            
            int exp = GameTools::intForKey("exp", m_pTaskConfigData);
            int coin = GameTools::intForKey("coin", m_pTaskConfigData);
            int rmb = GameTools::intForKey("rmb", m_pTaskConfigData);
            tmpTask->setTaskReword(exp, coin, rmb);
            tmpTask->setCardIds(GameTools::valueForKey("card_ids", m_pTaskConfigData));
            tmpTask->setPropItems(GameTools::valueForKey("items", m_pTaskConfigData));

            tmpTask->retain();
            CC_SAFE_RELEASE(m_pCurrentTask);
            m_pCurrentTask = tmpTask;
            m_nCurrentTaskId = tmpTask->getTaskId();
        }
        
       
    }
    
}