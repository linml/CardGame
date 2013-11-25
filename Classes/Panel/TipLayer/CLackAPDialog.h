//
//  CLackAPDialog.h
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#ifndef ___1_cube__CLackAPDialog__
#define ___1_cube__CLackAPDialog__

#include "CLackGPDialog.h"

class CLackAPDialog :public CLackGPDialog
{
public:
    CREATE_FUNC(CLackAPDialog);
public:
    CLackAPDialog();
    virtual ~CLackAPDialog();
    
public:
    virtual bool init();
protected:
    void initCLackAPDialog();
    // override method:
    void onClickRevive(CCObject *pObject);
    void onClickCancle(CCObject *pObject);
};

#endif /* defined(___1_cube__CLackAPDialog__) */
