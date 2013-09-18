//
//  CSceneConfigData.h
//  91.cube
//
//  Created by phileas on 13-9-11.
//
//

#ifndef ___1_cube__CSceneConfigData__
#define ___1_cube__CSceneConfigData__

#include <string>
#include "cocos2d.h"
#include "CSingleton.h"
using std::string;
using namespace cocos2d;

class CSceneData : public CCObject
{
public:
    CREATE_FUNC(CSceneData);
public:
    CSceneData();
    virtual ~CSceneData();
    virtual bool init(){return true;};
    
    CC_SYNTHESIZE(int, m_nSceneId, SceneId);
    CC_SYNTHESIZE(string, m_sSceneName, SceneName);
    CC_SYNTHESIZE(string, m_sScenePic, ScenePic);
    CC_SYNTHESIZE(string, m_sSceneMusic, SceneMusic);
    CC_SYNTHESIZE(string, m_sSceneSoudeffects, SceneSoundEffect);
    CC_SYNTHESIZE(string, m_sButtonSoundEffects, ButtonSoundEffects);
    CC_SYNTHESIZE(string, m_sSceneEffect, SceneEffect);
};


class CSceneDataConfigData : public CCObject
{
public:
    CSceneDataConfigData();
    virtual ~CSceneDataConfigData();
    
    CSceneData *getSceneDataById(int inId);
    
protected:
    int m_nCurrentSceneId;
    CSceneData *m_pCurrentSceneData;
};

typedef Singleton<CSceneDataConfigData> SingleSceneDataConfig;


#endif /* defined(___1_cube__CSceneConfigData__) */
