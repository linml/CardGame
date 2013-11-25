/********************************************************************
created:    2011/08/18
filename:   HBSpriteCache.cpp
author:     bqlf 
purpose:    sprite文件管理，所有加载在内存中的sprite文件都保存在这个类中
*********************************************************************/
#include "HBSpriteCache.h"

HBSpriteCache* HBSpriteCache::s_pSharedHBSpriteCache = NULL;

HBSpriteCache::HBSpriteCache()
: m_pHBSprites(NULL)
{
}

HBSpriteCache::~HBSpriteCache()
{
    CC_SAFE_RELEASE(m_pHBSprites);
}

void HBSpriteCache::purgesharedHBSpriteCache(void)
{
    CC_SAFE_RELEASE_NULL(s_pSharedHBSpriteCache);
}

HBSpriteCache* HBSpriteCache::sharedHBSpriteCache(void)
{
	if (! s_pSharedHBSpriteCache)
	{
		s_pSharedHBSpriteCache = new HBSpriteCache();
		s_pSharedHBSpriteCache->init();
	}

	return s_pSharedHBSpriteCache;
}

bool HBSpriteCache::init()
{
	m_pHBSprites = new CCDictionary();
	return true;
}

void HBSpriteCache::addHBSprite(HBSprite *sprite, const char * fileName)
{
	m_pHBSprites->setObject(sprite, std::string(fileName));
}

void HBSpriteCache::removeHBSpriteByName(const char* fileName)
{
	if (! fileName)
	{
		return;
	}

	m_pHBSprites->removeObjectForKey(std::string(fileName));
}

HBSprite* HBSpriteCache::HBSpriteByName(const char* fileName)
{
	return (HBSprite*)m_pHBSprites->objectForKey(std::string(fileName));
}