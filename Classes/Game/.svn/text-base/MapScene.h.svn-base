#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include <vector>
#include "Utility.h"
using namespace std;
USING_NS_CC;

USING_NS_CC_EXT;
class MapScene : public CCLayer
{
public:
	MapScene(void);
	~MapScene(void);
 	vector<TouchRect> vTouchRect;
	vector<TouchRect> vTouchMapRect;
    
public:
	CREATE_FUNC(MapScene);
public:
	virtual bool init();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher(void);
	virtual void onExit();

public:
	bool handleTouchSpritePool(CCPoint point);

protected:
	void refeshPlist();
	
	void timerRefesh(float dt);

protected:
	int index;

};
