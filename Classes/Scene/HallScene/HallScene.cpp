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
#include "CGameEmailLayer.h"
#include "CGameEmailManager.h"
#define  EMAILMAXNUMBERCOUNT 50
#define  EMAILTISHI 20

CCScene* CHallScene::scene()
{
    CCScene *scene = CCScene::create();
    CHallScene *layer = CHallScene::create();
    scene->addChild(layer);
    return scene;
}

CHallScene::CHallScene()
{
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CHallScene::updateEmailNumber), "youjiangengxin", NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CHallScene::showBackNotice), "CAONIMAXIANSHIBEIBAO", NULL);
}

CHallScene::~CHallScene()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "youjiangengxin");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,"CAONIMAXIANSHIBEIBAO");
    m_cplist->release();
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
        createEmailNumberUnread();
        bRet = true;
    } while (0);
    return bRet;
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

void CHallScene::showBackBag()
{
       CCLayer * layer = CBackpackContainerLayer::create();
    addChild(layer, 1000);
    CCLog("backpack...");
}
// protectd methdo:
bool CHallScene::initHall()
{
    bool bRet = false;
    
    m_touchSprite = NULL;
    
    do {
        CCSize size =  CCDirector::sharedDirector()->getWinSize();
        CCLayer *layer = CCLayer::create();
        m_cplist = LayoutLayer::create();
        m_cplist->retain();
        m_cplist->initWithFile(layer, CSTR_FILEPTAH(plistPath, "hall.plist"));
        
        
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
            
            string word = Utility::getWordWithFile("word.plist", "option");
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
            
            word = Utility::getWordWithFile("word.plist", "activity");
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
    CGameEmailLayer *gameEmailLayer=CGameEmailLayer::CreateEmailLayer();
    addChild(gameEmailLayer,900);
    
}
void CHallScene::handlerTouch()
{
    CCLayer * layer = NULL;
    CCLog("touch tag : %d", m_nTouchTag);
    if (m_nTouchTag != -1) {
        Utility::handleBtnCallBack(m_touchSprite, this, NULL);
    }
    switch (m_nTouchTag)
    {
        case 2001:
            
            break;            
        case 2002:
            createEmailLayer();
            break;
        case 2003:
            
            break;
        case 2004:
            
            break;
        case 2005:
            
            break;
        case 2006:
            // backpack-->
            showBackBag();
            break;
        case 2007:
            
            break;
        case 2008:
            
            break;
        default:
            break;
    }
}