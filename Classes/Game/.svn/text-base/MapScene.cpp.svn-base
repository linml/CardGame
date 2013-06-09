#include "MapScene.h"
#include "PtMapUtility.h"
#include "HBSpriteCache.h"
#include "MapLayerData.h"
#include "Utility.h"
#include "SceneManager.h"

using namespace  Utility;
char g_fileName[300]="";

MapScene::MapScene(void)
{
}

MapScene::~MapScene(void)
{
}

bool MapScene::init()
{
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	CCLog("MapScene::init");
	bool bRet = false;
	do 
	{
		char *tmp = NULL;
		CC_BREAK_IF(!CCLayer::init());
		tmp = (char *)this->getUserData();
        CCLog("tmp:%s",tmp);
        CCLayer *outLayer = CCLayer::create();
		PtMapUtility::addChildFromScript(outLayer,tmp);
		PtMapUtility::addTouchRectFromScript(tmp, outLayer, &vTouchMapRect);
		addChild(outLayer, 0, 10000);
		setTouchEnabled(true);
		bRet = true;
	} while (0);
	return bRet;
}


bool MapScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	
	CCLog("MapScene::ccTouchBegan %f, %f", pTouch->getLocation().x, pTouch->getLocation().y);
	
	return true;
}

void MapScene::registerWithTouchDispatcher( void )
{

	CCLog("MapScene::registerWithTouchDispatcher");
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void MapScene::onExit()
{
	CCLayer::onExit();
	CCLog("MapScene::onExit");
}

void MapScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint p = pTouch->getLocation();
	handleTouchSpritePool(p);
}

bool MapScene::handleTouchSpritePool( CCPoint point )
{

	bool bRet = false;
	CCSprite* touchSprite= CCSprite::create();
	int touch_tag = -1;
	touch_tag = TouchRect::SearchTouchTag(point, vTouchMapRect, &touchSprite);
	if(!SingleSceneManager::instance()->runSceneSelect(touch_tag))
    {
        
	}
	return bRet;
}

void MapScene::timerRefesh( float dt )
{
	this->refeshPlist();
}

void MapScene::refeshPlist()
{
//	CCNode * layer = this->getChildByTag(10000);
//	if (layer!=NULL)
//	{
//		removeChild(layer,true);
//	}
//
//	vTouchMapRect.clear();
//	CCLayer *outLayer = CCLayer::create();
//	PtMapUtility::addChildFromScript(outLayer,g_fileName);
//	PtMapUtility::addTouchRectFromScript(g_fileName, outLayer, &vTouchMapRect);
//	addChild(outLayer, 0, 10000);
}

