//
//  CFriendEventApplyDialog.h
//  91.cube
//
//  Created by phileas on 13-11-19.
//
//

#ifndef ___1_cube__CFriendEventApplyDialog__
#define ___1_cube__CFriendEventApplyDialog__

#include "CGameDialogLayer.h"
#include "CFriend.h"
class CFriendEventApplyDialog : public CPtDialog
{
public:
    static CFriendEventApplyDialog* create(Friend inFriend);
public:
    virtual bool init();
    
    void setCloseCallBack(CCObject *pObject, SEL_CallFunc inSelector);
protected:
    void initCFriendEventApplyDialog();
    void onClickYes(CCObject *pObject);
    void onClickNo(CCObject *pObject);
    void onReceiveMsg(CCObject *pObject);
    CC_SYNTHESIZE(Friend, m_sFriend, Friend);

    void closeDialog();
protected:
    CCObject *m_pCloseHandler;
    SEL_CallFunc m_pCloseSelector;

};

#endif /* defined(___1_cube__CFriendEventApplyDialog__) */
