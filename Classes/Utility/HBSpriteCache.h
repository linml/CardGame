/********************************************************************
created:    2011/08/18
filename:   HBSpriteCache.h
author:     bqlf
purpose:    sprite文件管理，所有加载在内存中的sprite文件都保存在这个类中
*********************************************************************/
#ifndef HBSPRITECACHE_H
#define HBSPRITECACHE_H
#pragma once

//#include "CCMutableDictionary.h"
#include "HBSprite.h"
#include "cocos2d.h"

using namespace cocos2d;

class HBSpriteCache : CCObject
{
public:
	HBSpriteCache();
	~HBSpriteCache();

	static HBSpriteCache* sharedHBSpriteCache(void);
    static void purgesharedHBSpriteCache(void);
	void addHBSprite(HBSprite *sprite, const char * fileName);
	void removeHBSpriteByName(const char* fileName);
	HBSprite* HBSpriteByName(const char* fileName);
	bool init();
private:
	CCDictionary* m_pHBSprites;
	static HBSpriteCache *s_pSharedHBSpriteCache;
};
#endif // HBSPRITECACHE_H
