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
        createAndUpdateLabel();
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

void CAsgardLayer::createAndUpdateLabel()
{
    //HP，体力，金币，现金币，领导力，等级
    //username
    CCSize wndSize=CCDirector::sharedDirector()->getWinSize();
    if (!getChildByTag(1000001))
    {
        CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 25);
        addChild(labelttf,3,1000001);
        labelttf->setPosition(ccp(100, 500));
        labelttf->setString(CCUserDefault::sharedUserDefault()->getStringForKey("name").c_str());
    }
    char data[30];
    // usercoin 金币
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000002);
        if (!getChildByTag(1000002))
        {
            labelttf=CCLabelTTF::create("", "Arial", 25);
            addChild(labelttf,3,1000002);
            labelttf->setPosition(ccp(300, 500));
        }
        sprintf(data, "金币:%d",SinglePlayer::instance()->getCoin());
        labelttf->setString(data);
        
    }
    //user exp;现金币
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000003);
        if (!getChildByTag(1000003))
        {
            labelttf=CCLabelTTF::create("", "Arial", 25);
            addChild(labelttf,3,1000003);
            labelttf->setPosition(ccp(500, 500));
        }
        sprintf(data, "现金:%d",SinglePlayer::instance()->getPlayerCash());
        labelttf->setString(data);
    }
    
    //user 体力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000004);
        if (!getChildByTag(1000004))
        {
            labelttf=CCLabelTTF::create("", "Arial", 25);
            addChild(labelttf,3,1000004);
            labelttf->setPosition(ccp(700, 500));
        }
        sprintf(data, "体力:%d",SinglePlayer::instance()->getPlayerAp());
        labelttf->setString(data);
    }
    
    //user 领导力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000005);
        if (!getChildByTag(1000005))
        {
            labelttf=CCLabelTTF::create("", "Arial", 25);
            addChild(labelttf,3,1000005);
            labelttf->setPosition(ccp(900, 500));
        }
        sprintf(data, "领导力:%d",SinglePlayer::instance()->getRVC());
        labelttf->setString(data);
    }
    // 等级
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000006);
        if (!getChildByTag(1000006))
        {
            labelttf=CCLabelTTF::create("", "Arial", 25);
            addChild(labelttf,3,1000006);
            labelttf->setPosition(ccp(900, 600));
        }
        sprintf(data, "等级:%d",SinglePlayer::instance()->getPlayerLevel());
        labelttf->setString(data);
    }
    
    // 体力
    {
        CCLabelTTF *labelttf=(CCLabelTTF *)getChildByTag(1000007);
        if (!getChildByTag(1000007))
        {
            labelttf=CCLabelTTF::create("", "Arial", 25);
            addChild(labelttf,3,1000007);
            labelttf->setPosition(ccp(700, 600));
        }
        sprintf(data, "神力:%d",SinglePlayer::instance()->getPlayerGp());
        labelttf->setString(data);
    }

}

// test:
#include "gameMiddle.h"
void CAsgardLayer::createChapterReward()
{
    
      CReward* chapter =  CExploration::getChapterReward();
     if (chapter)
     {
         char tips[200] = {0};
         chapter->excuteReward(ADD);
         sprintf(tips, "chapter reward add: ap: %d, gp: %d exp:%d, coin: %d, cash: %d card count: %d, prop count: %d", chapter->getEnergy(), chapter->getHP(), chapter->getExp(),
                     chapter->getCoin(), chapter->getCash(), chapter->getCardCount(), chapter->getPropCount());

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
    CCLayer * layer = NULL;
    CCLog("CAsgardLayer m_nTag : %d ", m_nTouchTag);
    if(m_nTouchTag != -1)
    {
        Utility::handleBtnCallBack(m_touchSprite, this, NULL);
    }
    switch (m_nTouchTag) {
        case GLADSHEIM_TOUCH_TAG:
            // to do:
            layer = CTaskLayer::create();
            this->getParent()->addChild(layer, 30000, 6002);
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

void CAsgardLayer::callBack(float dt)
{

    if(SinglePlayer::instance()->getLoadTaskInfo())
    {
        m_bLoadTaskInfo = true;
        createBiforestLayer();
        this->unschedule(schedule_selector(CAsgardLayer::callBack));
    }

}

void CAsgardLayer::onClickBiforest()
{
    m_bLoadTaskInfo = false;
    SinglePlayer::instance()->onGetTaskInfo();
    schedule(schedule_selector(CAsgardLayer::callBack), 0.2);
    
    
}