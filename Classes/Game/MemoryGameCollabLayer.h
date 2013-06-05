//
//  MemoryGameCollabLayer.h
//  91.cube
//
//  Created by linminglu on 13-5-28.
//
//

#ifndef ___1_cube__MemoryGameCollabLayer__
#define ___1_cube__MemoryGameCollabLayer__

#include <iostream>
#include "cocos2d.h"
#include "BaseLayer.h"
using namespace cocos2d;
class MemoryGameCollabLayer :public BaseLayer
{
public:
    MemoryGameCollabLayer();
    ~MemoryGameCollabLayer();
public:
    bool initWithMapFile(const char *fileName);
    void dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag);
private:
    string mapFileName;
    vector<int>m_vigameSpriteValue;
    bool  isEqualt(int indexBefore,int indexAfter);
    bool  addChildSprite();
    void needToScenePoint(CCPoint &point,CCTouch *pTouch);
    void actionMoveCoverDone(CCNode* node);
    void actionMoveCardDone(CCNode* node);
    void actionMoveRemoveDone(CCNode*  node);
    void dealWith(CCNode* node);
    void removeTouchRectBytag(int tag);
    void reSetCanTouch(CCNode*  node);
    void onAnimationEnd(CCNode *node);
private:
    CCPoint getSpritePosition(int &row,int &colum);
    int countSize;
    int m_iFanPanCount;
    int valueBefore;
    int valueAfter;
    int m_iCurrTouch_tag;
};

#endif /* defined(___1_cube__MemoryGameCollabLayer__) */
