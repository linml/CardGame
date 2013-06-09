//
//  CBaseLayer.cpp
//  kapeipuzzle
//
//  Created by linminglu on 13-5-22.
//
//

#include "CBaseLayer.h"
CBaseLayer::CBaseLayer()
{
    vCurrTouchRect.clear();
    vTouchMapRect.clear();
    //setTouchPriority(<#int priority#>)
}

CBaseLayer::~CBaseLayer()
{
    
    this->vCurrTouchRect.erase(vCurrTouchRect.begin(),vCurrTouchRect.end());
    this->vTouchMapRect.erase(vTouchMapRect.begin(),vTouchMapRect.end());
    
}

bool CBaseLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(vCurrTouchRect.size() >0)
    {
        return false;
    }
    CCPoint p = pTouch->getLocation();
    needToScenePoint(p,pTouch);
    return  appendTouchRect(p);
}

bool CBaseLayer::appendTouchRect(CCPoint point)
{
    CCLog("%d",vTouchMapRect.size());
    int tag = -1;
	tag = TouchRect::SearchTouchTag(point, vTouchMapRect, NULL);
    if(tag!=-1)
    {
        TouchRect::AppendCurrTouchTag(tag, vTouchMapRect, vCurrTouchRect);
        return true;
    }
    return false;
}

void CBaseLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    //调用移开的时候 取消显示的方法
    
}

void CBaseLayer::needToScenePoint(CCPoint &point,CCTouch *pTouch)
{
    return ;
}

void CBaseLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    needToScenePoint(point,pTouch);
    if(vCurrTouchRect.size()>0)
    {
	CCSprite* touchSprite= CCSprite::create();
	int touch_tag = -1;
	touch_tag = TouchRect::SearchTouchTag(point, vTouchMapRect, &touchSprite);
        if(touch_tag!=-1 && touch_tag==vCurrTouchRect[0]->tag)
        {
            dealWhithTouchEndSprite(touchSprite,touch_tag);
        }
	}
    vCurrTouchRect.erase(vCurrTouchRect.begin(),vCurrTouchRect.end());
}
void CBaseLayer::dealWhithTouchEndSprite(cocos2d::CCSprite *sprite,int touch_Tag)
{
    if(touch_Tag!=-1 && sprite)
    {
        CCLog("touch_Tag :%d",touch_Tag);
    }
}

void CBaseLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void CBaseLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
    
}


bool CBaseLayer::initWithMapFile(const char *fileName)
{
    return false;
}