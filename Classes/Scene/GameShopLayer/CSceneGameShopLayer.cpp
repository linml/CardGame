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
#include "cocos-ext.h"
#include "CCScrollView.h"
#include "gamePlayer.h"
#include "CStructShopSellItem.h"

#include "CGameRechargeLayer.h"
#include "CGameRechargeTip.h"
#include "CGameArrageBackpackTip.h"
#include "CReward.h"

#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "Utility.h"
#include "PtJsonUtility.h"

#include "CPtRecharge.h"
#include "CDrawCardLayer.h"

#include "CSceneGameSingleShopLayer.h"


#define TAG_SHOPITEM 1000
using namespace::cocos2d;
using namespace extension;


CSceneGameShopLayer::CSceneGameShopLayer()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"shangchenganniu.plist"));
     CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
    //m_enHttpReuslt=EN_LOADING_HTTPRESUALT_NONE;
//    m_nUpdateTime=INT_MAX;
    m_pPlayer = SinglePlayer::instance();
    m_tabArray=CCArray::create();
    m_tabArray->retain();
    initLayerData();
}
void CSceneGameShopLayer::initLayerData()
{
    for (map<int , CStructShopInfo *>::iterator it=m_pPlayer->m_gameShopManager->m_gameShopList.begin(); it!=m_pPlayer->m_gameShopManager->m_gameShopList.end();it++) {
        if (it->second) {
            m_vSaveShop.push_back(it->second);
            m_pPlayer->setReBackXiangLiang(it->second->getShopId());
        }
    }
}
CSceneGameShopLayer::~CSceneGameShopLayer()
{
    m_tabArray->release();
    CEmrysClearVectorMemory< CStructShopInfo  *> tempClear(m_vSaveShop) ;
    tempClear.clearVector();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"));

    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"shangchenganniu.plist"));
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
    m_currentTabIndex=-1;
    long value=clock();
    LayoutLayer *m_cMaps= LayoutLayer::create();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "shangdianjiemian.plist"));
    createQuNiuDanButton();
    createQuChongZhi();
    createShopTitle();
    updateQianBi();
    swithChangeTab(0);
    schedule(schedule_selector(CSceneGameShopLayer::schudelUpdateQianbi), 2.0);
    CCLog("stop:%f",(clock()-value)*1.0/CLOCKS_PER_SEC);
    setTouchEnabled(true);
    return true;
}

void CSceneGameShopLayer::swithChangeTab(int changeTab)
{
    
    if (m_currentTabIndex==changeTab) {
        return ;
    }
    if (changeTab>=0 &&changeTab<=m_tabArray->count()) {
        if (m_currentTabIndex!=-1) {
            CGameButtonControl *item=(CGameButtonControl *)m_tabArray->objectAtIndex(m_currentTabIndex);
            item->unselected();
        }
        m_currentTabIndex=changeTab;
        CGameButtonControl *item=(CGameButtonControl *)m_tabArray->objectAtIndex(m_currentTabIndex);
        item->selected();
        createShopItemLayer(m_vSaveShop[m_currentTabIndex]);
    }

}

void CSceneGameShopLayer::createShopItemLayer(CStructShopInfo *info)
{
    if (getChildByTag(TAG_SHOPITEM)) {
        removeChildByTag(TAG_SHOPITEM, true);//, <#bool cleanup#>
    }
    CSceneGameSingleShopLayer *layer=CSceneGameSingleShopLayer::Create(info);
    addChild(layer,3,TAG_SHOPITEM);
    
    
}



int CSceneGameShopLayer::getCheckTouchTab(CCPoint point)
{
    for (int i=0; i<m_tabArray->count(); i++) {
        CGameButtonControl * itemTab=(CGameButtonControl *)m_tabArray->objectAtIndex(i);
        if (itemTab->boundingBox().containsPoint(point)){
            return i;
        }
    }
    return -1;
}
void CSceneGameShopLayer::createShopTitle()
{
    CCLOG("shangdian geshu :%d",m_vSaveShop.size());
    for (int i=0; i<m_vSaveShop.size(); i++)
    {
        CGameButtonControl *gamebutton=CGameButtonControl::createButton(TEXTMID, m_vSaveShop[i]->getShopName().c_str(), "tabButton_normal.png", "tabButton_selected.png");
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        gamebutton->setPosition(ccp(150+i*130,700));
        addChild(gamebutton,2);
        m_tabArray->addObject(gamebutton);
    }
}

void CSceneGameShopLayer::schudelUpdateQianbi(float t)
{
    if (getChildByTag(114))
        {
            char data[20];
            sprintf(data, "%d",m_pPlayer->getPlayerCash());
            ((CCLabelTTF *)getChildByTag(114))->setString(data);
    }
    

}
void CSceneGameShopLayer::updateQianBi()
{
    if (!getChildByTag(114))
    {
        CCSprite *sprite=CCSprite::createWithSpriteFrameName("zuanshi.png");
        addChild(sprite,2,113);
        sprite->setPosition(ccp(480,700));
        CCLabelTTF *labelTTF=CCLabelTTF::create("", "Arial", 20);
        labelTTF->setColor(g_custom_color[3]);
        labelTTF->setPosition(ccp(540, 700));
        addChild(labelTTF,2,114);
    }
    char data[20];
    sprintf(data, "%d",m_pPlayer->getPlayerCash());
    ((CCLabelTTF *)getChildByTag(114))->setString(data);
}

bool CSceneGameShopLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCPoint touchPoint=pTouch->getLocation();
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
            if(m_tempTouchNode)
            {
                CGameButtonControl *tempdatabutton=dynamic_cast<CGameButtonControl *>(m_tempTouchNode);
              if(tempdatabutton)
              {
                  tempdatabutton->unselected();
              }
            }
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
}

void CSceneGameShopLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{

    CCNode *node=Utility::getNodeByTag(this, "1,1,1");
    CCPoint touchPoint=pTouch->getLocation();
    // m_newPoint=touchPoint;
    // CCLog("%f,%f,%f",m_oldPoint.x,m_newPoint.x,ccpDistance(m_oldPoint, m_newPoint));
    //  m_oldPoint=m_newPoint;
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
        if (m_tempTouchNode!=node && m_tempTouchNode)
        {
            if (m_tempTouchNode&&m_tempTouchNode!=Utility::getNodeByTag(this, "1,1,1")) {
                ((CGameButtonControl *)m_tempTouchNode)->unselected();
            }
        }
        m_tempTouchNode=NULL;
        if (getChildByTag(8)->boundingBox().containsPoint(touchPoint)) {
            CPtRecharge * layer = CPtRecharge::create();
            CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000000);
            //removeFromParentAndCleanup(true);
           // CCNotificationCenter::sharedNotificationCenter()->postNotification("QUNIUDAN");
            return;
        }
        if (getChildByTag(7)->boundingBox().containsPoint(touchPoint)) {
            removeFromParentAndCleanup(true);
             CCNotificationCenter::sharedNotificationCenter()->postNotification("NIUDANJIEMIAN");

            //CDrawCardLayer * layer = CDrawCardLayer::create();
            //CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 10000001);
            return;
        }
    }
    else if (m_tempTouchNode==getChildByTag(7)||m_tempTouchNode==getChildByTag(8)) {
        ((CGameButtonControl *)m_tempTouchNode)->unselected();
        m_tempTouchNode=NULL;
    }
    else{
        int  touchTabl;
        if ((touchTabl = getCheckTouchTab(touchPoint))!=-1) {
            swithChangeTab(touchTabl);
        }
    }
    
}
void CSceneGameShopLayer::ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*)
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