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
#include "CBaseLayer.h"

class CExplorationLayer :public CBaseLayer
{
public:
    CExplorationLayer();
    ~CExplorationLayer();
public:
    bool initWithMapFile(const char *fileName);
    void dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag);
protected:
    bool dealGotoRoom(CCSprite * touchSprite,int touch_tag);
public:
    static CCScene *scene();
private:
    string mapFileName;
};
#endif /* defined(__en_cube__ExplorationLayer__) */
