//
//  CTaskConfigData.h
//  91.cube
//
//  Created by phileas on 13-9-9.
//
//

#ifndef ___1_cube__CTaskConfigData__
#define ___1_cube__CTaskConfigData__

#include <string>
#include "cocos2d.h"
#include "CPtStarConfigData.h"
using namespace cocos2d;

using std::string;

#define TASKREWORDMAX 3
typedef struct
{
    int exp = 0;
    int coin = 0;
    int cash = 0;
    
} STaskReword;

class CardItem : public CCObject
{
    
public:
    int cardId;
    int cardCount;
};

class CPtTask : public CCObject
{
public:
    static CPtTask *create();
    
public:
    CPtTask();
    ~CPtTask();
    
    void setTaskReword(int inExp, int inCoin, int inRmb);
    void setCardIds(const char* inJsonString);
    void setPropItems(const char* inJsonString);
    
    const CCArray& getRewordCardIDs(){return m_cRewordCardIDs;};
    const CCArray& getReowrdCardPropItems(){return m_cRewordPropItems;};

protected:
    CCArray m_cRewordCardIDs;
    CCArray m_cRewordPropItems;

protected:
       
    void addRewordCardId(CCInteger * inAddCardId);
    void addRewordPropItem(PropItem * inAddPropItem);
   // CC_SYNTHESIZE_READONLY(CCArray*, m_pRewordCardIDs, TaskRewordCardIds);
   // CC_SYNTHESIZE_READONLY(CCArray*, m_pRewordPropItems, TaskRewordPropIds);
    
    CC_SYNTHESIZE(int, m_nTaskId, TaskId);       //任务ID
    CC_SYNTHESIZE(int, m_nTaskType, TaskType);   //任务类型
    CC_SYNTHESIZE(int, m_nTargetId, TargetId);   //任务目标ID
    CC_SYNTHESIZE(int, m_nTargetNum, TargetNum); //任务数量
    CC_SYNTHESIZE(int, m_nPreTaskId, PreTaskId); //前置任务ID
    CC_SYNTHESIZE(int, m_nTaskTipId, TaskTipId); //任务描述字典ID
    CC_SYNTHESIZE(int, m_nChapterId, ChapterId); //任务指定的章
    CC_SYNTHESIZE(int, m_nSectionId, SectionId); //任务指定的节
    
    CC_PROPERTY_READONLY(STaskReword, m_sTaskReword, TaskReword);
};

class CTaskConfigData : public CCObject
{

public:
    CTaskConfigData();
    virtual ~CTaskConfigData();
    
    CPtTask *getTaskById(int inId);
protected:
    int m_nCurrentTaskId;
    CCDictionary *m_pTaskConfigData;
    CPtTask *m_pCurrentTask;
    
};

typedef Singleton<CTaskConfigData> SingleTaskConfig;


#endif /* defined(___1_cube__CTaskConfigData__) */
