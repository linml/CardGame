//
//  CSceneGameShopLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-14.
//
//

#include "CSceneGameShopLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"

CSceneGameShopLayer::CSceneGameShopLayer()
{
     CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
}

CSceneGameShopLayer::~CSceneGameShopLayer()
{
     CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
}

void CSceneGameShopLayer::createQuNiuDanButton()
{
    string word = Utility::getWordWithFile("word.plist", "quniudan");
    
    CGameButtonControl *sprite=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
  
    CCNode *node=Utility::getNodeByTag(this, "1,1,2");
    CCPoint point;
    if(node)
    {
        sprite->setPosition(node->getPosition());
        sprite->setAnchorPoint(node->getAnchorPoint());
        addChild(sprite,node->getZOrder(),7);
        node->removeFromParentAndCleanup(true);
    }
   
}
void CSceneGameShopLayer::createQuChongZhi()
{
    string word = Utility::getWordWithFile("word.plist", "quchongzhi");
    
    CGameButtonControl *sprite=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    CCNode *node=Utility::getNodeByTag(this, "1,1,3");
    if(node)
    {
        sprite->setPosition(node->getPosition());
        sprite->setAnchorPoint(node->getAnchorPoint());
        addChild(sprite,node->getZOrder(),8);
        node->removeFromParentAndCleanup(true);

    }
}

bool  CSceneGameShopLayer::init()
{
    //his->m_bTimerStatus=EN_GLOBALTIMER_STOP;
  
    long value=clock();
    LayoutLayer *m_cMaps= LayoutLayer::create();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "shangdianjiemian.plist"));
    CCLog("stop:%f",(clock()-value)*1.0/CLOCKS_PER_SEC);
    createQuNiuDanButton();
    createQuChongZhi();
    setTouchEnabled(true);    
    
    return true;
}


bool CSceneGameShopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCLog("node=%x",node);
    
    CCPoint touchPoint=pTouch->getLocation();
    m_oldPoint=touchPoint;
    if(node && node->boundingBox().containsPoint(touchPoint))
    {
        m_tempTouchNode=node;
    }
    else if(getChildByTag(7) && getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        ((CGameButtonControl *)getChildByTag(7))->selected();
        m_tempTouchNode=getChildByTag(7);
    }
    else if(getChildByTag(8) && getChildByTag(8)->boundingBox().containsPoint(touchPoint))
    {
        ((CGameButtonControl *)getChildByTag(8))->selected();
        m_tempTouchNode=getChildByTag(8);
    }
    return true;
}

void CSceneGameShopLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCPoint touchPoint=pTouch->getLocation();
    if(node&&node->boundingBox().containsPoint(pTouch->getLocation()))
    {
        if(m_tempTouchNode!=node)
        {
            if(m_tempTouchNode==getChildByTag(7))
            {
                ((CGameButtonControl *)getChildByTag(7))->unselected();
            }
            m_tempTouchNode=node;
        }
    }
    else if(getChildByTag(7) && getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        if( m_tempTouchNode && m_tempTouchNode!=Utility::getNodeByTag(this, "1,1,1"))
        {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
        }
        ((CGameButtonControl *)getChildByTag(7))->selected();
         m_tempTouchNode=getChildByTag(7);
    } else if(getChildByTag(8)->boundingBox().containsPoint(touchPoint))
    {
        if( m_tempTouchNode && m_tempTouchNode!=Utility::getNodeByTag(this, "1,1,1"))
        {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
        }
        ((CGameButtonControl *)getChildByTag(8))->selected();
        m_tempTouchNode=getChildByTag(8);
    }
    else
    {
        if (m_tempTouchNode==getChildByTag(7)||m_tempTouchNode==getChildByTag(8)) {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
            m_tempTouchNode=NULL;
        }
    }
}
void CSceneGameShopLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCPoint touchPoint=pTouch->getLocation();
    m_newPoint=touchPoint;
    CCLog("%f,%f,%f",m_oldPoint.x,m_newPoint.x,ccpDistance(m_oldPoint, m_newPoint));
    m_oldPoint=m_newPoint;
    if(node&&node->boundingBox().containsPoint(touchPoint))
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        if(m_tempTouchNode==node)
        {
            removeFromParentAndCleanup(true);
        }
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint)|| getChildByTag(8)->boundingBox().containsPoint(touchPoint))
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        if (m_tempTouchNode!=node &&m_tempTouchNode) {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
        }
        m_tempTouchNode=NULL;
    }
    else if (m_tempTouchNode==getChildByTag(7)||m_tempTouchNode==getChildByTag(8)) {
        ((CGameButtonControl *)m_tempTouchNode)->unselected();
        m_tempTouchNode=NULL;
    }
   
}
void CSceneGameShopLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CSceneGameShopLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-8, true);
}
void CSceneGameShopLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}