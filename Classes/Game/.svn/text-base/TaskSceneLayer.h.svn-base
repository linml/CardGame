//
//  TaskSceneLayer.h
//  en.cube
//
//  Created by linminglu on 13-5-23.
//
//

#ifndef __en_cube__TaskSceneLayer__
#define __en_cube__TaskSceneLayer__

#include <iostream>
#include "CBaseLayer.h"
using namespace cocos2d;
using namespace std;
class TaskSceneLayer :public CBaseLayer
{
public:
    TaskSceneLayer();
    ~TaskSceneLayer();
public:
    bool initWithMapFile(const char *fileName);
    void dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag);
public:
private:
    string mapFileName;
};

#endif /* defined(__en_cube__TaskSceneLayer__) */
