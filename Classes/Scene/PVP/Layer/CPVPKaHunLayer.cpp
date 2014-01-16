//
//  CPVPKaHunLayer.cpp
//  91.cube
//
//  Created by phileas on 14-1-8.
//
//

#include "CPVPKaHunLayer.h"
#include "gameConfig.h"
#include "PVPSceneLayer.h"
#include "gamePlayer.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CReward.h"
#include "CCardBagFullDialog.h"
// implement class of EnchantCardItem

EnchantCardItem* EnchantCardItem::create(EnchantCards *inData)
{
    EnchantCardItem *item = new EnchantCardItem();
    if(item)
    {
        if(item->initEnchantCardItem(inData))
        {
            item->autorelease();
        }
        else
        {
            delete item;
            item = NULL;
        }
    }
    return item;
    
}
EnchantCardItem::EnchantCardItem()
{
    
}
EnchantCardItem::~EnchantCardItem()
{
    
}

bool EnchantCardItem::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    bool bRet = false;
    m_nTouchTag = -1;

    //if(CPtTool::isInRect(m_pDecomposeBtn,m_cTouchRect[1] ,pTouch))
    if(CPtTool::isInNode(m_pDecomposeBtn, pTouch))
    {
        bRet = true;
        m_nTouchTag = 2;
    }else if(CPtTool::isInNode(m_pComposeBtn, pTouch)) //(CPtTool::isInRect(m_pComposeBtn,m_cTouchRect[0] ,pTouch))
    {
        m_nTouchTag = 1;
        bRet = true;
    }
    return bRet;
    
}
void EnchantCardItem::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
   
}
void EnchantCardItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(CPtTool::isInNode(m_pDecomposeBtn, pTouch))//(CPtTool::isInRect(m_pDecomposeBtn,m_cTouchRect[1] ,pTouch))
    {
        if (m_nTouchTag == 2)
        {
            onClickDecompose();
        }
    }else if(CPtTool::isInNode(m_pComposeBtn, pTouch)) //(CPtTool::isInRect(m_pComposeBtn,m_cTouchRect[0] ,pTouch))
    {
        if(m_nTouchTag == 1)
        {
            onClickCompose();
        }
    }

}
void EnchantCardItem::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


bool EnchantCardItem::initEnchantCardItem(EnchantCards *inData)
{
    float factor = 1;
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CPtTableItem::init());
        CC_BREAK_IF(inData == NULL);
        m_pData = inData;
        CCNode *displayerNode = CCNode::create();
        CCNode *node = CGameCardFactory::getInstance()->createBasicCGameDaCardByCardId(inData->echantCards->cardid);
        CCSize size = node->getContentSize();
        node->setAnchorPoint(CCPointZero);
        node->setPositionY(40);
        m_pComposeBtn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
        if(inData->haveCards==NULL || inData->haveCards->size() == 0)
        {
            
            m_pDecomposeBtn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
            
        }
        else
        {
            m_pDecomposeBtn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Pressed.png"));
        }
        const char *title = "合成";
        CCLabelTTF *label = CCLabelTTF::create(title, "Arial", 14);
        CCPoint point(m_pComposeBtn->getContentSize().width*0.5, m_pComposeBtn->getContentSize().height*0.5);
        label->setPosition(point);
        m_pComposeBtn->addChild(label);
        
        title = "分解";
        label = CCLabelTTF::create(title, "Arial", 14);
        label->setPosition(point);
        m_pDecomposeBtn->addChild(label);
       
        m_pComposeBtn->setAnchorPoint(CCPointZero);
        m_pDecomposeBtn->setAnchorPoint(CCPointZero);
        m_pComposeBtn->setPositionX(5);
        m_pDecomposeBtn->setPositionX(m_pComposeBtn->getContentSize().width + 5);
        displayerNode->addChild(m_pComposeBtn);
        displayerNode->addChild(m_pDecomposeBtn);
        displayerNode->addChild(node);
    
        setDisplayView(displayerNode);
        
//        m_pComposeBtn->setContentSize(CCSizeMake(m_pComposeBtn->getContentSize().width*factor, m_pComposeBtn->getContentSize().height*factor));
//        m_pDecomposeBtn->setContentSize(CCSizeMake( m_pDecomposeBtn->getContentSize().width*factor, m_pDecomposeBtn->getContentSize().height*factor));
//        m_cTouchRect[0] = CCRect(m_pComposeBtn->getPositionX()*factor, m_pComposeBtn->getPositionY()*factor, m_pComposeBtn->getContentSize().width*factor, m_pComposeBtn->getContentSize().height*factor);
//        
//        m_cTouchRect[1] = CCRect(m_pDecomposeBtn->getPositionX()*factor, m_pDecomposeBtn->getPositionY()*factor, m_pDecomposeBtn->getContentSize().width*factor, m_pDecomposeBtn->getContentSize().height*factor);
        
        displayerNode->setScale(factor);
        displayerNode->setContentSize(CCSizeMake(size.width*factor, (size.height+40)*factor));
        bRet = true;
    } while (0);
    return bRet;
    
}



void EnchantCardItem::onClickCompose()
{
    CGamePlayer* player = SinglePlayer::instance();
    if (player->cardBagIsMoreThanConfig())
    {
        CCardBagFullDialog* layer = CCardBagFullDialog::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
        return;
    }

    if (SinglePlayer::instance()->getPlayerKaHun() > m_pData->echantCards->mergeValue)
    {
        CCMessageBox(CCString::createWithFormat("合成该卡耗费:%d", m_pData->echantCards->mergeValue)->getCString(), "tip");
        char buffer[200]={0};
        sprintf(buffer, "sig=%s&card_id=%d",STR_USER_SIG,m_pData->echantCards->cardid);
        ADDHTTPREQUESTPOSTDATA(STR_URL_COMPOSEKAHUN(194), "CALLBACK_EnchantCardItem::onClickCompose",
                               "REQUEST_EnchantCardItem::onClickCompose",buffer, callfuncO_selector(EnchantCardItem::onReceiveComposeMsg));
    }
    else
    {
        CCMessageBox("tip", "卡魂不够");
    }
    
}
void EnchantCardItem::onClickDecompose()
{
    int cardUserId = getDecomposeCardUserId();
    if (cardUserId != -1)
    {
        char buffer[200]={0};
        sprintf(buffer, "sig=%s&card_item_id=%d",STR_USER_SIG,cardUserId);
        ADDHTTPREQUESTPOSTDATA(STR_URL_DECOMPOSEKAHUN(194), "CALLBACK_EnchantCardItem::onClickDecompose",
                               "REQUEST_EnchantCardItem::onClickDecompose",buffer, callfuncO_selector(EnchantCardItem::onReceiveDecomposeMsg));
        
    }
    else
    {
        CCMessageBox("tip", "卡库力没有该卡种");
    }
    
}

int EnchantCardItem::getDecomposeCardUserId()
{
    int cardUserId = -1;
    if(m_pData && m_pData->haveCards && m_pData->haveCards->size() > 0)
    {
        m_nSelectIndex = 0;
        for (int i = 1; i < m_pData->haveCards->size(); i++)
        {
            if (m_pData->haveCards->at(m_nSelectIndex)->m_iCurrExp > m_pData->haveCards->at(i)->m_iCurrExp)
            {
                m_nSelectIndex = i;
            }
        }
        cardUserId =  m_pData->haveCards->at(m_nSelectIndex)->m_User_Card_ID;
    }
    return cardUserId;
}

void EnchantCardItem::onReceiveComposeMsg(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_EnchantCardItem::onClickCompose");
    char *buffer = (char*) pObject;
    if (buffer)
    {
        CCDictionary *tmpDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (tmpDict)
        {
            int code = GameTools::intForKey("code", tmpDict);
            if(code == 0)
            {
                SinglePlayer::instance()->subKaHun(m_pData->echantCards->mergeValue);
                CCDictionary *rewardDict = (CCDictionary*) tmpDict->objectForKey("result");
                const char * keys[3] = {"reward", "add", "palace_soul"};
                for (int i = 0;  i < 3; i++)
                {
                    rewardDict = (CCDictionary*) rewardDict->objectForKey(keys[i]);
                    if (rewardDict== NULL)
                    {
                        break;
                    }

                }
                if (rewardDict)
                {
                    CReward *reward = CReward::create(rewardDict);
                    if (reward)
                    {
                        reward->excuteReward(ADD);
                    }
                    tmpDict = (CCDictionary*) rewardDict->objectForKey("card");
                    if (tmpDict && tmpDict->allKeys()->count() == 1)
                    {
                       int carduserId = ((CCString*)tmpDict->allKeys()->objectAtIndex(0))->intValue();
                       CFightCard* card = SinglePlayer::instance()->findFightCardByCard_User_ID(carduserId);
                       m_pData->addCard(card);
                       updateUI();
                    }
                }
            }
            else
            {
                CCMessageBox("error", CCString::createWithFormat("the error code :%d",code)->getCString());
            }
        }
    }

}
void EnchantCardItem::onReceiveDecomposeMsg(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_EnchantCardItem::onClickDecompose");
    char *buffer = (char*) pObject;
    if (buffer)
    {
        CCDictionary *tmpDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (tmpDict)
        {
            int code = GameTools::intForKey("code", tmpDict);
            if(code == 0)
            {
                SinglePlayer::instance()->addKaHun(m_pData->echantCards->splitValue);
                ;
                SinglePlayer::instance()->subCardByUserId(m_pData->haveCards->at(m_nSelectIndex)->m_User_Card_ID);
                m_pData->removeCardDataByIndex(m_nSelectIndex);
                updateUI();
            }
            else
            {
                CCMessageBox("error", CCString::createWithFormat("the error code :%d",code)->getCString());
            }
        }
    }
}
void EnchantCardItem::updateUI()
{
    CCNotificationCenter::sharedNotificationCenter()->postNotification(UPDATEKAHUNNOTICE_TAG.c_str());
    CCTexture2D * texure = NULL;
    if(m_pData->haveCards==NULL || m_pData->haveCards->size() == 0)
    {
        texure = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    }
    else
    {
        texure = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Pressed.png"));
    }
    m_pDecomposeBtn->setTexture(texure);
}

// implement class of CPVPKaHunLayer:
CPVPKaHunLayer::CPVPKaHunLayer()
{
    m_pAllData = NULL;
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CPVPKaHunLayer::updateKaHunLabel), UPDATEKAHUNNOTICE_TAG.c_str(), NULL);
}
CPVPKaHunLayer::~CPVPKaHunLayer()
{
    CCardEnchantConfig::releaseEnchant(m_pAllData);
    delete m_pAllData;
    m_pAllData = NULL;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, UPDATEKAHUNNOTICE_TAG.c_str());
}
bool CPVPKaHunLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCPVPKaHunLayer();
        bRet = true;
    } while (0);
    return bRet;
}

bool CPVPKaHunLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_nTouchTag = -1;
    if (CPtTool::isInNode(m_pBackBtn, pTouch))
    {
        m_nTouchTag = 3;
        return true;
    }
    for (int i = 0; i < 3; i++)
    {
        if (CPtTool::isInNode(m_pStarBtns[i], pTouch))
        {
            m_nTouchTag = i;
            break;
        }
    }
    
    return true;
}
void CPVPKaHunLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPVPKaHunLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pBackBtn, pTouch))
    {
        if(m_nTouchTag == 3)
        {
            onClickBack();
        }
        return;
    }
    for (int i = 0; i < 3; i++)
    {
        if (CPtTool::isInNode(m_pStarBtns[i], pTouch))
        {
            if(m_nTouchTag == i)
            {
                updateKanHunData(i);
            }
            break;
        }
    }
    

}
void CPVPKaHunLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CPVPKaHunLayer::initCPVPKaHunLayer()
{
    CCArray * dataArray = CCArray::create();
    
    CCardEnchantConfig *config = CCardEnchantConfig::getInstance();
    m_pAllData = new EnchantCardsContaner();
    m_nSelectBtnIndex = -1;
    for (int i = 0; i  < 3; i++)
    {
        m_cDataExtent[i] = config->getDataExtentByStar(3+i);

    }
    
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(PVPSCENETOUCH_PRIORITY-1);
    
    CCSize size(CCDirector::sharedDirector()->getWinSize());
    CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "niudanbeijing.png"));
    bg->setPosition(ccp(size.width*0.5, size.height*0.5));
    size = bg->getContentSize();
    addChild(bg);
    
    int kahun = SinglePlayer::instance()->getPlayerKaHun();
    char buffer[30]={0};
    CCPoint point(size.width*0.9, size.height*0.9);
    // create 卡魂数值
    snprintf(buffer, sizeof(buffer), "%d", kahun);
    CCLabelTTF* label = CCLabelTTF::create(buffer, "Arial", 13);
    label->setPosition(point);
    bg->addChild(label);
    
    m_pKaHunLabel = label;
    // create back button
    CCSprite *tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpSprite->setPosition(ccp(point.x, size.width*0.15));
    bg->addChild(tmpSprite);
    
    sprintf(buffer, "返回");
    label = CCLabelTTF::create(buffer, "Arial", 13);
    CCPoint titlePoint(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    label->setPosition(titlePoint);
    tmpSprite->addChild(label);
    m_pBackBtn = tmpSprite;
    
    // 3 star button
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpSprite->setPosition(ccp(point.x, size.width*0.25));
    bg->addChild(tmpSprite);
    
    sprintf(buffer, "3星");
    label = CCLabelTTF::create(buffer, "Arial", 13);
    titlePoint=ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    label->setPosition(titlePoint);
    tmpSprite->addChild(label);
    m_pStarBtns[0] = tmpSprite;
    // 4 star button
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpSprite->setPosition(ccp(point.x, size.width*0.35));
    bg->addChild(tmpSprite);
    
    sprintf(buffer, "4星");
    label = CCLabelTTF::create(buffer, "Arial", 13);
    titlePoint=ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    label->setPosition(titlePoint);
    tmpSprite->addChild(label);
    m_pStarBtns[1] = tmpSprite;
    
    // 5 star button
    tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    tmpSprite->setPosition(ccp(point.x, size.width*0.45));
    bg->addChild(tmpSprite);
    
    sprintf(buffer, "5星");
    label = CCLabelTTF::create(buffer, "Arial", 13);
    titlePoint=ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5);
    label->setPosition(titlePoint);
    tmpSprite->addChild(label);
    m_pStarBtns[2] = tmpSprite;
    
    
    // create listview

    
    config->getEnchantCards(m_pAllData);
    for (int i = 0; i < m_pAllData->count; i++)
    {
        EnchantCardItem *item = EnchantCardItem::create(m_pAllData->head+i);
        dataArray->addObject(item);
    }
    
    CPtListViewWidget *listview = CPtListViewWidget::create(dataArray, CCSizeMake(size.width*0.81, size.height*0.7), kCCScrollViewDirectionVertical, CCSizeMake(60, 18), 2);
    listview->setBackGround(CCLayerColor::create(ccc4(125, 0, 0, 125)));
    listview->setPosition(ccp(size.width*0.01, size.height*0.1));
    listview->getTableView()->setTouchPriority(PVPSCENETOUCH_PRIORITY-10);
    ((TableView*)listview->getTableView())->setDelayMode(false);
    bg->addChild(listview);
    m_pHunDianContainer = listview;
}

void CPVPKaHunLayer::updateKaHunLabel(CCObject *pObject)
{
    char buffer[30]={0};
    // create 卡魂数值
    snprintf(buffer, sizeof(buffer), "%d", SinglePlayer::instance()->getPlayerKaHun());
    m_pKaHunLabel->setString(buffer);
}

void CPVPKaHunLayer::updateKanHunData(int inSelectIndex)
{
    if(inSelectIndex > 3)
    {
        return;
    }
    int startIndex = 0;
    int count = 0;
    if (m_nSelectBtnIndex == inSelectIndex)
    {
        startIndex = 0;
        count = m_pHunDianContainer->getItems()->count();
        m_nSelectBtnIndex = -1;
    }
    else
    {
        m_nSelectBtnIndex = inSelectIndex;
        startIndex = m_cDataExtent[inSelectIndex].startIndex;
        count = m_cDataExtent[inSelectIndex].count;
    }
    m_pHunDianContainer->setStartIndex(startIndex);
    m_pHunDianContainer->setItemCount(count);
    m_pHunDianContainer->reload();
}

void CPVPKaHunLayer::onClickBack()
{
    removeFromParentAndCleanup(true);
}