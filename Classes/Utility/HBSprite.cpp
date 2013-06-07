/********************************************************************
created:    2011/08/17
filename:   HBSprite.cpp
author:     bqlf
purpose:    地图精灵数据 
*********************************************************************/
#include "HBSprite.h"
#include "GameTools.h"
#include "sstream"
#include "Utility.h"
#include "gameConfig.h"

using namespace GameTools;
using namespace Utility;

HBSprite::HBSprite(const char* fileName)
{
    string sFileName = mapSpritePath + string(fileName);

    std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(sFileName.c_str());

	CCDictionary* spriteData = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());

    initHBSprite(spriteData);

}

void HBSprite::initHBSprite(CCDictionary* data)
{

	string imagePath = string(valueForKey("imagePath",data));
	int index = imagePath.rfind('/',imagePath.size());
	imageName = imagePath.substr(index + 1,imagePath.size() - index);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    imageName = mapImagesPath + imageName;
//#endif
	//imageName = "地表4-ok2.png";
	//切片数据
	CCDictionary* clipData = (CCDictionary*)data->objectForKey("clipData");
	CCArray* vKey = clipData->allKeys();
	int size = vKey->count();
	for(int i = 0; i < size; i++) {
        CCString* key = (CCString*)vKey->objectAtIndex(i);
		Clip* clip = new Clip();
		clip->clipName = key->m_sString;
		clip->clipRect = ccRectForKey(clip->clipName.c_str(),clipData);
		clipList.push_back(clip);
	}
	//精灵数据
	CCDictionary* mapSpriteData = (CCDictionary*)data->objectForKey("mapSpriteData");
	vKey->removeAllObjects();
	vKey = mapSpriteData->allKeys();
	size = vKey->count();
	for(int i = 0; i < size; i++) {
		MapSprite* mapSprite = new MapSprite();
		mapSprite->mapSpriteName = ((CCString*)vKey->objectAtIndex(i))->m_sString;
		//精灵属性
		CCDictionary* spritePropertyDataList = (CCDictionary*)mapSpriteData->objectForKey(mapSprite->mapSpriteName);
		//子精灵key--clipName
		CCArray* vSubClipKey = spritePropertyDataList->allKeys();
		//子精灵属性
		for(int j = 0; j < vSubClipKey->count(); j++) {
            CCString* key = (CCString*)vSubClipKey->objectAtIndex(j);
			CCDictionary* spritePropertyData = (CCDictionary*)spritePropertyDataList->objectForKey(key->m_sString);
			SpriteProperty spriteProperty;
			//clipRect
			spriteProperty.clipRect = getClipRect(key->m_sString,clipList);
			//pos
			spriteProperty.pos = ccPointForKey("pos",spritePropertyData);
			//flipType
			spriteProperty.intFlipType = intForKey("filp",spritePropertyData);
			//scale
			spriteProperty.scale = floatForKey("scale",spritePropertyData);
			//rotate
			spriteProperty.rotate = floatForKey("rotate",spritePropertyData);
			//anchor
			spriteProperty.anchor = ccPointForKey("anchor",spritePropertyData);
			mapSprite->spritePropertyList.push_back(spriteProperty);
		}
		mapSpriteList.push_back(mapSprite);
	}

}

HBSprite::~HBSprite(void)
{
	for(int i = 0; i < clipList.size(); i++) {
		delete clipList[i];
		clipList[i] = NULL;
	}
	for(int i = 0; i < mapSpriteList.size(); i++) {
		delete mapSpriteList[i];
		mapSpriteList[i] = NULL;
	}
}

CCRect HBSprite::getClipRect(const string& clipName,vector<Clip*> clipList)
{
	for(int i = 0; i < clipList.size(); i++) {
		Clip* clip = clipList.at(i);
		if(clip->clipName == clipName) {
			return clip->clipRect;
		}
	}

	return CCRect::CCRect(0,0,0,0);
}

//生成可用的精灵
CCSprite* HBSprite::createCCSpriteWithName(const string& mapSpriteName)
{
	for(int i = 0; i < mapSpriteList.size(); i++) {
		MapSprite* mapSprite = mapSpriteList.at(i);
		if(mapSprite->mapSpriteName == mapSpriteName) {

			if(mapSprite->spritePropertyList.size() <= 0) {
				CCSprite* sprite = new CCSprite();
				sprite->init();
                sprite->autorelease();
				return sprite;
			}
			//构建CCSprite;
            CCSprite* sprite;
            if (mapSprite->spritePropertyList.size() > 1) {
                //多个精灵组合 父节点为空精灵
                sprite = new CCSprite();
				sprite->init();
                sprite->autorelease();
                for(int i = 0; i < mapSprite->spritePropertyList.size();i++) {
                    //构建子CCSprite;
                    CCSprite* child = initHBSprite(imageName,mapSprite->spritePropertyList.at(i));              
                     sprite->addChild(child);
                }
            }
            else {
                //单个精灵
                sprite = initHBSprite(imageName,mapSprite->spritePropertyList.at(0));
                offset = sprite->getPosition();
                sprite->setPosition(ccp(0,0));
            }
            
			return sprite;

		}
	}
    CCLog("未找到 mapSpriteName = %s",mapSpriteName.c_str());
	return NULL;
}

//用CCSpriteBatchNode生成精灵
CCSprite* HBSprite::createCCSpriteWithCCSpriteBathcNode(CCSpriteBatchNode* spriteBatchNode,const string& mapSpriteName)
{
	for(int i = 0; i < mapSpriteList.size(); i++) {
		MapSprite* mapSprite = mapSpriteList.at(i);
		if(mapSprite->mapSpriteName == mapSpriteName) {

			if(mapSprite->spritePropertyList.size() <= 0) {
				CCSprite* sprite = new CCSprite();
				sprite->init();
                sprite->autorelease();
				return sprite;
			}
			//构建CCSprite;
            CCSprite* sprite;
            if (mapSprite->spritePropertyList.size() > 1) {
                //多个精灵组合 父节点为空精灵
                sprite = new CCSprite();
				sprite->init();
                sprite->autorelease();
                for(int i = 0; i < mapSprite->spritePropertyList.size();i++) {
                    //构建子CCSprite;
                    CCSprite* child = initHBSpriteBatchNode(spriteBatchNode,mapSprite->spritePropertyList.at(i));
                    sprite->addChild(child);
                }
            }
            else {
                //单个精灵
                sprite = initHBSpriteBatchNode(spriteBatchNode,mapSprite->spritePropertyList.at(0));
                offset = sprite->getPosition();
                sprite->setPosition(ccp(0,0));
            }
//			CCSprite* sprite = initHBSpriteBatchNode(spriteBatchNode,mapSprite->spritePropertyList.at(0));
//            offset = sprite->getPositionInPixels();
//            sprite->setPositionInPixels(ccp(0,0));
//			for(int i = 1; i < mapSprite->spritePropertyList.size();i++) {
//				//构建子CCSprite;
//				CCSprite* child = initHBSpriteBatchNode(spriteBatchNode,mapSprite->spritePropertyList.at(i));
//				sprite->addChild(child);
//			}
			return sprite;

		}
	}
	return NULL;
}

//初始化精灵属性
CCSprite* HBSprite::initHBSpriteBatchNode(CCSpriteBatchNode* spriteBatchNode,const SpriteProperty& property)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	CCSprite* sprite = CCSprite::spriteWithBatchNode(spriteBatchNode, property.clipRect);
#else
	//CCSprite* sprite = spriteBatchNode->createSpriteWithRect(property.clipRect);
#endif
	//initSpriteProperty(sprite,property);
	return NULL;
}

//初始化精灵属性
CCSprite* HBSprite::initHBSprite(const string& imageName,const SpriteProperty& property)
{
    CCRect rect = property.clipRect;
	CCSprite* sprite = CCSprite::create(imageName.c_str(),rect);
	initSpriteProperty(sprite,property);
	return sprite;
}

//初始化精灵属性
void HBSprite::initSpriteProperty(CCSprite* sprite,const SpriteProperty& property)
{
	//pos
	sprite->setPosition(property.pos);
	//缩放
	sprite->setScale(property.scale);
	//旋转
	sprite->setRotation(property.rotate);
	//翻转
	if(property.intFlipType == 1) {
		sprite->setFlipX(true);
	}
	else if(property.intFlipType == 2) {
		sprite->setFlipY(true);
	}
	else if(property.intFlipType == 3) {
		sprite->setFlipX(true);
		sprite->setFlipY(true);
	}
	//锚点
	sprite->setAnchorPoint(property.anchor);
    //sprite->setAnchorPoint(ccp(0,0));
    //offset = ccp(-property.clipRect.size.width * property.anchor.x,-property.clipRect.size.height * property.anchor.y);
    //offset = ccp((int)offset.x,(int)offset.y);
    //CCLOG("offset x = %f,y = %f",offset.x,offset.y);
}

CCSpriteBatchNode* HBSprite::createCCSpriteBatchNode()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CCSpriteBatchNode* spriteBatchNode = CCSpriteBatchNode::batchNodeWithFile(imageName.c_str());
#else
	CCSpriteBatchNode* spriteBatchNode = CCSpriteBatchNode::create(imageName.c_str());
#endif
	return spriteBatchNode;
}

string HBSprite::getImage()
{
	return imageName;
}

CCPoint HBSprite::getSpriteOffset()
{
    return offset;
}

//TODO未读取sprite文件，直接生成
CCSprite* HBSprite::HBSpriteWithName(const char* fileName,const char* mapSpriteName)
{

	return (CCSprite*)CCSprite::create();
}
