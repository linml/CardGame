/************************************************************************/
/* 动画缓存类                                                           */
/************************************************************************/

#pragma once

#include "CCObject.h"
#include "HBActionAni.h"

#include <string>

using namespace cocos2d;

class HBActionAniCache : public CCObject
{
public:
	~HBActionAniCache();
	HBActionAniCache();

	static HBActionAniCache* sharedActionAniCache(void);

	static void purgeSharedActionAniCache(void);

	void addActionAni(HBActionAni *ani, const char * name);

	void removeActionAniByName(const char* name);

	HBActionAni* getActionAniByName(const char* name);

	bool init(void);

	void removeAll();

	HBActionAni* addActionAniWithDictionary(CCDictionary* dict,const char* name);
	//从配置文件中添加
	HBActionAni* addActionAniWithFile(const char* name);

private:
	CCDictionary* m_pAnimations;
	static HBActionAniCache *s_pSharedActionAniCache;
};
