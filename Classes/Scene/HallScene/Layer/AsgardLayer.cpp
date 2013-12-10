//
//  CGoldPlaceLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-8.
//
//

#include "AsgardLayer.h"
#include "gameConfig.h"
#include "Biforest.h"
#include "TaskLayer.h"
#include "CCardSettingScene.h"
#include "SceneManager.h"
#include "ExplorationScene.h"
#include "CPanelGamePlayerInfoLayer.h"
#include "CSceneActionGameLayer.h"
#include "CScreenHandBookLayer.h"
#include "CSceneHospitalLayer.h"
#include "CSceneFriendMainLayer.h"
#include "CGameRankLayer.h"
#include "CRankDataManager.h"
#include "CEveryDayLoginLayer.h"


CAsgardLayer::CAsgardLayer()
{
    m_bLoadTaskInfo = true;
    m_pFriendBtn = NULL;
    m_pRankBtn = NULL;
    m_pQianDao=NULL;
}

CAsgardLayer::~CAsgardLayer()
{
    m_cMaps->release();
}

bool CAsgardLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        initArsgard();
        createOrUpdatePlayerData();
        bRet = true;
    } while (0);
    return bRet;
}

void CAsgardLayer::createBiforestLayer(EXPLORATIONTYPE inType,int inSelectSectionId)
{
   CCLayer*  layer = CBiforestLayer::create(inType,inSelectSectionId);
   this->getParent()->addChild(layer, 30000, 6001);

}

bool CAsgardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    // test:
    if (m_bLoadTaskInfo == false)
    {
        return false;
    }
    
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if(m_nTouchTag != -1)
    {
        return true;
    }
    
    return false;
}
void CAsgardLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CAsgardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    if (m_nTouchTag ==  TouchRect::SearchTouchTag(touchPoint, m_cTouches, &m_touchSprite))
    {
        handlerTouch();
    }

}

void CAsgardLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CAsgardLayer::createOrUpdatePlayerData()
{
    CPanelGamePlayerInfoLayer *layer=NULL;
    if(!getChildByTag(777111))
    {
        CPanelGamePlayerInfoLayer *layer=CPanelGamePlayerInfoLayer::create();
        addChild(layer,0,777111);
    }
    layer=(CPanelGamePlayerInfoLayer *)getChildByTag(777111);
    layer->updateInfo();
}

// test:
#include "gameMiddle.h"
void CAsgardLayer::createChapterReward()
{
    
      CReward* chapter =  CExploration::getChapterReward();
     if (chapter)
     {
         char tips[200] = {0};
         int flag = chapter->excuteReward(ADD);
         sprintf(tips, "chapter reward add: ap: %d, gp: %d exp:%d, coin: %d, cash: %d card count: %d, prop count: %d", chapter->getEnergy(), chapter->getHP(), chapter->getExp(),
                     chapter->getCoin(), chapter->getCash(), chapter->getCardCount(), chapter->getPropCount());
         
         if(REWARD_IS_LEVEL_UP_SUCCESS(flag))
         {
             SinglePlayer::instance()->updatePlayerDataWithExp();
         }
         Middle::showAlertView(tips);
     }
}

void CAsgardLayer::initArsgard()
{
    // init data:
    m_nTouchTag  = -1;
    m_touchSprite = NULL;
    
    // init map
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "asgard.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY);
    createChapterReward();
    createHospitolButton();
    createFriendButton();
    createRankButton();
    createQianDaoButton();
    
 
}

void CAsgardLayer::createHospitolButton()
{
    pHospital= CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
    pHospital->setAnchorPoint(CCPointZero);
    pHospital->setPosition(ccp(500, 500));
    addChild(pHospital, 200, 999);
    string word = Utility::getWordWithFile("yiwushi");
    CCLabelTTF *label=CCLabelTTF::create(word.c_str(), "Arial", 20);
    label->setColor(g_custom_color[14]);
    label->setPosition(ccp(65, 30));
    pHospital->addChild(label,2,999);
   // pHospital->schedule(schedule_selector(CAsgardLayer::updateHospitolData), 1.0);
    Utility::addTouchRect(HOSPITAL_TOUCH_TAG, pHospital, m_cTouches);
}

void CAsgardLayer::createFriendButton()
{
    m_pFriendBtn= CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
    m_pFriendBtn->setAnchorPoint(CCPointZero);
    m_pFriendBtn->setPosition(ccp(650, 450));
    addChild(m_pFriendBtn, 200, 888);
    string word = Utility::getWordWithFile("friend");
    CCLabelTTF *label=CCLabelTTF::create(word.c_str(), "Arial", 20);
    label->setColor(g_custom_color[14]);
    label->setPosition(ccp(65, 30));
    m_pFriendBtn->addChild(label,2,999);
    Utility::addTouchRect(FRIEND_TOUCH_TAG, m_pFriendBtn, m_cTouches);

}

void CAsgardLayer::createQianDaoButton()
{

        m_pQianDao= CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
        m_pQianDao->setAnchorPoint(CCPointZero);
        m_pQianDao->setPosition(ccp(450, 560));
        addChild(m_pQianDao, 200, 77);
        string word = Utility::getWordWithFile("qiandao");
        CCLabelTTF *label=CCLabelTTF::create(word.c_str(), "Arial", 20);
        label->setColor(g_custom_color[14]);
        label->setPosition(ccp(65, 30));
        m_pQianDao->addChild(label,2,999);
        Utility::addTouchRect(QIANDAO_TOUCH_TAG, m_pQianDao, m_cTouches);
    
}
void CAsgardLayer::createRankButton()
{
    m_pRankBtn= CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
    m_pRankBtn->setAnchorPoint(CCPointZero);
    m_pRankBtn->setPosition(ccp(680, 560));
    addChild(m_pRankBtn, 200, 77);
    string word = Utility::getWordWithFile("rank");
    CCLabelTTF *label=CCLabelTTF::create(word.c_str(), "Arial", 20);
    label->setColor(g_custom_color[14]);
    label->setPosition(ccp(65, 30));
    m_pRankBtn->addChild(label,2,999);
    Utility::addTouchRect(RANK_TOUCH_TAG, m_pRankBtn, m_cTouches);
}



void CAsgardLayer::handlerTouch()
{
    CCLog("CAsgardLayer m_nTag : %d ", m_nTouchTag);
    if(m_nTouchTag != -1)
    {
        Utility::handleBtnCallBack(m_touchSprite, this, NULL);
    }
    switch (m_nTouchTag) {
        case GLADSHEIM_TOUCH_TAG:
        {
            CSceneActionGameLayer *active=CSceneActionGameLayer::create();
            addChild(active,1000,20000);
        }
            break;
        case HOSPITAL_TOUCH_TAG:
            createHospitalLayer();
            break;
        case FRIEND_TOUCH_TAG:
            createFriendLayer();
            break;
        case RANK_TOUCH_TAG:
            createRankLayer();
            break;
        case QIANDAO_TOUCH_TAG:
            createQianDaoLayer();
            break;
        case VALHALLA_TOUCH_TAG:
            // to do:
     //       SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_CARDSETTINGSCENE);
            SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_CARDSETTINGSCENE);
            break;
        case BIFROST_TOUCH_TAG:
            // to do:
           // createBiforestLayer();
            onClickBiforest();
            break;
        case DUPLICATE_TOUCH_TAG:
            // to do:
            showHandBook();
            break;
        default:
            break;
    }
}

void CAsgardLayer::createHospitalLayer()
{
    CCLayer *layer=CSceneHospitalLayer::create();
    addChild(layer,1000);
    CCLog("CSceneHospitalLayer create");

}
void CAsgardLayer::createFriendLayer()
{
    CCLayer *layer = CSceneFriendMainLayer::create();
    addChild(layer, 1000);
    
}

void CAsgardLayer::createQianDaoLayer()
{
    CEveryDayLoginLayer *pLayer=CEveryDayLoginLayer::create();
    addChild(pLayer,202,1);

}

void CAsgardLayer::createRankLayer()
{
    CRankDataManager::getInstance()->startRankDataManger();
    CCLayer *layer = CGameRankLayer::create();
    addChild(layer, 1000);
}
void CAsgardLayer::showHandBook()
{
    CCLayer *layer=CScreenHandBookLayer::create();
    addChild(layer,1000);
    CCLog("CScreenHandBookLayer create");
}

void CAsgardLayer::onClickBiforest()
{
    createBiforestLayer();
}