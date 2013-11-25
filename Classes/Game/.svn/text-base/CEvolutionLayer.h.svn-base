//
//  CEvolution.h
//  91.cube
//
//  Created by linminglu on 13-5-27.
//
//

#ifndef ___1_cube__CEvolutionLayer__
#define ___1_cube__CEvolutionLayer__

#include <iostream>
#include "cocos2d.h"
#include  "CBaseLayer.h"
using namespace std;
using namespace cocos2d;
class CEvolutionLayer :public CBaseLayer
{
public:
    CEvolutionLayer();
    ~CEvolutionLayer();
public:
    bool initWithMapFile(const char *fileName);
    void dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag);
public:
    static CCScene *scene();
private:
    string mapFileName;
};
#endif /* defined(___1_cube__CEvolution__) */
