//
//  RegisterLayer.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-8-29.
//
//

#include "RegisterLayer.h"
#include "LoginScene.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"

CRegisterLayer* CRegisterLayer::create(void* parent)
{
    CRegisterLayer* layer = CRegisterLayer::create();
    layer->m_Parent = parent;
    return layer;
}

bool CRegisterLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    m_Parent = NULL;
    
    CCLayerColor* bg= CCLayerColor::create(ccc4(0, 0, 0, 230));
    this->addChild(bg,-1);
    
    int x=300,y=660,h=80;

    CCLabelTTF* pTTFName = CCLabelTTF::create();
    pTTFName->setString("昵称   ");
    pTTFName->setFontSize(30);
    pTTFName->setPosition(ccp(x,y));
    addChild(pTTFName);
    
    // name
    m_pEditName = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditName->setPosition(ccp(x+300,y));
    m_pEditName->setPlaceHolder("Name");
    addChild(m_pEditName);
    
    CCLabelTTF* pTTFEmail = CCLabelTTF::create();
    pTTFEmail->setString("账号   ");
    pTTFEmail->setFontSize(30);
    pTTFEmail->setPosition(ccp(x,y-h*1));
    addChild(pTTFEmail);
    
    // Email
    m_pEditEMail = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditEMail->setPosition(ccp(x+300,y-h*1));
    m_pEditEMail->setPlaceHolder("E-Mail");
    m_pEditEMail->setInputMode(kEditBoxInputModeEmailAddr);
    addChild(m_pEditEMail);
    
    CCLabelTTF* pTTFPassword = CCLabelTTF::create();
    pTTFPassword->setString("密码   ");
    pTTFPassword->setFontSize(30);
    pTTFPassword->setPosition(ccp(x,y-h*2));
    addChild(pTTFPassword);
    
    // Password
    m_pEditPassword = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditPassword->setPosition(ccp(x+300,y-h*2));
    m_pEditPassword->setPlaceHolder("Password");
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    addChild(m_pEditPassword);
    
    CCLabelTTF* pTTFPassword_re = CCLabelTTF::create();
    pTTFPassword_re->setString("密码确认");
    pTTFPassword_re->setFontSize(30);
    pTTFPassword_re->setPosition(ccp(x,y-h*3));
    addChild(pTTFPassword_re);
    
    // Password_re
    m_pEditPassword_re = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditPassword_re->setPosition(ccp(x+300,y-h*3));
    m_pEditPassword_re->setPlaceHolder("Password");
    m_pEditPassword_re->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword_re->setInputMode(kEditBoxInputModeSingleLine);
    addChild(m_pEditPassword_re);

    CCMenuItemImage* pSureItem = CCMenuItemImage::create(
                                                    "resource_cn/img/tabButton_normal.png",
                                                    "resource_cn/img/tabButton_selected.png",
                                                    this,
                                                    menu_selector(CRegisterLayer::menuRegisterCallback) );
    CCLabelTTF* inf = CCLabelTTF::create();
    inf->setString("确定");
    inf->setFontSize(25);
    inf->setPosition(ccp(63, 17));
    pSureItem->setPosition( ccp(432, 200) );
    pSureItem->addChild(inf);
    
    CCMenuItemImage* pCancelItem = CCMenuItemImage::create(
                                                         "resource_cn/img/tabButton_normal.png",
                                                         "resource_cn/img/tabButton_selected.png",
                                                         this,
                                                         menu_selector(CRegisterLayer::menuCancelCallback) );
    CCLabelTTF* infCancel = CCLabelTTF::create();
    infCancel->setString("取消");
    infCancel->setFontSize(25);
    infCancel->setPosition(ccp(63, 17));
    
    pCancelItem->setPosition( ccp(592, 200) );
    pCancelItem->addChild(infCancel);

    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pSureItem,pCancelItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    m_pErrInf = CCLabelTTF::create();
    m_pErrInf->setFontSize(28);
    m_pErrInf->setPosition(ccp(512,720));
    addChild(m_pErrInf);

    return true;
}
void CRegisterLayer::menuCancelCallback(CCObject* pSender)
{
    removeFromParentAndCleanup(true);
}

void CRegisterLayer::menuRegisterCallback(CCObject* pSender)
{
    const char* pchData = m_pEditName->getText();
    if(!m_pEditName->getText())
    {
        m_pErrInf->setString("昵称不能为空！");
    }
    else if(!m_pEditEMail->getText())
    {
        m_pErrInf->setString("账号不能为空！");
    }
    else if(!strchr(m_pEditEMail->getText(),'@'))
    {
        m_pErrInf->setString("账号格式异常！");
    }
    else if(!m_pEditPassword->getText())
    {
        m_pErrInf->setString("密码不能为空！");
    }
    else if(!m_pEditPassword_re->getText())
    {
        m_pErrInf->setString("确认密码不能为空！");
    }
    else if(strcmp(m_pEditPassword->getText(), m_pEditPassword_re->getText()))
    {
        m_pErrInf->setString("两次密码输入不一致！");
    }
    else
    {
        m_pErrInf->setString("注册中……");
        doRegiter();
    }
    
}

void  CRegisterLayer::doRegiter()
{
    char achData[256]={};
    memset(achData, 0, 256);
    sprintf(achData, "name=%s&password=123",m_pEditEMail->getText());
    ADDHTTPREQUESTPOSTDATA(STR_URL_REGISTER,
                           "CALLBACK_CRegisterLayer_doRegiter",
                           "REQUEST_CRegisterLayer_doRegiter",
                           achData,
                           callfuncO_selector(CRegisterLayer::onReceiveRegiterMsg));
//    CPtHttpClient::addRequest(STR_URL_REGISTER,"CALLBACK_CRegisterLayer_doRegiter" , "REQUEST_CRegisterLayer_doRegiter");
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(REGITER_SUCCESS,NULL);
////    STR_URL_REGISTER;
//    
//    removeFromParentAndCleanup(true);

}

void CRegisterLayer::onReceiveRegiterMsg(CCObject *pOject)
{
    char* data = (char*)pOject;
    if(!data)
    {
        m_pErrInf->setString("网络异常,请重试");
    }
    else if(strstr(data,"10102"))
    {
        m_pErrInf->setString("邮箱已注册");
    }
    else
    {
        m_pErrInf->setString("注册成功");
        CCDictionary* dic = (CCDictionary*)PtJsonUtility::JsonStringParse(data)->objectForKey("result");
        CCDictionary* usrData = (CCDictionary*)dic->objectForKey("user");
        CCString* uid = (CCString*)usrData->objectForKey("puid");
        CCUserDefault::sharedUserDefault()->setIntegerForKey("uid", atoi(uid->m_sString.c_str()));
        CCNotificationCenter::sharedNotificationCenter()->postNotification(REGITER_SUCCESS,NULL);
        removeFromParentAndCleanup(true);
        //        m_pErrInf->setString(uid->m_sString.c_str());
    }
}