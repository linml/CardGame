//
//  CPanelGamePlayerInfoLayer.h
//  91.cube
//
//  Created by linminglu on 13-10-11.
//
//

#ifndef ___1_cube__CPanelGamePlayerInfoLayer__
#define ___1_cube__CPanelGamePlayerInfoLayer__

#include <iostream>
using namespace std;

#include "cocos2d.h"

using namespace cocos2d;

class CPanelGamePlayerInfoLayer : public CCLayer
{
public:
    CPanelGamePlayerInfoLayer();
    ~CPanelGamePlayerInfoLayer();
public:
    bool init();
    void updateInfo();
    void updateShowData(float t);
    CREATE_FUNC(CPanelGamePlayerInfoLayer);
};

#endif /* defined(___1_cube__CPanelGamePlayerInfoLayer__) */
