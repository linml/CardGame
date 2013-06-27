//
//  CCard.h
//  91.cube
//
//  Created by linminglu on 13-6-26.
//
//

#ifndef ___1_cube__CCard__
#define ___1_cube__CCard__

#include <iostream>

#include <string.h>
#include "cocos2d.h"
using namespace std;
class CCard
{
public:
    CCard();
    int m_icard_id;
    string m_scard_name;
    unsigned char  m_ccard_color;  ///背景底色
    unsigned short int  m_sicard_star;
    int m_icard_stirps;   //种族
    int m_icard_suit;      //随机数值
    int m_icard_leadership;
    int m_icard_exp;    //吃掉该卡牌的 经验
    int m_icard_price;  //卖掉该卡佩的 价格
    int m_ileve_max;   //等级最高级别
    int m_icard_attack; //攻击力
    int m_icard_defend; //防御力
    int m_icardhp;    //卡牌的总的HP
    int m_iskillLine;
    int m_iskillHelp;
    int m_iskillDead;
    int m_iskillBuff;
    string m_scard_tips;
    string m_scard_resources;  
    string m_scard_head;
    string m_scard_groud;
};


//FIGHTING CARD
class  CFightCard {
public:
    CFightCard(CCard *card);
    ~CFightCard();
public:
    int m_iCurrHp; //当前HP
    int tag; //当前卡牌的tag
public:
    CCard *m_pCard;
    
};




#endif /* defined(___1_cube__CCard__) */
