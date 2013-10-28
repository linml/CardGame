//
//  CEffectInterface.h
//  91.cube
//
//  Created by linminglu on 13-7-23.
//
//

#ifndef ___1_cube__CEffectInterface__
#define ___1_cube__CEffectInterface__

#include <iostream>
#include <string>
using namespace std;
//定义一个计算effect的 抽象类
class CFightCard;
class CImapact;
class CEffectInterface   //效果影响表格的抽象类
{
public:
    CEffectInterface();
    virtual ~CEffectInterface();
    virtual void logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact)=0;
protected:
   virtual  void dealWithBufferAndImmediately(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact,std::string bufferfile="");
    int iChangeShanghaiHp;
    int iChangeatk;
    int iChangedef;
    int iChangeengry;
};

#endif /* defined(___1_cube__CEffectInterface__) */
