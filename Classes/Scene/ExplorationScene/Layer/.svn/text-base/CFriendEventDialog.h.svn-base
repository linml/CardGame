//
//  CFriendEventDialog.h
//  91.cube
//
//  Created by phileas on 13-11-19.
//
//

#ifndef ___1_cube__CFriendEventDialog__
#define ___1_cube__CFriendEventDialog__

#include "CGameDialogLayer.h"
#include "CFriend.h"

class CFriendEventDialog : public CPtDialog
{
public:
    static CFriendEventDialog* create(Friend inFriend);
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
protected:
    void initCFriendEventDialog();
    CCNode* createFriendInfo();
    CC_SYNTHESIZE(Friend, m_sFriend, Friend);
};

#endif /* defined(___1_cube__CFriendEventDialog__) */
