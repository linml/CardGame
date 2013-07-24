//
//  CCardSettingScene.cpp
//  91.cube
//
//  Created by phileas on 13-7-18.
//
//

#include "CCardSettingScene.h"
#include "CBattleArrayLayer.h"
#include "gameConfig.h"

CCScene* CCardSettingScene::scene()
{
    CCScene *scene = CCScene::create();
    CCardSettingScene *layer = CCardSettingScene::create();
    if (layer)
    {
        scene->addChild(layer);
    }
    return scene;
}

CCardSettingScene::CCardSettingScene()
{
    
}

CCardSettingScene::~CCardSettingScene()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}

bool CCardSettingScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCCardSetting();
        bRet = true;
    } while (0);
    return bRet;
}

bool CCardSettingScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return false;
}
void CCardSettingScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CCardSettingScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    handlerTouch();
}
void CCardSettingScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CCardSettingScene::initCCardSetting()
{
    // add bg:
    CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardsettingscene_bg.png"));
    addChild(bg, 0);
    bg->setAnchorPoint(CCPointZero);
    m_nTouchTag = -1;
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "zhenxing.plist"));
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-1);
    m_cMaps->getTouchRects(m_cTouches);
    
    // add other layer:
    CBattleArrayLayer * layer = CBattleArrayLayer::create();
    addChild(layer, 2, 10);
    
    bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardBag.png"));
  //  CCSprite *bg_header = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardBg_header.png"));
   // CCSize size = bg->boundingBox().size;
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(ccp(547, 0));
//    bg_header->setAnchorPoint(CCPointZero);
//    bg_header->setPosition(ccp(10, 635));
//    bg->addChild(bg_header, 20);
    this->addChild(bg);


}

void CCardSettingScene::handlerTouch()
{
}
