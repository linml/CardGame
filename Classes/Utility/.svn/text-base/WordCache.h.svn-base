/********************************************************************
created:    2011/12/14
filename:   WordCache.h
author:     bqlf 
purpose:    文字文件管理，所有加载在内存中的sprite文件都保存在这个类中
*********************************************************************/
#ifndef WORDCACHE_H
#define WORDCACHE_H
#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class WordCache : CCObject
{
public:
	WordCache();
	~WordCache();

	static WordCache* sharedWordCache(void);
    static void purgesharedWordCache(void);
	void addWord(CCDictionary*, const char * fileName);
	void removeWordByName(const char* fileName);
	CCDictionary* WordByName(const char* fileName);
	bool init();
private:
	CCDictionary* m_pWord;
	static WordCache *s_pSharedWordCache;
};
#endif // WORDCACHE_H
