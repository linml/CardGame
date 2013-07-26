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
    m_pBattleArrayCards = CCDictionary::create();
    m_pBattleArrayCards->retain();
}

CCardSettingScene::~CCardSettingScene()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    if (m_pBattleArrayCards)
    {
        m_pBattleArrayCards->release();
    }
}

bool CCardSettingScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCCardSetting();
     //   load();
        bRet = true;
    } while (0);
    return bRet;
}

bool CCardSettingScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag =  TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if (m_nTouchTag != -1)
    {
        return true;
    }
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
    m_nTouchTag = -1;
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
    m_pCardSetting = CBattleArrayLayer::create();
    addChild(m_pCardSetting, 2, 10);
    bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardBag.png"));
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(ccp(547, 0));

    this->addChild(bg);
    
    m_nCurrentTableId = 3001;
    m_pCardSetting->addTeamArrayPanel();
}

void CCardSettingScene::handlerTouch()
{
    if (m_nTouchTag == m_nCurrentTableId)
    {
        return;
    }
    switch (m_nTouchTag) {
        case 3001:
            m_pCardSetting->removeLeft();
            m_pCardSetting->addTeamArrayPanel();
            m_pCardSetting->setTableClickEnable(false);

            CCLog("go to team array:");
            break;
        case 3002:
            m_pCardSetting->setTableClickEnable(true);
            m_pCardSetting->removeLeft();
            m_pCardSetting->addEnhance();
            
            CCLog("go to enhance");
            break;
//        case 3003:
//            m_pCardSetting->removeLeft();
//            CCLog("go to evolution:");
//            break;
//            m_pCardSetting->removeLeft();
//        case 3004:
//            CCLog("go to sell:");
//            break;
        default:
            return;
    }
    m_nCurrentTableId = m_nTouchTag;
}


// test:
void CCardSettingScene::load()
{

    CGamePlayer* m_pGamePlayer = SinglePlayer::instance();
    m_pGamePlayer->loadGamesConfig();
    m_pGamePlayer->loadServerCardBag();
    m_pGamePlayer->loadCardTeamInfo();
    this->schedule(schedule_selector(CCardSettingScene::callback));
}
void CCardSettingScene::callback(float dt)
{
     CGamePlayer* m_pGamePlayer = SinglePlayer::instance();
    if (m_pGamePlayer->isLoadCardBagEnd && m_pGamePlayer->isLoadEndCardTeam)
    {
        initCCardSetting();
        this->unschedule(schedule_selector(CCardSettingScene::callback));
    }

    
}