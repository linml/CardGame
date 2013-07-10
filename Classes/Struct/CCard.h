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
    unsigned char  m_ccard_next;  ///背景底色
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
    int m_iusually_attack;
    int m_iskillLine;
    int m_iskillHelp;
    int m_iskillDead;
    int m_iskillBuff;
    string m_scard_tips; //描述
    string m_scard_resources;  //资源
    string m_scard_head;  //头像
    string m_scard_groud; //
};

class CCardBufferStatus;
//FIGHTING CARD
class  CFightCard {
public:
    CFightCard();
    CFightCard(CCard *card,int level=1);
    ~CFightCard();
private:
    void init();
public:
    void initFighting();
public:
    int m_iCurrHp;  //当前HP
    int m_iHp;      //当前总的HP
    int m_attack;   //当前按的攻击力量
    int m_defend;   //当前的防御力
    int tag;        //当前卡牌的tag
    int m_iCurrEngry; //当前卡牌的怒气值
    int m_iEngryMax ; //当前卡牌的最大怒气值得
    int m_iMaxExp;    //卡牌该等级的max exp
    int m_User_Card_ID; //卡牌在卡牌背包里面的id
    int m_iCurrLevel; //当前卡牌的等级
    int m_iCurrExp;   //当前卡牌拥有的exp
    bool isDead;     //卡牌是否已经死亡
public:
    CCard *m_pCard;    
    list<CCardBufferStatus *>m_vBuffer; //卡牌拥有的buffer;
};




#endif /* defined(___1_cube__CCard__) */
