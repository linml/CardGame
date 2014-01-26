//
//  CFriendEventApplyDialog.cpp
//  91.cube
//
//  Created by phileas on 13-11-19.
//
//

#include "CFriendEventApplyDialog.h"
#include "gamePlayer.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"

CFriendEventApplyDialog* CFriendEventApplyDialog::create(Friend inFriend)
{
    CFriendEventApplyDialog *dialog = new CFriendEventApplyDialog();
    if(dialog)
    {
        dialog->setFriend(inFriend);
        if (dialog->init())
        {
            dialog->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(dialog);
            dialog = NULL;
        }
    }
    return dialog;
}

bool CFriendEventApplyDialog::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CPtDialog::init());
        initCFriendEventApplyDialog();
        bRet = true;
    } while (0);
    return bRet;
}

void CFriendEventApplyDialog::initCFriendEventApplyDialog()
{
    m_pCloseSelector = NULL;
    m_pCloseHandler = NULL;
    const char *pre = "是否向";
    const char *last = "发出好友请求";
    char buffer[100]={0};
    snprintf(buffer, 100, "%s%s%s",pre, m_sFriend.username.c_str(),last);
    m_pTip->setString(buffer);
    setButtonText("是", "否");
    setLeftHandler(this, callfuncO_selector(CFriendEventApplyDialog::onClickYes));
    setRightHandler(this, callfuncO_selector(CFriendEventApplyDialog::onClickNo));
}

void CFriendEventApplyDialog::onClickYes(CCObject *pObject)
{
    char buffer[200]={0};
    snprintf(buffer,200, "%d",m_sFriend.fried_uid);
    string connectData="sig=";
    connectData += SinglePlayer::instance()->getUserSig();
    connectData +=string("&friend_uid=")+buffer;
    ADDHTTPREQUESTPOSTDATA(STR_URL_SEARCHFRIEND(196), "CALLBACK_CFriendEventApplyDialog::onClickYes", "REQUEST_CFriendEventApplyDialog::onClickYes",connectData.c_str(),callfuncO_selector(CFriendEventApplyDialog::onReceiveMsg));
}
void CFriendEventApplyDialog::onClickNo(CCObject *pObject)
{
    closeDialog();
}



void CFriendEventApplyDialog::onReceiveMsg(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CFriendEventApplyDialog::onClickYes");
    if (!object )
    {
        CCMessageBox("服务端传输的是", "error");
        return ;
    }
    char * tempdata=(char *)object;
    //解析字符串。 赋值与vector中
    CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdata);
    CCLog("tempdata:%s",tempdata);
    delete [] tempdata;
    tempdata = NULL;
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    CCDictionary *resultDict=(CCDictionary*)(dict->objectForKey("result"));
    if (GameTools::intForKey("info", resultDict)==1)
    {
       
    }
    else
    {
        CCMessageBox("没有该ID","");
    }
    closeDialog();
}

void CFriendEventApplyDialog::setCloseCallBack(cocos2d::CCObject *pObject, SEL_CallFunc inSelector)
{
    m_pCloseHandler = pObject;
    m_pCloseSelector = inSelector;
}


void CFriendEventApplyDialog::closeDialog()
{
    removeFromParentAndCleanup(true);
    if (m_pCloseHandler && m_pCloseSelector)
    {
        (m_pCloseHandler->*m_pCloseSelector)();
    }
}