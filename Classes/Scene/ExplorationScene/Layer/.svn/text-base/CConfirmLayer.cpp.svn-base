//
//  CConfirmLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-14.
//
//

#include "CConfirmLayer.h"
#include "gameConfig.h"
#include "AsgardLayer.h"
#include "ExplorationScene.h"
#include "SceneManager.h"
#include "CFightLayerScene.h"
#include "CFightingLayerScene.h"

CConfirmLayer::CConfirmLayer()
{
    
}

CConfirmLayer::~CConfirmLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
}

bool CConfirmLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125,125,125,125)));
        
        initConfirm();
        bRet  = true;
    } while (0);
    return  bRet;
}

bool CConfirmLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CConfirmLayer::ccTouchBegan");

    return true;
}
void CConfirmLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CConfirmLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    handlerTouch();
}
void CConfirmLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CConfirmLayer::initConfirm()
{
    // init data:
    m_nTouchTag  = -1;
 
    
    // init map
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "confirm.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-2);
    
    m_cMaps->getElementByTags("2,1")->setVisible(false);
    m_cMaps->getElementByTags("2,2")->setVisible(false);

    //
    
    if (g_array[g_index] == 0 || g_array[g_index] == 1 || g_nLevle == 9)
    {
        m_bFight = true;
        m_cMaps->getElementByTags("2")->setVisible(false);
        m_cMaps->getElementByTags("3")->setVisible(true);
        
        CCNode * node = m_cMaps->getElementByTags("3,0,0");
        
        if (node)
        {
            CCSprite* enemy = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "red_wolf_1.png"));
            enemy->setAnchorPoint(CCPointZero);
            enemy->setPosition(ccp(65,25));
            node->addChild(enemy);
        }
       
        
    }else if(g_array[g_index] == 2 || g_array[g_index] == 3)
    {
         m_bFight = false;
         m_cMaps->getElementByTags("3")->setVisible(false);
         m_cMaps->getElementByTags("2")->setVisible(true);
        if (g_array[g_index] == 2)
        {
             m_cMaps->getElementByTags("2,2")->setVisible(true);
        }else
        {
            m_cMaps->getElementByTags("2,1")->setVisible(true);
        }
    }
   }


void CConfirmLayer::handlerTouch()
{
    
    if (m_bFight)
    {
        switch (m_nTouchTag)
        {
            case 2002:
                if (g_nLevle == 9)
                {
                  //  g_nLevle = 0;
                }else
                {
                  //  g_nLevle++;
                }
            SingleSceneManager::instance()->runSceneSelect(1007);
                removeFromParentAndCleanup(true);
                break;
            default:
                break;
        }

    }else
    {
        switch (m_nTouchTag)
        {
            case 2001:
                if (g_nLevle == 9)
                {
                    g_nLevle = 0;
                }else
                {
                    g_nLevle++;
                }
                CCDirector::sharedDirector()->replaceScene(CExploration::scene());
             //   SingleSceneManager::instance()->runTargetScene(EN_CURRSCENE_EXPLORATIONSCENE);
                removeFromParentAndCleanup(true);
                break;
            default:
                break;
        }

    }
    
}