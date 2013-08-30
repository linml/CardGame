//
//  CGameEmailLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#include "CGameEmailLayer.h"
#include "LayoutLayer.h"
#include "gameConfig.h"
#include "CGameButtonControl.h"
#include "CGameEmailTableView.h"
#include "CGameEmailManager.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "gameStruct.h"
#include "PtJsonUtility.h"
#define GOLDPLACE_TOUCH_PRORITY -2
#include "CGameDialogLayer.h"

CGameEmailLayer::CGameEmailLayer()
{
    m_enhttpStatus=EN_EMAILHTTPREQUEST_NONE;
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
    m_tempTouchNode=NULL;
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CGameEmailLayer::showDialogBagFull), "BEIBAOMANLEXIANSHIDUIHUAKUAN", NULL);
}

CGameEmailLayer::~CGameEmailLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "BEIBAOMANLEXIANSHIDUIHUAKUAN");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(CSTR_FILEPTAH(g_mapImagesPath,"youxianniu.plist"));
}

CGameEmailLayer *CGameEmailLayer::CreateEmailLayer()
{
    CGameEmailLayer *gameEmailLayer=new CGameEmailLayer();
    if (!gameEmailLayer || !gameEmailLayer->initCreate()) {
        delete gameEmailLayer;
        gameEmailLayer=NULL;
    }
    gameEmailLayer->autorelease();
    return gameEmailLayer;
}

bool CGameEmailLayer::initCreate()
{
    loadPlistFile();
    createRecvAllButton();
    creaetEmailTableView();
    sendPostHttpChangeEmailStatus();
    setTouchEnabled(true);
    return true;
}

void CGameEmailLayer::sendPostHttpChangeEmailStatus()
{
    int value=G_GAMESINGEMAIL::instance()->getCurrentEmailUnreadCount();
    if(value>0)
    {
        m_enhttpStatus=EN_EMAILHTTPREQUEST_CHANEGSTATUS;
        ADDHTTPREQUESTPOSTDATA(STR_URL_EMAILREADSTATUS(194), "MERLINEMAILSTATUS", "EMAILSTATUS","sig=2ac2b1e302c46976beaab20a68ef95",callfuncO_selector(CGameEmailLayer::recvBockHttpCallBack));
    }
}
bool CGameEmailLayer::decodeRecvBackStr(char *strdata)
{
    CCLog("%s",strdata);
    CCDictionary *dict=PtJsonUtility::JsonStringParse(strdata);
    delete []strdata;
    int codeReslut=GameTools::intForKey("code", dict);
    if(codeReslut!=0)
    {
        return false;
    }
    else
    {
        CCDictionary *dictresult=(CCDictionary *)dict->objectForKey("result");
        if (m_enhttpStatus==EN_EMAILHTTPREQUEST_GETSINGLEITEM  ||
            m_enhttpStatus==EN_EMAILHTTPREQUEST_GETALLEMAIL )
        {
            CCArray *array=(CCArray*)dictresult->objectForKey("mail_ids");
            vector<int >livetable;
            for (int i=0; i<array->count();i++) {
                CCString * cocosstr=(CCString *)array->objectAtIndex(i);
                livetable.push_back(atoi(cocosstr->m_sString.c_str()));
            }
            if(livetable.size()!=G_GAMESINGEMAIL::instance()->getCurrentTotalEmail())
            {
                G_GAMESINGEMAIL::instance()->deleteEmailData(livetable);
                ((CGameEmailTableView*)getChildByTag(8))->reloadData();
            }
            CCNotificationCenter::sharedNotificationCenter()->postNotification("youjiangengxin");
            return true;
        }
        else
        {
            CCString *str=(CCString *)dictresult->objectForKey("info");
            CCLog("%s",str->m_sString.c_str());
            if(str->compare("1")==0)
            {
                G_GAMESINGEMAIL::instance()->changeEmailStatus();
                 CCNotificationCenter::sharedNotificationCenter()->postNotification("youjiangengxin");
                 return true;
            }
            else
            {
                return  false;
            }
        }
        return true;
    }

}
void CGameEmailLayer::recvBockHttpCallBack(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "MERLINEMAILSTATUS");
    if(!object)
    {
        m_enhttpStatus=EN_EMAILHTTPREQUEST_NONE;
        return;
    }
    decodeRecvBackStr((char *)object);
    G_GAMESINGEMAIL::instance()->writeToFile();
    m_enhttpStatus=EN_EMAILHTTPREQUEST_NONE;
    
}

void CGameEmailLayer::getAllEmailItem()
{
   //  ((CGameEmailTableView*)getChildByTag(8))->reloadData();
   // return;
    CCLog("getAllEmailItem");
    if(m_enhttpStatus!=EN_EMAILHTTPREQUEST_NONE)
    {
        return;
    }
    vector<EMAIL_DATA >vemaildatalist;
    G_GAMESINGEMAIL::instance()->copyDataTovectory(vemaildatalist);
    canereadList.clear();
    CGamePlayer *player=SinglePlayer::instance();
    canereadList=player->getCanAddToBackPackEmals(vemaildatalist);
    if(canereadList.size()>0)
    {
        // & info="[1,2,3]"
        m_enhttpStatus=EN_EMAILHTTPREQUEST_GETALLEMAIL;
        string str="&info=[";
        char data[12];
        for (int i=0; i<canereadList.size()-1; i++) {
            sprintf(data, "%d,",canereadList[i]);
            str +=data;
        }
        sprintf(data, "%d]",canereadList[canereadList.size()-1]);
        str+=data;
        CCLog("post get data:%s",str.c_str());
        str +="&sig=2ac2b1e302c46976beaab20a68ef95";
        ADDHTTPREQUESTPOSTDATA(STR_URL_EMAILGETITEMS(194), "MERLINEMAILSTATUS", "EMAILSTATUS",str.c_str(),callfuncO_selector(CGameEmailLayer::recvBockHttpCallBack));
    }
    else  if(G_GAMESINGEMAIL::instance()->getMailCount()>0 && canereadList.size()==0)
    {
        showDialogBagFull(NULL);
    }
}
void CGameEmailLayer::showDialogBagFull(cocos2d::CCObject *obect)
{
    string word = Utility::getWordWithFile("word.plist", "caonima");
    CPtDialog *ptDialog=CPtDialog::create(word.c_str() , this , callfuncO_selector(CGameEmailLayer::dialogOkButtonSetFunc), callfuncO_selector(CGameEmailLayer::dialogCancelButtonSetFunc), NULL, NULL);
    addChild(ptDialog,1000,10000);
}

void CGameEmailLayer::dialogOkButtonSetFunc(CCObject *object)
{
    removeFromParentAndCleanup(true);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("CAONIMAXIANSHIBEIBAO");
    
}


void CGameEmailLayer::dialogCancelButtonSetFunc(CCObject *object)
{
    removeChildByTag(10000, true);
    
}


void CGameEmailLayer::sendPostHttpGetAllItem()
{
    
}

void CGameEmailLayer::sendPostHttpGetSingleItem()
{
    
}


void CGameEmailLayer::createRecvAllButton()
{
    string word = Utility::getWordWithFile("word.plist", "quanbulingqu");

    CGameButtonControl *sprite=CGameButtonControl::createButton(TEXTMID, word.c_str(), "jieshouanniu_Normal.png", "jieshouanniu_Pressed.png");
    addChild(sprite,900,7);
    CCNode *node=Utility::getNodeByTag(this, "100,3,0");
    CCPoint point;
    if(node)
    {
       point=node->getPosition();
       sprite->setPosition(ccp(point.x,point.y-32));
       sprite->setAnchorPoint(ccp(0,0));
    }
}

void CGameEmailLayer::creaetEmailTableView()
{
    int cellCount=G_GAMESINGEMAIL::instance()->getMailCount();
    CCLog("cellCount:%d",cellCount);
    CGameEmailTableView *tableViewLayer=CGameEmailTableView::creat(ccp(200,150), CCSizeMake(800, 400),cellCount, CCSizeMake(800, 100), CCSizeMake(600, 120));
    
    addChild(tableViewLayer,901,8);
}


bool CGameEmailLayer::loadPlistFile()
{
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(this, CSTR_FILEPTAH(plistPath, "youjianjiemian.plist"));
    return true;
}

bool CGameEmailLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "100,7,41");
    CCPoint touchPoint=pTouch->getLocation();
    if(node&&node->boundingBox().containsPoint(touchPoint))
    {
        CCLog("aaaaaaa1");
        m_tempTouchNode=node;
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        ((CGameButtonControl *)getChildByTag(7))->selected();
        m_tempTouchNode=getChildByTag(7);
    }
    return true;
}

void CGameEmailLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "100,7,41");
    CCPoint touchPoint=pTouch->getLocation();
    if(node->boundingBox().containsPoint(pTouch->getLocation()))
    {
        CCLog("aaaaaaa2");
        if(m_tempTouchNode!=node)
        {
            if(m_tempTouchNode==getChildByTag(7))
            {
                ((CGameButtonControl *)getChildByTag(7))->unselected();
            }
            m_tempTouchNode=node;
        }
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        if(m_tempTouchNode!=getChildByTag(7))
        {
            ((CGameButtonControl *)getChildByTag(7))->selected();
            m_tempTouchNode=getChildByTag(7);
        }
    } else if(m_tempTouchNode ==getChildByTag(7))
    {
         ((CGameButtonControl *)m_tempTouchNode)->unselected();
            m_tempTouchNode=NULL;
    }
    
    
}

void CGameEmailLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCNode *node=Utility::getNodeByTag(this, "100,7,41");
    CCPoint touchPoint=pTouch->getLocation();
    if(node->boundingBox().containsPoint(touchPoint))
    {
         PtSoundTool::playSysSoundEffect("UI_click.wav");
         CCLog("aaaaaaa4");
        if(m_tempTouchNode==node)
        {
            removeFromParentAndCleanup(true);
        }
    }
    else if(getChildByTag(7)->boundingBox().containsPoint(touchPoint))
    {
        CCLog("aaaaaaa3");
         PtSoundTool::playSysSoundEffect("UI_click.wav");
        if (((CGameButtonControl *)m_tempTouchNode))
        {
                    ((CGameButtonControl *)m_tempTouchNode)->unselected();
        }

        if(m_tempTouchNode==getChildByTag(7))
        {
            ((CGameButtonControl *)m_tempTouchNode)->unselected();
            m_tempTouchNode=NULL;
            getAllEmailItem();
        }
    }
    else
    {
        ((CGameButtonControl *)getChildByTag(7))->unselected();
    }
}

void CGameEmailLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CGameEmailLayer::registerWithTouchDispatcher(void)
{
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,GOLDPLACE_TOUCH_PRORITY-1, true);
}

void CGameEmailLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}