//
//  MapLayerData.cpp
//  testMap
//
//  Created by bqlf1907 on 11-9-22.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#include "MapLayerData.h"

MapLayerDataCache* MapLayerDataCache::s_pSharedMapLayerDataCache = NULL;

MapLayerDataCache::MapLayerDataCache()
: m_pMapLayerData(NULL)
{
}

MapLayerDataCache::~MapLayerDataCache()
{
	if(s_pSharedMapLayerDataCache) {
		delete s_pSharedMapLayerDataCache;
		CC_SAFE_RELEASE(m_pMapLayerData);
	}
}

MapLayerDataCache* MapLayerDataCache::sharedMapLayerDataCache(void)
{
	if (! s_pSharedMapLayerDataCache)
	{
		s_pSharedMapLayerDataCache = new MapLayerDataCache();
		s_pSharedMapLayerDataCache->init();
	}
    
	return s_pSharedMapLayerDataCache;
}

bool MapLayerDataCache::init()
{
	m_pMapLayerData = new CCDictionary();
	return true;
}

void MapLayerDataCache::addMapLayerData(MapLayerData *sprite, const char * fileName)
{
	m_pMapLayerData->setObject(sprite, std::string(fileName));
}

void MapLayerDataCache::removeMapLayerDataByName(const char* fileName)
{
	if (! fileName)
	{
		return;
	}
    
	m_pMapLayerData->removeObjectForKey(std::string(fileName));
}

void MapLayerDataCache::removeAll()
{
    m_pMapLayerData->removeAllObjects();
}

MapLayerData* MapLayerDataCache::MapLayerDataByName(const char* fileName)
{
	return (MapLayerData*)m_pMapLayerData->objectForKey(std::string(fileName));
}



MapLayerData::MapLayerData(HBMap* hbMap)
{
    vMapLayerName = hbMap->getMapLayerName();
    vMapLayerTag = hbMap->getMapLayerTag();
}







