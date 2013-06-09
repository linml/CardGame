//
//  CHallScene.cpp
//  91.cube
//
//  Created by phileas on 13-6-7.
//
//

#include "HallScene.h"
#include "HBSpriteCache.h"
#include "MapLayerData.h"
#include "gameConfig.h"

CCScene* CHallScene::scene()
{
    CCScene *scene = CCScene::create();
    CHallScene *layer = CHallScene::create();
    scene->addChild(layer);
    return scene;
}

CHallScene::CHallScene()
{
    
}

CHallScene::~CHallScene()
{
    //clean the cache:
    HBSpriteCache::sharedHBSpriteCache()->purgesharedHBSpriteCache();
    MapLayerDataCache::sharedMapLayerDataCache()->removeAll();
}


bool CHallScene::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        CC_BREAK_IF(!initHall());
        bRet = true;
    } while (0);
    return bRet;
}


bool CHallScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return  true;
}

void CHallScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CHallScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CHallScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


// protectd methdo:
bool CHallScene::initHall()
{
    bool bRet = false;
    do {
        CCSize size =  CCDirector::sharedDirector()->getWinSize();
        CCLayer *layer = CCLayer::create();
        m_cplist = LayoutLayer::create();
               m_cplist->initWithFile(layer, CSTR_FILEPTAH(plistPath, "hall.plist"));
        
        
        // add the bg,maybe it will be separate to a layer
        CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "hall.png"));
        bg->setPosition(ccp(size.width/2, size.height/2));

        CCNode * node  = m_cplist->getElementByTags("0");
        if (node) {
            node->addChild(bg, -1);
        }
        
        
        
        this->addChild(layer);
        
        bRet = true;
    } while (0);
    
    return bRet;
}