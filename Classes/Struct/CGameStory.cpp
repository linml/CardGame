//
//  CGameStory.cpp
//  91.cube
//
//  Created by linminglu on 13-9-3.
//
//

#include "CGameStory.h"
#include "gameConfig.h"
CGameStory::CGameStory()
{
    
}

CGameStory::~CGameStory()
{
    m_mapStory.erase(m_mapStory.begin(),m_mapStory.end());
}

void CGameStory::initGameStoryData()
{
    CCDictionary *directory = CCDictionary::createWithContentsOfFile((resRootPath+"story.plist").c_str());
    CCArray *vKeyArray=directory->allKeys();
    if(vKeyArray)
    {
        for (int i=0; i<vKeyArray->count(); i++) {
            CCString *key=(CCString *)vKeyArray->objectAtIndex(i);
            CCDictionary *story=(CCDictionary*)(directory->objectForKey(key->m_sString));
            int storyId=GameTools::intForKey("story_id", story);
            this->m_mapStory[storyId]=GameTools::valueForKey("dialoguelist_id", story);
        }
    }
}

string CGameStory::getDialogTalkId(int index)
{
    return m_mapStory[index];
}