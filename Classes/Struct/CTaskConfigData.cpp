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
#include "CPanelGameLog.h"

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

void CPtTask::setTaskReword(int inExp, int inCoin, int inCash)
{
    m_sTaskReword.exp = inExp;
    m_sTaskReword.coin = inCoin;
    m_sTaskReword.cash = inCash;
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

CPtTaskLogic::CPtTaskLogic()
{
    m_iPtTaskTargetID=0;
    m_iPtTaskCurrentNumber=0;
    m_iPtTaskTotalNumber=0;
    m_iPtTaskType=0;
}
CPtTaskLogic::~CPtTaskLogic()
{
    
}
void CPtTaskLogic::setInitDataByCPtTask(CPtTask *ptTask)
{
    this->m_iPtTaskType=ptTask->getTaskType();
    this->m_iPtTaskTotalNumber=ptTask->getTargetNum();
    this->m_iPtTaskCurrentNumber=0;
    this->m_iPtTaskTargetID=ptTask->getTargetId();
}


template<class T,class TargetId>
class CGetVectorEquitTargetId : public binary_function<typename std::vector<T>::value_type , TargetId, bool>
{
public:
    bool operator()(const typename vector<T>::value_type iter, const TargetId& value) const
    {
        if (iter == value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

void CPtTaskLogic::addTaskOperator(int taskType,  vector<int > *targetIdVector)
{
    if (m_iPtTaskType!=1 && !targetIdVector)
    {
        return ;
    }
    switch (m_iPtTaskType) {
        case 1:
            m_iPtTaskCurrentNumber++;
            break;
        case 2:
        {//打怪
            //这边可以直接调用如下方法
            int value=count(targetIdVector->begin(), targetIdVector->end(),m_iPtTaskTargetID);
//            int value= count_if(targetIdVector->begin(), targetIdVector->end(), bind2nd(CGetVectorEquitTargetId<int,int>(),m_iPtTaskTargetID));
            m_iPtTaskCurrentNumber +=value;
        }
            break;
        // 寻找物品
        case 3:
            break;
            
        default:
            break;
    }
    
}
void CPtTaskLogic::subTaskOperator(int taskType,  vector<int > *targetIdVector)
{
    if (m_iPtTaskType!=1 && !targetIdVector) {
        return ;
    }
    
    
}
// implement class of CTaskConfigData

CTaskConfigData::CTaskConfigData()
{
    m_pTaskConfigData = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "task_config.plist"));
    m_pTaskConfigData->retain();
    m_nCurrentTaskId = -1;
    m_pCurrentTask = NULL;
    list<int>taskIdListId;
    list<int>taskIdPre;
    CCArray *array=m_pTaskConfigData->allKeys();
    for (int i=0; i<array->count(); i++)
    {
        CCString *key=(CCString *)array->objectAtIndex(i);
        int iValue=key->intValue();
        CCDictionary *temDict=(CCDictionary *)m_pTaskConfigData->objectForKey(key->m_sString);
        int preValue=GameTools::intForKey("pre_task", temDict);
        taskIdListId.push_back(iValue);
        taskIdPre.push_back(preValue);
        if(preValue==0)
        {
            m_nMinTaskId=iValue;
        }
   }
    
    bool isPreExsit=false;
    for (list<int>::iterator it=taskIdListId.begin(); it!=taskIdListId.end(); it++) {
        isPreExsit=false;
        for (list<int>::iterator itPre=taskIdPre.begin(); itPre!=taskIdPre.end();itPre++)
        {
            if( *it==*itPre)
            {
                isPreExsit=true;
                break;
            }
        }
        if(!isPreExsit)
        {
            m_nMaxTaskId=*it;
        }
    }
   
    CCLog("MaxTaskId %d ;MinTaskId %d",m_nMaxTaskId,m_nMinTaskId);

}

CTaskConfigData::~CTaskConfigData()
{
    CC_SAFE_RELEASE(m_pTaskConfigData);
    CC_SAFE_RELEASE(m_pCurrentTask);
}

CPtTask *CTaskConfigData::getNextByPreTask(int inId)
{
    if (inId<0) {
        return NULL;
    }
    if (inId==0 || inId==m_nMaxTaskId) {
        char buff[10] = {0};
        if (inId==0)
        {
            sprintf(buff, "%d", m_nMinTaskId);
            
        }
        else if(inId==m_nMaxTaskId)
        {
            sprintf(buff, "%d", m_nMaxTaskId);
        }

            CCDictionary * tmpDic = (CCDictionary*) m_pTaskConfigData->objectForKey(buff);
            CPtTask *tmpTask =NULL;
            if (tmpDic)
            {
                tmpTask=CPtTask::create();
                tmpTask->setTaskId(GameTools::intForKey("task_id", tmpDic ));
                tmpTask->setTaskType(GameTools::intForKey("type", tmpDic));
                tmpTask->setTargetId(GameTools::intForKey("target_id", tmpDic));
                tmpTask->setTargetNum(GameTools::intForKey("num", tmpDic));
                tmpTask->setPreTaskId(GameTools::intForKey("pre_task", tmpDic));
                tmpTask->setChapterId(GameTools::intForKey("chapter_id", tmpDic));
                tmpTask->setSectionId(GameTools::intForKey("part_id", tmpDic));
                tmpTask->setTaskTipId(GameTools::intForKey("task_tags", tmpDic));
            
            
                int exp = GameTools::intForKey("exp", tmpDic);
                int coin = GameTools::intForKey("coin", tmpDic);
                int cash = GameTools::intForKey("cash", tmpDic);
                tmpTask->setTaskReword(exp, coin, cash);
                tmpTask->setCardIds(GameTools::valueForKey("card_ids", tmpDic));
                tmpTask->setPropItems(GameTools::valueForKey("items", tmpDic));
            
                tmpTask->retain();
                CC_SAFE_RELEASE(m_pCurrentTask);
                m_pCurrentTask = tmpTask;
                m_nCurrentTaskId = tmpTask->getTaskId();
            }
        return tmpTask;
    }
    else 
    {
        
        CCArray *array=m_pTaskConfigData->allKeys();
        for (int i=0; i<array->count(); i++)
        {
            CCString *key=(CCString *)array->objectAtIndex(i);
            CCDictionary * tmpDic = (CCDictionary*) m_pTaskConfigData->objectForKey(key->m_sString);
            if(GameTools::intForKey("pre_task", tmpDic)==m_nCurrentTaskId)
            {
                CPtTask *tmpTask = CPtTask::create();
                tmpTask->setTaskId(GameTools::intForKey("task_id", tmpDic ));
                tmpTask->setTaskType(GameTools::intForKey("type", tmpDic));
                tmpTask->setTargetId(GameTools::intForKey("target_id", tmpDic));
                tmpTask->setTargetNum(GameTools::intForKey("num", tmpDic));
                tmpTask->setPreTaskId(GameTools::intForKey("pre_task", tmpDic));
                tmpTask->setChapterId(GameTools::intForKey("chapter_id", tmpDic));
                tmpTask->setSectionId(GameTools::intForKey("part_id", tmpDic));
                tmpTask->setTaskTipId(GameTools::intForKey("task_tags", tmpDic));
                int exp = GameTools::intForKey("exp", tmpDic);
                int coin = GameTools::intForKey("coin", tmpDic);
                int cash = GameTools::intForKey("cash", tmpDic);
                tmpTask->setTaskReword(exp, coin, cash);
                tmpTask->setCardIds(GameTools::valueForKey("card_ids", tmpDic));
                tmpTask->setPropItems(GameTools::valueForKey("items", tmpDic));
            
                tmpTask->retain();
                CC_SAFE_RELEASE(m_pCurrentTask);
                m_pCurrentTask = tmpTask;
                m_nCurrentTaskId = tmpTask->getTaskId();
                return m_pCurrentTask;
            }
        }
        
        
    }
    return m_pCurrentTask;
}

CPtTask * CTaskConfigData::getTaskById(int inId)
{
    if (inId <  0)
    {
        return  NULL;
        
    }
    if (inId==0 || inId==m_nMaxTaskId) {
        char buff[10] = {0};
        if (inId==0)
        {
            sprintf(buff, "%d", m_nMinTaskId);
            
        }
        else if(inId==m_nMaxTaskId)
        {
            sprintf(buff, "%d", m_nMaxTaskId);
        }
        
        CCDictionary * tmpDic = (CCDictionary*) m_pTaskConfigData->objectForKey(buff);
        CPtTask *tmpTask =NULL;
        if (tmpDic)
        {
            tmpTask=CPtTask::create();
            tmpTask->setTaskId(GameTools::intForKey("task_id", tmpDic ));
            tmpTask->setTaskType(GameTools::intForKey("type", tmpDic));
            tmpTask->setTargetId(GameTools::intForKey("target_id", tmpDic));
            tmpTask->setTargetNum(GameTools::intForKey("num", tmpDic));
            tmpTask->setPreTaskId(GameTools::intForKey("pre_task", tmpDic));
            tmpTask->setChapterId(GameTools::intForKey("chapter_id", tmpDic));
            tmpTask->setSectionId(GameTools::intForKey("part_id", tmpDic));
            tmpTask->setTaskTipId(GameTools::intForKey("task_tags", tmpDic));
            
            
            int exp = GameTools::intForKey("exp", tmpDic);
            int coin = GameTools::intForKey("coin", tmpDic);
            int cash = GameTools::intForKey("cash", tmpDic);
            tmpTask->setTaskReword(exp, coin, cash);
            tmpTask->setCardIds(GameTools::valueForKey("card_ids", tmpDic));
            tmpTask->setPropItems(GameTools::valueForKey("items", tmpDic));
            
            tmpTask->retain();
            CC_SAFE_RELEASE(m_pCurrentTask);
            m_pCurrentTask = tmpTask;
            m_nCurrentTaskId = tmpTask->getTaskId();
        }
        return tmpTask;
    }
    else if (inId !=m_nCurrentTaskId)
    {   
        char buff[10] = {0};
        sprintf(buff, "%d", inId);
        CCDictionary * tmpDic = (CCDictionary*) m_pTaskConfigData->objectForKey(buff);
        if (tmpDic)
        {
            CPtTask *tmpTask = CPtTask::create();
            
            tmpTask->setTaskId(GameTools::intForKey("task_id", tmpDic ));
            tmpTask->setTaskType(GameTools::intForKey("type", tmpDic));
            tmpTask->setTargetId(GameTools::intForKey("target_id", tmpDic));
            tmpTask->setTargetNum(GameTools::intForKey("num", tmpDic));
            tmpTask->setPreTaskId(GameTools::intForKey("pre_task", tmpDic));
            tmpTask->setChapterId(GameTools::intForKey("chapter_id", tmpDic));
            tmpTask->setSectionId(GameTools::intForKey("part_id", tmpDic));
            tmpTask->setTaskTipId(GameTools::intForKey("task_tags", tmpDic));
    
            
            int exp = GameTools::intForKey("exp", tmpDic);
            int coin = GameTools::intForKey("coin", tmpDic);
            int cash = GameTools::intForKey("cash", tmpDic);
            tmpTask->setTaskReword(exp, coin, cash);
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