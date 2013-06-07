/********************************************************************
created:    2011/08/17
filename:   HBMap.cpp
author:     bqlf
purpose:    地图/界面类
*********************************************************************/
#include "HBMap.h"
#include "GameTools.h"
#include "sstream"
#include "HBSpriteCache.h"
#include "Utility.h"
using namespace GameTools;
using namespace Utility;
 
HBMap::HBMap(const char* fileName)
{
    string sFileName = string(fileName);
    std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(sFileName.c_str());
	CCDictionary* mapData = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
    initHBMap(mapData);
}

void HBMap::initHBMap(CCDictionary* data)
{
	//地图信息数据
	CCDictionary* mapInfoData = (CCDictionary*)data->objectForKey("mapInfo");

	mapName = string(valueForKey("mapName",mapInfoData));
	CCRect rect = ccRectForKey("gridInfo",mapInfoData);    
    gridWidth = rect.origin.x;
	gridHeight = rect.origin.y;
	gridWCount = rect.size.width;
	gridHCount = rect.size.height;
    mapW = gridWidth * gridWCount;
	mapH = gridHeight * gridHCount;
	rect = ccRectForKey("backgroundColor",mapInfoData);

	mapBackGroundColor = ccc4(rect.origin.x,rect.origin.y,rect.size.width,rect.size.height);

    //图层
    string sMapLayerNameArray = string(valueForKey("layerNameArray",mapInfoData));
    vector<string> vMapLayerNameArray = splitArgString(sMapLayerNameArray.c_str(),",");
    
	
	//图层数据
	CCDictionary* mapLayers = (CCDictionary*)data->objectForKey("mapLayers");
	CCArray* vMapLayerSprite=CCArray::create();
    int size = vMapLayerNameArray.size();
	for(int i = 0; i < size; i++) {
		MapLayer* mapLayer = new MapLayer;
		mapLayer->layer = i;
        mapLayer->layerName = vMapLayerNameArray[i];
		CCDictionary* mapLayerData = (CCDictionary*)mapLayers->objectForKey(vMapLayerNameArray[i]);
		vMapLayerSprite = mapLayerData->allKeys();
		for(int j = 0; j < vMapLayerSprite->count(); j++) {
            CCString* key=(CCString*)vMapLayerSprite->objectAtIndex(j);
			CCDictionary* aMapLayerSpriteData = (CCDictionary*)mapLayerData->objectForKey(key->m_sString);
			MapLayerSprite mapSprite;

			mapSprite.spriteName = string(valueForKey("spriteName",aMapLayerSpriteData));
			mapSprite.mapSpriteName = string(valueForKey("mapSpriteName",aMapLayerSpriteData));
			mapSprite.pos = ccPointForKey("pos",aMapLayerSpriteData);
			mapSprite.tag = intForKey("tag",aMapLayerSpriteData);
			mapSprite.z = intForKey("z",aMapLayerSpriteData);

			mapLayer->vMapLayerData.push_back(mapSprite);
			
			addName(mapSprite.spriteName,vHBSprite);
		}
		vMapLayer.push_back(mapLayer);
	}
	
	//载入HBSprite;
	HBSpriteCache* spriteCache = HBSpriteCache::sharedHBSpriteCache();
	for(int i = 0; i < vHBSprite.size(); i++) {
		//string fileName = vHBSprite[i]+".sprite";
		//IBM EDIT
		string fileName = vHBSprite[i];
		fileName.append(".sprite");
		HBSprite* hbSprite = new HBSprite(fileName.c_str());
		spriteCache->addHBSprite(hbSprite,vHBSprite[i].c_str());
        hbSprite->release();
	}
}


HBMap::~HBMap(void)
{
	for(int i = 0; i < vMapLayer.size(); i++) {
		delete vMapLayer[i];
		vMapLayer[i] = NULL;
	}
}

void HBMap::addName(const string& name,vector<string>& vName)
{
	for(int i = 0; i < vName.size(); i++) {
		if(vName.at(i) == name) {
			return;
		}
	}
	vName.push_back(name);
}

int HBMap::indexOfName(const string& name,vector<string>& vName)
{
	for(int i = 0; i < vName.size(); i++) {
		if(vName.at(i) == name) {
			return i;
		}
	}
	return -1;
}

//生成地图Layer
CCLayer* HBMap::createCCLayer()
{
	CCSize size = CCDirector::sharedDirector()->getWinSizeInPixels();
	HBSpriteCache* spriteCache = HBSpriteCache::sharedHBSpriteCache();
	CCLayer* map = new CCLayer();
    map->autorelease();
	////载入CCSpriteBatchNode to map
	//记录每层用到图片的名字
//	vector<string> vBatchNodeImageName;
//	vector<CCSpriteBatchNode*> vSpriteBatchNode;
	for(int i = 0; i < vMapLayer.size(); i++) {
		MapLayer* mapLayer = vMapLayer.at(i);
		CCLayer* childLayer = new CCLayer();
		childLayer->autorelease();
		//不用BatchNode情况
		for(int j = 0; j < mapLayer->vMapLayerData.size(); j++) {
			MapLayerSprite spriteData = mapLayer->vMapLayerData[j];
			//取得需要的精灵文件
			HBSprite* hbSprite = spriteCache->HBSpriteByName(spriteData.spriteName.c_str());
			//
			CCSprite* sprite = hbSprite->createCCSpriteWithName(spriteData.mapSpriteName);
			childLayer->addChild(sprite,spriteData.z,spriteData.tag);
            sprite->setPosition(ccp(spriteData.pos.x + hbSprite->getSpriteOffset().x,size.height - (spriteData.pos.y + hbSprite->getSpriteOffset().y) + (mapH - size.height)));
//            setSpriteAction(sprite,spriteData.spriteName,spriteData.mapSpriteName);
		}
		map->addChild(childLayer,i,i);
	}
	return map;
}

string HBMap::getMapName()
{
	return mapName;
}

ccColor4B HBMap::getMapBackGroundColor()
{
    return mapBackGroundColor;
}

//取得图层名字
vector<string> HBMap::getMapLayerName()
{
    vector<string> mapLayerName;
    for (int i = 0; i < vMapLayer.size(); i++) {
        MapLayer* mapLayer = vMapLayer[i];
        mapLayerName.push_back(mapLayer->layerName);
    }
    return mapLayerName;
}
//取得图层tag
vector<int> HBMap::getMapLayerTag()
{
    vector<int> mapLayerTag;
    for (int i = 0; i < vMapLayer.size(); i++) {
        MapLayer* mapLayer = vMapLayer[i];
        mapLayerTag.push_back(mapLayer->layer);
    }
    return mapLayerTag;
}


