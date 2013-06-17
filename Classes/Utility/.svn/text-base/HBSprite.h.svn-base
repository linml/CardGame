/********************************************************************
created:    2011/08/17
filename:   HBSprite.h
author:     bqlf 
purpose:    地图精灵数据，载入sprite文件数据类，生成CCSprite
*********************************************************************/
#ifndef HBSPRITE_H
#define HBSPRITE_H

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class SpriteProperty;
//用于地图上精灵
class MapSprite
{
public:
	string mapSpriteName;
	vector<SpriteProperty> spritePropertyList;
};

//切片数据
class Clip
{
public:
	string clipName;
	CCRect clipRect;
};

//精灵的属性
class SpriteProperty
{
public:
	//图片切割区域
	CCRect clipRect;
	//在父节点的位置
	CCPoint pos;
	//翻转类型 0---无翻转 1--水平 2--垂直 3--水平+垂直
	int intFlipType;
	//缩放1.0--1倍
	float scale;
	//旋转[0,360]
	float rotate;
	//锚点
	CCPoint anchor;
};

//载入sprite文件数据类，生成CCSprite
class HBSprite : public CCObject
{
public:
    HBSprite(const char* fileName);
	virtual ~HBSprite(void);
    //读取数据
	void initHBSprite(CCDictionary* data);
    
	//TODO未读取sprite文件，直接生成
	static CCSprite* HBSpriteWithName(const char* fileName,const char* mapSpriteName);
	//生成可用的精灵--前提是已经读取sprite文件
	CCSprite* createCCSpriteWithName(const string& mapSpriteName);
	//用CCSpriteBatchNode生成精灵
	CCSprite* createCCSpriteWithCCSpriteBathcNode(CCSpriteBatchNode* spriteBatchNode,const string& mapSpriteName);
	//test 用offset代替anchor
    CCPoint getSpriteOffset();
    
	CCSpriteBatchNode* createCCSpriteBatchNode();
	string getImage();
private:
	//初始化精灵属性
	CCSprite* initHBSprite(const string& imageName,const SpriteProperty& property);
	//初始化精灵属性
	CCSprite* initHBSpriteBatchNode(CCSpriteBatchNode* spriteBatchNode,const SpriteProperty& property);
	//初始化精灵属性
	void initSpriteProperty(CCSprite* sprite,const SpriteProperty& property);
	//遍历clipList 找clipName的clipRect
	CCRect getClipRect(const string& clipName,vector<Clip*> clipList);
    CCPoint offset;
	string imageName;
	vector<Clip*> clipList;
	vector<MapSprite*> mapSpriteList;
};
#endif // HBSPRITE_H