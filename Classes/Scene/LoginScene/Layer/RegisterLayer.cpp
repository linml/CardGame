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
    
    int x=150,y=660,h=80;

//    CCLabelTTF* pTTFName = CCLabelTTF::create();
//    pTTFName->setString("昵称   ");
//    pTTFName->setFontSize(30);
//    pTTFName->setPosition(ccp(x,y));
//    pTTFName->setAnchorPoint(ccp(0,0.5));
//    addChild(pTTFName);
//    
//    // name
//    m_pEditName = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
//    m_pEditName->setPosition(ccp(x+300,y));
//    m_pEditName->setMaxLength(14);
//    m_pEditName->setPlaceHolder("Name");
//    addChild(m_pEditName);
    
    CCLabelTTF* pTTFEmail = CCLabelTTF::create();
    pTTFEmail->setString("账号                                               （请填写邮箱）");
    pTTFEmail->setFontSize(30);
    pTTFEmail->setPosition(ccp(x,y-h*1));
    pTTFEmail->setAnchorPoint(ccp(0,0.5));
    addChild(pTTFEmail);
    
    // Email
    m_pEditEMail = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditEMail->setPosition(ccp(x+300,y-h*1));
    m_pEditEMail->setPlaceHolder("E-Mail");
    m_pEditEMail->setInputMode(kEditBoxInputModeEmailAddr);
    addChild(m_pEditEMail);
    
    CCLabelTTF* pTTFPassword = CCLabelTTF::create();
    pTTFPassword->setString("密码                                               （请输入6-8位密码）");
    pTTFPassword->setFontSize(30);
    pTTFPassword->setPosition(ccp(x,y-h*2));
    pTTFPassword->setAnchorPoint(ccp(0,0.5));
    addChild(pTTFPassword);
    
    // Password
    m_pEditPassword = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditPassword->setPosition(ccp(x+300,y-h*2));
    m_pEditPassword->setPlaceHolder("Password");
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setMaxLength(8);
    addChild(m_pEditPassword);
    
    CCLabelTTF* pTTFPassword_re = CCLabelTTF::create();
    pTTFPassword_re->setString("密码确认                                        （请确认两次输入是否一致）");
    pTTFPassword_re->setFontSize(30);
    pTTFPassword_re->setPosition(ccp(x,y-h*3));
    pTTFPassword_re->setAnchorPoint(ccp(0,0.5));
    addChild(pTTFPassword_re);
    
    // Password_re
    m_pEditPassword_re = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditPassword_re->setPosition(ccp(x+300,y-h*3));
    m_pEditPassword_re->setPlaceHolder("Password");
    m_pEditPassword_re->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword_re->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditPassword_re->setMaxLength(8);
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
    
    CCMenuItemFont* pLoginItem = CCMenuItemFont::create("已有账号？点此登录", this,menu_selector(CRegisterLayer::menuLoginCallback) );
    pLoginItem->setAnchorPoint(ccp(1, 0));
    pLoginItem->setPosition(ccp(1010, 10));
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pSureItem,pCancelItem, pLoginItem,NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    m_pErrInf = CCLabelTTF::create();
    m_pErrInf->setString("入学申请书");
    m_pErrInf->setFontSize(28);
    m_pErrInf->setPosition(ccp(512,720));
    addChild(m_pErrInf);
    
//    m_pEditName->setText("wood");
    m_pEditPassword->setText("123456");
    m_pEditPassword_re->setText("123456");
    char email[256]="";
    sprintf(email, "wood@%d",int(time(0)));
    m_pEditEMail->setText(email);
    
    return true;
}
void CRegisterLayer::menuCancelCallback(CCObject* pSender)
{
    removeFromParentAndCleanup(true);
}

void CRegisterLayer::menuLoginCallback(CCObject* pSender)
{
    CSubLoginLayer* login = CSubLoginLayer::create(m_Parent);
    this->getParent()->addChild(login,10000,361008);
    this->removeFromParentAndCleanup(true);
}

void CRegisterLayer::menuRegisterCallback(CCObject* pSender)
{
    
    //const char* pchData = m_pEditName->getText();
//    if(!m_pEditName->getText()||!strcmp(m_pEditName->getText(), ""))
//    {
//        Middle::showAlertView("昵称不能为空！");
//    }
//    else if(m_pEditName->getText() && strchr(m_pEditName->getText(),' '))
//    {
//        Middle::showAlertView("昵称不能含空格！");
//    }
    if(!m_pEditEMail->getText())
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
    char keydata[]="cube";
    Pt_AES::sharePtAESTool(keydata)->EncryptString(strPassword.c_str(),pass);
    //modify xianbei    sprintf(achData, "name=%s&password=%s",m_pEditEMail->getText(),pass);
    sprintf(achData, "name=%s&password=%s",m_pEditEMail->getText(),Pt_AES::sharePtAESTool()->getStringMD5(strPassword.c_str()));

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
//        CCUserDefault::sharedUserDefault()->setStringForKey("name", m_pEditName->getText());
        CCUserDefault::sharedUserDefault()->setStringForKey("account", m_pEditEMail->getText());
        CCNotificationCenter::sharedNotificationCenter()->postNotification(REGITER_SUCCESS,NULL);
        char inf[256]="";
        sprintf(inf, "登录(%s)",m_pEditEMail->getText());        
        CCLabelTTF* account = (CCLabelTTF*)(this->getParent()->getChildByTag(12345));
        account->setString(inf);
        removeFromParentAndCleanup(true);
    }
}

bool CRegisterLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

CSubLoginLayer* CSubLoginLayer::create(void* parent)
{
    CSubLoginLayer* layer = CSubLoginLayer::create();
    layer->m_Parent = parent;
    return layer;
}

CSubLoginLayer* create(void* parent,const char* pchName ,const char* pchPassword)
{
    CSubLoginLayer* layer = CSubLoginLayer::create();
    layer->m_Parent = parent;
    layer->m_pEditEMail->setText(pchName);
    layer->m_pEditPassword->setText(pchPassword);
    return layer;
}

bool CSubLoginLayer::init()
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
        
    
    CCLabelTTF* pTTFEmail = CCLabelTTF::create();
    pTTFEmail->setString("账号");
    pTTFEmail->setFontSize(30);
    pTTFEmail->setPosition(ccp(x,y-h*1));
    pTTFEmail->setAnchorPoint(ccp(0,0.5));
    addChild(pTTFEmail);
    
    // Email
    m_pEditEMail = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditEMail->setPosition(ccp(x+300,y-h*1));
    m_pEditEMail->setPlaceHolder("E-Mail");
    m_pEditEMail->setInputMode(kEditBoxInputModeEmailAddr);
    addChild(m_pEditEMail);
    
    CCLabelTTF* pTTFPassword = CCLabelTTF::create();
    pTTFPassword->setString("密码");
    pTTFPassword->setFontSize(30);
    pTTFPassword->setPosition(ccp(x,y-h*2));
    pTTFPassword->setAnchorPoint(ccp(0,0.5));
    addChild(pTTFPassword);
    
    // Password
    m_pEditPassword = CCEditBox::create(CCSizeMake(300, 50), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditPassword->setPosition(ccp(x+300,y-h*2));
    m_pEditPassword->setPlaceHolder("Password");
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setMaxLength(8);
    addChild(m_pEditPassword);
    
    
    CCMenuItemImage* pSureItem = CCMenuItemImage::create(
                                                         "resource_cn/img/tabButton_normal.png",
                                                         "resource_cn/img/tabButton_selected.png",
                                                         this,
                                                         menu_selector(CSubLoginLayer::menuLoginCallback) );
    CCLabelTTF* inf = CCLabelTTF::create();
    inf->setString("登录");
    inf->setFontSize(25);
    inf->setPosition(ccp(63, 17));
    pSureItem->setPosition( ccp(432, 200) );
    pSureItem->addChild(inf);
    
    CCMenuItemImage* pCancelItem = CCMenuItemImage::create(
                                                           "resource_cn/img/tabButton_normal.png",
                                                           "resource_cn/img/tabButton_selected.png",
                                                           this,
                                                           menu_selector(CSubLoginLayer::menuCancelCallback) );
    CCLabelTTF* infCancel = CCLabelTTF::create();
    infCancel->setString("取消");
    infCancel->setFontSize(25);
    infCancel->setPosition(ccp(63, 17));
    
    pCancelItem->setPosition( ccp(592, 200) );
    pCancelItem->addChild(infCancel);
    
    CCMenuItemFont* pLoginItem = CCMenuItemFont::create("没有账号？点此注册", this, menu_selector(CSubLoginLayer::menuNoAccountCallback));
    pLoginItem->setAnchorPoint(ccp(1, 0));
    pLoginItem->setPosition(ccp(1010, 10));
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pSureItem,pCancelItem, pLoginItem,NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    m_pErrInf = CCLabelTTF::create();
    m_pErrInf->setString("");
    m_pErrInf->setFontSize(28);
    m_pErrInf->setPosition(ccp(512,720));
    addChild(m_pErrInf);
    
    m_pEditEMail->setText(CCUserDefault::sharedUserDefault()->getStringForKey("account").c_str());
    
    return true;
}

void CSubLoginLayer::menuNoAccountCallback(CCObject* pSender)
{
    CRegisterLayer* login = CRegisterLayer::create();
    this->getParent()->addChild(login,10000);
    this->removeFromParentAndCleanup(true);
}

void CSubLoginLayer::menuLoginCallback(CCObject *pSender)
{
    if(!m_pEditEMail->getText())
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
    else
    {
        m_pErrInf->setString("注册中……");
        doLogin();
    }
 
}


void CSubLoginLayer::menuCancelCallback(CCObject* pSender)
{
    removeFromParentAndCleanup(true);
}

void CSubLoginLayer::doLogin()
{
    if(m_Parent)
    {
        char pass[64] ="";
        memset(pass, 0, 64);
        string strPassword = m_pEditPassword->getText();
        Pt_AES::sharePtAESTool()->EncryptString(strPassword.c_str(),pass);        
        CCUserDefault::sharedUserDefault()->setStringForKey("account", m_pEditEMail->getText());
        string fileName = CCFileUtils::sharedFileUtils()->getWriteablePath()+"password";
        FILE* file = fopen(fileName.c_str(),"wb");
        if(file)
        {
            fputs(pass, file);
        }
        fclose(file);
        ((CLoginScene*)m_Parent)->doLogin(m_pEditEMail->getText(),Pt_AES::sharePtAESTool()->getStringMD5(m_pEditPassword->getText()));
    }
}

bool CSubLoginLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}



CRegisterNickNameLayer* CRegisterNickNameLayer::create(void* parent)
{
    CRegisterNickNameLayer *pRet = new CRegisterNickNameLayer();
    pRet->m_Parent = parent;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet; 
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CRegisterNickNameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    m_nGender = 0;
    
    CCLayerColor* bg = CCLayerColor::create(ccc4(0, 0, 0,200));
    addChild(bg,-1);
    
    m_pEditName = CCEditBox::create(CCSizeMake(400, 60), CCScale9Sprite::create("resource_cn/img/tabButton_normal.png"));
    m_pEditName->setPosition(ccp(650,400));
    m_pEditName->setMaxLength(14);
    m_pEditName->setPlaceHolder("请填写昵称");
    addChild(m_pEditName);
    
    m_hero = CCSprite::create("resource_cn/img/play_male.png");
    m_hero->setPosition(ccp(200,384));
    addChild(m_hero);
    
    
    m_roleMale = CCSprite::create("resource_cn/img/headIcon_male.png");
    m_roleMale->setPosition(ccp(500,584));
    addChild(m_roleMale);
    Utility::addTouchRect(2000, m_roleMale, touchRect);
    
    m_roleFemale = CCSprite::create("resource_cn/img/headIcon_female.png");
    m_roleFemale->setPosition(ccp(800,584));
    addChild(m_roleFemale);
    Utility::addTouchRect(2001, m_roleFemale, touchRect);
    
    m_selectSprite = CCSprite::create("resource_cn/img/select.png");
    CCSize headIconSize = m_roleMale->getContentSize();
    m_selectSprite->setPosition(ccp(headIconSize.width/2-5,headIconSize.height/2+5));
    m_roleMale->addChild(m_selectSprite);
    CCActionInterval* rotate = CCRotateBy::create(0.2, 15);
    CCActionInterval* fade1 = CCFadeTo::create(0.5,50);
    CCActionInterval* fade2 = CCFadeTo::create(0.5,250);
    CCActionInterval*  fadeseq = (CCActionInterval*)(CCSequence::create(fade1, fade2, NULL));
    CCFiniteTimeAction* faderep = CCRepeatForever::create(fadeseq);
    CCFiniteTimeAction* rotaterep = CCRepeatForever::create(rotate);
    m_selectSprite->runAction(faderep);
    m_selectSprite->runAction(rotaterep);

    CCSprite* confirm = CCSprite::create("resource_cn/img/tabButton_normal.png");
    confirm->setPosition(ccp(500, 200));
    addChild(confirm);
    Utility::addTouchRect(1000, confirm, touchRect);
    CCLabelTTF* textConfirm = CCLabelTTF::create();
    textConfirm->setString("确认");
    textConfirm->setFontSize(24);
    textConfirm->setPosition(ccp(70, 15));
    confirm->addChild(textConfirm);
    
    CCSprite* cancel = CCSprite::create("resource_cn/img/tabButton_normal.png");
    cancel->setPosition(ccp(800, 200));
    addChild(cancel);
    Utility::addTouchRect(1001, cancel, touchRect);
    CCLabelTTF* textCancel = CCLabelTTF::create();
    textCancel->setString("取消");
    textCancel->setFontSize(24);
    textCancel->setPosition(ccp(70, 15));
    cancel->addChild(textCancel);


    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    return true;
}

bool CRegisterNickNameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CRegisterNickNameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    handleTouchSpritePool(pTouch->getLocation());
}

bool CRegisterNickNameLayer::handleTouchSpritePool(CCPoint point)
{
    bool bRet = false;
    int tag = -1;
    CCSprite* touchSprite = NULL;
    tag = TouchRect::SearchTouchTag(point, touchRect, &touchSprite);
    switch (tag) {
        case 1000:
            if(!m_pEditName->getText()||!strcmp(m_pEditName->getText(), ""))
            {
                Middle::showAlertView("昵称不能为空！");
            }
            else if(m_pEditName->getText() && strchr(m_pEditName->getText(),' '))
            {
                Middle::showAlertView("昵称不能含空格！");
            }
            else{
                if(m_Parent)
                {
                    CLoginScene* tmp = (CLoginScene*)m_Parent;
                    tmp->m_nickNameTip->setVisible(false);
                    tmp->m_nGender = m_nGender;
                    tmp->m_pEditName->setString(m_pEditName->getText());
                    removeFromParentAndCleanup(true);
                }
            }
            break;
        case 1001:
            removeFromParentAndCleanup(true);
            break;
        case 2000:
            if(m_selectSprite->getParent() != touchSprite)
            {
                m_selectSprite->removeFromParentAndCleanup(false);
                touchSprite->addChild(m_selectSprite);
                CCSprite* tex = CCSprite::create("resource_cn/img/play_male.png");
                m_hero->setTexture(tex->getTexture());
                m_nGender = 0;
            }
            break;
        case 2001:
            if(m_selectSprite->getParent() != touchSprite)
            {
                m_selectSprite->removeFromParentAndCleanup(false);
                touchSprite->addChild(m_selectSprite);
                CCSprite* tex = CCSprite::create("resource_cn/img/play_female.png");
                m_hero->setTexture(tex->getTexture());
                m_nGender = 1;
            }
            break;
    }
    return true;
}
