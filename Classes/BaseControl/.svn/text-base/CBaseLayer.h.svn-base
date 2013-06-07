//
//  BaseLayer.h
//  kapeipuzzle
//
//  Created by linminglu on 13-5-22.
//
//

#ifndef __kapeipuzzle__BaseLayer__
#define __kapeipuzzle__BaseLayer__

#include <iostream>
#include "cocos2d.h"
#include "Utility.h"
using namespace cocos2d;
using namespace std;
class BaseLayer :public CCLayer {
public:
    BaseLayer();
    virtual  ~BaseLayer();
public:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher(void);
	virtual void onExit();
    virtual bool initWithMapFile(const char *fileName);
    virtual void needToScenePoint(CCPoint &point,CCTouch *pTouch);
    template <class T>
    static T *Create(const char *fileName);
protected:
    bool appendTouchRect(CCPoint point);
    virtual void dealWhithTouchEndSprite(CCSprite *sprite,int touch_Tag);
    
    
protected:
//vector<TouchRect> vTouchRect;
	vector<TouchRect> vTouchMapRect;
    vector<TouchRect *>vCurrTouchRect;
    string fileName;
};

template <class T>
T *BaseLayer::Create(const char *fileName)
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

#endif /* defined(__kapeipuzzle__BaseLayer__) */
