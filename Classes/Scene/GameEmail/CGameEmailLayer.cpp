//
//  CGameEmailLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#include "CGameEmailLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
CGameEmailLayer::CGameEmailLayer()
{
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
}

CGameEmailLayer::~CGameEmailLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
}

CGameEmailLayer *CGameEmailLayer::CreateEmailLayer()
{
    CGameEmailLayer *gameEmailLayer=new CGameEmailLayer();
    if (!gameEmailLayer || !gameEmailLayer->initCreate()) {
        delete gameEmailLayer;
        gameEmailLayer=NULL;
    }
    gameEmailLayer->autorelease();
    return gameEmailLayer;
}

bool CGameEmailLayer::initCreate()
{
    loadPlistFile();
    createRecvAllButton();
    setTouchEnabled(true);
    return true;
}

void CGameEmailLayer::createRecvAllButton()
{
    CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    addChild(sprite,0,100);
}


bool CGameEmailLayer::loadPlistFile()
{
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(this, CSTR_FILEPTAH(plistPath, "youjianjiemian.plist"));
    return true;
}
bool CGameEmailLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CGameEmailLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameEmailLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameEmailLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CGameEmailLayer::registerWithTouchDispatcher(void)
{
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -8, true);
}
void CGameEmailLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}