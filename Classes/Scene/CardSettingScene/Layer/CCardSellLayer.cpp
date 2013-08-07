//
//  CCardSellLayer.cpp
//  91.cube
//
//  Created by phileas on 13-7-31.
//
//

#include "CCardSellLayer.h"
#include "CPtTool.h"
#include "CBattleArrayLayer.h"
#include <list>
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CSaveConfirmLayer.h"

CCardSellLayer::CCardSellLayer()
{
    m_nTouchTag = -1;
    m_pPlayer = SinglePlayer::instance();
    m_cMaps = NULL;
}

CCardSellLayer::~CCardSellLayer()
{
    CCLog("CCardSellLayer::~CCardSellLayer()");
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    restCardFlag();
}

bool CCardSellLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initSelll();
        bRet = true;
    } while (0);
    return bRet;
}
bool CCardSellLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pSaveButton, pTouch))
    {
        return  true;
    }
    return false;
}
void CCardSellLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CCardSellLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pSaveButton, pTouch))
    {
        handlerTouch(pTouch);
    }
    
}
void CCardSellLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CCardSellLayer::initSelll()
{
    CCSprite * bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardsetting_bg.png"));
    bg->setScaleY(1.4f);
    
    addChild(bg, -1);
    bg->setPosition(ccp(10, 10));
    bg->setAnchorPoint(CCPointZero);
    
    m_nTouchTag = -1;
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "chushoujiemian.plist"));
    m_pSaveButton = m_cMaps->getElementByTags("2001,1,25");
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-1);
    
    
    // ajust panel position:
    m_cMaps->getElementByTags("2001,0")->setPosition(ccp(0, -20));
    
    // init left
    CCArray * array = CCArray::create();
    m_pSellPackage = CPtListViewWidget::create(array, CCSizeMake(476, 450), kCCScrollViewDirectionVertical , CCSizeMake(5, 2) , 3);
    //m_pSellPackage->setBackGround(CCLayerColor::create(ccc4(125,125, 25, 125)));
    ((TableView *)(m_pSellPackage->getTableView()))->setDelayMode(true);
    m_pSellPackage->setAnchorPoint(CCPointZero);
    m_pSellPackage->setPosition(ccp(40, 140));
    m_pSellPackage->setItemSize(CCSizeMake(139, 202));
    addChild(m_pSellPackage);
    ((TableView*)(m_pSellPackage->getTableView()))->setDelayMode(false);
    updateTexture();
}

void CCardSellLayer::handlerTouch(cocos2d::CCTouch *pTouch)
{
    saveOnClick();
}

void CCardSellLayer::saveOnClick()
{
    // send message to server:
    char buff[500]={0};
    sprintf(buff, "&sig=2ac2b1e302c46976beaab20a68ef95&item_ids=[");
    char tmpBuff [200]={0};
    char cBuff[20]={0};;
    CPtBattleArrayItem * tmp = NULL;
    CCArray * array = m_pSellPackage->getItems();
    int index = 0;
    for (int i = 0; i < array->count(); i++)
    {
        tmp = (CPtBattleArrayItem*)(array->objectAtIndex(i));
        if (tmp && tmp->getDisplayView())
        {
           index =  ((CPtDisPlayCard*)(tmp->getDisplayView()))->getCardData()->m_User_Card_ID;
           sprintf(cBuff, "\"%d\",",index);
           strcat(tmpBuff, cBuff);
        }
    }
    strncat(buff, tmpBuff, strlen(tmpBuff)-1);
    strcat(buff, "]");
    CCLog("%s",buff);
    
    save();
  //  ADDHTTPREQUESTPOSTDATA("http://cube.games.com/api.php?m=CardItem&a=sellCardItems&uid=194", "cardsell","sell", buff, callfuncO_selector(CCardSellLayer::receiveCallBack));
    
}

void CCardSellLayer::receiveCallBack(cocos2d::CCObject *pSender)
{
    // if result is zero save data
    char *buffer = (char *) pSender;
    CCLog("callback: %s", buffer);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "cardsell");
    
    CCDictionary* dic = PtJsonUtility::JsonStringParse(buffer);
    CSaveConfirmLayer * layer = new CSaveConfirmLayer();
    int result = GameTools::intForKey("code", dic);
    
    
    if (result == 0)
    {
        save();
        layer->setUserData((void *) 40001);
    }else
    {
        layer->setUserData((void *) 40002);
        CCLog("error");
    }
    
    delete [] buffer;
    
    layer->init();
    layer->autorelease();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000);
    

}
void CCardSellLayer::save()
{
    // save data:
    removeCardInCardBag();
}

void CCardSellLayer::resetData()
{
    // reset data:
}

void CCardSellLayer::updateSell()
{
    
}

void CCardSellLayer::updateData()
{
    
}

void CCardSellLayer::updateTexture()
{
    
}

void CCardSellLayer::restCardFlag()
{
    if (m_pSellPackage)
    {
        CPtBattleArrayItem * tmp = NULL;
        CCArray * array = m_pSellPackage->getItems();
        
        for (int i = 0; i < array->count(); i++)
        {
          tmp = (CPtBattleArrayItem*)(array->objectAtIndex(i));
            if (tmp && tmp->getDisplayView())
            {
                ((CPtDisPlayCard*)(tmp->getDisplayView()))->getCardData()->setEnConsume(false);
                ((CPtDisPlayCard*)(tmp->getDisplayView()))->getInCardBagPointer()->setLive();
            }
        }
    }
}

void CCardSellLayer::removeCardInCardBag()
{
    vector<CFightCard *> & r_CardBag = m_pPlayer->m_vCardBag;
    CCArray * arrayBag = m_pCardBag->getItems();
    std::list<int> ids;
    
    CPtBattleArrayItem * tmp = NULL;
    CCArray * array = m_pSellPackage->getItems();
    if (m_pSellPackage)
    {
        
       
        int index = -1;
        for (int i = 0; i < array->count(); i++)
        {
            tmp = (CPtBattleArrayItem*)(array->objectAtIndex(i));
            if (tmp && tmp->getDisplayView())
            {
               index = ((CPtDisPlayCard*)(tmp->getDisplayView()))->getIndex();
                ids.push_back(index);
            }
        }

        ids.sort();

        for (std::list<int>::reverse_iterator i = ids.rbegin(); i != ids.rend(); i++)
        {
            index = *i;
            CCLog("index:%d",index);
            arrayBag->removeObjectAtIndex(index);
            std::vector<CFightCard*>::iterator j = r_CardBag.begin()+index;
            r_CardBag.erase(j);
        }

        m_pCardBag->reload();
        clearPanel();
   }
}

void CCardSellLayer::clearPanel()
{
    if (m_pSellPackage)
    {
        CCArray * array = m_pSellPackage->getItems();
        array->removeAllObjects();
        m_pSellPackage->reload();
    }

}
