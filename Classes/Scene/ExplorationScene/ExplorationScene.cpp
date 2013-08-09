//
//  ExplorationScene.cpp
//  91.cube
//
//  Created by phileas on 13-6-9.
//
//

#include "ExplorationScene.h"
#include "gameConfig.h"
#include "HallScene.h"
#include "CConfirmLayer.h"
#include "SceneManager.h"
#include "Utility.h"

// test:
int g_nLevle = 0;
int g_array[3]={0};
int g_index = -1;



CCScene* CExploration::scene()
{
    CCScene *scene = CCScene::create();
    CExploration *layer = CExploration::create();
    if (layer)
    {
        scene->addChild(layer);
    }
    return scene;
}

CExploration::CExploration()
{
    for (int i = 0; i < 3; i++)
    {
        m_pBtn[i] = NULL;
    }
}

CExploration::~CExploration()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    
}


bool CExploration::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        // todo:
        CC_BREAK_IF(!initExploration());
        bRet = true;
    } while (0);
    return bRet;
}

bool CExploration::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    // todo:
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag =  TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    if (m_nTouchTag != -1)
    {
        switch (m_nTouchTag)
        {
            case 3001:
            case 3002:
            case 3003:
                m_pBtn[m_nTouchTag-3001]->setPress();
                break;
            default:
                break;
        }
        
        return true;
    }
    return false;

}

void CExploration::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CExploration::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CExploration::ccTouchEnded");
    CCPoint touchPoint = pTouch->getLocation();
    
    if (m_nTouchTag == TouchRect::SearchTouchTag(touchPoint, m_cTouches))
    {
        handlerTouch();
    }else
    {
        switch (m_nTouchTag)
        {
            case 3001:
            case 3002:
            case 3003:
                m_pBtn[m_nTouchTag-3001]->setNormal();
                break;
            default:
                break;
        }
        
    }
    
    
}

void CExploration::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
   
}


// protectd method:

bool CExploration::initExploration()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    bool bRet = false;
    do {
        // init data:
        m_nTouchTag = -1;
        
        // init map:
        m_cMaps = LayoutLayer::create();
        m_cMaps->retain();
        m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "exploration.plist"));
        
        CCNode *node = m_cMaps->getElementByTags("0");
        if (node) {
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCSprite *sprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "ironforest.png"));
            sprite->setPosition(ccp(size.width/2, size.height/2));
            node->addChild(sprite, -1);
        }
        
        // add the bg,maybe it will be separate to a layer
        
        node = NULL;
        node  = m_cMaps->getElementByTags("0,0,143");
        if (node) {
            node->setVisible(false);
            
            
        }
        
        
        
        // add buttons:
        CCSprite *btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
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
        
        char buffer[10] = {0};
        sprintf(buffer, "%d/10",g_nLevle+1);
        btnActivity = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "mission.png"));
        btnActivity->setPosition(ccp(size.width/2, btnActivity->getContentSize().height/2 +15.0f));
        pLabel = CCLabelTTF::create(buffer, "Scissor Cuts", 20);
        pLabel->setPosition(ccp(150,60));
        btnActivity->addChild(pLabel);
        this->addChild(btnActivity, 200, 2004);
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

        
        // add CPtButton:
        CCLayer * outLayer = CCLayer::create();
        addChild(outLayer, 10, 100);
        
        for (int i = 0; i < 3; i++)
        {
            m_pBtn[i] = CPtButtonWidget::create("");
            m_pBtn[i]->setAnchorPoint(CCPointZero);
            m_pBtn[i]->setPosition(ccp(180+290*i, 380));
            outLayer->addChild(m_pBtn[i], 3001+i, 200);
            Utility::addTouchRect(3001+i, m_pBtn[i], m_cTouches);
        }
       
        
        
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
        setTouchPriority(-1);

        
        // init arrows:
        
        randonArrows(g_nLevle);
        bRet = true;
    } while (0);
    
    return bRet;
}


void CExploration::handlerTouch()
{
    
    CCLog("CExploration:%d, %d", m_nTouchTag, g_nLevle
          );
    
    if (g_nLevle == 9 )
    {
        if (m_nTouchTag == CENTER_TOUCH_TAG)
        {
            CCLog("CExploration:: center.. break");
            attachConfirm();
        }else if(m_nTouchTag == 2008)
        {
//            CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CHallScene::scene()));
            SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE);
            
        }
            
        return;
        
    }
    
    switch (m_nTouchTag) {
        case LEFT_TOUCH_TAG:
            CCLog("CExploration:: left");
            g_index = 0;            // to do:
            m_pBtn[m_nTouchTag-3001]->setPress();
            attachConfirm();
            break;
        case CENTER_TOUCH_TAG:
            g_index = 1;
            CCLog("CExploration:: center");
            // to do:
            m_pBtn[m_nTouchTag-3001]->setPress();
            attachConfirm();
            break;
            
        case RIGHT_TOUCH_TAG:
            CCLog("CExploration:: right");
            m_pBtn[m_nTouchTag-3001]->setPress();
            g_index = 2;
            attachConfirm();
            // to do:
            break;
            
        case 2008:
            //CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CHallScene::scene()));
            SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_HALLSCENE);
            
            break;
        default:
            break;
    }
}

void CExploration::randonArrows(const int inLevle)
{
    
    char buffer[50] = {};
    
    int wordInex = 0;
    srand(time(0));
    
    if (inLevle >= 0 && inLevle < 9)
    {
        for (int i = 0; i < 3; i++)
        {
            m_pBtn[i]->setType(rand()%4);
            wordInex = (rand()%30)+1;
            sprintf(buffer, "%d", wordInex);
            g_array[i] = m_pBtn[i]->getType();
            m_pBtn[i]->setText(Utility::getWordWithFile("tips.plist", buffer).c_str());
        }
        
        
    }else if(g_nLevle == 9)
    {
        m_pBtn[0]->setVisible(false);
        m_pBtn[2]->setVisible(false);
        m_pBtn[1]->setType(1);
        g_array[1] = m_pBtn[1]->getType();
        
        wordInex = (rand()%30)+1;
        sprintf(buffer, "%d", wordInex);
        m_pBtn[1]->setText(Utility::getWordWithFile("tips.plist", buffer).c_str());
    }
    
    
}

void CExploration::attachConfirm()
{
    CCNode *node = NULL;
    node = getChildByTag(100);
    if(node)
    {
        CCLog("the ...");
      //  removeChild(node, true);
        node->setVisible(false);
        CConfirmLayer *layer = CConfirmLayer::create();
        this->addChild(layer, 5, 10);
    }
    
    
}