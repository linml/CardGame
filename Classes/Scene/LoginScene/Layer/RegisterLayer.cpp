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
#include "gameMiddle.h"
#include "Pt_AES.h"

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
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
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
    m_pEditName->setMaxLength(14);
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
    
    m_pEditName->setText("wood");
    m_pEditPassword->setText("yu");
    m_pEditPassword_re->setText("yu");
    char email[256]="";
    sprintf(email, "wood@%d",int(time(0)));
    m_pEditEMail->setText(email);

    return true;
}
void CRegisterLayer::menuCancelCallback(CCObject* pSender)
{
    removeFromParentAndCleanup(true);
}

void CRegisterLayer::menuRegisterCallback(CCObject* pSender)
{
    
    //const char* pchData = m_pEditName->getText();
    if(!m_pEditName->getText()||!strcmp(m_pEditName->getText(), ""))
    {
        Middle::showAlertView("昵称不能为空！");
    }
    else if(m_pEditName->getText() && strchr(m_pEditName->getText(),' '))
    {
        Middle::showAlertView("昵称不能含空格！");
    }
    else if(!m_pEditEMail->getText())
    {
        Middle::showAlertView("账号不能为空！");
    }
    else if(!strchr(m_pEditEMail->getText(),'@'))
    {
        Middle::showAlertView("账号格式异常！");
    }
    else if(!m_pEditPassword->getText())
    {
        Middle::showAlertView("密码不能为空！");
    }
    else if(!m_pEditPassword_re->getText())
    {
        Middle::showAlertView("确认密码不能为空！");
    }
    else if(strcmp(m_pEditPassword->getText(), m_pEditPassword_re->getText()))
    {
        Middle::showAlertView("两次密码输入不一致！");
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
    char pass[64] ="";
    memset(pass, 0, 64);
    string strPassword = m_pEditPassword->getText();
    char pKey[]="cube";
    Pt_AES::sharePtAESTool(pKey)->EncryptString(strPassword.c_str(),pass);     
    sprintf(achData, "name=%s&password=%s",m_pEditEMail->getText(),pass);
    ADDHTTPREQUESTPOSTDATA(STR_URL_REGISTER,
                           "CALLBACK_CRegisterLayer_doRegiter",
                           "REQUEST_CRegisterLayer_doRegiter",
                           achData,
                           callfuncO_selector(CRegisterLayer::onReceiveRegiterMsg));
    string fileName = CCFileUtils::sharedFileUtils()->getWriteablePath()+"password";
    FILE* file = fopen(fileName.c_str(),"wb");
    if(file)
    {
        fputs(pass, file);
    }
    fclose(file);

}

void CRegisterLayer::onReceiveRegiterMsg(CCObject *pOject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CRegisterLayer_doRegiter");
    char* data = (char*)pOject;
    if(!data)
    {
        Middle::showAlertView("网络异常,请重试");
    }
    else if(strstr(data,"10102"))
    {
        Middle::showAlertView("邮箱已注册");
    }
    else
    {
        m_pErrInf->setString("注册成功");
        CCUserDefault::sharedUserDefault()->setStringForKey("name", m_pEditName->getText());
        CCUserDefault::sharedUserDefault()->setStringForKey("account", m_pEditEMail->getText());
        CCNotificationCenter::sharedNotificationCenter()->postNotification(REGITER_SUCCESS,NULL);
        removeFromParentAndCleanup(true);
    }
}

bool CRegisterLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}