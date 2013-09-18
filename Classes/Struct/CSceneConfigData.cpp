//
//  CSceneConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-9-11.
//
//

#include "CSceneConfigData.h"
#include "gameConfig.h"
// implement class of CSceneData


CSceneData::CSceneData()
{
    m_nSceneId = -1;
}
CSceneData:: ~CSceneData()
{
    
}

// implement class of CSceneConfigData

CSceneDataConfigData::CSceneDataConfigData()
{
    m_nCurrentSceneId = -1;
    m_pCurrentSceneData = NULL;
}
CSceneDataConfigData::~CSceneDataConfigData()
{
    CC_SAFE_RELEASE(m_pCurrentSceneData);
}

CSceneData * CSceneDataConfigData::getSceneDataById(int inId)
{
    if (inId < 0)
    {
        return NULL;
    }else
    {
        if (m_nCurrentSceneId != inId)
        {
            CCDictionary *tmp = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(resRootPath, "scean.plist"));
            /*
             *
             <key>scene_id</key>
             <string>11001</string>
             <key>name</key>
             <string>话哈u</string>
             <key>scene_pic</key>
             <string>c.png</string>
             <key>scene_music</key>
             <string>m.wav</string>
             <key>scene_soundeffects</key>
             <string>m.wav</string>
             <key>button_soundeffects</key>
             <string>m.wav</string>
             <key>scene_effect</key>
             <string>aa.plist</string>
             */
           
            CCAssert(tmp, "the scean.plist can't be load ");
            char buff[20] = {0};
            sprintf(buff, "%d", inId);
            tmp = (CCDictionary*)tmp->objectForKey(buff);
            
            CCAssert(tmp, "the id not in the scean.plist");
            
            CSceneData *sceneData = CSceneData::create();
            sceneData->setSceneId(GameTools::intForKey("scene_id", tmp));
            sceneData->setSceneName(GameTools::valueForKey("name", tmp));
            sceneData->setScenePic(GameTools::valueForKey("scene_pic", tmp));
            sceneData->setSceneMusic(GameTools::valueForKey("scene_music", tmp));
            sceneData->setSceneSoundEffect(GameTools::valueForKey("scene_soundeffects", tmp));
            sceneData->setButtonSoundEffects(GameTools::valueForKey("button_soundeffects", tmp));
            sceneData->setSceneEffect(GameTools::valueForKey("scene_effect", tmp));
            sceneData->retain();
            CC_SAFE_RELEASE(m_pCurrentSceneData);
            m_pCurrentSceneData = sceneData;

        }
    }
    return m_pCurrentSceneData;
}