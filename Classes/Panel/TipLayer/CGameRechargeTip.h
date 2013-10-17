//
//  CGameRechargeTip.h
//  91.cube
//
//  Created by phileas on 13-10-14.
//
//

#ifndef ___1_cube__CGameRechargeTip__
#define ___1_cube__CGameRechargeTip__

#include "cocos2d.h"
#include "CGameDialogLayer.h"

using namespace cocos2d;

class CGameRechargeTip : public CPtDialog
{
public:
    CREATE_FUNC(CGameRechargeTip);
public:
    CGameRechargeTip();
    virtual ~CGameRechargeTip();
    virtual bool init();
    
protected:
    
    void initCGameRechargeTip();
    
    void onRechargeCallBack(CCObject *pObject);
    void onCancelCallBack(CCObject *pObject);
    
};

#endif /* defined(___1_cube__CGameRechargeTip__) */
