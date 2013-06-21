#include "HBActionAni.h"

HBActionAni::HBActionAni(const char *name, CCArray* pScritps,float time)
{
    CCLog("xianbei %d",pScritps->count());
	nameStr = name;
	duration = time;
    scripts = CCArray::create();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pScritps, pObj)
    {
        scripts->addObject(pObj);
    }
    //bq modify
	scripts->retain();
    autorelease();
}

HBActionAni::HBActionAni(HBActionScript* script,float time)
{
	duration = time;
	scripts = CCArray::create();
	scripts->addObject(script);
    //bq modify
//    script->release();
	scripts->retain();
    autorelease();
}


HBActionAni::~HBActionAni(void)
{
	nameStr.clear();
	CC_SAFE_RELEASE_NULL(scripts);
}

void HBActionAni::runAnimationBy(CCNode* node)
{
	for (int i= 0;i<scripts->count();i++)
	{
		HBActionScript* as = (HBActionScript*)scripts->objectAtIndex(i);
		if (as)
		{
			as->runScriptBy(node);
		}else
		{
//			CCLOG("script error:%s",nameStr);
		}
		
	}
}
