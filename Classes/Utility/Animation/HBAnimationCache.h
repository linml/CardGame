#ifndef __HB_ANIMATION_CACHE_H__
#define __HB_ANIMATION_CACHE_H__

#include "CCObject.h"
#include "CCAnimation.h"

#include <string>

using namespace cocos2d;

class HBAnimationCache : public CCObject
{
public:
	~HBAnimationCache();
	HBAnimationCache();

	/** Retruns ths shared instance of the Animation cache */
	static HBAnimationCache* sharedAnimationCache(void);

	/** Purges the cache. It releases all the CCAnimation objects and the shared instance.
	*/
	static void purgeSharedAnimationCache(void);

	/** Adds a CCAnimation with a name.
	*/
	void addAnimation(CCAnimation *animation, const char * name);

	/** Deletes a CCAnimation from the cache.
	*/
	void removeAnimationByName(const char* name);

	/** Returns a CCAnimation that was previously added.
	If the name is not found it will return nil.
	You should retain the returned copy if you are going to use it.
	*/
	CCAnimation* animationByName(const char* name);

	bool init(void);


	//add
	void addAnimation(CCAnimation* ani);

	//帧大小一样的动画添加
	CCAnimation* addAnimationWithFile(const char* name,int w,int h, char** aniNameCallBack = NULL);

	CCAnimation* addAnimationWithDictionary(CCDictionary* dict,const char* name);
	//从配置文件中添加动画
	CCAnimation* addAnimationWithFile(const char* name, char** aniNameCallBack = NULL);
	//删除所有动画
	void removeAnimations();
	//删除不用的动画
	void removeUnusedAnimations();

	//获取反向动画，如果找不到建立一个
	CCAnimation* reverseAnimationByName(const char* name);

	CCAnimation* reverseAnimationByAni(CCAnimation* ani,const char* name = NULL);

    //add bq
    //强制删除动画
    void setHBAnimationIsSingle();
    //打印出保存在Cash中的动画信息
    void dumpHBAnimationCachedInfo();
    
private:
	CCDictionary* m_pAnimations;
	static HBAnimationCache *s_pSharedAnimationCache;
	float animationDelay;
};


#endif // __HB_ANIMATION_CACHE_H__