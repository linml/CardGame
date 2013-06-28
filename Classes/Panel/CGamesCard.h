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
    static  CGamesCard  *Create(CCard *card,bool isYongHu);
public:
    bool initCreate(CCard *card,bool isYongHu);
    void setDead();
private:
    bool initBg(CCard *card);
    void createCardName(const char *str);
    void createStart(int start);
    void createHero(const char *str);
    
    
    
};
#endif /* defined(___1_cube__CGamesCard__) */
