//
//  CGameRoleAnimation.h
//  91.cube
//
//  Created by linminglu on 13-9-5.
//
//

#ifndef ___1_cube__CGameRoleAnimation__
#define ___1_cube__CGameRoleAnimation__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CGameTalkDialog;
class CGameRoleAnimation :public CCObject
{
public:
    CGameRoleAnimation();
   virtual  ~CGameRoleAnimation();
    virtual void runActionAnimation(CCLayer *layer,CGameTalkDialog *newTalkDialog,CGameTalkDialog *oldDialog);
    CCAction * animation1(float &value);
    CCAction * animation2(float &value);
    CCAction * animation3(float &value);
    CCAction * animation4(float &value);
    CCAction * animation5(float &value);
    CCAction * animation6(float &value);
    CCAction * animation7(float &value);
    CCAction * animation8(float &value);
    CCAction * animation9(float &value);
    CCAction * animation10(float &value);
    void showEnd(CCObject *object);
    void dealAnimation();
    void moveDialogToUpEnd();
    void setDialogPosition(CCNode *node);
    void endShowText(float seconed,CCNode *node);
    void disappearDialog();
    void callbackShowTouchIcon();
    void startDealAction(CCAction *action, float startLableValue,bool isLeft);
private:
    CCLayer *m_Player;
    CGameTalkDialog *m_pGameDialog;
    CCSize wndSize;
};

#endif /* defined(___1_cube__CGameRoleAnimation__) */