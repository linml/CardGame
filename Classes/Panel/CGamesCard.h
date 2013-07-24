//
//  CGamesCard.h
//  91.cube
//
//  Created by linminglu on 13-6-28.
//
//

#ifndef ___1_cube__CGamesCard__
#define ___1_cube__CGamesCard__

#include <iostream>
using namespace  std;
#include "cocos2d.h"
using namespace cocos2d;
class CCard;
class CGamesCard :public  CCSprite
{
public:
    CGamesCard();
    ~CGamesCard();
    static  CGamesCard  *Create(CCard *card);
public:
    bool initCreate(CCard *card);
    void setDead();
    void setLive();
    CGamesCard *getCopy();
    bool getSuit(int &outSuit, int &outSequence);
    CCard *getCardData(){ return m_pCardData;};
protected:
    void setLive(CCNode *node);
    void setDead(CCNode *node);
    bool initBg(CCard *card);
    void createCardName(const char *str);
    void createStart(int start);
    void createHero(const char *str);
    void createBackground(const char *str);
    void createSuit(int inSuit);
    void createStirps(int inStirps);
    void createData(const int &inAtk, const int &inHp, const int &inRcv, const int &inDef);
       
protected:
    CCLabelTTF *label;
    CCard *m_pCardData;
public:
    bool isAddTexiao;   
};
#endif /* defined(___1_cube__CGamesCard__) */
