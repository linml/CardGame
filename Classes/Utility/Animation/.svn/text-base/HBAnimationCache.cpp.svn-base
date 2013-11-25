#include "HBAnimationCache.h"
#include "ccMacros.h"
#include "GameTools.h"
#include "gameConfig.h"

using namespace GameTools;


HBAnimationCache* HBAnimationCache::s_pSharedAnimationCache = NULL;

HBAnimationCache* HBAnimationCache::sharedAnimationCache(void)
{
	if (! s_pSharedAnimationCache)
	{
		s_pSharedAnimationCache = new HBAnimationCache();
		s_pSharedAnimationCache->init();
	}

	return s_pSharedAnimationCache;
}

void HBAnimationCache::purgeSharedAnimationCache(void)
{
	CC_SAFE_RELEASE_NULL(s_pSharedAnimationCache);
}

bool HBAnimationCache::init()
{
	m_pAnimations = new CCDictionary();
	animationDelay = 0.1f;
	return true;
}

HBAnimationCache::HBAnimationCache()
: m_pAnimations(NULL)
{
}

HBAnimationCache::~HBAnimationCache()
{
	CC_SAFE_RELEASE(m_pAnimations);
}

void HBAnimationCache::addAnimation(CCAnimation *animation, const char * name)
{
	m_pAnimations->setObject(animation, std::string(name));
}

void HBAnimationCache::removeAnimationByName(const char* name)
{
	if (! name)
	{
		return;
	}

	m_pAnimations->removeObjectForKey(std::string(name));
}

CCAnimation* HBAnimationCache::animationByName(const char* name)
{
	return (CCAnimation*)m_pAnimations->objectForKey(std::string(name));
}


void HBAnimationCache::addAnimation(CCAnimation* ani)
{
    assert("HBAnimationCache::addAnimation function no match");
//	m_pAnimations->setObject(ani, std::string(ani->getName()));
}

CCAnimation* HBAnimationCache::addAnimationWithFile(const char* name,int w,int h, char** aniNameCallBack)
{

	CCArray* Frames = new CCArray();

	std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(CSTR_FILEPTAH(g_ActionFilePath, name));
	int pos0 = path.find_last_of("/");
	int pos1 = path.find_last_of("\\");
	if (pos1<pos0)
	{
		pos1 = pos0;
	}
	int pos2 = path.find_last_of(".");
	std::string aniName = path.substr(pos1+1,pos2-pos1-1);
    sprintf(*aniNameCallBack,"%s", aniName.c_str());
	//判断是否存在该动画，如果有返回该动画，没有新建动画
	CCAnimation* ani = animationByName(aniName.c_str());
	if (ani)
	{
		return ani;
	}

	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(path.c_str());

	//	if (texture.contentSize.width%w==0&&texture.contentSize.height%h==0) {
	//		
	//	}

	int row = (int)(texture->getContentSizeInPixels().height/h);
	int col = (int)(texture->getContentSizeInPixels().width/w);

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			CCSpriteFrame* spriteFrame;
			spriteFrame = CCSpriteFrame::createWithTexture(texture,CCRectMake((float)j*w, (float)i*h, (float)w, (float)h), false, CCPointZero, CCSizeMake((float)w, (float)h));
			// add sprite frame
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(spriteFrame,(aniName+GameTools::ConvertToString(j+i*col)).c_str());
			Frames->addObject(spriteFrame);
		}
	}
    
	ani = CCAnimation::createWithSpriteFrames(Frames, animationDelay);
//	ani->setName(aniName.c_str());	
	m_pAnimations->setObject(ani,aniName);
    //bq modify
    Frames->release();
	return ani;
}

CCAnimation* HBAnimationCache::addAnimationWithDictionary(CCDictionary* dict,const char* name)
{
	CCDictionary* metadataDict = (CCDictionary*) dict->objectForKey("metadata");
	CCDictionary* framesDict = (CCDictionary*) dict->objectForKey("frames");
	int format = 0;

	// get the format
	if(metadataDict != NULL) {
		format = intForKey("format",metadataDict);
	}

	// check the format
	if(format < 0 || format > 1) {
		//NSAssert(NO,@"cocos2d: WARNING: format is not supported for CCSpriteFrameCache addSpriteFramesWithDictionary:texture:");
		return NULL;
	}


	//get texture from dict
	bool isShareTexture = true;
	std::string texturePath;
	CCTexture2D *texture;
	if(metadataDict) {
		texturePath = valueForKey("texturePath",metadataDict);
	}
	if(texturePath.empty()) {
		//texturePath = [aniName stringByAppendingPathExtension:@"png"];
		isShareTexture = false;
	}else {
        texturePath = g_ActionFilePath + texturePath;
		texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(texturePath.c_str());
		texture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());
	}


	//	if (!texture) {
	//		NSAssert(NO,@"cocos2d: WARNING: texture is nil");
	//		return;
	//	}

	CCArray* Frames = new CCArray();
	int count = 0;

    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(framesDict, pElement)
    {
        CCDictionary* frameDict = (CCDictionary*)pElement->getObject();
        CCSpriteFrame* spriteFrame;
		if (!isShareTexture) {
			texturePath = GameTools::valueForKey("texturePath",frameDict);
            texturePath = g_ActionFilePath + texturePath;
			texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(texturePath.c_str());
			texture = CCTextureCache::sharedTextureCache()->addImage(texturePath.c_str());
		}
		count++;
		if(format == 0) {
			float x = 0;
			float y = 0;
			float w = texture->getContentSize().width;
			float h = texture->getContentSize().height;
			float ox = 0;
			float oy = 0;
			int ow = w;
			int oh = h;
            
			if (frameDict->objectForKey("x")) x = floatForKey("x",frameDict);
			if (frameDict->objectForKey("y")) y = floatForKey("y",frameDict);
			if (frameDict->objectForKey("width")) w = floatForKey("width",frameDict);
			if (frameDict->objectForKey("height")) h = floatForKey("height",frameDict);
			if (frameDict->objectForKey("offsetX")) ox = floatForKey("offsetX",frameDict);
			if (frameDict->objectForKey("offsetY")) oy = floatForKey("offsetY",frameDict);
			if (frameDict->objectForKey("originalWidth")) ow = intForKey("originalWidth",frameDict);
			if (frameDict->objectForKey("originalHeight")) oh = intForKey("originalHeight",frameDict);
            
			// check ow/oh
			if(!ow || !oh) {
				CCLOG("WARNING: originalWidth/Height not found on the CCSpriteFrame. AnchorPoint won't work as expected. Regenerate the .plist");
			}
			// abs ow/oh
			ow = abs(ow);
			oh = abs(oh);
			// create frame
			spriteFrame = CCSpriteFrame::createWithTexture(texture,CCRectMake(x, y, w, h),false,CCPointMake(ox, oy),CCSizeMake(ow, oh));
            
		}
        else {
			CCLOG("Unsupported Zwoptex version.");
		}
        
		// add sprite frame
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(spriteFrame,pElement->getStrKey());
    }

	std::string aniName = name;
	for(int i = 0; i < count; i++) {
        char achFrameName[64]="";
        sprintf(achFrameName, "%s%04d",name,i+1);
//        CCLog("xianbei %s",achFrameName);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(achFrameName);
		Frames->addObject(frame);
	}

	CCAnimation* ani = CCAnimation::createWithSpriteFrames(Frames,animationDelay);
//	ani->setName(aniName.c_str());
	m_pAnimations->setObject(ani,aniName);
    //bq modify
    Frames->release();
    
	return ani;
}

CCAnimation* HBAnimationCache::addAnimationWithFile(const char* name, char** aniNameCallBack)
{
	//CCLOG("HBAnimationCache::addAnimationWithFile name=%s",name);
	std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(CSTR_FILEPTAH(g_ActionFilePath, name));
//    path = g_ActionFilePath + path;
	int pos0 = path.find_last_of("/");
	int pos1 = path.find_last_of("\\");
	if (pos1<pos0)
	{
		pos1 = pos0;
	}
	int pos2 = path.find_last_of(".");
	std::string aniName = path.substr(pos1+1,pos2-pos1-1);
    sprintf(*aniNameCallBack,"%s", aniName.c_str());
	//判断是否存在该动画，如果有返回该动画，没有新建动画
	CCAnimation* ani = animationByName(aniName.c_str());
	if (ani)
	{
		return ani;
	}

	CCDictionary* dict = CCDictionary::createWithContentsOfFile(path.c_str());

	
	return addAnimationWithDictionary(dict,aniName.c_str());
}

void HBAnimationCache::removeAnimations()
{
	m_pAnimations->removeAllObjects();
}


void HBAnimationCache::removeUnusedAnimations()
{
	
	CCArray* keys = m_pAnimations->allKeys();
	CCLOG("HBAnimationCache: aniNumber: %d", keys->count());
	for(int i = 0;i<keys->count();i++) {
        CCString* key = (CCString*)keys->objectAtIndex(i);
		CCAnimation* ani = (CCAnimation*)m_pAnimations->objectForKey(key->m_sString);
		if( ani->retainCount()  == 1) {
			//CCLOG("HBAnimationCache: removing unused ani: %s", keys[i]);
			m_pAnimations->removeObjectForKey(key->m_sString);
		}
	}
}

CCAnimation* HBAnimationCache::reverseAnimationByAni(CCAnimation* ani,const char* name)
{
//    assert("HBAnimationCache::reverseAnimationByAni function error");
	//TODO IBM EDIT
	std::string reverseAniName = std::string(name);
	reverseAniName.append("_reverse");
	//std::string reverseAniName = std::string(ani->getName())+"_reverse";
	CCAnimation* pTempAni =(CCAnimation*)m_pAnimations->objectForKey(std::string(reverseAniName));
	if (pTempAni!=NULL){
		return pTempAni;
	}
	CCArray* oFrameArray = new CCArray();

	for (int i = ani->getFrames()->count()-1; i>=0; i--)
    {
		oFrameArray->addObject(ani->getFrames()->objectAtIndex(i));
	}
    
	CCAnimation *reverseAni = CCAnimation::create(oFrameArray,ani->getDelayPerUnit(),false);
    //reverseAni->setName(reverseAniName.c_str());
    //bq modify
    oFrameArray->release();
    
	return reverseAni;
}

CCAnimation* HBAnimationCache::reverseAnimationByName(const char* name)
{
    assert("HBAnimationCache::reverseAnimationByName function error");
	CCAnimation *ani = (CCAnimation*)m_pAnimations->objectForKey(std::string(name));
	if( ! ani )
		CCLOG("HBAnimationCache: reverseAni '%s' not found", name);

	return reverseAnimationByAni(ani,name);
}

//强制删除动画
void HBAnimationCache::setHBAnimationIsSingle()
{
   	CCArray* keys = m_pAnimations->allKeys();
    for (int i = 0; i < keys->count(); i++)
	{
        CCString* key = (CCString*)keys->objectAtIndex(i);
        CCAnimation* ani = (CCAnimation*)m_pAnimations->objectForKey(key->m_sString);
        if(ani->retainCount() > 1) {
            ani->release();
        }
//        CCLog("aniName = %s,count = %d",keys[i].c_str(),ani->retainCount());
	} 
}
//打印出保存在Cash中的动画信息
void HBAnimationCache::dumpHBAnimationCachedInfo()
{
//    unsigned int count = 0;
//	unsigned int totalBytes = 0;
    
	CCArray* keys = m_pAnimations->allKeys();
    for (int i = 0; i < keys->count(); i++)
	{
 //       CCString* key = (CCString*)keys->objectAtIndex(i);
//        CCAnimation* ani = (CCAnimation*)m_pAnimations->objectForKey(key->m_sString);
//        CCLog("aniName = %s,count = %d",keys[i].c_str(),ani->retainCount());
	}
    
}
