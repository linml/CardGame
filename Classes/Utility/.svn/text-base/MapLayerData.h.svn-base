//
//  MapLayerData.h
//  testMap
//  保存map的相关信息,（文件名字，图层名字（关联具体的图层tag），生成的cclayer，等）
//  Created by bqlf1907 on 11-9-22.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#ifndef MAPLAYERDATA_H
#define MAPLAYERDATA_H

#include "cocos2d.h"
#include "HBMap.h"
using namespace cocos2d;
using namespace std;

class MapLayerData : public CCObject
{
public:
    MapLayerData(HBMap* hbMap);
    //地图文件名字
    string mapFileName;
    //地图中图层名字
    vector<string> vMapLayerName;
    //地图中图层相对应的tag
    vector<int> vMapLayerTag;
    //生成的CCLayer;
    CCLayer* map;
};


class MapLayerDataCache
{
public:
	MapLayerDataCache();
	~MapLayerDataCache();
    
	static MapLayerDataCache* sharedMapLayerDataCache(void);
    
	void addMapLayerData(MapLayerData *mapLayerData, const char * fileName);
	void removeMapLayerDataByName(const char* fileName);
    void removeAll();
	MapLayerData* MapLayerDataByName(const char* fileName);
	bool init();
private:
	CCDictionary* m_pMapLayerData;
	static MapLayerDataCache *s_pSharedMapLayerDataCache;
};

#endif