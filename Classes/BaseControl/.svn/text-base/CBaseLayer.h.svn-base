//
//  CBaseLayer.h
//  kapeipuzzle
//
//  Created by linminglu on 13-5-22.
//
//

#ifndef __kapeipuzzle__CBaseLayer__
#define __kapeipuzzle__CBaseLayer__

#include <iostream>
#include "cocos2d.h"
#include "Utility.h"
using namespace cocos2d;
using namespace std;

class CBaseLayer :public CCLayer {
public:
    CBaseLayer();
    virtual  ~CBaseLayer();
    
protected:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher(void);
	virtual void onExit();
    virtual bool initWithMapFile(const char *fileName);
    virtual void needToScenePoint(CCPoint &point,CCTouch *pTouch);
    bool appendTouchRect(CCPoint point);
public:
    template <class T>
    static T *Create(const char *fileName);

protected:
    virtual void dealWhithTouchEndSprite(CCSprite *sprite,int touch_Tag);
protected:
	vector<TouchRect> vTouchMapRect;
    vector<TouchRect *>vCurrTouchRect;
    string fileName;
};

template <class T>
T *CBaseLayer::Create(const char *fileName)
{
    T *layer=new T;
    if(!layer || !layer->initWithMapFile(fileName))
    {
        delete layer;
        layer=NULL;
        return NULL;
    }
    layer->autorelease();
    return layer;    
}

#endif /* defined(__kapeipuzzle__CBaseLayer__) */
