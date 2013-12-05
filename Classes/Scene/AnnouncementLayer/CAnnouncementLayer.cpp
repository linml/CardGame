//
//  CAnnouncementLayer.cpp
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#include "CAnnouncementLayer.h"
#include "gameTools.h"
#include "gamePlayer.h"
#include "CAnnouncementDataManager.h"
#define TAG_COLORLAYER 1

CAnnouncementLayer::CAnnouncementLayer()
{
    this->plabelTTF=NULL;
    m_tempPlayer=SinglePlayer::instance();
}
CAnnouncementLayer::~CAnnouncementLayer()
{
    
}


void CAnnouncementLayer::createColorLayer()
{
    CCLayerColor *pColorLayer=CCLayerColor::create(ccc4(255, 0, 0, 120), 800, 50);//用红色来表示下
    pColorLayer->ignoreAnchorPointForPosition(false);
    addChild(pColorLayer,1,TAG_COLORLAYER);
    setContentSize(CCSizeMake(800, 50));
}

void CAnnouncementLayer::showLayer()
{
    if (getChildByTag(1)) {
        getChildByTag(1)->setVisible(true);
    }
}
void CAnnouncementLayer::hideColorLayer()
{
    
    if (getChildByTag(1)) {
        getChildByTag(1)->setVisible(false);
    }
}

void CAnnouncementLayer::updateSchu(float t)
{
    if (m_tempPlayer->m_pAnno->getListSize()) {
        if (!isVisible()) {
            setVisible(true);
        }
        createContextLayer(m_tempPlayer->m_pAnno->getShowData());
    }
    
}

void CAnnouncementLayer::removeCallBack(CCObject *object)
{
    CCLayer *pLayer=(CCLayer *)object;
    if (pLayer) {
        removeChild(pLayer, true);
    }
}
bool CAnnouncementLayer::init()
{
    createColorLayer();
    schedule(schedule_selector(CAnnouncementLayer::updateSchu), 20.0f,kCCRepeatForever, 1.0f);
    return true;
}
void CAnnouncementLayer::createContextLayer(string str)
{
    if(str.empty())
    {
        return ;
    }
    CCSize size=getContentSize();
    CCLayer *pLayer=GameTools::createDiffColorWord(str, "Arial", 45);
    pLayer->setAnchorPoint(CCPointZero);
    pLayer->setPosition(CCPointMake(size.width*0.5,0));
    addChild(pLayer,2);
    float totalLength=size.width*0.5+pLayer->getContentSize().width;
    float totoalTime=int(totalLength/120)+1;
    CCPoint endPoint=CCPointMake(-totalLength, 0);
    pLayer->runAction(CCSequence::create(CCMoveTo::create(totoalTime, endPoint),CCCallFuncN::create(this, callfuncN_selector(CAnnouncementLayer::removeCallBack)),NULL));
   
}