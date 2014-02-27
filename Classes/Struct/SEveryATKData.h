//
//  SEveryATKData.h
//  91.cube
//
//  Created by linminglu on 13-7-30.
//
//

#ifndef ___1_cube__SEveryATKData__
#define ___1_cube__SEveryATKData__
#include <stdio.h>
struct  SSpriteStatus
{
    int m_iCurrHp;
    int m_iCurrTotalHp;
    int m_iEngry;
    int m_HouxuKaDeEngry[3];//用来保存剩下未上场的 卡牌的怒气
};

class SEveryATKData
{
public:
    SEveryATKData();
    int m_nCurrAction;
    ~SEveryATKData();
    SSpriteStatus *data[2];
};

#endif /* defined(___1_cube__SEveryATKData__) */
