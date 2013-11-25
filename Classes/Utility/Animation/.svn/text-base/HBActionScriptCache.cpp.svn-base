#include "HBActionScriptCache.h"
#include "ccMacros.h"
#include "GameTools.h"

using namespace GameTools;


HBActionScriptCache* HBActionScriptCache::s_pSharedActionScriptCache = NULL;

HBActionScriptCache* HBActionScriptCache::sharedActionScriptCache(void)
{
	if (! s_pSharedActionScriptCache)
	{
		s_pSharedActionScriptCache = new HBActionScriptCache();
		s_pSharedActionScriptCache->init();
	}

	return s_pSharedActionScriptCache;
}

void HBActionScriptCache::purgeSharedActionScriptCache(void)
{
	CC_SAFE_RELEASE_NULL(s_pSharedActionScriptCache);
}

bool HBActionScriptCache::init()
{
	m_pScripts = new CCDictionary();
	return true;
}

void HBActionScriptCache::addActionScript(HBActionScript *script, const char * name)
{
	m_pScripts->setObject(script,string(name));
}


void HBActionScriptCache::removeActionScriptByName(const char* name)
{
	m_pScripts->removeObjectForKey(string(name));
}


HBActionScript* HBActionScriptCache::getActionScriptByName(const char* name)
{
	return (HBActionScript*)m_pScripts->objectForKey(string(name));
}

int HBActionScriptCache::getActionScriptTagByName(const char* name)
{
	HBActionScript* script = (HBActionScript*)m_pScripts->objectForKey(string(name));
	if (script)
	{
		return script->tag;
	}
	return 0;
}

HBActionScriptCache::HBActionScriptCache()
: m_pScripts(NULL)
{
}

HBActionScriptCache::~HBActionScriptCache()
{
	CC_SAFE_RELEASE(m_pScripts);
}

