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
    m_pTaskConfigData->retain();
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
            
            tmpTask->setTaskId(GameTools::intForKey("mission_id", tmpDic ));
            tmpTask->setTaskType(GameTools::intForKey("type", tmpDic));
            tmpTask->setTargetId(GameTools::intForKey("target_id", tmpDic));
            tmpTask->setTargetNum(GameTools::intForKey("num", tmpDic));
            tmpTask->setPreTaskId(GameTools::intForKey("pre_task", tmpDic));
            tmpTask->setChapterId(GameTools::intForKey("chapter_id", tmpDic));
            tmpTask->setSectionId(GameTools::intForKey("part_id", tmpDic));
            tmpTask->setTaskTipId(GameTools::intForKey("task_tags", tmpDic));
    
            
            int exp = GameTools::intForKey("exp", tmpDic);
            int coin = GameTools::intForKey("coin", tmpDic);
            int rmb = GameTools::intForKey("rmb", tmpDic);
            tmpTask->setTaskReword(exp, coin, rmb);
            tmpTask->setCardIds(GameTools::valueForKey("card_ids", tmpDic));
            tmpTask->setPropItems(GameTools::valueForKey("items", tmpDic));

            tmpTask->retain();
            CC_SAFE_RELEASE(m_pCurrentTask);
            m_pCurrentTask = tmpTask;
            m_nCurrentTaskId = tmpTask->getTaskId();
        }
        
       
    }
    return m_pCurrentTask;
    
}