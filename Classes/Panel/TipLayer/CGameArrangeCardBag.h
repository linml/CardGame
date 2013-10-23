//
//  CGameArrangeCardBag.h
//  91.cube
//
//  Created by linminglu on 13-10-23.
//
//

#ifndef ___1_cube__CGameArrangeCardBag__
#define ___1_cube__CGameArrangeCardBag__

#include <iostream>
#include "cocos2d.h"
#include "CGameDialogLayer.h"
using namespace std;
using namespace cocos2d;


class CGameArrangeCardBag : public CPtDialog
{
public:
    CREATE_FUNC(CGameArrangeCardBag);
public:
    CGameArrangeCardBag();
    virtual ~CGameArrangeCardBag();
    virtual bool init();
    
protected:
    
    void initCGameArrageBackpackTip();
    
    void onArrageCallBack(CCObject *pObject);
    void onCancelCallBack(CCObject *pObject);
    
    void showBackPack();
};
#endif /* defined(___1_cube__CGameArrangeCardBag__) */
