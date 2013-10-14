//
//  LoginLayer.cpp
//  91.cube
//
//  Created by phileas on 13-6-5.
//
//

#include "HallScene.h"

#include "LoginScene.h"
#include "LogoLayer.h"
#include "SceneManager.h"
#include "Utility.h"
#include "PtJsonUtility.h"
#include "CPtListViewWidget.h"
#include "CPtTableItem.h"
#include "gamePlayer.h"
#include "RegisterLayer.h"
#include "gameMiddle.h"
#include "Pt_AES.h"
#include "gameMiddle.h"
#include "Pt_AES.h"
#include "CPanelGameLog.h"

// implement of the CLoginScene:

CCScene* CLoginScene::scene()
{
    CCScene *scene = CCScene::create();
    
    do {
        
        CLoginScene *login = CLoginScene::create();
        scene->addChild(login, LOGIN_ZORDER);
    } while (0);
    
    return scene;
}


CLoginScene::CLoginScene()
{
    initLogCreate("log.log");
    isTouchPlayerGame=false;
}
CLoginScene::~CLoginScene()
{
    maps->release();
}

bool CLoginScene::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        CC_BREAK_IF(!initLogin());
        ///add  info;
        
        addLabelToShowPrecessInfo();
        
        bRet = true;
    } while (0);
    return bRet;
}

bool CLoginScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bLogoOver==false) {
        return false;
    }
    int tag = -1;
    CCPoint point = pTouch->getLocation();
    tag = TouchRect::SearchTouchTag(point, touchRect);
    if (tag != -1)
    {
        return true;
    }
    return  false;
}
void CLoginScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CLoginScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("LoginLayer::ccTouchEnded");
    handleTouchSpritePool(pTouch->getLocation());
}

void CLoginScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CLoginScene::doingCheckComplateTask()
{
    CGamePlayer *gamePlayer=SinglePlayer::instance();
    if (!gamePlayer->isHaveSendComplate()) {
        gamePlayer->postCompleteTask(gamePlayer->getCurrentTaskId(), this, callfuncO_selector(CLoginScene::dealWithCompleteTask), "CALLBACK_CLOGINSCENE_COMPLATETASK");
    }else
    {
        if(gamePlayer->getCurrentTaskId()==SingleTaskConfig::instance()->getMaxTaskId())
        {
            playGame();
            CCLog("完成了所有的任务了");
        }
        else{
            dointAddTask();
        }
    }
}
void CLoginScene::dointAddTask()
{
    CGamePlayer *gamePlayer=SinglePlayer::instance();
    int value=gamePlayer->getCurrentTaskId();
    CCLog("get Value %d",value);
    CPtTask *pttask=SingleTaskConfig::instance()->getNextByPreTask(value);
    CCLog("pttask->getTaskId() %d",pttask->getTaskId());
    gamePlayer->setCurrentTaskId(pttask->getTaskId());
    gamePlayer->postAddTask(gamePlayer->getCurrentTaskId(), this, callfuncO_selector(CLoginScene::dealWithAddTask), "CALLBACK_CLOGINSCENE_ADDTASK");
}

void CLoginScene::dealWithCompleteTask(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CLOGINSCENE_COMPLATETASK");
    if (!object) {
        CCMessageBox("服务端传输NULL数据Complatetask", "ERROR");
    }
    //判断code 是否等于0  如果等于0 代表校验成功， 之后执行doin
    int codeValue=0;
    char *buff=(char *)object;
    CCDictionary *tmp= PtJsonUtility::JsonStringParse(buff);
    delete [] buff;

    if (tmp)
    {
        codeValue  = GameTools::intForKey("code", tmp);
    }
    if(!codeValue)
    {
        CCDictionary *resualt=(CCDictionary*)(tmp->objectForKey("result"));
        //获得info的答案。
        if (resualt)
        {
            if(GameTools::intForKey("info", resualt)==1)
            {
                if(SinglePlayer::instance()->getCurrentTaskId()==SingleTaskConfig::instance()->getMaxTaskId())
                {
                    playGame();
                    CCLog("完成了所有的任务了");
                }
                else
                {
                    dointAddTask();
                }
                return;
            }
        }
    }
    char messageData[100];
    sprintf(messageData, "在初始化的时候服务端校验任务完成过不去,不能玩游戏 谢谢CODE::%d",codeValue);
    CCMessageBox(messageData,"ALTER");
}

void CLoginScene::dealWithAddTask(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CLOGINSCENE_COMPLATETASK");
    if (!object) {
        CCMessageBox("服务端传输NULL数据Complatetask", "ERROR");
    }
    int codeValue=0;
    char *buff=(char *)object;
    CCDictionary *tmp= PtJsonUtility::JsonStringParse(buff);
    delete [] buff;
    buff=NULL;
    if (tmp)
    {
        codeValue  = GameTools::intForKey("code", tmp);
        if (!codeValue) {
            CCDictionary *resualt=(CCDictionary*)(tmp->objectForKey("result"));
            //获得info的答案。
            if (resualt)
            {
                if(GameTools::intForKey("info", resualt)==1)
                   {
                       playGame();
                       return;
                   }
            }
        }
    }

    
    char messageData[100];
    sprintf(messageData, "服务端说不能添加本地的当前任务 出错CODE::%d",codeValue);
    CCMessageBox(messageData,"ALTER");
}

bool CLoginScene::handleTouchSpritePool(CCPoint point)
{
    bool bRet = false;
    int tag = -1;
    CCSprite* touchSprite = NULL;
    tag = TouchRect::SearchTouchTag(point, touchRect, &touchSprite);
    string strAccount = "";
    string strPassword = "";
    switch (tag) {
        case -1:
            
            break;
        case BUTTON_PLAY_TAG:
            strAccount = CCUserDefault::sharedUserDefault()->getStringForKey("account");
            //            strPassword = CCUserDefault::sharedUserDefault()->getStringForKey("password");
            if(strAccount != ""  )
            {
                doLogin();
            }
            else
            {
                CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLoginScene::notificationRegiterRecevice), REGITER_SUCCESS, NULL);
                addChild(CRegisterLayer::create(this),10000);
            }
            break;
        case 2000:
            CCUserDefault::sharedUserDefault()->setStringForKey("account", "");
            remove(CCUserDefault::sharedUserDefault()->getXMLFilePath().c_str());
            Middle::showAlertView("清除账号信息");
            CCUserDefault::sharedUserDefault()->setStringForKey("account", "");
            // CCUserDefault::sharedUserDefault()->flush();
            break;
        default:
            break;
    }
    
    return bRet;
}

void CLoginScene::doLogin()
{
    if (isTouchPlayerGame) {
        return ;
    }
    char achData[256]={};
    memset(achData, 0, 256);
    string strAccount = CCUserDefault::sharedUserDefault()->getStringForKey("account");
    //    string strPassword = CCUserDefault::sharedUserDefault()->getStringForKey("password").c_str();
    string fileName = CCFileUtils::sharedFileUtils()->getWriteablePath()+"password";
    FILE* file = fopen(fileName.c_str(), "rb");
    char achPassword[64] = "";
    if(file)
    {
        fgets(achPassword, 64, file);
    }
    fclose(file);
    sprintf(achData, "name=%s&password=%s",strAccount.c_str(),achPassword);
    ADDHTTPREQUESTPOSTDATA(STR_URL_LOGIN,
                           "CALLBACK_CLoginScene_doLogin",
                           "REQUEST_CLoginScene_doLogin",
                           achData,
                           callfuncO_selector(CLoginScene::onReceiveLoginMsg));
}

void CLoginScene::onReceiveLoginMsg(CCObject* obj)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CLoginScene_doLogin");
    char* data = (char*)obj;
    if(!data)
    {
        Middle::showAlertView("网络连接出错");
    }
    else if(strstr(data,"10101"))
    {
        Middle::showAlertView("密码错误");
    }
    else if(strstr(data,"10001"))
    {
        Middle::showAlertView("参数错误");
    }

    else
    {
        CCDictionary* dic = (CCDictionary*)PtJsonUtility::JsonStringParse(data)->objectForKey("result");
        CCDictionary* usrData = (CCDictionary*)dic->objectForKey("user");
        CCString* uid = (CCString*)usrData->objectForKey("puid");
        CCString* sig = (CCString*)usrData->objectForKey("sig");
        SinglePlayer::instance()->setUserId(uid->m_sString);
        SinglePlayer::instance()->setUserSig(sig->m_sString);
        scheudoLoadGameConfig();
    }
    
}

void CLoginScene::playGame()
{
    if(Utility::getNodeByTag(this, "0,2,0")->isVisible())
    {
        SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_HALLSCENE);
    }
}


// implement of the CLoginScene's callback method:

void CLoginScene::loginCallBack(cocos2d::CCNode *pSender)
{
    // to do the call back
}

void CLoginScene::setLogoOverCallBack()
{
    m_bLogoOver = true;
}

// implement ofthe  protected methods of CLoginScene:

bool CLoginScene::initLogin()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool bRet = false;
    do {
        // to do init
        // init variable
        m_bLogoOver = false;
        
        // add logo
        LogoLayer *logo = LogoLayer::create();
        this->addChild(logo, LOGO_ZORDER);;
        
        
        // note: remember release maps
        maps = LayoutLayer::create();
        maps->retain();
        maps->initWithFile(this, CSTR_FILEPTAH(plistPath, "login.plist"));
        
        
        CCNode* map = maps->getElementByTags("0");
        if (map)
        {
            CCSprite* bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "bg.png"));
            bg->setPosition(ccp(winSize.width/2, winSize.height/2));
            map->addChild(bg, -1);
        }
        
        if(Utility::getNodeByTag(this, "0,2,0"))
        {
            Utility::getNodeByTag(this, "0,2,0")->setVisible(true);
        }
        // set layer touche enable
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
        
        maps->getTouchRects(touchRect);
        bRet = true;
        isLoadCardBag=false;
        isLoadEndConfig=false;
        isLoadTeam=false;
        isGameInit=false;
        isLoadBackPack = false;
        //scheudoLoadGameConfig(); //by merlin
        
        CCLabelTTF* LogOut = CCLabelTTF::create();
        LogOut->setString("注销账号");
        LogOut->setFontSize(24);
        LogOut->setPosition(ccp(970,30));
        addChild(LogOut);
        Utility::addTouchRect(2000, LogOut, touchRect);
    } while (0);
    return bRet;
    
}

void CLoginScene::serverInf(CCDictionary* dic)
{
    CCDictElement* ele = NULL;
    STC_SERVER_INF inf;
    CCDICT_FOREACH(dic,ele)
    {
        CCDictionary* item = (CCDictionary*)ele->getObject();
        inf.m_nSid = item->valueForKey("sid")->intValue();
        inf.m_strName = item->valueForKey("name")->m_sString;
        inf.m_strIp = item->valueForKey("ip")->m_sString;
        inf.m_bIsUse = item->valueForKey("is_use")->intValue();
        vSerInf.push_back(inf);
    }
    
}

void CLoginScene::msgCallback(CCObject* obj)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "xianbeiTest0");
    CCDictionary* dic = PtJsonUtility::JsonStringParse((char*)obj);
    
    CCDictElement* ele = NULL;
    CCDICT_FOREACH(dic,ele)
    {
        string key = ele->getStrKey();
        if (key == "result") {
            CCDictionary* childDic = (CCDictionary*)ele->getObject();
            serverInf(childDic);
        }
    }
    printf("xianbei msg:%s\n",(char*)obj);
    CCArray * array = CCArray::create();
    for (int i = 0; i < vSerInf.size(); i++)
    {
        CCLabelTTF * label = CCLabelTTF::create(vSerInf[i].m_strName.c_str(), "arial", 12.0f);
        CCSprite * tmp = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "HelloWorld.png"), CCRectMake(0, 0 , 100, 40));
        CPtTableItem *item = CPtTableItem::create();
        item->addChild(tmp);
        item->addChild(label);
        label->setPosition(ccp(0, 0));
        label->setAnchorPoint(CCPointZero);
        array->addObject(item);
        // array->addObject(label);
        //item->setTouchNode(label);
        
    }
    
    CPtListViewWidget * listServer = CPtListViewWidget::create(array, CCSizeMake(300, 60), kCCScrollViewDirectionHorizontal, CCSizeMake(2, 5), 1);
    this->addChild(listServer);
    listServer->setAnchorPoint(CCPointZero);
    listServer->setPosition(ccp(30, 40));
    listServer->setBackGround(CCLayerColor::create(ccc4(125, 0, 0, 125)));
    
}

void CLoginScene::setText(const char *str)
{
    if(getChildByTag(1000))
    {
        CCLabelTTF *label=(CCLabelTTF *)(getChildByTag(1000));
        label->setString(str);
    }
}

bool CLoginScene::addLabelToShowPrecessInfo()
{
    CCLabelTTF *labelttf=CCLabelTTF::create("app init", "Arial", 40);
    addChild(labelttf,2,1000);
    labelttf->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width*0.5, CCDirector::sharedDirector()->getWinSize().height*0.5-200));
    labelttf->setColor(ccc3(255, 0, 0));
    return true;
}

void CLoginScene::scheudoLoadGameConfig()
{
    if(!isTouchPlayerGame)
    {
        isTouchPlayerGame=true;
        schedule(schedule_selector(CLoginScene::addFunctionInitGames));
    }
}

void CLoginScene::addFunctionInitGames(float t)
{
    CGamePlayer *pGamePlayer=SinglePlayer::instance();
    if(!isLoadEndConfig)
    {
        setText("loading  config");
        isLoadEndConfig=true;
    }
    else
    {
        if (!isGameInit) {
            setText("onGameInit");
            pGamePlayer->onGameBegin();
            isGameInit=true;
        }
        else{
            if(pGamePlayer->getGameInitStatus()!=0)
            {
                setText("game init ok");
                unschedule(schedule_selector(CLoginScene::addFunctionInitGames));
                if(pGamePlayer->getGameInitStatus()==1)
                {
                    setText("SetGameInit info");
                    if (pGamePlayer->getCurrentTaskId()!=0)
                    {
                        //先判断当前的任务是否完成 如果完成了 需要调用校验 然后添加新的任务 最后才执行play
                        if (!SinglePlayer::instance()->isSuccessFinishTask())
                        {
                            playGame();
                        }
                        else
                        {
                            int value=SinglePlayer::instance()->getCurrentTaskId();
                            if (value==SingleTaskConfig::instance()->getMaxTaskId())
                            {
                                SinglePlayer::instance()->setAllTaskCompleted(true);
                                playGame();
                                CCLog("完成了所有的任务了，恭喜");
                            }
                            else
                            {
                                doingCheckComplateTask();
                            }
                            //当前任务是完成的任务。
                        }

                    }
                    else{
                        dointAddTask();
                    }
                    
                }
                else
                {
                    Middle::showAlertView("下载数据出错");
                    isTouchPlayerGame=false;
                    isGameInit=false;
                    isLoadCardBag=false;
                    isLoadEndConfig=false;
                    isLoadTeam=false;;
                    isGameInit=false;
                    isLoadBackPack=false;
                }
            }
            else{
                
            }
        }
    }
}

void CLoginScene::notificationRegiterRecevice(CCObject* obj)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, REGITER_SUCCESS);
    Middle::showAlertView("注册成功");
//    doLogin();
}


