//
//  CPtSectionConfigData.h
//  91.cube
//
//  Created by phileas on 13-9-4.
//
//

#ifndef ___1_cube__CPtSectionConfigData__
#define ___1_cube__CPtSectionConfigData__

#include <map>
#include <string>
#include "cocos2d.h"
#include "CSingleton.h"

#define INVALID 0
#define BASEVALUE 300000
#define INTERVALVALUE 100

using namespace cocos2d;

typedef struct
{
    int itemId;
    int number;
}RewordCard;

class CPtSection : public CCObject
{
public:
    static CPtSection* create();
public:
    
    void setTriggers(const char * string);
    const vector<int>& getTriggers();
    
    CC_SYNTHESIZE(int, m_nSectionId, SectionId);              // 小节Id
    CC_SYNTHESIZE(std::string, m_sSectionName, SectionName);  //小节名称
    CC_SYNTHESIZE(int, m_nChapterId, ChapterId);              // 该小节所属章节ID
    CC_SYNTHESIZE(int, m_nSequence, Sequence);                // 该小节在所属章节中的次序
    CC_SYNTHESIZE(std::string ,m_sTipId, TipId);                      // 小节描述的字典ID
    CC_SYNTHESIZE(std::string, m_sSectionPicName, SectionPicName);// 小节图片名
    CC_SYNTHESIZE(int, m_nSceneId, SceneId);                  // 小节对应场景ID
    CC_SYNTHESIZE(int, m_nMaxStep, MaxStep);                  // 该小节的最大步数
    
    CC_SYNTHESIZE(int, m_nAp, AP);              //进度每前进一步所消耗的体力值
    CC_SYNTHESIZE(int, m_nBounsExp, BounsExp);                // 小节前进经验奖励
    CC_SYNTHESIZE(int, m_nBounsMoney, BounsMoney);            // 小节前进金币奖励
    
    CC_SYNTHESIZE(int, m_nBounsRmb, BounsRmb);                //小节通关现金币奖励
    CC_SYNTHESIZE(int, m_nEndBounsExp, EndBounsExp);          //小节通关经验奖励
    CC_SYNTHESIZE(int, m_nEndBounsMoney, EndBounsMoney);      // 小节通关金币奖励

    CC_SYNTHESIZE(int, m_nTaskId, TaskId);                    // 任务ID
    CC_SYNTHESIZE(int, m_nRandomEventId, RandomEventId);      //随机ID
    
   // CC_SYNTHESIZE(int, m_nTriggerId, TriggerId);              // 触发ID组
    
    
protected:
    vector<int> m_cTriggers;
};


class CPtSectionConfigData : public CCObject
{
public:
    static CPtSectionConfigData* create(int inChapterId);
    CPtSectionConfigData(CCDictionary *inSectionDictionary);
    virtual ~CPtSectionConfigData();
    CPtSection *getSectionById(int inSectionId);
    CPtSection *getSectionByIndex(int inSectionIdex);
    CCArray *getSectionsBeforeId(int inSectionId);
    CCArray *getSectionByOrder();
protected:
    void loadSectionDataByChapter(CCDictionary *inSectionDictionary);
protected:
    CCDictionary *m_pIdToSequence;  // store: key --> sectionID, value --> sequence in chapter:
    CCDictionary* m_pSectionsInChapter; // key -> sequence:  value -> CPtSection
};

class CPtSectionManager : public CCObject
{
public:
    CPtSectionManager();
    virtual ~CPtSectionManager();
    CPtSectionConfigData * getSectionByChapterId(int inChapterId);
protected:
  //  std::map<int, CPtSectionConfigData*> m_cSections;
    CCDictionary *m_pSections;
};

typedef  Singleton<CPtSectionManager> SingleSectionManager;


class CPtChapter : public CCObject
{
public:
    static CPtChapter* create();
public:
    CPtChapter();
    virtual ~CPtChapter();

    void setReword(int inRewordId, int number);
    void setRewordCard(int inCardId, int number);
    const RewordCard& getRewordCard(){return m_cRewordCard;};
    CPtSectionConfigData *getSections();
protected:
    CPtSectionConfigData *m_pSectionData;
    CCDictionary * m_pReword;
    RewordCard m_cRewordCard;
    
    

    CC_SYNTHESIZE(int, m_nChapterId, ChapterId);
    CC_SYNTHESIZE(std::string, m_sChapterName, ChapterName);
    CC_SYNTHESIZE(std::string, m_sChapterTipId, ChapterTipid);
    CC_SYNTHESIZE(std::string, m_sChapterPicName, ChapterPicName);
    

};

class CPtChapterConfigData : public CCObject
{
public:
    CPtChapterConfigData();
    virtual ~CPtChapterConfigData();
    CCArray *getOpenChapter(int inMaxId);
    CPtChapter *getChapterById(int inChapterId);
protected:
    void loadChapterConifg();
    
protected:
    CCDictionary* m_pChapters;
    

};

typedef  Singleton<CPtChapterConfigData> SingleCPtChapters;
#endif /* defined(___1_cube__CPtSectionConfigData__) */
