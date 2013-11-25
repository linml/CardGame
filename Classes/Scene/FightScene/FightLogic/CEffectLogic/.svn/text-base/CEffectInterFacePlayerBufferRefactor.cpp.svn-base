//
//  CEffectInterFacePlayerBufferRefactor.cpp
//  91.cube
//
//  Created by linminglu on 13-10-28.
//
//

#include "CEffectInterFacePlayerBufferRefactor.h"
CEffectInterFacePlayerBufferRefactor::CEffectInterFacePlayerBufferRefactor()
{
    
}
CEffectInterFacePlayerBufferRefactor::~CEffectInterFacePlayerBufferRefactor()
{
    
}
void CEffectInterFacePlayerBufferRefactor::dealLogicSuanfa(int &value,int prameValue,int prarmValue1,char fuhao)
{
    switch (fuhao) {
        case '+':
            value+= prameValue + value * prarmValue1/100;
            break;
        case '-':
            value-= (prameValue + value * prarmValue1/100);
            break;
        default:
            break;
    }
}