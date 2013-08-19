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
#include "CPtLevelConfigData.h"
#include "CCardBufferStatus.h"
using namespace std;
using namespace cocos2d;
class CCard
{
public:
    CCard();
    int m_icard_id;
    string m_scard_name;
    int  m_ccard_next;  ///next id
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
    int m_iskillLine; //怒气
    int m_iskillHelp; //拥护技能
    int m_iskillDead; //死亡技能
    int m_iskillBuff; //增幅技能
    string m_scard_tips; //描述
    string m_scard_resources;  //资源
    string m_scard_head;  //头像
    string m_scard_ground; //
    string m_scard_role;
    
};
//FIGHTING CARD
class  CFightCard
{
public:
    CFightCard();
    CFightCard(CCard *card,int level=1);
    
    ~CFightCard();
private:
    void init();
public:
    void initFighting();
    void updateFight(CCard *card, int level =1);
    void updateCard(int level);
    
    int  getAddValue(int level, int type);
    int  getNeedValue(int level, int type);
    int  getSupportValue(int type);
    int  getCostConin();
    
    void setInBattleArray(const int &inType);
    int  getInWhichBattleArray();
    
    bool appendBuffer(CCardBufferStatus *buffer);
    void needRebackAtkAndBuf(CCardBufferStatus *buffer);
    bool isHaveBuffer(int prameid);
    void appendEngry(int iEngry);
    void appendHp(int iHp);
    bool hasAssistantSkill();
    void setNegativeToZero();
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

    
    int m_nCurrentPrice;
    int m_nNeedExp;
    int m_nLeadShip;
    int addAtk;      //绿色属性攻击 暂定，基本没用
    int addDef;      //绿色属性防御暂定， 基本没用
    bool isSendZengfu;
    int m_iSuit;
public:
    CCard *m_pCard;    
    list<CCardBufferStatus *>m_vBuffer; //卡牌拥有的buffer;
    // change by phileas.chen 2013.7.22
    CC_SYNTHESIZE(bool, m_bConsume, EnConsume);
protected:
    int m_nWhichBattleArray;
public: //for test
    friend inline ostream & operator << (ostream & os, CFightCard &t1)
    {
        cout <<"{"
             <<"\t卡牌HP   :"<< t1.m_iCurrHp<<"\n"
             << "\t卡牌ENGRY:"<< t1.m_iCurrEngry<<"\n"
             << "\t卡牌ATK  :"<< t1.m_attack<<"\n"
             << "\t卡牌DEF  :"<< t1.m_defend<<std::endl
             <<"}"<<endl;
        return os;
    }

};




#endif /* defined(___1_cube__CCard__) */
