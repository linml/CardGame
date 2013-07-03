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
        bRet = true;
    } while (0);
    return bRet;
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
    handlerTouch();
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

void CHallScene::handlerTouch()
{
    CCLog("touch tag : %d", m_nTouchTag);
    switch (m_nTouchTag)
    {
        case 2001:
            
            break;
            
        case 2002:
            
            break;
        case 2003:
            
            break;
        case 2004:
            
            break;
        case 2005:
            
            break;
        case 2006:
            
            break;
        case 2007:
            
            break;
        case 2008:
            
            break;
        default:
            break;
    }
}