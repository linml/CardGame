//
//  CardFactoryLayer.h
//  kapeipuzzle
//
//  Created by linminglu on 13-5-22.
//
//

#ifndef __kapeipuzzle__CardFactoryLayer__
#define __kapeipuzzle__CardFactoryLayer__

#include <iostream>
#include "CBaseLayer.h"
using namespace cocos2d;
using namespace std;
struct pagSprite {
    pagSprite(int cardPuzzleindex,int cardSpriteGGtag,int count,int tag)
    {
        index=cardPuzzleindex;
        spriteTag=tag;
        icount=count;
        SpriteGGtag=cardSpriteGGtag;
    }
    int  index;
    int  SpriteGGtag;
    int  icount;
    int  spriteTag;
    
};


class CardFactoryLayer :public CBaseLayer
{
public:
    CardFactoryLayer();
    ~CardFactoryLayer();
public:
    bool initWithMapFile(const char *fileName);
private:
    void setCardBgGray(int tag);
    void hideCardPuzzle(int index,int tag);
    void setLeftPingHeButton(bool flag);
    void setRightPingHeButton(bool flag);
    void backButtonTextture();
    void reInitButton();
    void pinheCurrCard(int SpriteGGtag);
protected:
   void dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag);
private:
    string mapFileName;
private:
    vector<pagSprite>vCardSpritePuzzle;
protected:
    void updateAddCount(float ctime);
    void reInit(int index,int cspriteCard);
private:
    CCTexture2D *leftButton;
    bool leftButtonIsFull;
    CCTexture2D *rightButton;
      bool RightButtonIsFull;
private:
    void leftButtonClickOn();

    
};

#endif /* defined(__kapeipuzzle__CardFactoryLayer__) */
