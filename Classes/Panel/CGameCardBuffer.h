//
//  CGameCardBuffer.h
//  91.cube
//
//  Created by linminglu on 13-8-5.
//
//

#ifndef ___1_cube__CGameCardBuffer__
#define ___1_cube__CGameCardBuffer__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace  cocos2d;
class CGameCardBuffer :public CCSprite
{
public:
    static CGameCardBuffer *CreateBuffer(const char *pngFileName,int buffer);
    void setBuffer(int count);
private:
    bool init(const char *pngFileName,int bufferNum);
};
#endif /* defined(___1_cube__CGameCardBuffer__) */
