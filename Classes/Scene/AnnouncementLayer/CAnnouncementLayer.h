//
//  CAnnouncementLayer.h
//  91.cube
//
//  Created by linminglu on 12/3/13.
//
//

#ifndef ___1_cube__CAnnouncementLayer__
#define ___1_cube__CAnnouncementLayer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class  CAnnouncementLayer :public CCLayer
{
    
  public:
    CAnnouncementLayer();
    ~CAnnouncementLayer();
    bool init();
    CREATE_FUNC(CAnnouncementLayer);
    void updateSchu(float t); //定时5s后继续调用下一条公告信息
private:
    void createColorLayer();
    CCLabelTTF *plabelTTF;
    void showLayer();
    void hideColorLayer();
};

#endif /* defined(___1_cube__CAnnouncementLayer__) */
