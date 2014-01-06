//
//  CPVPRegulationLayer.cpp
//  91.cube
//
//  Created by linminglu on 14-1-6.
//
//

#include "CPVPRegulationLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"

#define TAG_PVP_BACKGROUDMAP 2
#define TAG_PVP_QUITBUTTON 3

CPVPRegulationLayer::CPVPRegulationLayer()
{
    size=CCDirector::sharedDirector()->getWinSize();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
}

CPVPRegulationLayer::~CPVPRegulationLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"));
}

bool CPVPRegulationLayer::init()
{
    createBackGround();
    createQuitButton();
    setTouchEnabled(true);
    addChild(createContianLayer(),3);
    setTouchPriority(-9);
    return  true;
}

CCLayer *CPVPRegulationLayer::createContianLayer()
{
    
    CCLayer *pContextlayer=CCLayer::create();
    const string noticeValue[8]={"319911","319912","319913","319914","319915","319916","319917","319918"};
    string word;
    float totalHeight=0.0f;
    for (int i=7; i>=0; i--) {
        word=Utility::getWordWithFile("dictionary.plist", noticeValue[i].c_str());
        CCLayer *layer=GameTools::createDiffColorWord(word, "Arial", 25);
        pContextlayer->addChild(layer, 2);
        float height=layer->getContentSize().height;
        layer->setPosition(ccp(0,height+totalHeight));
        totalHeight+=height;
    }
    return pContextlayer;
    
    
}

void CPVPRegulationLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void CPVPRegulationLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool CPVPRegulationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CPVPRegulationLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CPVPRegulationLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point=pTouch->getLocation();
    if (getChildByTag(TAG_PVP_QUITBUTTON)->boundingBox().containsPoint(point))
    {
        removeFromParentAndCleanup(true);
    }
    
}

void CPVPRegulationLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CPVPRegulationLayer::createBackGround()
{
    CCLayer *layer=CCLayer::create();
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(layer, CSTR_FILEPTAH(plistPath, "haoyoujiemian.plist"));
    addChild(layer,1,TAG_PVP_BACKGROUDMAP);
    
}

void CPVPRegulationLayer::createQuitButton()
{
    CGameButtonControl *gameButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameButton,2,TAG_PVP_QUITBUTTON);
    gameButton->setPosition(ccp(size.width*0.5+320, size.height*0.5+220));

}
