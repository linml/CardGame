//
//  CCardPanel.h
//  91.cube
//
//  Created by linminglu on 13-6-14.
//
//

#ifndef ___1_cube__CCardPanel__
#define ___1_cube__CCardPanel__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CCardPanel :public CCSprite
{
public:
    CCardPanel();
    ~CCardPanel();
public:
    static  CCardPanel  *Create(const char *pngFileName, const char *name);
    CCLabelTTF *getLabel();
    void setDead();
private:
    bool initSprite(const char *pngFileName,const char *name);
public:
    bool isAddTexiao;
};

#endif /* defined(___1_cube__CCardPanel__) */
