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
    CCardSprite(CCardSprite &t1);
    virtual ~CCardSprite();
    virtual  std::string getName();
public:
    CGameCardProperty m_cardData;
};


class SFightCardSprite {
public:
    SFightCardSprite()
    {
        m_ibingdong=0;
        tag=0;
        m_iXuanYun=0;
        cardsprite=NULL;
        m_iHuduanshanghai=0;
        m_iJianMianShanghai=0;
        m_iHuduanshanghai=0;
        m_iJiaFang=0;
    }
    ~SFightCardSprite()
    {
        if(cardsprite)
        {
            delete cardsprite;
            cardsprite=NULL;
        }
        
    }
    
    bool isCannotATK(); //判断当前是否处于可以攻击的状态
    void initShangHai();
     
    void dealLastHp(); //处理  最后的 HP  加上减免伤害
    void fantanShanghai(SFightCardSprite **cardsprite);
    
    CCardSprite *cardsprite;
    bool isDead;
    int  tag;
    
    int  m_ibingdong;
    int  m_iXuanYun;
    int  m_iJiaFang;
    int  m_iHuduanshanghai;//护盾伤害  反弹
    int  m_iJianMianShanghai;//减免伤害
    class CCardPanel *m_CardPanel;
    
};

#endif /* defined(__en_cube__CardSprite__) */
