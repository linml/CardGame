//
//  CEffectInterFaceTwentyOneRefactor.h
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#ifndef ___1_cube__CEffectInterFaceTwentyOneRefactor__
#define ___1_cube__CEffectInterFaceTwentyOneRefactor__

#include <iostream>
#include "CEffectInterFacePlayerBufferRefactor.h"
class CFightCard;
using namespace std;

class CEffectInterFaceTwentyOneRefactor :public CEffectInterFacePlayerBufferRefactor
{
public:
    CEffectInterFaceTwentyOneRefactor();
    ~CEffectInterFaceTwentyOneRefactor();
    void logicFightingCardByFightAndMonster(CFightCard *pCard,CFightCard *pMonster,CImapact *pImapact);
};
#endif /* defined(___1_cube__CEffectInterFaceTwentyOneRefactor__) */
