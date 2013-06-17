/********************************************************************
 created:    2011/12/14
 filename:   WordCache.h 
 author:     bqlf
 purpose:    文字文件管理，所有加载在内存中的sprite文件都保存在这个类中
 *********************************************************************/
#include "WordCache.h"

WordCache* WordCache::s_pSharedWordCache = NULL;

WordCache::WordCache()
: m_pWord(NULL)
{
}

WordCache::~WordCache()
{
//	if(s_pSharedWordCache) {
//		delete s_pSharedWordCache;
		CC_SAFE_RELEASE(m_pWord);
//	}
}

WordCache* WordCache::sharedWordCache(void)
{
	if (! s_pSharedWordCache)
	{
		s_pSharedWordCache = new WordCache();
		s_pSharedWordCache->init();
	}

	return s_pSharedWordCache;
}
void WordCache::purgesharedWordCache(void)
{
    CC_SAFE_RELEASE_NULL(s_pSharedWordCache);
}

bool WordCache::init()
{
	m_pWord = new CCDictionary();
	return true;
}

void WordCache::addWord(CCDictionary* obj, const char * fileName)
{
	m_pWord->setObject(obj, std::string(fileName));
}

void WordCache::removeWordByName(const char* fileName)
{
	if (! fileName)
	{
		return;
	}

	m_pWord->removeObjectForKey(std::string(fileName));
}

CCDictionary* WordCache::WordByName(const char* fileName)
{
	return (CCDictionary*)m_pWord->objectForKey(std::string(fileName));
}