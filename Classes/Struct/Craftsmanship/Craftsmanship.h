//
//  Craftsmanship.h
//  en.cube
//
//  Created by linminglu on 13-5-24.
//
//

#ifndef __en_cube__Craftsmanship__
#define __en_cube__Craftsmanship__

#include <iostream>
using namespace std;
#include "CardSprite.h"
//技能的父类   利用装饰模式。
enum EN_JINENGLEVELE
{
    EN_JINENGLEVELE_PUTONGJINENG  = 0,
    EN_JINENGLEVELE_TESHUJINENG  = 1,
    EN_JINENGLEVELE_TESHUJINENG2 =2,
    EN_JINENGLEVELE_FUZHUJINENG =3
};


class CCraftsmanship :public CCardSprite
{
protected:
    CCardSprite *cardSprite;
public:
    void SetComponent(CCardSprite *component); //任何种族 添加了这个技能都对应增加什么东西
    void GetAbilityValue();
    unsigned int getHp();//血量
    unsigned int getPlayerCardAttack();
    unsigned int getPlayerCardDefence();
    int getGeDang();
    int getPojia();
    virtual string getName();
protected:
    EN_JINENGLEVELE  m_jineng;
};
#endif /* defined(__en_cube__Craftsmanship__) */
