//
//  CPtSectionConfigData.cpp
//  91.cube
//
//  Created by phileas on 13-9-4.
//
//

#include "CPtSectionConfigData.h"
#include "gameTools.h"
#include "gameConfig.h"

//implement class of CPtSection
CPtSection* CPtSection::create()
{
    CPtSection *section = new CPtSection();
    section->autorelease();
    return section;
}


// implement class of CPtSectionConfigData

CPtSectionConfigData* CPtSectionConfigData::create(const char *fileName)
{
    CCDictionary * tmp = NULL;
    tmp = CCDictionary::createWithContentsOfFile(fileName);
    CPtSectionConfigData * data = new CPtSectionConfigData(tmp);
    data->autorelease();
    return data;

}
CPtSectionConfigData* CPtSectionConfigData::create(int inChapterId)
{

    inChapterId = (inChapterId -BASEVALUE )/100+1;
    char buff[150]={0};
    sprintf(buff, "%s_%d.plist", CSTR_FILEPTAH(g_chapterPath, "partlist"), inChapterId);
    return create(buff);
}

CPtSectionConfigData* CPtSectionConfigData::createActivitySectionConfigData()
{
    return create( CSTR_FILEPTAH(g_chapterPath, activityfile.c_str()));
}

CPtSectionConfigData::CPtSectionConfigData(CCDictionary* inSectionDictionary)
{
    m_pIdToSequence = CCDictionary::create();
    m_pSectionsInChapter = CCDictionary::create();
    m_pSectionsInChapter->retain();
    m_pIdToSequence->retain();
    loadSectionDataByChapter(inSectionDictionary);
}

CPtSectionConfigData::~CPtSectionConfigData()
{
    CC_SAFE_RELEASE(m_pIdToSequence);
    CC_SAFE_RELEASE(m_pSectionsInChapter);
}

void CPtSectionConfigData::loadSectionDataByChapter(cocos2d::CCDictionary *inSectionDictionary)
{
    CCDictElement *element = NULL;
    CCDictionary *tmpValue = NULL;
    CPtSection *tmpSection;
    CCDICT_FOREACH(inSectionDictionary, element)
    {
        tmpValue = (CCDictionary*) element->getObject();
        if (tmpValue)
        {
            tmpSection = CPtSection::create();
            
            tmpSection->setSectionId(GameTools::intForKey("part_id", tmpValue));
            tmpSection->setSectionName(GameTools::valueForKey("part_name", tmpValue));
            tmpSection->setChapterId(GameTools::intForKey("chapter_id", tmpValue));
            tmpSection->setSequence(GameTools::intForKey("order_by", tmpValue));
            tmpSection->setSectionPicName(GameTools::valueForKey("part_pic", tmpValue));
            tmpSection->setTipId(GameTools::valueForKey("part_tips", tmpValue));
            tmpSection->setSceneId(GameTools::intForKey("scene_id", tmpValue));
            tmpSection->setMaxStep(GameTools::intForKey("max_step", tmpValue));
            tmpSection->setAP(GameTools::intForKey("ap", tmpValue));
            tmpSection->setBounsExp(GameTools::intForKey("bouns_exp", tmpValue));
            tmpSection->setBounsMoney(GameTools::intForKey("bouns_money", tmpValue));
            tmpSection->setBounsRmb(GameTools::intForKey("bouns_rmb", tmpValue));
            tmpSection->setEndBounsExp(GameTools::intForKey("endbouns_exp", tmpValue));
            tmpSection->setEndBounsMoney(GameTools::intForKey("endbouns_money",tmpValue));
            tmpSection->setTaskId(GameTools::intForKey("mission_id", tmpValue));
            tmpSection->setRandomEventId(GameTools::intForKey("randomevent_id", tmpValue));
            
            m_pIdToSequence->setObject(CCInteger::create(tmpSection->getSequence()),tmpSection->getSectionId());
            m_pSectionsInChapter->setObject(tmpSection, tmpSection->getSequence());
            
            CCLog("the size : %d", m_pSectionsInChapter->count());
            
        }
    }
}

CPtSection * CPtSectionConfigData::getSectionById(int inSectionId)
{
    CCInteger * integer = ((CCInteger*) m_pIdToSequence->objectForKey(inSectionId));
    CCAssert(integer != NULL, "no this section id");
    int sequence = integer->getValue();
    
    return (CPtSection *) m_pSectionsInChapter->objectForKey(sequence);
    
}

/*
 * @breif: 
 * @param: from 0
 */
CPtSection * CPtSectionConfigData::getSectionByIndex(int inSectionIdex)
{
    inSectionIdex++;
    return (CPtSection *) m_pSectionsInChapter->objectForKey(inSectionIdex);
}
/*
 * @breif: 返回该ID之前的小节数组
 * @param inSectionId : 当前小节的ID
 * @return 数组是临时变量，create的
 */

CCArray* CPtSectionConfigData::getSectionsBeforeId(int inSectionId)
{
    CCArray * array = CCArray::create();
    CPtSection * section = NULL;
    
    CCInteger * integer = ((CCInteger*) m_pIdToSequence->objectForKey(inSectionId));
    CCAssert(integer != NULL, "no this section id");
    int sequence = integer->getValue();
    
    {
        for (int i = 1; i <= sequence; i++)
        {
            section = (CPtSection *) m_pSectionsInChapter->objectForKey(i);
            array->addObject(section);
        }
    }
    return array;
}

/*
 * @breif: 返回该章的小节数组
 * @return 数组是临时变量，create的
 */

CCArray * CPtSectionConfigData::getSectionByOrder()
{
    CCArray * array = CCArray::create();
    CPtSection * section = NULL;
    for (int i = 1; i <= m_pSectionsInChapter->count(); i++)
    {
        section = (CPtSection *) m_pSectionsInChapter->objectForKey(i);
        array->addObject(section);
    }
    return array;
}

CPtSectionManager::CPtSectionManager()
{
    m_pSections = CCDictionary::create();
    m_pSections->retain();
}

CPtSectionManager::~CPtSectionManager()
{
    CC_SAFE_RELEASE(m_pSections);
}

CPtSectionConfigData * CPtSectionManager::getSectionByChapterId(int inChapterId)
{
    CPtSectionConfigData * tmp = (CPtSectionConfigData*) m_pSections->objectForKey(inChapterId);
    if (tmp == NULL)
    {
        CCLog("inChapterId:%d",inChapterId);
        if (inChapterId >= ACTIVITYCHAPTERID)
        {
            tmp = CPtSectionConfigData::createActivitySectionConfigData();
        }else
        {
            tmp = CPtSectionConfigData::create(inChapterId);
        }
 
         if (tmp)
         {
             m_pSections->setObject(tmp, inChapterId);
         }
    }

    return tmp;
}

CPtSectionConfigData *CPtSectionManager::getActivitySections()
{
    CPtSectionConfigData * tmp = (CPtSectionConfigData*) m_pSections->objectForKey(ACTIVITYCHAPTERID);
    if (tmp == NULL)
    {
        tmp = CPtSectionConfigData::createActivitySectionConfigData();
        if (tmp)
        {
            m_pSections->setObject(tmp, ACTIVITYCHAPTERID);
        }
    }
    
    return tmp;
}

// implement class of CPtChapter

CPtChapter* CPtChapter::create()
{
    CPtChapter *chapter = new CPtChapter();
    if (chapter)
    {
        chapter->autorelease();
    }
    return chapter;
}

CPtChapter::CPtChapter()
{
    m_pSectionData = NULL;
    m_pReword = CCDictionary::create();
    m_pReword->retain();
}

CPtChapter::~CPtChapter()
{
    CC_SAFE_RELEASE(m_pReword);
}

void CPtChapter::setReword(int inRewordId, int number)
{
    if (inRewordId == INVALID || number == 0 )
    {
        return;
    }
    m_pReword->setObject(CCInteger::create(number), inRewordId);
}

void CPtChapter::setRewordCard(int inCardId, int number)
{
    if (inCardId == INVALID || number == 0 )
    {
        return;
    }
    m_cRewordCard.itemId = inCardId;
    m_cRewordCard.number = number;
    
}
CPtSectionConfigData * CPtChapter::getSections()
{
    if (m_pSectionData == NULL)
    {
        m_pSectionData = SingleSectionManager::instance()->getSectionByChapterId(m_nChapterId);
    }
    
    return m_pSectionData;
}
// implement class of CPtChapterConfigData

CPtChapterConfigData::CPtChapterConfigData()
{
    m_pChapters = CCDictionary::create();
    m_pChapters->retain();
    loadChapterConifg();
}

CPtChapterConfigData::~CPtChapterConfigData()
{
    CC_SAFE_RELEASE(m_pChapters);
}

/*
 * @param inMaxId start from 300000, 间隔 100 
 */

CCArray * CPtChapterConfigData::getOpenChapter(int inMaxId)
{
    if(inMaxId == ACTIVITYCHAPTERID)
    {
        return NULL;
    }
    
    if ((inMaxId-BASEVALUE)/INTERVALVALUE + 1 > m_pChapters->count())
    {
        return NULL;
    }else
    {
        CCArray *array = CCArray::create();
        CPtChapter * chapter = NULL;
        for (int i = BASEVALUE; i <= inMaxId ; i+=INTERVALVALUE)
        {
            chapter = (CPtChapter*) m_pChapters->objectForKey(i);
            if (chapter)
            {
                array->addObject(chapter);
            }
         
        }
        return array;
    }

}

CPtChapter * CPtChapterConfigData::getAcitivityChapter()
{
    return getChapterById(ACTIVITYCHAPTERID);
}

CPtChapter * CPtChapterConfigData::getChapterById(int inChapterId)
{
    
    CPtChapter * chapter= (CPtChapter*) m_pChapters->objectForKey(inChapterId);

    return chapter;
}

void CPtChapterConfigData::loadChapterConifg()
{
    CCDictionary * tmp = CCDictionary::createWithContentsOfFile(CSTR_FILEPTAH(g_chapterPath, "chapterlist.plist"));
    CCDictElement *element = NULL;
    CCDictionary *elementValue = NULL;
    CPtChapter *chapter = NULL;
    CCDICT_FOREACH(tmp, element)
    {
        elementValue = (CCDictionary*) element->getObject();
        chapter = CPtChapter::create();
        int chapterId = GameTools::intForKey("chapter_id", elementValue);
        chapter->setChapterId(chapterId);
        chapter->setChapterName(GameTools::valueForKey("chapter_name", elementValue));
        chapter->setChapterPicName(GameTools::valueForKey("chapter_pic", elementValue));
        chapter->setChapterTipid(GameTools::valueForKey("chapter_tips", elementValue));

        chapter->setRewordCard(GameTools::intForKey("reword_card_item_id", elementValue), GameTools::intForKey("number", elementValue));
        chapter->setReword(GameTools::intForKey("reword_item_id_1", elementValue), GameTools::intForKey("number", elementValue));
        chapter->setReword(GameTools::intForKey("reword_item_id_2", elementValue), GameTools::intForKey("number", elementValue));
        chapter->setReword(GameTools::intForKey("reword_item_id_3", elementValue), GameTools::intForKey("number", elementValue));
        m_pChapters->setObject(chapter, chapterId);
    }
}