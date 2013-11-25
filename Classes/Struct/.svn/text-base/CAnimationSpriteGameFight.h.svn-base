//
//  CAnimationSpriteGameFight.h
//  91.cube
//
//  Created by linminglu on 13-7-17.
//
//

#ifndef ___1_cube__CAnimationSpriteGameFight__
#define ___1_cube__CAnimationSpriteGameFight__

#include <iostream>
#include "cocos2d.h"
#include "gameConfig.h"


class CAnimationSpriteGameFight
{
    //暂且先定义这些
public:
    CAnimationSpriteGameFight();
    CAnimationSpriteGameFight(
                              EN_ANIMATIONTYPE enanimationtype,
                              EN_ATKFIGHT_INDEX enAtkFight,
                              int iAtkindex,
                              int iDefIndex,
                              int iAddHp,
                              int iSubHp,
                              int iAddEngry,
                              int iSubEngry,
                              int skillid,
                              int spritetag,string strplist="");
    
    EN_ANIMATIONTYPE m_enAnimationType;
    EN_ATKFIGHT_INDEX m_enAtkFightIndex;
    int  m_iATKindex;
    int  m_iDefIndex;
    int  m_iAddHp;
    int  m_iSubHp;
    int  m_iAddEngry;
    int  m_iSubEngry;
    int  m_iSKillId;
    int  spritetag;
    bool isToOwn; //如果死亡技能是给自己的话 那么攻击的对方不需要动哦。 

    string m_sBufferPlist;
public:
    
    friend inline ostream & operator << (ostream & os, CAnimationSpriteGameFight &t1)
    {
        cout <<"{"
        <<"\t卡牌发动者   :"<< t1.m_iATKindex<<"\n"
        << "\t卡牌受害者  :"<< t1.m_iDefIndex<<"\n"
        << "\t卡牌HP     :"<< t1.m_iSubHp<<"\n"
        << "\t卡牌ANGRY  :"<< t1.m_iSubEngry<<"\n"
        << "\t发动技能    :"<< t1.m_iSKillId<<"\n"
        <<"}"<<endl;
        return os;
   }
};


class CAnimationSctrip
{
    //暂且先定义这些
public:
    CAnimationSctrip();
    virtual ~CAnimationSctrip();
    CAnimationSctrip(EN_ANIMATIONTYPEREFACTOR entype,EN_ATKFIGHT_INDEX enAtkIndex,
                     int skillId,
                     int nATKindex,
                     int nDefIndex,
                     int nAddHp,int nSubHp,string m_sRunActionFile);
public:
    EN_ANIMATIONTYPEREFACTOR m_enAnimationType;
    EN_ATKFIGHT_INDEX m_enAtkFightIndex;
    int m_nSubHp;
    int m_nAddHp;
    int m_nATKindex;
    int m_nDEFindex;
    int m_nSpritetag;
    std::string m_sRunActionFile; //每个技能 需要调用的plist
    int m_nSKillId;
public:
    friend inline ostream & operator << (ostream & os, CAnimationSctrip &t1)
    {
        cout <<"{"
        <<"\t卡牌发动者   :"<< t1.m_nATKindex<<"\n"
        << "\t卡牌受害者  :"<< t1.m_nDEFindex<<"\n"
        << "\t卡牌HP     :"<< t1.m_nSubHp<<"\n"
        << "\t卡牌ANGRY  :"<< t1.m_nAddHp<<"\n"
        << "\t发动技能    :"<< t1.m_nSKillId<<"\n"
        <<"}"<<endl;
        return os;
    }
};

#endif /* defined(___1_cube__CAnimationSpriteGameFight__) */
