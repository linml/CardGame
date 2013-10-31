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
#include "PtHttpURL.h"
#include "CBattleArrayLayer.h"

CCardSellLayer::CCardSellLayer()
{
    m_nTouchTag = -1;
    m_pPlayer = SinglePlayer::instance();
    m_cMaps = NULL;
    m_nCoin = 0;
    m_pConinlabel = NULL;
 
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
        CCPoint point = m_pSaveButton->getAnchorPoint();
        CCTexture2D * pressed = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_pressed.png"));
        m_pSaveButton->initWithTexture(pressed);
        m_pSaveButton->setAnchorPoint(point);
        return  true;
    }
    return false;
}
void CCardSellLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CCardSellLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint point = m_pSaveButton->getAnchorPoint();
    CCTexture2D * noraml = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_normal.png"));
    m_pSaveButton->initWithTexture(noraml);
    m_pSaveButton->setAnchorPoint(point);
    if (CPtTool::isInNode(m_pSaveButton, pTouch))
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
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
    m_pSaveButton = (CCSprite*) m_cMaps->getElementByTags("2001,1,25");
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-1);
    
    m_pConinlabel = (CCLabelTTF*) m_cMaps->getElementByTags("2001,1,19,4003");
    CCNode *parent = m_pConinlabel->getParent();
    parent->getParent()->reorderChild(parent, 3000);
    // ajust panel position:
    m_cMaps->getElementByTags("2001,0")->setPosition(ccp(0, -20));
    
    // init left
    CCArray * array = CCArray::create();
    m_pSellPackage = CPtListViewWidget::create(array, CCSizeMake(476, 450), kCCScrollViewDirectionVertical , CCSizeMake(5, 2) , 3);
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
    CSaveConfirmLayer * layer = CSaveConfirmLayer::create();
    CCArray * array = m_pSellPackage->getItems();
    CPtBattleArrayItem * tmp = NULL;
    if (array&& array->count()== 0)
    {
        layer->setResultCode(9);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000, 2000);
        return;
    }
    // send message to server:
    char buff [200]={0};
    char p[20]={0};
    int index = 0;
    
//xianbei modify    sprintf(buff, "&sig=2ac2b1e302c46976beaab20a68ef95&item_ids=[");
    sprintf(buff, "&sig=%s&item_ids=[",STR_USER_SIG);
    std::string  buffer(buff);
    for (int i = 0; i < array->count(); i++)
    {
        tmp = (CPtBattleArrayItem*)(array->objectAtIndex(i));
        if (tmp && tmp->getDisplayView())
        {
            index =  ((CPtDisPlayCard*)(tmp->getDisplayView()))->getCardData()->m_User_Card_ID;
            if (i == 0)
            {
                sprintf(p, "%d",index);
            }else
            {
                sprintf(p, ",%d",index);
            }
            
            buffer.append(p);
        }
    }
    buffer.append("]");
    CCLog("the buffer: %s", buffer.c_str());
    ADDHTTPREQUESTPOSTDATA(STR_URL_SELL_CARD(194),"cardsell","sell",buffer.c_str(), callfuncO_selector(CCardSellLayer::receiveCallBack));
    
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000, 2000);
}

void CCardSellLayer::receiveCallBack(cocos2d::CCObject *pSender)
{
    static int i = 0;
    // if result is zero save data
    char *buffer = (char *) pSender;
    CCLog("callback: %s", buffer);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "cardsell");
    CSaveConfirmLayer * layer = (CSaveConfirmLayer*) CCDirector::sharedDirector()->getRunningScene()->getChildByTag(2000);
    if(!buffer)
    {
        if(i==3)
        {
            i = 0;
            layer->setResultCode(200);
            return;
        }else
        {
           
            i++;
            
            CCLog("send again sell request");
            char buff[1000]={0};
            char param[200]={0};
            char p[20]={0};
            int index = 0;
            //xianbei modify        sprintf(buff, "&sig=2ac2b1e302c46976beaab20a68ef95&item_ids=[");
            sprintf(buff, "&sig=%s&item_ids=[",STR_USER_SIG);
            
            
            CCArray * array = m_pSellPackage->getItems();
            CPtBattleArrayItem * tmp = NULL;
            
            for (int i = 0; i < array->count(); i++)
            {
                tmp = (CPtBattleArrayItem*)(array->objectAtIndex(i));
                if (tmp && tmp->getDisplayView())
                {
                    index =  ((CPtDisPlayCard*)(tmp->getDisplayView()))->getCardData()->m_User_Card_ID;
                    sprintf(p, "%d,",index);
                    strcat(param, p);
                }
            }
            
            CCLog("%s", param);
            strncat(buff, param, strlen(param)-1);
            strcat(buff, "]");
            
            ADDHTTPREQUESTPOSTDATA(STR_URL_SELL_CARD(194),"cardsell","sell",buff, callfuncO_selector(CCardSellLayer::receiveCallBack));
            delete [] buffer;
            return;

        }
    }
    i = 0;
    CCDictionary* dic = PtJsonUtility::JsonStringParse(buffer);
    
    int result = GameTools::intForKey("code", dic);
    
    layer->setResultCode(result);
    if (result == 0)
    {
       
        save();
    }else
    {
        
      
        CCLog("error");
    }
    
    delete [] buffer;
   
    

}
void CCardSellLayer::save()
{
    // save data:
    saveData();
    removeCardInCardBag();
    clearCoin();
}
void CCardSellLayer::saveData()
{
    CCLog("sell before : %d", m_pPlayer->getCoin());
    m_pPlayer->addCoin(m_nCoin);
    CCLog("sell after : %d", m_pPlayer->getCoin());
    
}

void CCardSellLayer::updateTexture()
{
    char buff[50] = {0};
    sprintf(buff, "%d", m_nCoin);
    m_pConinlabel->setString(buff);
}

void CCardSellLayer::addCoin(int conin)
{
    m_nCoin += conin;
    m_nCoin = m_nCoin >=0 ? m_nCoin:0;
    updateTexture();
}
void CCardSellLayer::subCoin(int coin)
{
    m_nCoin -= coin;
    m_nCoin = m_nCoin >=0 ? m_nCoin:0;
    updateTexture();
}
void CCardSellLayer::clearCoin()
{
    m_nCoin = 0;
    updateTexture();
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
    vector<CFightCard *> & r_CardBag = m_pPlayer->getCardBagVector();
    CCArray * arrayBag = m_pCardBag->getItems();
    std::list<int> ids;
    
    CPtBattleArrayItem * tmp = NULL;
    CCArray * array = m_pSellPackage->getItems();
    if (m_pSellPackage)
    {
        CBattleArrayLayer *parent = (CBattleArrayLayer*)getParent();
        CPtDisPlayCard *preManifier = parent->getPreCardManifier();
        int index = -1;
        for (int i = 0; i < array->count(); i++)
        {
            tmp = (CPtBattleArrayItem*)(array->objectAtIndex(i));
            if (tmp && tmp->getDisplayView())
            {
                CPtDisPlayCard * tmpCard = ((CPtDisPlayCard*)(tmp->getDisplayView()));
               index = tmpCard->getIndex();
               ids.push_back(index);
                if (preManifier && preManifier->getCardData() == tmpCard->getCardData())
                {
                    parent->setPreCardManifier(NULL);
                }
            }
        }

        ids.sort();

        for (std::list<int>::reverse_iterator i = ids.rbegin(); i != ids.rend(); i++)
        {
            index = *i;
            CCLog("index:%d",index);
            arrayBag->removeObjectAtIndex(index);
            std::vector<CFightCard*>::iterator j = r_CardBag.begin()+index;
            delete *j;
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
