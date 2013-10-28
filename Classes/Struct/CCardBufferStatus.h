//
//  CCardBufferStatus.h
//  91.cube
//
//  Created by linminglu on 13-7-9.
//
//

#ifndef ___1_cube__CCardBufferStatus__
#define ___1_cube__CCardBufferStatus__
/**
 * 定义一些战斗卡牌处于的buffer 表格 
 * 
 *
 */
 
#include <iostream>
#include "gameConfig.h"
using namespace std;
class CCardBufferStatusRefactor
{
public:
    CCardBufferStatusRefactor(){};
    ~CCardBufferStatusRefactor(){};
    CCardBufferStatusRefactor(
                      int Hp,int Atk,int Def,int Engry,
                      int effectid,string bufferfile="");
public:
    int m_iHp;
    int m_iEngry;
    int m_iAtk;
    int m_iDef;
    int m_iEffectid;    //效果ID
    int m_iMutex;       //是否互斥
    int m_iMutexLevel;  //等级
    int m_iNeedAddBack; //是否需要返
    int m_iKeepTime;    //是否是持续次数
    int m_iEffect_time; //是否是影响次数
    
    string m_sbfferFile;
    
};
#endif /* defined(___1_cube__CCardBufferStatus__) */
