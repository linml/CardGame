//
//  CPVPMonsterPlayerLayer.cpp
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#include "CPVPMonsterPlayerLayer.h"
#include "gameConfig.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "CPVPMonsterData.h"
#include "CGameButtonControl.h"
#include "Utility.h"
#include "gameTools.h"
#include "CGamesCard.h"
#include "PtJsonUtility.h"





#define TAG_TASKPVPMONSTER_TOUCH_PRORITY  -3

#define TAG_TASKPVPMONSTER_FANHUI 2
#define TAG_TASKPVPMONSTER_SEARCH 3
#define TAG_TASKPVPMONSTER_ZHANDOU 4
#define TAG_TASKPVPMONSTER_USERNAME 5
#define TAG_TASKPVPMONSTER_USERINFO 6

CPVPMonsterPlayerLayer::CPVPMonsterPlayerLayer()
{
    m_pCustomTable=NULL;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.plist"), CSTR_FILEPTAH(g_mapImagesPath, "friendjiemian.png"));
    size=CCDirector::sharedDirector()->getWinSize();
    m_touchArray=CCArray::create();
    m_touchArray->retain();
}

CPVPMonsterPlayerLayer::~CPVPMonsterPlayerLayer()
{
    CC_SAFE_RELEASE(m_touchArray);//(m_touchArray);
    CC_SAFE_RELEASE(this->m_pMonsterPvp);
    
}
CPVPMonsterPlayerLayer *CPVPMonsterPlayerLayer::CreateByUserID(CPVPMonsterData *pvp,bool isSearCh)
{
    CPVPMonsterPlayerLayer *pNewMonster =new CPVPMonsterPlayerLayer ();
    if (!pNewMonster|| !pNewMonster->initCreateByUserId(pvp, isSearCh))
    {
        delete pNewMonster;
        pNewMonster=NULL;
        return NULL;
    }
    pNewMonster->autorelease();
    return pNewMonster;
}
bool CPVPMonsterPlayerLayer::initCreateByUserId(CPVPMonsterData *pvp,bool isSearCh)
{
    this->isSearch=isSearCh;
    this->m_pMonsterPvp=pvp;
    createBackGround();
    createBackButton();
    createUserInfo();
    createUserName();
    createZhanButton();
    createSearchButton();
    createTableView();
    //setTouchPriority(-100);
    setTouchEnabled(true);

    return true;
}

void CPVPMonsterPlayerLayer::createTableView()
{
    
    if (m_pCustomTable) {
        m_pCustomTable->removeFromParentAndCleanup(true);
    }
    m_pCustomTable = CCTableView::create(this, CCSizeMake(650, 200),NULL);
    m_pCustomTable->setDirection(kCCScrollViewDirectionHorizontal);
    m_pCustomTable->setPosition(ccp(200,160));
    m_pCustomTable->setDelegate(this);
    m_pCustomTable->setTouchPriority(TAG_TASKPVPMONSTER_TOUCH_PRORITY-2);
    m_pCustomTable->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(m_pCustomTable,1,999);
}
void CPVPMonsterPlayerLayer::createBackGround()
{
    CCSprite *bgSprite=CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath,"niudanbeijing.png"));
    addChild(bgSprite,0);
    bgSprite->setPosition(ccp(size.width*0.5,size.height*0.5));
}

void CPVPMonsterPlayerLayer::createBackButton()
{
    string word = Utility::getWordWithFile("word.plist", "fanhui");
    CGameButtonControl *fanhuiButton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(fanhuiButton,1,TAG_TASKPVPMONSTER_FANHUI);
    if (isSearch) {
        fanhuiButton->setPosition(ccp(size.width*0.5-200, size.height*0.5-230));
    }
    else
    {
        fanhuiButton->setPosition(ccp(size.width*0.5, size.height*0.5-230));
    }
    m_touchArray->addObject(fanhuiButton);
    
}

void CPVPMonsterPlayerLayer:: createZhanButton()
{
    CGameButtonControl *fanhuiButton=CGameButtonControl::createButton(TEXTMID, "zhan", "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(fanhuiButton,1,TAG_TASKPVPMONSTER_ZHANDOU);
    fanhuiButton->setPosition(ccp(size.width*0.5, size.height*0.5-180));
    m_touchArray->addObject(fanhuiButton);
}

void CPVPMonsterPlayerLayer::createSearchButton()
{
    if (isSearch) {
       string word = Utility::getWordWithFile("word.plist", "jixusousuo");
       CGameButtonControl *fanhuiButton=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
       addChild(fanhuiButton,1,TAG_TASKPVPMONSTER_SEARCH);

        fanhuiButton->setPosition(ccp(size.width*0.5+200, size.height*0.5-230));
       m_touchArray->addObject(fanhuiButton);
   }
}

void CPVPMonsterPlayerLayer::createCishuLayer()
{
    
}

void CPVPMonsterPlayerLayer::createUserInfo()
{
    vector<string>datalist;
    datalist.push_back(string("等级:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->level));
    datalist.push_back(string("积分:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->credits));
    datalist.push_back(string("排名:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->rank));
    datalist.push_back(string("战力:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->fightpointer));
    for (int i=0; i<datalist.size(); i++) {
        CCLabelTTF *pLabel=CCLabelTTF::create(datalist[i].c_str(), "Arial", 20);
        pLabel->setPosition(ccp(size.width *0.5-200 +i*100,size.height*0.5-160));
        pLabel->setAnchorPoint(CCPointZero);
        addChild(pLabel,2,TAG_TASKPVPMONSTER_USERINFO+i);
    }
}


void CPVPMonsterPlayerLayer::createUserName()
{
    string value=m_pMonsterPvp->getPVPUserData()->name;
    CCLabelTTF *pLabel=CCLabelTTF::create(m_pMonsterPvp->getPVPUserData()->name.c_str(), "Arial", 20);
    pLabel->setPosition(ccp(size.width*0.5,size.height*0.5+200));
    addChild(pLabel,2,TAG_TASKPVPMONSTER_USERNAME);
}

void CPVPMonsterPlayerLayer::getUserIdTeam(int UserId)
{
   

    
}

void CPVPMonsterPlayerLayer::getRandTeam()
{
    //发送请求搜索对手
    string str;
    str+="&sig=";
    str+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETPVPSEARCHDUISHOU(194), "CALLBACK_CPVPMonsterPlayerLayer_onClickSearchOpponent", "REQUEST_CPVPMonsterPlayerLayer_onClickSearchOpponent",str.c_str(),callfuncO_selector(CPVPMonsterPlayerLayer::callBackGetRandTeam))
}

void CPVPMonsterPlayerLayer::callBackGetRandTeam(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_PVPSceneLayer_onClickSearchOpponent");
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
    tempdata=NULL;
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    CCDictionary *resultDict=(CCDictionary *)dict->objectForKey("result");
    if (resultDict)
    {
        PVPRankData *tempData=m_pMonsterPvp->getPVPUserData();
        CCDictionary *peoInfo=(CCDictionary *)resultDict->objectForKey("pk_user");
        tempData->name=GameTools::valueForKey("username", peoInfo);
        tempData->rank=GameTools::intForKey("pvp_num", peoInfo);
        tempData->uid=GameTools::intForKey("uid", peoInfo);
        tempData->level=GameTools::intForKey("level", peoInfo);
        tempData->fightpointer=GameTools::intForKey("power", peoInfo);
        tempData->credits =GameTools::intForKey("score", peoInfo);
        //pMonsterData->setPVPUserData(pp);
        if (CPtTool::isDictionary(resultDict->objectForKey("team_info"))) {
            CCDictionary *temp_info=(CCDictionary *)resultDict->objectForKey("team_info");
            CCDictionary *team=(CCDictionary *)temp_info->objectForKey("team");
            //
            m_pMonsterPvp->clearFightingCardData();
            m_pMonsterPvp->m_vCardList.resize(5);
            CCDictElement *element = NULL;
            CCDictionary *onlyCard=NULL;
            CCDICT_FOREACH(team, element)
            {
                CCObject * object = element->getObject();
                
                if (object && (onlyCard = (CCDictionary *)(object)))
                {
                    int postion=GameTools::intForKey("position", onlyCard);
                    int cardid=GameTools::intForKey("card_id", onlyCard);
                    int level=GameTools::intForKey("level", onlyCard);
                    CCard *card=SinglePlayer::instance()->getCardByCardId(cardid);
                    if(card)
                    {
                        CFightCard *pFightCard=new CFightCard(card,level);
                        m_pMonsterPvp->m_vCardList[postion]=pFightCard;
                    }
                }
            }
            
        }
        else{
            m_pMonsterPvp->clearFightingCardData();
            m_pMonsterPvp->m_vCardList.resize(5);
        }
        updateLayerData();
    }
}

void CPVPMonsterPlayerLayer::updateLayerData()
{
    ((CCLabelTTF *)getChildByTag(TAG_TASKPVPMONSTER_USERNAME))->setString(m_pMonsterPvp->getPVPUserData()->name.c_str());
    
    vector<string>datalist;
    datalist.push_back(string("等级:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->level));
    datalist.push_back(string("积分:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->credits));
    datalist.push_back(string("排名:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->rank));
    datalist.push_back(string("战力:")+ConvertToString(m_pMonsterPvp->getPVPUserData()->fightpointer));
    for (int i=0; i<4; i++)
    {
        ((CCLabelTTF *)getChildByTag(TAG_TASKPVPMONSTER_USERINFO+i))->setString(datalist[i].c_str());
    }
    m_pCustomTable->reloadData();
}

void CPVPMonsterPlayerLayer::callBackGetUserId(CCObject *object)
{
    
}

void CPVPMonsterPlayerLayer::schudelUpdateTime(float t)
{
    
}

void CPVPMonsterPlayerLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void CPVPMonsterPlayerLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
    
}

bool CPVPMonsterPlayerLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CPVPMonsterPlayerLayer::handleDealWithTag(int tag)
{
    switch (tag) {
        case TAG_TASKPVPMONSTER_FANHUI:
            removeFromParentAndCleanup(true);
            break;
        case TAG_TASKPVPMONSTER_SEARCH:
            getRandTeam();
            break;
        case TAG_TASKPVPMONSTER_ZHANDOU:
            startFighting();
            break;
        default:
            break;
    }
}

void CPVPMonsterPlayerLayer::startFighting()
{
    CCLog("STARTFIGHTING");
}


void CPVPMonsterPlayerLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CPVPMonsterPlayerLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    for (int i=0; i<m_touchArray->count(); i++) {
        CCNode *node=(CCNode *)m_touchArray->objectAtIndex(i);
        if (node->boundingBox().containsPoint(pTouch->getLocation())) {
            handleDealWithTag(node->getTag());
        }
    }
    
}

void CPVPMonsterPlayerLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

CCSize CPVPMonsterPlayerLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(100, 200);
}


CCTableViewCell* CPVPMonsterPlayerLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->cellAtIndex(idx);
    if (!cell) {
        cell = new CCTableViewCell();
        cell->autorelease();
        initCellItem(cell, idx);
    }
    return cell;
    
}

unsigned int CPVPMonsterPlayerLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_pMonsterPvp->m_vCardList.size();
}

void CPVPMonsterPlayerLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

void CPVPMonsterPlayerLayer::scrollViewDidScroll(CCScrollView* view)
{
    
}

void CPVPMonsterPlayerLayer::scrollViewDidZoom(CCScrollView* view)
{
    
}
void CPVPMonsterPlayerLayer::initCellItem(CCTableViewCell*cell, unsigned int idx)
{
    if(cell)
    {
        CFightCard *fightcard=m_pMonsterPvp->m_vCardList[idx];
        if(fightcard)
        {
            CGamesCard *gameCard=CGamesCard::Create(m_pMonsterPvp->m_vCardList[idx]);
            cell->addChild(gameCard, 2, 1000+idx);
            gameCard->setPosition(ccp(100, 100));
            
        }
    }
}
