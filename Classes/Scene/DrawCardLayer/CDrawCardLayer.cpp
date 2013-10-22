//
//  CDrawCardLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-10-18.
//
//

#include "CDrawCardLayer.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "gameTools.h"
#include "CReward.h"
#include "CGameButtonControl.h"
#include "CPanelLoadingLayer.h"

const int g_niudanjiage=1; //钻石抽单价

CDrawCardLayer::CDrawCardLayer()
{
    m_pTouchBackUpd=NULL;
    size=CCDirector::sharedDirector()->getWinSize();
    m_tempGamePlayer=SinglePlayer::instance();
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"niudanchouka.plist"));
    touchVector.clear();
}

CDrawCardLayer::~CDrawCardLayer()
{
   CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"niudanchouka.plist"));
}

void CDrawCardLayer::createBackGround()
{
    //CCLayerColor *layercolor=CCLayerColor::create(ccc4(0,0,0,200));
    //addChild(layercolor,0);
    CCSprite *bgSprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,"niudanbeijing.png"));
    addChild(bgSprite,0);
    bgSprite->setPosition(ccp(size.width*0.5,size.height*0.5));
    
}
void CDrawCardLayer::removeLoadingLayer()
{
    if(getChildByTag(777))
    {
        removeChildByTag(777, true);//, bool cleanup
    }
}

void CDrawCardLayer::createTopButton()
{
    float scenemidPointx=size.width*0.5;
    float scenemidPointy=size.height*0.5+228;
    string word = Utility::getWordWithFile("word.plist", "fanhui");
    CGameButtonControl *fanhuiButton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(fanhuiButton,1,100);
    fanhuiButton->setPosition(ccp(scenemidPointx+180, scenemidPointy));
    word = Utility::getWordWithFile("word.plist", "qushangcheng");
    
    CGameButtonControl *qushangcheng=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(qushangcheng,1,101);
    qushangcheng->setPosition(ccp(scenemidPointx+300, scenemidPointy));
    touchVector.push_back(fanhuiButton);
    touchVector.push_back(qushangcheng);
}

void CDrawCardLayer::createLoadingLayer()
{
    CPanelLoadingLayer *loadinglayer=CPanelLoadingLayer::create();
    addChild(loadinglayer,5,777);
    loadinglayer->setPosition(ccp(size.width*0.5,size.height*0.5));
}

void CDrawCardLayer::createBackYingLing()
{
    if (!getChildByTag(103)) {
        CGameButtonControl *putongyingling=CGameButtonControl::createButton(TEXTMID, "", "putongjitain.png", "putongjitaip.png");
        putongyingling->setOpacity(0);
        addChild(putongyingling,1,103);
        putongyingling->setPosition(ccp(size.width *0.5-400,size.height*0.5));
    }
    if (!getChildByTag(104)) {
        
        CGameButtonControl *jueshiyingling=CGameButtonControl::createButton(TEXTMID, "", "juesejitain.png", "juesejitaip.png");
        addChild(jueshiyingling,1,104);
        jueshiyingling->setOpacity(0);
        jueshiyingling->setPosition(ccp(size.width *0.5+400,size.height*0.5));
    }
    getChildByTag(103)->runAction(CCSpawn::create(CCMoveBy::create(0.2, ccp(200,0)),CCFadeIn::create(0.2),NULL));
    getChildByTag(104)->runAction(CCSpawn::create(CCMoveBy::create(0.2, ccp(-200,0)),CCFadeIn::create(0.2),NULL));
    touchVector.push_back((CGameButtonControl *)getChildByTag(103));
    touchVector.push_back((CGameButtonControl *)getChildByTag(104));
}


bool CDrawCardLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    createBackGround();
    createTopButton();
    createLoadingLayer();
    setTouchEnabled(true);
    m_httpStatus=EN_NIUDANHTTPSTATUS_INIT;
    setTouchPriority(-9);
    scheduleOnce(schedule_selector(CDrawCardLayer::initSendGetData), 0.5);
    return true;
}

void CDrawCardLayer::createGetCardLayer(EN_NIUDANTYPE  tempEntype)
{
    
}




//检查是否可以扭蛋。
bool CDrawCardLayer::checkIsCanDoDrawCard(EN_NIUDANTYPE entype)
{
    if (entype==EN_NIUDANTYPE_CASH) {
        return m_stempScanData[1].m_nCount>0;
    }
    else
        return m_stempScanData[0].m_nCount>0;
}

void CDrawCardLayer::updateFriendTimer(float t)
{
    m_stempScanData[0].m_nNextTimer--;
    createLabelFriendAndUpdate();
    if (m_stempScanData[0].m_nNextTimer==0)
    {
        unschedule(schedule_selector(CDrawCardLayer::updateFriendTimer));
        sendGetDataHttp(EN_NIUDANTYPE_FRIEND);
    }
    
}

void CDrawCardLayer::updateDiamondTimer(float t)
{
    m_stempScanData[1].m_nNextTimer--;
    createLabelCashAndUpdate();
    if (m_stempScanData[1].m_nNextTimer==0) {
        unschedule(schedule_selector(CDrawCardLayer::updateDiamondTimer));
        sendGetDataHttp(EN_NIUDANTYPE_CASH);//(0.0);
    }
}

//更新界面
void CDrawCardLayer::updateButton()
{
    
}

//和下面的那个 代码可以重构
void CDrawCardLayer::createLabelFriendAndUpdate()
{
    if (!getChildByTag(11)) {
        CCLabelTTF *label=CCLabelTTF::create("", "Arial", 15);
        label->setColor(ccc3(0, 255, 0));
        label->setPosition(ccp(size.width *0.5 -200,size.height*0.5-180));
        addChild(label,1,11);
    }
    CCLabelTTF *label=(CCLabelTTF *)getChildByTag(11);
    
    if(m_stempScanData[0].m_nCount>0)
    {
        char data[40];
        sprintf(data, "可以免费抽取%d次",m_stempScanData[0].m_nCount);
        label->setString(data);
    }
    else{
        label->setString(CPtTool::stringForObjectValue(m_stempScanData[0].m_nNextTimer));
    }
    
}

void CDrawCardLayer::createLabelCashAndUpdate()
{
    if (!getChildByTag(12)) {
        CCLabelTTF *label=CCLabelTTF::create("", "Arial", 15);
        label->setColor(ccc3(0, 255, 0));
        label->setPosition(ccp(size.width *0.5 +200,size.height*0.5-180));
        addChild(label,1,12);
    }
    CCLabelTTF *label=(CCLabelTTF *)getChildByTag(12);
    if(m_stempScanData[1].m_nCount>0)
    {
        char data[40];
        sprintf(data, "可以免费抽取%d次",m_stempScanData[1].m_nCount);
        label->setString(data);
    }
    else
    {
        label->setString(CPtTool::stringForObjectValue(m_stempScanData[1].m_nNextTimer));
    }
}
void CDrawCardLayer::hideLable()
{
    getChildByTag(11)->setVisible(false);
    getChildByTag(12)->setVisible(false);
}
void CDrawCardLayer::decodeParseFriendDraw(CCObject *object)
{
    decodeGetData(object,EN_NIUDANTYPE_FRIEND);
}

void CDrawCardLayer::decodeParseCashDraw(CCObject *object)
{
    decodeGetData(object,EN_NIUDANTYPE_CASH);

}

void CDrawCardLayer::sendHttpToFriendGetValue(float t)
{
    if(!sendGetDataHttp(EN_NIUDANTYPE_FRIEND))
    {
        scheduleOnce(schedule_selector(CDrawCardLayer::sendHttpToFriendGetValue), 5.0);
    }
}

void CDrawCardLayer::sendHttpToCashGetValue(float t)
{
    if(!sendGetDataHttp(EN_NIUDANTYPE_CASH))
    {
        scheduleOnce(schedule_selector(CDrawCardLayer::sendHttpToCashGetValue), 5.0);
    }
}
void CDrawCardLayer::sendGetCardRandom(EN_NIUDANTYPE entype, int number,bool isFree)
{

    createLoadingLayer();
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    postStrdata+="&type="+ConvertToString((int)entype)+"&num="+ConvertToString(number)+"&is_free="+ConvertToString((int)isFree);
;
       ADDHTTPREQUESTPOSTDATA(STR_URL_CARDRANDOM(194), "CALLBACK_CDrawCardLayer_sendGetCardRandom", "REQUEST_CSceneGameShopLayer_sendGetCardRandom",postStrdata.c_str(),callfuncO_selector(CDrawCardLayer::decodeGetCardRandom));
    
}

void CDrawCardLayer::showCardData(CCDictionary *dict)
{
    CCArray *array=dict->allKeys();;
    for (int i=0; i<array->count(); i++)
    {
        CCString *key=(CCString *)array->objectAtIndex(i);
        CFightCard *card=m_tempGamePlayer->findFightCardByCard_User_ID(key->intValue());
        
    }
    
}

void CDrawCardLayer::decodeGetCardRandom(CCObject *object)
{
    removeLoadingLayer();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CDrawCardLayer_sendGetCardRandom");
    if (!object) {
        CCMessageBox("error", "error");
        return ;
    }
    char *strdata=(char *)object;
    CCLog("%s",strdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut ==0)
    {
        CCDictionary *resultdict=(CCDictionary *)dict->objectForKey("result");
        if (resultdict) {
            //更新下 数据  然后添加卡包数据。 最后调用更新界面数据 最后
            //更新下数据
            unschedule(schedule_selector(CDrawCardLayer::updateDiamondTimer));
            unschedule(schedule_selector(CDrawCardLayer::updateFriendTimer));
            startUpdateCashAndFriendly(resultdict);
            //添加卡牌
            CCDictionary * tmp = (CCDictionary*) resultdict->objectForKey("reward");
            if (tmp)
            {
                CReward *reward =NULL;
                if (tmp->objectForKey("add") &&((CCDictionary*)tmp->objectForKey("add"))->objectForKey("card_item") ) {
                    reward = CReward::create((CCDictionary *)((CCDictionary*)tmp->objectForKey("add"))->objectForKey("card_item"));
                    reward->excuteReward(ADD);
                    
                }
                //显示卡牌数据
                
                
                
//                if(tmp->objectForKey("dec") &&((CCDictionary*)tmp->objectForKey("dec"))->objectForKey("card_item"))
//                {
//                    reward = CReward::create((CCDictionary *)((CCDictionary*)tmp->objectForKey("dec"))->objectForKey("card_item"));
//                    reward->excuteReward(DEC);
//                }
                //获得卡牌的数组并显示出来;
            }
            m_httpStatus=EN_NIUDANHTTPSTATUS_END;
            return ;
        }
    }
    m_httpStatus=EN_NIUDANHTTPSTATUS_END;
    CCMessageBox("服务端传递数据有错误", "error");
    return ;

    
    
}

void CDrawCardLayer::initSendGetData(float t)
{
//开始读取服务器的 友情点数时间和 钻石 各免费时间
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETCARDRANDOM(194), "CALLBACK_CDrawCardLayer_initSendGetData", "REQUEST_CSceneGameShopLayer_getLimitItems",postStrdata.c_str(),callfuncO_selector(CDrawCardLayer::decodeInitEnterDrawCardLayer));
}

bool CDrawCardLayer::getScanData(const char *value,CCDictionary *dict,ScanDrawValue &rebackValue)
{
    CCDictionary *StrcutValue=(CCDictionary *)dict->objectForKey(value);
    if(StrcutValue)
    {
        rebackValue.m_nCount=GameTools::intForKey("free_times", StrcutValue);
        rebackValue.m_nNextTimer=GameTools::intForKey("leave_time", StrcutValue);
        return true;
    }
    return false;
    
}

void CDrawCardLayer::moveOutEyeSize()
{
    for(vector<CGameButtonControl *>::iterator it=touchVector.begin(); it!=touchVector.end(); )
    {
        if(*it == getChildByTag(103)||*it == getChildByTag(104))
        {
            it = touchVector.erase(it); //不能写成arr.erase(it);
        }
        else
        {
            ++it;
        }
    }
    CCNode *node=getChildByTag(103);
    node->runAction(CCSpawn::create(CCMoveBy::create(0.2, ccp(-200,0)),CCFadeOut::create(0.2),NULL));
    node=getChildByTag(104);
    node->runAction(CCSpawn::create(CCMoveBy::create(0.2, ccp(+200,0)),CCFadeOut::create(0.2),NULL));
}

bool CDrawCardLayer::touchNumberAndType(int number, EN_NIUDANTYPE typeTouch,bool isFree)
{
    if (isFree) {
        m_httpStatus=EN_NIUDANHTTPSTATUS_RUNNINT;
        moveOutEyeSize();
        hideLable();
        if (typeTouch == EN_NIUDANTYPE_CASH)
        {
            if (m_stempScanData[1].m_nCount >= number) {
                sendGetCardRandom(typeTouch, number, true);
            }
        }
        else if(typeTouch==EN_NIUDANTYPE_FRIEND)
        {
            if (m_stempScanData[0].m_nCount >= number) {
                sendGetCardRandom(typeTouch, number, true);
            }
        }
    }
    else{
        
        //先移动走两个按钮
        //添加两个按钮移动到
    }
    
}



//初始化的时候，只需要一次请求就得到所有的值得 然后启动定时器
void CDrawCardLayer::decodeInitEnterDrawCardLayer(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CDrawCardLayer_initSendGetData");
    if(!object)
    {
        CCMessageBox("服务端发送数据为空", "服务端");
        return;
    }
    char *strdata=(char *)object;
    CCLog("%s",strdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut ==0)
    {
        string teamStrType;
        CCDictionary *resualt=(CCDictionary*)(dict->objectForKey("result"));
        if(resualt)
        {
            startUpdateCashAndFriendly(resualt);
            removeLoadingLayer();
            createBackYingLing();
            m_httpStatus=EN_NIUDANHTTPSTATUS_END;
        }
    }
}

bool CDrawCardLayer::sendGetDataHttp(EN_NIUDANTYPE type)
{
    ScanDrawValue *pData=NULL;
    string callBackStr;
    SEL_CallFuncO callBackFunc;
    if (type==EN_NIUDANTYPE_FRIEND) {
        pData=&m_stempScanData[0];
        callBackStr="CALLBACK_CDrawCardLayer_sendHttpToFriendGetValue";
        callBackFunc=callfuncO_selector(CDrawCardLayer::decodeParseFriendDraw);
    }
    else if(type==EN_NIUDANTYPE_CASH)
    {
        pData=&m_stempScanData[1];
        callBackStr="CALLBACK_CDrawCardLayer_sendHttpToCashGetValue";
        callBackFunc=callfuncO_selector(CDrawCardLayer::decodeParseCashDraw);
    }else{
        return  false;
    }
        CCLog("sendHttpToFriendGetValue %d",m_stempScanData[0].m_nCount);
        //开始读取服务器的 友情点数时间和 钻石 各免费时间
        string postStrdata="sig=";
        postStrdata+=SinglePlayer::instance()->getUserSig();
        ADDHTTPREQUESTPOSTDATA(STR_URL_GETCARDRANDOM(194), callBackStr.c_str(), "REQUEST_CDrawCardLayer_NiuDan",postStrdata.c_str(),callBackFunc);
    return true;
}

void CDrawCardLayer::createTableView()
{
    
}




void CDrawCardLayer::decodeGetData(cocos2d::CCObject *object, EN_NIUDANTYPE type)
{
    ScanDrawValue *pData;
    string callStr;
    string valueStr;
    SEL_SCHEDULE schFunction;
    if (type==EN_NIUDANTYPE_FRIEND) {
        pData=&m_stempScanData[0];
        callStr="CALLBACK_CDrawCardLayer_sendHttpToFriendGetValue";
        valueStr="friendly";
        schFunction=schedule_selector(CDrawCardLayer::updateFriendTimer);
    }
    else if(type==EN_NIUDANTYPE_CASH)
    {
        pData=&m_stempScanData[1];
        callStr="CALLBACK_CDrawCardLayer_sendHttpToCashGetValue";
        valueStr="Cash";
        schFunction=schedule_selector(CDrawCardLayer::updateDiamondTimer);
    }
    else
        return;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,callStr.c_str());
    if (!object)
    {
        CCMessageBox("请求服务器扭蛋次数的数据为NULL", "错误");
        return;
    }
    else
    {
        char *strdata=(char *)object;
        CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
        delete []strdata;
        strdata=NULL;
        int codeReslut=GameTools::intForKey("code", dict);
        if(codeReslut ==0)
        {
            string teamStrType;
            CCDictionary *resualt=(CCDictionary*)(dict->objectForKey("result"));
            if(resualt)
            {
                
                if(!getScanData(valueStr.c_str(),resualt,*pData))
                {
                    CCMessageBox("返回没有扭蛋信息 服务端错误", "服务端错误");
                }
            }
        }
        if(pData->m_nNextTimer>0)
        {
            schedule(schFunction, 1.0);
        }
    }
   
}

void CDrawCardLayer::startUpdateCashAndFriendly(cocos2d::CCDictionary *resultDict)

{
    if(!getScanData("friendly",resultDict,m_stempScanData[0]))
    {
        CCMessageBox("返回没有friendly信息 服务端错误", "服务端错误");
        
    }
    if (m_stempScanData[0].m_nNextTimer>0) {
        schedule(schedule_selector(CDrawCardLayer::updateFriendTimer),1.0);//,kCCRepeatForever, m_stempScanData[0].m_nNextTimer);
    }
    else
    {
        createLabelFriendAndUpdate();
    }
    
    if(!getScanData("cash", resultDict, m_stempScanData[1]))
    {
        CCMessageBox("返回没有cash抽卡信息 服务端错误", "服务端错误");
    }
    
    if(m_stempScanData[1].m_nNextTimer>0)
    {
        schedule(schedule_selector(CDrawCardLayer::updateDiamondTimer), 1.0);//,kCCRepeatForever, m_stempScanData[0].m_nNextTimer);
    }
    else{
       createLabelCashAndUpdate();
    }
    
}

void CDrawCardLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,getTouchPriority(), true);
}

void CDrawCardLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

CGameButtonControl *CDrawCardLayer::getTouchButton(CCPoint point)
{
    for (int i=0; i<touchVector.size(); i++)
    {
        if(touchVector[i]->boundingBox().containsPoint(point))
        {
            if (m_pTouchBackUpd != touchVector[i])
            {
                if (m_pTouchBackUpd)
                {
                       m_pTouchBackUpd->unselected();
                }
                m_pTouchBackUpd=touchVector[i];
                m_pTouchBackUpd->selected();
                return m_pTouchBackUpd;
            }
            else{
                return m_pTouchBackUpd;
            }
        }
    }
    if(m_pTouchBackUpd)
    {
        m_pTouchBackUpd->unselected();
        m_pTouchBackUpd=NULL;
    }
    return NULL;
}

bool CDrawCardLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_httpStatus!=EN_NIUDANHTTPSTATUS_END) {
        return true;
    }
    m_pTouchBackUpd=NULL;
    CCPoint touchPoint=pTouch->getLocation();
    getTouchButton(touchPoint);
    return true;
}

void CDrawCardLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_httpStatus!=EN_NIUDANHTTPSTATUS_END) {
        return ;
    }
    CCPoint touchPoint=pTouch->getLocation();
    getTouchButton(touchPoint);
}

void CDrawCardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_httpStatus!=EN_NIUDANHTTPSTATUS_END) {
        return ;
    }
    CCPoint touchPoint=pTouch->getLocation();
    m_pTouchBackUpd=getTouchButton(touchPoint);
    CCLog("m_pTouchBackUpd:%x",m_pTouchBackUpd);
    if (m_pTouchBackUpd!=NULL)
    {
        m_pTouchBackUpd->unselected();
        dealWithTag(m_pTouchBackUpd->getTag());
        m_pTouchBackUpd=NULL;
    }
}

void CDrawCardLayer::dealWithTag(int tag)
{

    switch (tag) {
        case 100:
            removeFromParentAndCleanup(true);
            break;
        case 101:
            //发送调用商城界面
            break;
        case 102:
            break;
        case 103:
            if(m_stempScanData[0].m_nCount >= 1)
            {
                touchNumberAndType(1, EN_NIUDANTYPE_FRIEND, true);
            }
            else
            {
               touchNumberAndType(1, EN_NIUDANTYPE_FRIEND, false);
            }
            break;
        case 104:
            if(m_stempScanData[1].m_nCount >= 1)
            {
                touchNumberAndType(1, EN_NIUDANTYPE_CASH, true);
            }
            else{
               touchNumberAndType(1, EN_NIUDANTYPE_CASH, false);
            }
            break;
        default:
            break;
    }
}

void CDrawCardLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

