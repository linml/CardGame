//
//  PuzzleStruct.h
//  puzzle
//
//  Created by linminglu on 13-5-21.
//
//

#ifndef __puzzle__PuzzleStruct__
#define __puzzle__PuzzleStruct__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
struct Puzzle {
public:
    Puzzle():iCount(0)
    {
        memset(sPuzzleFileName, 0, 20);
        point.x=0;
        point.y=0;
    }
    Puzzle(int count,const char *pFile,CCPoint pPoint)
    {
        iCount=count;
        memcpy(sPuzzleFileName, pFile, 20);
        point=pPoint;
    }
    int iCount; //puzzleCount;
    char sPuzzleFileName[20];
    CCPoint point;
};

class PuzzleSprite :public CCSprite
{
public:
    PuzzleSprite();
    virtual ~PuzzleSprite();
    static PuzzleSprite* create(const char* pszFileName);
    bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
    virtual void draw();
};
#endif /* defined(__puzzle__PuzzleStruct__) */
