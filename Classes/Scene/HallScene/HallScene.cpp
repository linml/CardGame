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
#include "AsgardLayer.h"
#include "WordCache.h"
#include "CPtScrollWordsWidget.h"
#include "CBackpackContainerLayer.h"
#include "CGameEmailTableView.h"
#include "CGameEmailManager.h"
#include "SceneManager.h"
#include "CSceneGameShopLayer.h"
#include "CPtRecharge.h"
#include "CDrawCardLayer.h"
#include "CGamePropBufferTipLayer.h"
#include "CAnnouncementLayer.h"
#include "CEveryDayLoginLayer.h"

#define  EMAILMAXNUMBERCOUNT 50
#define  EMAILTISHI 40



/*
 * @param pType: 1: create hall with biforest layer:
 */

CCScene *CHallScene::scene(int pType)
{
  
    CCScene *scene = CCScene::create();
    CHallScene *layer = CHallScene::create(pType);
    scene->addChild(layer, 0, 100);
    
    CAnnouncementLayer * gamelayer=CAnnouncementLayer::create();
    CCSize size=CCDirector::sharedDirector()->getWinSize();
    gamelayer->setPosition(ccp(size.width*0.5,size.height*0.5+200));
    layer->addChild(gamelayer,1);

    return scene;
    
}

CHallScene * CHallScene::create(int pType)
{
    CHallScene *pRet = new CHallScene();
    if (pRet && pRet->init(pType))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }

}

CHallScene::CHallScene()
{
    m_pPropBuffNode = NULL;
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CHallScene::updateEmailNumber), "youjiangengxin", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CHallScene::showBackNotice), "CAONIMAXIANSHIBEIBAO", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CHallScene::showRechargeView), "CHONGZHIJIEMIAN", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CHallScene::showNiuDanJieMian), "NIUDANJIEMIAN", NULL);
}

CHallScene::~CHallScene()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "youjiangengxin");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CAONIMAXIANSHIBEIBAO");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,  "NIUDANJIEMIAN");
    //CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CHONGZHIJIEMIAN");
    m_cplist->release();
    //clean the cache:
    HBSpriteCache::sharedHBSpriteCache()->purgesharedHBSpriteCache();
    MapLayerDataCache::sharedMapLayerDataCache()->removeAll();
}


bool CHallScene::init(int inType)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        CC_BREAK_IF(!initHall(inType));
        createEmailNumberUnread();
        if (SinglePlayer::instance()->m_bIsLogin) {
           // createEveryDataLogin();
            SinglePlayer::instance()->m_bIsLogin=false;
        }
        bRet = true;
    } while (0);
    return bRet;
}

bool CHallScene::createEveryDataLogin()
{
    //签到的layer
    CEveryDayLoginLayer *pLayer=CEveryDayLoginLayer::create();
    addChild(pLayer,202,1);
    CCLayerColor* bg = CCLayerColor::create(ccc4(0, 0, 0, 200));
    pLayer->addChild(bg, -1);
    
    return true;
    
}

void CHallScene::onClickQuitGame()
{
    // release source:
    if (m_pPropBuffNode)
    {
        m_pPropBuffNode->removeFromParentAndCleanup(true);
    }
    SinglePlayer::instance()->onExitGameApp();
    SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_LOGINSCENE);
}


void CHallScene::createEmailNumberUnread()
{
    int getUnreadCount=G_GAMESINGEMAIL::instance()->getCurrentEmailUnreadCount();
    char data[4];
    sprintf(data, "%d",getUnreadCount);
    
    if(!getChildByTag(601))
    {
        CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "emailUnreadNumber.png"));
        if(getChildByTag(600)){
            CCSprite *nodetemp=(CCSprite *)getChildByTag(600);
            CCPoint point=getChildByTag(600)->getPosition();
            int width=nodetemp->boundingBox().size.width;
            int height=nodetemp->boundingBox().size.height;
            sprite->setPosition(ccp(point.x+width-20,point.y+height-20));
            addChild(sprite,201,601);
        }
    }
    if(!getChildByTag(603))
    {
        CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 15);
        labelttf->setColor(g_custom_color[0]);
        addChild(labelttf,201,603);
        CCPoint point=getChildByTag(600)->getPosition();
        labelttf->setPosition(ccp(point.x,point.y-20));
    }
    if(G_GAMESINGEMAIL::instance()->getMailCount()>=EMAILTISHI)
    {
        if(!getChildByTag(600)->getChildByTag(604))
        {
            string word=Utility::getWordWithFile("word.plist", "youjiankuaimanle");
            GameTools::createTip(getChildByTag(600),word.c_str(),CCSizeMake(50,-5),1,604);
        }
    }
    else
    {
        if( getChildByTag(600)->getChildByTag(604))
        {
           getChildByTag(600)->removeChildByTag(604,true);
        }
    }
    if(!getChildByTag(602))
    {
        CCLabelTTF *labelttf=CCLabelTTF::create("", "Arial", 15);
        labelttf->setColor(g_custom_color[0]);
        addChild(labelttf,201,602);
        labelttf->setPosition(getChildByTag(601)->getPosition());
    }
    if(getUnreadCount==0)
    {
        getChildByTag(601)->setVisible(false);
        getChildByTag(602)->setVisible(false);
    }
    else
    {
        if(getUnreadCount>=EMAILMAXNUMBERCOUNT)
        {
            sprintf(data, "%s","MAX");
        }
        CCLabelTTF *labelttf=(CCLabelTTF *)(getChildByTag(602));
        labelttf->setString(data);
    }    
}
void CHallScene::updateEmailNumber(CCObject *object)
{
    createEmailNumberUnread();
}

bool CHallScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if(m_nTouchTag != -1)
    {
        return true;
    }
    
    return false;

}

void CHallScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CHallScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    if (m_nTouchTag ==TouchRect::SearchTouchTag(touchPoint, m_cTouches, &m_touchSprite))
    {
        handlerTouch();
    }

}
void CHallScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CHallScene::showBackNotice(cocos2d::CCObject *object)
{
    showBackBag();
}
void CHallScene::callBackToShowRechargeDialog(cocos2d::CCObject *object)
{
    showRechargeView();
}




void CHallScene::showBackBag()
{
    CCLayer * layer = CBackpackContainerLayer::create();
    addChild(layer, 1000);
    CCLog("backpack...");
}


void CHallScene::showNiuDanJieMian()
{
    CDrawCardLayer * layer = CDrawCardLayer::create();
    addChild(layer, 10000);
}

void CHallScene::showRechargeView()
{
    CPtRecharge* lRecharge = CPtRecharge::create();
    addChild(lRecharge,10000);
}

void CHallScene::createPropBufferTips()
{
    CGamePropBufferTipLayer *layer = CGamePropBufferTipLayer::create();
    addChild(layer);
    m_pPropBuffNode = layer;
}

// protectd method:
/*
 * @param inType : 1--> normalexploration, 2--> activityexploration
 */
bool CHallScene::initHall(int inType)
{
    bool bRet = false;
    
    m_touchSprite = NULL;
    
    do {
        CCSize size =  CCDirector::sharedDirector()->getWinSize();
        CCLayer *layer = CCLayer::create();
        m_cplist = LayoutLayer::create();
        m_cplist->retain();
        m_cplist->initWithFile(layer, CSTR_FILEPTAH(plistPath, "hall.plist"));
        
        char fileName[64]="";
        sprintf(fileName, "%sheadIcon_%d_h.png",g_mapImagesPath.c_str(),SinglePlayer::instance()->getPlayerGender());
        //没有资源
        //CCSprite* playIcon = CCSprite::create(fileName);
        //playIcon->setPosition(ccp(80, size.height-80));
        //addChild(playIcon,100);
        
        // add the bg,maybe it will be separate to a layer
        CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "hall.png"));
        bg->setPosition(ccp(size.width/2, size.height/2));

        CCNode * node  = m_cplist->getElementByTags("0");
        if (node) {
            node->addChild(bg, -1);
        }
        
        node = NULL;
        node  = m_cplist->getElementByTags("0,0,143");
        if (node) {
            node->setVisible(false);
          
            
        }

    
        
        // add buttons:
        CCSprite *btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
        if (btnActivity)
        {
            btnActivity->setAnchorPoint(CCPointZero);
            btnActivity->setPosition(ccp(850, 680));
            this->addChild(btnActivity, 200, 2001);
            Utility::addTouchRect(2001, btnActivity, m_cTouches);
            
            string word = Utility::getWordWithFile("word.plist", "quit");
            CCLog("word :%s",word.c_str());
            CCLabelTTF* pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setPosition(ccp(65,30));
            btnActivity->addChild(pLabel);
            
            
            btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
            btnActivity->setAnchorPoint(CCPointZero);
            btnActivity->setPosition(ccp(720, 680));
            this->addChild(btnActivity, 200, 600);
            Utility::addTouchRect(2002, btnActivity, m_cTouches);
            
            word = Utility::getWordWithFile("word.plist", "email");
            CCLog("word :%s",word.c_str());
            pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setPosition(ccp(65,30));
            btnActivity->addChild(pLabel);
            
            btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
            btnActivity->setAnchorPoint(CCPointZero);
            btnActivity->setPosition(ccp(590, 680));
            this->addChild(btnActivity, 200, 2003);
            Utility::addTouchRect(2003, btnActivity, m_cTouches);
            
            word = Utility::getWordWithFile("word.plist", "mall");
            CCLog("word :%s",word.c_str());
            pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setPosition(ccp(65,30));
            btnActivity->addChild(pLabel);
            
            // bottom buttons:
            word = Utility::getWordWithFile("word.plist", "synopsis");
            CScrollWordSprite *scrollWord = CScrollWordSprite::create(word.c_str(), "arial",20, CCSizeMake(250, 0), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
            scrollWord->setScroll(true);
            scrollWord->setScrollRect(CCRectMake(0, 0, 250, 60));
            scrollWord->setPosition(ccp(30, 20));
            scrollWord->setScrollStep(0.50f);
            scrollWord->setAnchorPoint(CCPointZero);

            btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "mission.png"));
            
            btnActivity->setPosition(ccp(size.width/2, btnActivity->getContentSize().height/2 +15.0f));
            this->addChild(btnActivity, 200, 2004);
            btnActivity->addChild(scrollWord);
            Utility::addTouchRect(2004, btnActivity, m_cTouches);
            
            
            btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
            btnActivity->setPosition(ccp(size.width-230, btnActivity->getContentSize().height/2 +15.0f));
            this->addChild(btnActivity, 200, 2005);
            Utility::addTouchRect(2005, btnActivity, m_cTouches);
            
            word = Utility::getWordWithFile("word.plist", "voltron");
            CCLog("word :%s",word.c_str());
            pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setPosition(ccp(65,30));
            btnActivity->addChild(pLabel);
            
            btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
            btnActivity->setPosition(ccp(size.width-100, btnActivity->getContentSize().height/2 +15.0f));
            this->addChild(btnActivity, 200, 2006);
            Utility::addTouchRect(2006, btnActivity, m_cTouches);
            
            
            word = Utility::getWordWithFile("word.plist", "packsack");
            CCLog("word :%s",word.c_str());
            pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setPosition(ccp(65,30));
            btnActivity->addChild(pLabel);
            
            btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button2.png"));
            
            btnActivity->setPosition(ccp(330, btnActivity->getContentSize().height/2+25.0f ));
            this->addChild(btnActivity, 200, 2007);
            Utility::addTouchRect(2007, btnActivity, m_cTouches);
            
            word = Utility::getWordWithFile("word.plist", "indeterminate");
            CCLog("word :%s",word.c_str());
            pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setDimensions(CCSizeMake(1, 100));
            pLabel->setPosition(ccp(25,18));
            btnActivity->addChild(pLabel);
            
            
            btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button2.png"));
            btnActivity->setPosition(ccp(695, btnActivity->getContentSize().height/2+25.0f ));
            this->addChild(btnActivity, 200, 2008);
            Utility::addTouchRect(2008, btnActivity, m_cTouches);
            
            word = Utility::getWordWithFile("word.plist", "back");
            CCLog("word :%s",word.c_str());
            pLabel = CCLabelTTF::create(word.c_str(), "Scissor Cuts", 20);
            pLabel->setDimensions(CCSizeMake(1, 100));
            pLabel->setPosition(ccp(25,18));
            btnActivity->addChild(pLabel);
            
            CCSprite * card;
            // add card:
            for (int i = 0; i < 5; i++)
            {
                card = NULL;
                card = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "d_card.png"));
                if (card)
                {
                    card->setAnchorPoint(CCPointZero);
                    card->setPosition(ccp(160-i*30, 20));
                    card->setScale(0.35f);
                    this->addChild(card, 200, 30+i);
                }
                
            }

        }
               
        this->addChild(layer);
        
        CAsgardLayer *asgard = CAsgardLayer::create();
        this->addChild(asgard, 200);
        if (inType == 1)
        {
            asgard->createBiforestLayer();
        }else if(inType == 2)
        {
            asgard->createBiforestLayer(ACTIVITYEXPLORATION);
        }
        m_pAsgardLayer = asgard;
        createPropBufferTips();
        
        // set touch
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
        setTouchPriority(GOLDPLACE_TOUCH_PRORITY+1);
        
        
        bRet = true;
    } while (0);
    
    return bRet;
}
void CHallScene::createEmailLayer()
{
    if(getChildByTag(88888))
    {
        removeChildByTag(88888, true);
    }
    CGameEmailTableView *gameEmailLayer=CGameEmailTableView::CreateEmailLayer();
    //gameEmailLayer->setScale(0.01);
    //gameEmailLayer->setTouchEnabled(false);
    addChild(gameEmailLayer,900,88888);
    //gameEmailLayer->runAction(CCSequence::create(CCEaseBounceOut::create(CCScaleTo::create(0.8, 1.0)),CCCallFuncND::create(this, callfuncND_selector(CHallScene::setHallSceneLayerCanTouch), (void*)getChildByTag(88888)),NULL));
    
}
void CHallScene::setHallSceneLayerCanTouch(void *tag)
{
    CCNode *node=static_cast<CCNode *>(tag);
    switch (node->getTag()) {
        case 88888:
            ((CGameEmailTableView *)getChildByTag(88888))->setTouchEnabled(true);
            break;
            
        default:
            break;
    }
    
}
void CHallScene::handlerTouch()
{
    //CCLayer * layer = NULL;
    CCLog("touch tag : %d", m_nTouchTag);
    if (m_nTouchTag != -1) {
        Utility::handleBtnCallBack(m_touchSprite, this, NULL);
    }
    switch (m_nTouchTag)
    {
        case 2001:
            // quit game:
            onClickQuitGame();
            break;            
        case 2002:
            createEmailLayer();
            break;
        case 2003:
            if(!getChildByTag(17777))
            {
                addChild(CSceneGameShopLayer::create(),201,17777);
            }
            break;
        case 2004:
            
            break;
        case 2005:

            SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_CARDSETTINGSCENE);
            break;
        case 2006:
            // backpack-->
            showBackBag();
            break;
        case 2007:
            showRechargeView();
            break;
        case 2008:
            
            break;
        default:
            break;
    }
}