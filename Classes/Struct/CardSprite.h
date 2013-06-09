//
//  CardSprite.h
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#ifndef __en_cube__CardSprite__
#define __en_cube__CardSprite__

#include <iostream>
#include "gameCardProperty.h"
//卡牌的父类
using namespace std;

/**
 * 利用CCardsprite 分配4中卡牌的结构,同时4中卡牌又拥有不同属性
 * 技能也是利用继承这个类 才可以利用装饰模式参考
 * http://www.cnblogs.com/Traxex/archive/2012/07/17/2587916.html dota 里面英雄和物品之间的装饰
 */

class CCardSprite
{
public:
    CCardSprite();
    virtual ~CCardSprite();
    virtual  std::string getName();
public:
    CGameCardProperty m_cardData;
};


struct SFightCardSprite {
    SFightCardSprite()
    {
        m_ibingdong=0;
        tag=0;
        cardsprite=NULL;
    }
    CCardSprite *cardsprite;
    bool isDead;
    int  tag;
    int  m_ibingdong;
    int  m_huduanshanghai;
};

#endif /* defined(__en_cube__CardSprite__) */
