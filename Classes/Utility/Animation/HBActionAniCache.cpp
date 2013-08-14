#include "HBActionAniCache.h"
#include "ccMacros.h"
#include "GameTools.h"
#include "HBActionScriptCache.h"

//using namespace GameTools;


HBActionAniCache* HBActionAniCache::s_pSharedActionAniCache = NULL;

HBActionAniCache* HBActionAniCache::sharedActionAniCache(void)
{
	if (! s_pSharedActionAniCache)
	{
		s_pSharedActionAniCache = new HBActionAniCache();
		s_pSharedActionAniCache->init();
	}

	return s_pSharedActionAniCache;
}

void HBActionAniCache::purgeSharedActionAniCache(void)
{
	CC_SAFE_RELEASE_NULL(s_pSharedActionAniCache);
}

bool HBActionAniCache::init()
{
	m_pAnimations = new CCDictionary();
	return true;
}

void HBActionAniCache::addActionAni(HBActionAni *ani, const char * name)
{
	m_pAnimations->setObject(ani, std::string(name));
}


void HBActionAniCache::removeActionAniByName(const char* name)
{
	if (!name)
	{
		return;
	}

	m_pAnimations->removeObjectForKey(std::string(name));
}


HBActionAni* HBActionAniCache::getActionAniByName(const char* name)
{
	return (HBActionAni*)m_pAnimations->objectForKey(std::string(name));
}



HBActionAni* HBActionAniCache::addActionAniWithFile(const char* name)
{
	//判断是否存在该动画，有就返回
	std::string stdName = name;
	std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(name);
	//CCLOG("HBActionAniCache::addActionAniWithFile path is %s", path.c_str());
	if (path.empty())
	{
		return NULL;
	}

	int pos0 = path.find_last_of("/");
	int pos1 = path.find_last_of("\\");
	if (pos1<pos0)
	{
		pos1 = pos0;
	}
	int pos2 = path.find_last_of(".");
	stdName = path.substr(pos1+1,pos2-pos1-1);
	HBActionAni* ani = (HBActionAni*)m_pAnimations->objectForKey(stdName);
	if (ani) 
		return ani;

	//判断文件是否存在
	CCFileUtils::sharedFileUtils()->setPopupNotify(false);
	unsigned long size;
	unsigned char* bCheckFile = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(),"r",&size);
    //CCLog("xianbei bCheckFile %s",path.c_str());
	if (!bCheckFile){
		CCLOG("load error %s", path.c_str());
        CC_SAFE_DELETE_ARRAY(bCheckFile);
		return NULL;
	}
    CC_SAFE_DELETE_ARRAY(bCheckFile);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCMutableDictionary<std::string, CCObject*>* dict;
    if (path.find("animationScript/")!=string::npos) {
        CCLog("xianbei animation use json %s",name);
       dict =HBDataBase::JsonParse(path.c_str());
    }
    else
    {
        CCFileUtils::setIsPopupNotify(true);
        dict = CCFileUtils::dictionaryWithContentsOfFile(path.c_str());
    }
#else
    CCFileUtils::sharedFileUtils()->setPopupNotify(true);
    CCDictionary* dict = CCDictionary::createWithContentsOfFile(path.c_str());
    CCLog("HBActionAniCache::addActionAniWithFile %s",path.c_str());
#endif
//	CCLOG("HBActionAniCache::addActionAniWithFile ok");
	return addActionAniWithDictionary(dict,stdName.c_str());
}

HBActionAni* HBActionAniCache::addActionAniWithDictionary(CCDictionary* dict,const char* name)
{
	//CCLOG("HBActionAniCache::addActionAniWithDictionary 1");
	CCArray* actionScripts = CCArray::create();
	float duration = 0;
    
	CCDictionary* actDict = NULL;

	//动作相关参数
	bool bolResetData = true;
	actDict = (CCDictionary*)dict->objectForKey("param");
	if (actDict)
	{
		//参数里面放置该动作的时间等
		duration = GameTools::floatForKey("time",actDict);
		if(actDict->objectForKey("bolResetData")) bolResetData = GameTools::boolForKey("bolResetData",actDict);
	}
	
	CCArray* keys = dict->allKeys();
    
    for (int i=0; i<keys->count(); i++)
    {
        CCString* actDictKey = (CCString*)keys->objectAtIndex(i);
       // CCLog("xianbei actDictKey %s",actDictKey->m_sString.c_str());
        if(actDictKey->m_sString != "param")
        {
            actDict = (CCDictionary*)dict->objectForKey(actDictKey->m_sString);
            //			CCLOG("HBActionAniCache::addActionAniWithDictionary 123");
			HBActionScript* script = new HBActionScript(actDict,bolResetData);
            //			CCLOG("HBActionAniCache::addActionAniWithDictionary 1234");
			string path = string(name);//IBM EDIT
			path.append("_");
			path.append(actDictKey->m_sString);
            //
            CCLOG("HBActionAniCache::addActionAniWithDictionary path = %s,%s",path.c_str(),actDictKey->m_sString.c_str());
			HBActionScriptCache::sharedActionScriptCache()->addActionScript(script, path.c_str());
//            actionScripts->addObject(CCString::create("xianbei"));
            if(actionScripts)
            {
                actionScripts->addObject(script);
            }
        }
    }
   	HBActionAni* ani = new HBActionAni(name,actionScripts,duration);
	m_pAnimations->setObject(ani,std::string(name));

    //bq modify
//    actionScripts->release();
//    CCLOG("HBActionAniCache::addActionAniWithDictionary 2");
	return ani;
}




void HBActionAniCache::removeAll()
{
	m_pAnimations->removeAllObjects();
}

HBActionAniCache::HBActionAniCache()
: m_pAnimations(NULL)
{
    
}


HBActionAniCache::~HBActionAniCache()
{
//	CC_SAFE_RELEASE(m_pAnimations);
    if(m_pAnimations)
        m_pAnimations->release();
}

