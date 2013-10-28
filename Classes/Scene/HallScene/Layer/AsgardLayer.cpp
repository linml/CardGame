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


CAsgardLayer::CAsgardLayer()
{
    m_bLoadTaskInfo = true;
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

void CAsgardLayer::createBiforestLayer()
{
   CCLayer*  layer = CBiforestLayer::create();
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
            // to do:
//            layer = CTaskLayer::create();
//            this->getParent()->addChild(layer, 30000, 6002);
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
            
            break;
        default:
            break;
    }
}


void CAsgardLayer::onClickBiforest()
{
    createBiforestLayer();
}