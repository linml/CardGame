//
//  CLackGPDialog.h
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#ifndef ___1_cube__CLackGPDialog__
#define ___1_cube__CLackGPDialog__

#include "CGameSelectDialog.h"
#include "CSpecialPropUserDialog.h"


class CLackGPDialog :public CGameSelectDialog
{
public:
    CREATE_FUNC(CLackGPDialog);
public:
    CLackGPDialog();
    virtual ~CLackGPDialog();
    
public:
    virtual bool init();
    void setGoHallBackCallBack(CCObject *pObject, SEL_CallFunc pGoHallCallBack);
protected:
    void initCLackGPDialog();
    // detail event handler:
    void onClickRevive(CCObject *pObject);
    void onClickFile(CCObject *pObject);
    CC_SYNTHESIZE(SPECIALPROPDATA, m_uSpecialPropData, SpecialPropData);
    
    CCObject *m_pGoHallHandler;
    SEL_CallFunc m_pGoHallCallBack;
};

#endif /* defined(___1_cube__CLackGPDialog__) */
