#pragma once
#include "HBActionScript.h"

class HBActionAni :
	public CCObject
{
public:
	HBActionAni(const char *name, CCArray* pScritps,float time);
	HBActionAni(HBActionScript* script,float time);
	~HBActionAni(void);


	inline const char* getName(void) { return nameStr.c_str(); }

	inline void setName(const char *pszName){ nameStr = pszName; }

	inline float getDuration(void) { return duration; }

	inline void setDuration(float time){ duration = time; }

	inline CCArray* getScripts(void) { return scripts; }

	inline void setScripts(CCArray* pScripts)
	{
		CC_SAFE_RETAIN(pScripts);
		CC_SAFE_RELEASE(scripts);
		scripts = pScripts;
	}

	void runAnimationBy(CCNode* node);

protected:
	std::string nameStr;

	float duration;

	CCArray* scripts;
};
