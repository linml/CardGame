//
//  ExplorationLayer.h
//  en.cube
//
//  Created by linminglu on 13-5-23.
//
//

#ifndef __en_cube__ExplorationLayer__
#define __en_cube__ExplorationLayer__

#include <iostream>
#include "BaseLayer.h"

class ExplorationLayer :public BaseLayer
{
public:
    ExplorationLayer();
    ~ExplorationLayer();
public:
    bool initWithMapFile(const char *fileName);
    void dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag);
protected:
    bool dealGotoRoom(CCSprite * touchSprite,int touch_tag);
public:
private:
    string mapFileName;
};
#endif /* defined(__en_cube__ExplorationLayer__) */
