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
#include "gameStruct.h"
#include "CGamesCard.h"
#include "CGameRechargeTip.h"
#include "CGameArrangeCardBag.h"
#include "CCShake.h"
#include "CCardBagFullDialog.h"
#include "gameConfig.h"
#include "CAnnouncementDataManager.h"
#include "CDrawGonggaoTable.h"


const int g_niudanjiage=300; //钻石抽单价
const int g_niudanyouqingjiage=200;

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
    CEmrysClearVectorMemory< CFightCard  *> tempClear(m_vectorGetCardList) ;
    tempClear.clearVector();
    CEmrysClearVectorMemory< CGameButtonControl   *> tempClear2(touchVector) ;
    tempClear2.clearVector();
   CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"niudanchouka.plist"));
}

void CDrawCardLayer::createBackGround()
{
    CCSprite *bgSprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,"niudanbeijing.png"));
    addChild(bgSprite,0);
    bgSprite->setPosition(ccp(size.width*0.5,size.height*0.5));
    
}

void CDrawCardLayer::createJiantou()
{
    CCSprite *sprite=CCSprite::createWithSpriteFrameName("zuoN.png");
    addChild(sprite,4,51);
    sprite->setPosition(ccp(180,size.height*0.5));
    sprite=CCSprite::createWithSpriteFrameName("youN.png");
    addChild(sprite,4,52);
    sprite->setPosition(ccp(size.width*0.5+320,size.height*0.5));
}

void CDrawCardLayer::removeJiantouAndClearVector()
{
    m_vectorGetCardList.clear();
    CEmrysClearVectorMemory< CFightCard  *> tempClear(m_vectorGetCardList) ;
    tempClear.clearVector();
    for (int i=51; i<=52; i++)
    {
        if (getChildByTag(i)) {
            removeChildByTag(i, true);
        }
    }
   
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
    CGameButtonControl *gameQuitButton=CGameButtonControl::createButton(TEXTMID, "", "QuitButton_Normal.png", "QuitButton_Actived.png");
    addChild(gameQuitButton,2,100);
    gameQuitButton->setPosition(ccp(scenemidPointx+300, scenemidPointy));
    string word = Utility::getWordWithFile("word.plist", "qushangcheng");
    CGameButtonControl *qushangcheng=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(qushangcheng,1,101);
    qushangcheng->setPosition(ccp(scenemidPointx+180, scenemidPointy));
    touchVector.push_back(gameQuitButton);
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
    m_enLayerStatus=EN_NIUDANLAYER_CHOOSETYPE;
    m_httpStatus=EN_NIUDANHTTPSTATUS_INIT;
    createGoldAndCash();
    setTouchPriority(-9);
    scheduleOnce(schedule_selector(CDrawCardLayer::initSendGetData), 0.5);
    return true;
}

void CDrawCardLayer::clearSelectFreeItemLayer()
{
    if (m_enLayerStatus==EN_NIUDANLAYER_CHOOSETYPE) {
        bool flag=true;
        //删除100 101两个。
        if (getChildByTag(101)) {
            flag=true;
            removeChildByTag(101, true);
        }
        if (getChildByTag(100)) {
            removeChildByTag(100, true);
        }
        if (getChildByTag(103)) {
            removeChildByTag(103, true);
        }
        if (getChildByTag(104)) {
            removeChildByTag(104, true);
        }
        if (getChildByTag(11)) {
            removeChildByTag(11, true);
        }
        if (getChildByTag(12)) {
            removeChildByTag(12, true);
        }
        if(flag)
        {
            touchVector.clear();
        }
        m_enLayerStatus=EN_NIUDANLAYER_ENDGETCARD;
    }
 }

void CDrawCardLayer::createGoldAndCash()
{
    char data[40];
    if (!getChildByTag(21))
    {
        CCSprite *goldsprite=CCSprite::createWithSpriteFrameName("jinbi.png");
        addChild(goldsprite,3,22);
        goldsprite->setPosition(ccp(200,size.height*0.5+225));
        
        CCLabelTTF *goldlabel=CCLabelTTF::create("", "Arial", 15);
        addChild(goldlabel,3,21);
        goldlabel->setPosition(ccp(250,size.height*0.5+225));
    }
    
    sprintf(data, "%d",m_tempGamePlayer->getFriendly());
    ((CCLabelTTF *)getChildByTag(21))->setString(data);
    if (!getChildByTag(23))
    {
        CCSprite *cashsprite=CCSprite::createWithSpriteFrameName("zuanshiniudan.png");
        addChild(cashsprite,3,24);
        cashsprite->setPosition(ccp(size.width*0.5-30,size.height*0.5+225));
        CCLabelTTF *cashlabel=CCLabelTTF::create("", "Arial", 15);
        addChild(cashlabel,3,23);
        cashlabel->setPosition(ccp(size.width*0.5,size.height*0.5+225));
    }
     sprintf(data, "%d",m_tempGamePlayer->getPlayerCash());
     ((CCLabelTTF *)getChildByTag(23))->setString(data);
}

void CDrawCardLayer::createGetCardLayer(EN_NIUDANTYPE  tempEntype)
{

    clearSelectFreeItemLayer();
    createTableView();
    createNiudanButton(tempEntype);
}

void CDrawCardLayer::scrollViewDidScroll(CCScrollView* view)
{
   
}

void CDrawCardLayer::scrollViewDidZoom(CCScrollView* view)
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
    if (m_enLayerStatus!=EN_NIUDANLAYER_CHOOSETYPE) {
        return;
    }
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
    if (m_enLayerStatus!=EN_NIUDANLAYER_CHOOSETYPE) {
        return;
    }
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
    if(getChildByTag(11))
    {
        getChildByTag(11)->setVisible(false);
    }
    if (getChildByTag(12)) {
        getChildByTag(12)->setVisible(false);
    }

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



void CDrawCardLayer::showCardData(CCDictionary *dict)
{
    CCArray *array=dict->allKeys();;
    for (int i=0; i<array->count(); i++)
    {
        CCString *key=(CCString *)array->objectAtIndex(i);
        CFightCard *card=m_tempGamePlayer->findFightCardByCard_User_ID(key->intValue());
        m_vectorGetCardList.push_back(card);
        char data[500];
        if(card)
        {
        for (int i=0; i<m_tempGamePlayer->m_gonggaoCard->count(); i++)
        {
            
            if (((CDrawGonggaoTable *)m_tempGamePlayer->m_gonggaoCard->objectAtIndex(i))->cardId==card->m_pCard->m_icard_id) {
                CDrawGonggaoTable *temp=(CDrawGonggaoTable *)m_tempGamePlayer->m_gonggaoCard->objectAtIndex(i);
                sprintf(data, temp->stringName.c_str(),m_tempGamePlayer->getPlayerName(),temp->cardName.c_str());
                string valueData=data;
                m_tempGamePlayer->m_pAnno->appendData(valueData, EN_ANNOUNCEMENT_CONTEXTTYPE_OWNDRAWCARD);
            }
        }
        }
        
    }
    
}


void CDrawCardLayer::sendGetCardRandom(EN_NIUDANTYPE entype, int number,bool isFree)
{
    if(m_tempGamePlayer->cardBagIsMoreThanConfig())
    {
        CCLayer *layer= CCardBagFullDialog::create();//CGameArrangeCardBag::create();
        addChild(layer,1000);
        m_httpStatus=EN_NIUDANHTTPSTATUS_END;
        return;
    }
    if (!isFree) {
        //判断对应的东西是否够
        if (entype==EN_NIUDANTYPE_CASH) {
            if (m_tempGamePlayer->getPlayerCash() < number * g_niudanjiage) {
                CCLayer * layer = CGameRechargeTip::create();
                addChild(layer,1000);
                m_httpStatus=EN_NIUDANHTTPSTATUS_END;
                return;
            }
        }
    }
    
    m_httpStatus=EN_NIUDANHTTPSTATUS_RUNNINT;
 #ifndef DDEBUG__
    removeJiantouAndClearVector();
    m_enSaveSendValue=entype;
    if (getChildByTag(77777))
    {
        removeChildByTag(77777, true);
    }
    createLoadingLayer();
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    postStrdata+="&type="+ConvertToString((int)entype)+"&num="+ConvertToString(number)+"&is_free="+ConvertToString((int)isFree);
;
       ADDHTTPREQUESTPOSTDATA(STR_URL_CARDRANDOM(194), "CALLBACK_CDrawCardLayer_sendGetCardRandom", "REQUEST_CSceneGameShopLayer_sendGetCardRandom",postStrdata.c_str(),callfuncO_selector(CDrawCardLayer::decodeGetCardRandom));
#else
    const  char *data=CPtTool::readFileName((resRootPath +"cardrandomtxt.txt").c_str()).c_str();
    char *tt=new char[strlen(data+1)];
    memcpy(tt, data, strlen(data));
    decodeGetCardRandom((CCObject *)tt);
#endif
}


void CDrawCardLayer::decodeGetCardRandom(CCObject *object)
{
    removeLoadingLayer();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CDrawCardLayer_sendGetCardRandom");
    if (!object) {
        m_httpStatus=EN_NIUDANHTTPSTATUS_END;
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
                if (reward&&reward->getCards()) {
                   showCardData(reward->getCards());
                }
              
                if (tmp->objectForKey("dec") &&((CCDictionary*)tmp->objectForKey("dec"))->objectForKey("card_item") ) {
                    reward = CReward::create((CCDictionary *)((CCDictionary*)tmp->objectForKey("dec"))->objectForKey("card_item"));
                    reward->excuteReward(DEC);
                }
                this->runAction(CCSequence::create(CCShake::create(2.0,3.0),CCCallFunc::create(this,callfunc_selector(CDrawCardLayer::createTexiao)),NULL));
                //显示卡牌数据
                //createGetCardLayer(m_enSaveSendValue);
                

            }
            m_httpStatus=EN_NIUDANHTTPSTATUS_END;
            return ;
        }
    }
    m_httpStatus=EN_NIUDANHTTPSTATUS_END;
    CCMessageBox("服务端传递数据有错误", "error");
    return ;

    
    
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
    CCNode *node=getChildByTag(103);
    if(node)
    {
        node->runAction(CCSpawn::create(CCMoveBy::create(0.2, ccp(-200,0)),CCFadeOut::create(0.2),NULL));
    }
    node=getChildByTag(104);
    if(node)
    {
        node->runAction(CCSpawn::create(CCMoveBy::create(0.2, ccp(+200,0)),CCFadeOut::create(0.2),NULL));
    }
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
        moveOutEyeSize();
        clearSelectFreeItemLayer();
        hideLable();
        createNiudanButton(typeTouch);
    }
    return true;
}



void CDrawCardLayer::initSendGetData(float t)
{
#ifndef DDEBUG__
//开始读取服务器的 友情点数时间和 钻石 各免费时间
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETCARDRANDOM(194), "CALLBACK_CDrawCardLayer_initSendGetData", "REQUEST_CSceneGameShopLayer_getLimitItems",postStrdata.c_str(),callfuncO_selector(CDrawCardLayer::decodeInitEnterDrawCardLayer));
#else
    const  char *data=CPtTool::readFileName((resRootPath +"getcardrandom.txt").c_str()).c_str();
    char *tt=new char[strlen(data+1)];
    memcpy(tt, data, strlen(data));
    decodeInitEnterDrawCardLayer((CCObject *)tt);
#endif
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
        CCLog("sendHttp Value %d",pData->m_nCount);
        //开始读取服务器的 友情点数时间和 钻石 各免费时间
        string postStrdata="sig=";
        postStrdata+=SinglePlayer::instance()->getUserSig();
        ADDHTTPREQUESTPOSTDATA(STR_URL_GETCARDRANDOM(194), callBackStr.c_str(), "REQUEST_CDrawCardLayer_NiuDan",postStrdata.c_str(),callBackFunc);
    return true;
}

void CDrawCardLayer::rebackFirstLayer()
{
    removeJiantouAndClearVector();
    if (getChildByTag(52)) {
        removeChildByTag(52, true);
    }
    if(getChildByTag(77777))
    {
        removeChildByTag(77777, true);
    }
    
    for (int i=105; i<=108; i++) {
        if (getChildByTag(i)) {
            removeChildByTag( i, true);
        }
    }
    for (int i=21; i<=24; i++) {
        if (getChildByTag(i)) {
            removeChildByTag( i, true);
        }
    }
    if (getChildByTag(200)) {
        removeChildByTag(200, true);
    }
    touchVector.clear();
    createBackYingLing();
    createTopButton();
    m_enLayerStatus=EN_NIUDANLAYER_CHOOSETYPE;
    createLabelFriendAndUpdate();
    createLabelCashAndUpdate();
}

void CDrawCardLayer::createTexiao()
{
       createGetCardLayer(m_enSaveSendValue);
      m_httpStatus=EN_NIUDANHTTPSTATUS_END;;
    
}
void CDrawCardLayer::createNiudanButton(EN_NIUDANTYPE entype)
{
    if (!getChildByTag(200)) {
       string word = Utility::getWordWithFile("word.plist", "fanhui");
        CGameButtonControl *fanhuiButton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
        addChild(fanhuiButton,1,200);
        fanhuiButton->setPosition(ccp(size.width*0.5+300, size.height*0.5+228));
        touchVector.push_back(fanhuiButton);
    }
    if (entype==EN_NIUDANTYPE_CASH) {
        if (!getChildByTag(52)) {
            string everyZuanshi=Utility::getWordWithFile("word.plist", "niudanstrcash");
            char data[100];
            sprintf(data, everyZuanshi.c_str(),g_niudanjiage,g_niudanjiage*11);
            CCLabelTTF *labelttf=CCLabelTTF::create(data, "Arial", 15);
            labelttf->setPosition(ccp(size.width*0.5,size.height *0.5-200));
            addChild(labelttf,2,52);
            
        }
        if (!getChildByTag(105))
        {
            
            string everyZuanshi= Utility::getWordWithFile("word.plist", "chouyici");
            CGameButtonControl *chouquyici=CGameButtonControl::createButton(TEXTMID, everyZuanshi.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
            addChild(chouquyici,2,105);
            chouquyici->setPosition(ccp(size.width*0.5-200, size.height*0.5-230));
            touchVector.push_back(chouquyici);
        }
        if (!getChildByTag(106))
        {
            string everyshiyishi= Utility::getWordWithFile("word.plist", "choushiyici");
            CGameButtonControl *chouqushiyici=CGameButtonControl::createButton(TEXTMID, everyshiyishi.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
            addChild(chouqushiyici,2,106);
            chouqushiyici->setPosition(ccp(size.width*0.5+200, size.height*0.5-230));
            touchVector.push_back(chouqushiyici);
        }
    }
    else{
        if (!getChildByTag(107))
        {
             string everyZuanshi= Utility::getWordWithFile("word.plist", "chouyici");
             CGameButtonControl *chouquyici=CGameButtonControl::createButton(TEXTMID, everyZuanshi.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
            addChild(chouquyici,2,107);
            chouquyici->setPosition(ccp(size.width*0.5-200, size.height*0.5-230));
            touchVector.push_back(chouquyici);
        }
        if (!getChildByTag(108))
        {
            CGameButtonControl *chouqushiyici=CGameButtonControl::createButton(TEXTMID, "123", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
            addChild(chouqushiyici,2,108);
            chouqushiyici->setPosition(ccp(size.width*0.5+200, size.height*0.5-230));
            updateFriendButton();
            touchVector.push_back(chouqushiyici);
            
        }
        else{
            updateFriendButton();
        }
        if (!getChildByTag(52)) {
            string everyZuanshi=Utility::getWordWithFile("word.plist", "niudanstrfriend");
            char data[100];
            sprintf(data, everyZuanshi.c_str(),g_niudanyouqingjiage);
            CCLabelTTF *labelttf=CCLabelTTF::create(data, "Arial", 15);
            labelttf->setPosition(ccp(size.width*0.5,size.height *0.5-200));
            addChild(labelttf,2,52);
            
        }
    }
}

void CDrawCardLayer::updateShengxiaChiShu()
{
    if (!getChildByTag(107)) {
        CCLabelTTF 
    }
}

void CDrawCardLayer::updateFriendButton()
{
    if (getChildByTag(108)) {
        int value=m_tempGamePlayer->getFriendly();
        if (value>10*g_niudanyouqingjiage) {
           m_nBackValue=value=10;
        }
        else{
           m_nBackValue=value=m_tempGamePlayer->getFriendly()/g_niudanyouqingjiage;
        }
        if (value==0) {
            m_nBackValue=value=1;
            ((CGameButtonControl *)getChildByTag(107))->setEnable(false);
            ((CGameButtonControl *)getChildByTag(108))->setEnable(false);
        }
        char data[20];
        sprintf(data, "抽取%d次",value);
        ((CGameButtonControl *)getChildByTag(108))->getTextLabel()->setString(data);
    }
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
        valueStr="cash";
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
                if (type==EN_NIUDANTYPE_CASH) {
                    createLabelCashAndUpdate();
                }
                else if(type==EN_NIUDANTYPE_FRIEND)
                {
                    createLabelFriendAndUpdate();
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
        case 200:
            rebackFirstLayer();
            break;
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
        case 105:
            sendGetCardRandom(EN_NIUDANTYPE_CASH,1,false);
            break;
        case 106:
            sendGetCardRandom(EN_NIUDANTYPE_CASH,11,false);
            break;
        case 107:
            if(g_niudanyouqingjiage*1 <= m_tempGamePlayer->getFriendly())
            {
                sendGetCardRandom(EN_NIUDANTYPE_FRIEND,1,false);
            }
            break;
        case 108:
            if(m_nBackValue!=0&&g_niudanyouqingjiage*m_nBackValue <= m_tempGamePlayer->getFriendly())
            {
                sendGetCardRandom(EN_NIUDANTYPE_FRIEND,m_nBackValue, false);
            }
            break;
        default:
            break;
    }
}

void CDrawCardLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
CCSize CDrawCardLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(300, 200);
}

void CDrawCardLayer::createTableView()
{
    if(m_vectorGetCardList.size()>0)
    {
        float width=2.0;
        if (m_vectorGetCardList.size()==1) {
            width=1.0;
        }
        CCTableView* tableView = CCTableView::create(this,CCSizeMake(width*300, 200));
        tableView->setDirection(kCCScrollViewDirectionHorizontal);
        tableView->setAnchorPoint(ccp(0.5, 0.5));
        if (m_vectorGetCardList.size()==1) {
            tableView->setPosition(ccp(size.width*0.5-150,size.height*0.5-100));
            m_fPostionX=150.0f;
        }
        else{
            m_fPostionX=100.0f;
            tableView->setPosition(ccp(220,300));
            createJiantou();
        }
        tableView->setDelegate(this);
        tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        tableView->setTouchPriority(-125);
        addChild(tableView,1,77777);
    }
}


CCTableViewCell* CDrawCardLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{table->dequeueCell();
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        initCellItem(cell, idx);
    }
    //cell->setPosition(ccp(300,0));
    return cell;

}

void CDrawCardLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    if(cell)
    {
        CFightCard *fightcard=m_vectorGetCardList[idx];
        if(fightcard)
        {
            CGamesCard *gameCard=CGamesCard::Create(m_vectorGetCardList[idx]);
            cell->addChild(gameCard, 2, 1000+idx);
//            gameCard->setAnchorPoint(CCPointZero);
              gameCard->setPosition(ccp(m_fPostionX, 100));
            
        }
    }
}

unsigned int CDrawCardLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_vectorGetCardList.size();
}

void CDrawCardLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    int value =cell->getIdx();
    CCLog("you touch cell at %d", value);
}
