//
//  CActivityEncounterManager.h
//  91.cube
//
//  Created by phileas on 13-11-11.
//
//

#ifndef ___1_cube__CActivityEncounterManager__
#define ___1_cube__CActivityEncounterManager__

#include "CSingleton.h"
#include "CPtSectionConfigData.h"
#include "CCDictionary.h"
using namespace cocos2d;

class CActivityEncounter : public CCObject
{
public:
    CREATE_FUNC(CActivityEncounter);
public:
    bool init(){return true;};
    bool haveTime(){return m_nCurrentTime > 0 ;};
protected:
    CC_SYNTHESIZE(CPtSection*, m_pSectionInfo, SectionInfo);
    CC_SYNTHESIZE(int, m_nAllTimes, AllTimes);
    CC_SYNTHESIZE(int, m_nCurrentTime, CurrentTime);
};
using namespace cocos2d;
class CActivityEncounterManager
{
public:
    CActivityEncounterManager();
    ~CActivityEncounterManager();
    void parseActivityByDic(CCDictionary * inDict);
    
    CCArray *getCurrentActivityByDict(CCDictionary *inDict);
    bool haveEncounterBySectionId(int inSectionId);
protected:
    CActivityEncounter* getActivityEncounter(int sectionId, int num);
protected:
    map<int, int>  m_vAllActivites;
    CPtSectionManager *m_pSectionManager;
};

typedef Singleton<CActivityEncounterManager> SingleActivityEncounterManager;
#endif /* defined(___1_cube__CActivityEncounterManager__) */
