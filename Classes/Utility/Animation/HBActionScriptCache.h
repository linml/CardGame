/************************************************************************/
/*
动画
*/
/************************************************************************/


#pragma once

#include "CCObject.h"
#include "HBActionScript.h"

#include <string>

using namespace cocos2d;

class HBActionScriptCache : public CCObject
{
public:
	~HBActionScriptCache();
	HBActionScriptCache();

	static HBActionScriptCache* sharedActionScriptCache(void);

	static void purgeSharedActionScriptCache(void);

	void addActionScript(HBActionScript *script, const char * name);


	void removeActionScriptByName(const char* name);


	HBActionScript* getActionScriptByName(const char* name);

	int getActionScriptTagByName(const char* name);

	bool init(void);

private:
	CCDictionary* m_pScripts;
	static HBActionScriptCache *s_pSharedActionScriptCache;
};

