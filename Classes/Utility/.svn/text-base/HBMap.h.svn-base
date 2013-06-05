/********************************************************************
created:    2011/08/17
filename:   HBMap.h
author:     bqlf 
purpose:    地图/界面类
*********************************************************************/
#ifndef HBMAP_H
#define HBMAP_H

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;
class MapLayer;
class MapLayerSprite;
class HBMap
{
public:
	//HBMap(void);
    HBMap(const char* fileName);
	virtual ~HBMap(void);
	void initHBMap(CCDictionary* data);
	//生成地图Layer
	CCLayer* createCCLayer();
    ccColor4B getMapBackGroundColor();
    //取得图层名字
    vector<string> getMapLayerName();
    //取得图层tag
    vector<int> getMapLayerTag();
    string getMapName();
private:
	void addName(const string& name,vector<string>& vName);
	int indexOfName(const string& name,vector<string>& vName);
	string mapName;
    int gridWidth;
	int gridHeight;
	int gridWCount;
	int gridHCount;
	int mapW,mapH;
	ccColor4B mapBackGroundColor;
	//图层
	vector<MapLayer*> vMapLayer;
	//所需的精灵文件
	vector<string> vHBSprite;
};
//图层
class MapLayer
{
public:
	int layer;
    string layerName;
	vector<MapLayerSprite> vMapLayerData;
};
//地图精灵
class MapLayerSprite
{
public:
	string spriteName;
	string mapSpriteName;
	CCPoint pos;
	int z;
	int tag;
};
#endif // HBMAP_H