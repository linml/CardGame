//
//  CEventBoxConfigData.h
//  91.cube
//
//  Created by phileas on 13-9-11.
//
//

#ifndef ___1_cube__CEventBoxConfigData__
#define ___1_cube__CEventBoxConfigData__

#include <map>
#include <string>
#include "cocos2d.h"
#include "CSingleton.h"

using namespace cocos2d;
using std::string;
using std::map;
class CEventBoxData : public CCObject
{
public:
    CREATE_FUNC(CEventBoxData);
    
public:
    CEventBoxData(){};
    virtual ~CEventBoxData(){};
    virtual bool init();
    CC_SYNTHESIZE(int, m_nBoxId, BoxId);
    CC_SYNTHESIZE(int, m_nDictionaryId, DictionaryId);
    CC_SYNTHESIZE(int, m_nType, BoxType);
    CC_SYNTHESIZE(int, m_nTitleId, TitleId);
    CC_SYNTHESIZE(string, m_sPlist, Plist);
    CC_SYNTHESIZE(string, m_sPic, IconPic);
    
};

class CEventBoxConfigData : public CCObject
{
public:
    CEventBoxConfigData();
    virtual ~CEventBoxConfigData();
    CEventBoxData *getEventBoxDataById(int inId);
protected:
    void loadDataFromFile();
protected:
    map<int, CEventBoxData*> m_cEventBoxs;
    
};


typedef Singleton<CEventBoxConfigData> SingleEventBoxes ;
#endif /* defined(___1_cube__CEventBoxConfigData__) */
