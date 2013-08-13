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
#include "SceneManager.h"
#include "Utility.h"
CCDictionary * CCardSettingScene::s_pBattleArrayCards = NULL;

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
    if (s_pBattleArrayCards == NULL)
    {
        s_pBattleArrayCards = CCDictionary::create();
        s_pBattleArrayCards->retain();
    }
    for (int i = 0;  i< 4; i++)
    {
        m_pTabButton[i] = NULL;
    }
   
}

CCardSettingScene::~CCardSettingScene()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    if (s_pBattleArrayCards)
    {
        s_pBattleArrayCards->release();
        s_pBattleArrayCards = NULL;
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
    if (m_nTouchTag == m_nCurrentTableId)
    {
        return false;
    }
    
    CCPoint point ;
    CCTexture2D *pressed =NULL;
    if (m_nTouchTag != -1)
    {
        switch (m_nTouchTag)
        {
            case 3001:
            case 3002:
            case 3003:
            case 3004:
                point = m_pTabButton[m_nTouchTag-3001]->getAnchorPoint();
                pressed = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "tabButton_pressed.png"));
                m_pTabButton[m_nTouchTag-3001]->initWithTexture(pressed);
                m_pTabButton[m_nTouchTag-3001]->setAnchorPoint(point);
                break;
            case 3006:
                m_pBackBtn->initWithFile(CSTR_FILEPTAH(g_mapImagesPath, "back_active.png"));
                m_pBackBtn->setAnchorPoint(CCPointZero);
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}
void CCardSettingScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CCardSettingScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    if (m_nTouchTag ==  TouchRect::SearchTouchTag(touchPoint, m_cTouches))
    {
         handlerTouch();
    }else
    {
        CCTexture2D * noraml = NULL;
        switch (m_nTouchTag)
        {
            case 3001:
            case 3002:
            case 3003:
            case 3004:
                touchPoint = m_pTabButton[m_nTouchTag-3001]->getAnchorPoint();
                noraml = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "tabButton_normal.png"));
                m_pTabButton[m_nTouchTag-3001]->initWithTexture(noraml);
                m_pTabButton[m_nTouchTag-3001]->setAnchorPoint(touchPoint);
                break;
            case 3006:
                noraml = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "back_normal.png"));
                m_pBackBtn->initWithTexture(noraml);
                m_pBackBtn->setAnchorPoint(CCPointZero);
                break;
            default:
                break;
        }

    }


  
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
    setTouchPriority(-10000);
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
    
    // test: add close button:
    CCSprite *btn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "back_normal.png"));
    this->addChild(btn, 100000, 3006);
    btn->setAnchorPoint(CCPointZero);
    btn->setPosition(ccp(950, 700));
    m_pBackBtn = btn;
    Utility::addTouchRect(3006, btn, m_cTouches);
    
    //tab button:
    
    CCNode *parent = m_cMaps->getElementByTags("1,1");
    CCNode *tmp = parent->getChildByTag(2);
    CCPoint an = tmp->getAnchorPoint();
    CCPoint pos = tmp->getPosition();
    CCLabelTTF * label = NULL;
    int array[4]={1,1,2,0};
    label =(CCLabelTTF*) m_cMaps->getElementByTags(array, 4);
    CCPoint wordPos = label->getPosition();
    for (int i = 0;  i< 4; i++)
    {
        if (i == 0)
        {
            CCTexture2D * selected = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "tabButton_selected.png"));
            m_pTabButton[i] = CCSprite::createWithTexture(selected);
            array[2]=1;
            
        }
        else
        {
            array[3]=i;
            label =(CCLabelTTF*) m_cMaps->getElementByTags(array, 4);
            
            CCTexture2D * normal = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "tabButton_normal.png"));
            m_pTabButton[i] = CCSprite::createWithTexture(normal);
            pos.x += (normal->getContentSize().width);

        }
          m_pTabButton[i]->setAnchorPoint(an);
          m_pTabButton[i]->setPosition(pos);
         parent->addChild(m_pTabButton[i],200);
         label->removeFromParentAndCleanup(true);
         label->setPosition(wordPos);
        m_pTabButton[i]->addChild(label);
    }
    
}

void CCardSettingScene::handlerTouch()
{

    if (m_nTouchTag == m_nCurrentTableId)
    {
        return;
    }
    switch (m_nTouchTag)
    {
        case 3001:
            m_pCardSetting->removeLeft();
            m_pCardSetting->addTeamArrayPanel();
            m_pCardSetting->setTableClickEnable(false);

            CCLog("go to team array:");
            break;
        case 3002:
            m_pCardSetting->removeLeft();
            m_pCardSetting->setTableClickEnable(false);
            m_pCardSetting->addEnhance();
            
            CCLog("go to enhance");
            break;
        case 3003:
            CCLog("go to evolution:");
            m_pCardSetting->removeLeft();
            m_pCardSetting->setTableClickEnable(true);
            m_pCardSetting->addEvolution();
            break;
        
        case 3004:
           // CCLog("go to sell:");
             m_pCardSetting->removeLeft();
             m_pCardSetting->addSell();
             m_pCardSetting->setTableClickEnable(false);
            break;
            
        case 3006:
            SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE);
            break;
        default:
            return;
    }
 
    if (m_nTouchTag != 3006)
    {
        CCTexture2D * texture = NULL;
        CCPoint  anPos = m_pTabButton[m_nTouchTag-3001]->getAnchorPoint();
        texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "tabButton_normal.png"));
        m_pTabButton[m_nCurrentTableId-3001]->initWithTexture(texture);
        m_pTabButton[m_nCurrentTableId-3001]->setAnchorPoint(anPos);
        
        texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "tabButton_selected.png"));
        m_pTabButton[m_nTouchTag-3001]->initWithTexture(texture);
        m_pTabButton[m_nTouchTag-3001]->setAnchorPoint(anPos);
        m_nCurrentTableId = m_nTouchTag;

    }
    PtSoundTool::playSysSoundEffect("UI_click.wav");


    
 
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