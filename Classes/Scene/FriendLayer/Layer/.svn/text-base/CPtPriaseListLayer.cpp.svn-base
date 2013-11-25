//
//  CPtPriaseListLayer.cpp
//  91.cube
//
//  Created by phileas on 13-11-15.
//
//

#include "CPtPriaseListLayer.h"
#include "gameConfig.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CPtListViewWidget.h"
#include "gamePlayer.h"
#include "CReward.h"

CPtPriaseItem* CPtPriaseItem::create(Friend inFriendData, int inIndex)
{
    CPtPriaseItem *item = new CPtPriaseItem(inFriendData);
    
    if (item)
    {
        item->setIndex(inIndex);
        if (item->init())
        {
            item->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(item);
            item = NULL;
        }
        
    
    }
    return item;
}

CPtPriaseItem::CPtPriaseItem(Friend inFriendData)
{
    m_sFriendData = inFriendData;
    m_pOweBtn = NULL;
    m_pOweSelector = NULL;
    m_pHandler = NULL;
    texture[1]=NULL;
    texture[0]=NULL;
}
CPtPriaseItem::~CPtPriaseItem()
{
    
}
bool CPtPriaseItem::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CPtTableItem::init());
        initPtPriase();
        bRet = true;
    } while (0);
    return bRet;
}

void CPtPriaseItem::initPtPriase()
{
    char buffer[200] = {0};
    
    CCNode *displayView = CCNode::create();
    sprintf(buffer, "你感受到了来自%s的善意， 赞美使你们的友谊更加坚固",m_sFriendData.username.c_str());
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial", 15);
    label->setAnchorPoint(ccp(0,0.5));
    displayView->addChild(label);
    label->setPosition(ccp(20, 0));
    texture[0] = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    texture[1] = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Pressed.png"));
    m_pOweBtn = CCSprite::createWithTexture(texture[0]);
    setTouchNode(m_pOweBtn);
    m_pOweBtn->setPosition(ccp(450, 0));
    m_pOweBtn->setAnchorPoint(ccp(0,0.5));
    displayView->addChild(m_pOweBtn);

    this->setContentSize(m_pOweBtn->getContentSize());
    CCRect rect1 = label->boundingBox();
    CCRect rect2 = m_pOweBtn->boundingBox();
    
    CCPoint point1 = CCPointMake(MIN(rect1.getMinX(), rect2.getMinX()), MIN(rect1.getMinY(), rect2.getMinY()));
    CCPoint point2 = CCPointMake(MAX(rect2.getMaxX(), rect1.getMaxX()), MAX(rect1.getMaxY(), rect2.getMaxY()));
    
    CCSize size = CCSizeMake(fabsf(point1.x-point2.x), fabsf(point1.y-point2.y));
    displayView->setContentSize(size);
    
    label = CCLabelTTF::create("感恩", "Arial", 16);
    label->setPosition(ccp(60, 20));
    m_pOweBtn->addChild(label);
    setDisplayView(displayView);
}


bool CPtPriaseItem::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (parent)
    {
        if(((CPtPriaseListLayer*)parent)->getHandlerHttp())
        {
            return false;
        }
    }

    bool bRet = CPtTableItem::ccTouchBegan(pTouch, pEvent);
    if (bRet)
    {
        m_pOweBtn->setTexture(texture[1]);
    }
    return bRet;
}
void CPtPriaseItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_pOweBtn->setTexture(texture[0]);
    CPtTableItem::ccTouchEnded(pTouch, pEvent);
    if (CPtTool::isInNode(m_pOweBtn, pTouch)&& m_pHandler && m_pOweSelector)
    {
        (m_pHandler->*m_pOweSelector)(this);
    }
}

// implement CPtPriaseListLayer
CPtPriaseListLayer::CPtPriaseListLayer()
{
    initData();
}
CPtPriaseListLayer::~CPtPriaseListLayer()
{
    CC_SAFE_RELEASE_NULL(m_pPriases);
    deletePraseListInfo();
}
bool CPtPriaseListLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        onSendGetPraiseListRequest();
        //testData();
        bRet = true;
    } while (0);
    return bRet;
}


void CPtPriaseListLayer::onClickAllGanEn()
{
    if (!SinglePlayer::instance()->getPlayrHadRecharged())
    {
        CCMessageBox("土豪不差钱", "充值福利");
        return;
    }
    vector<int> friendIds;
    int friendId;
    m_nSelectIndex = -2;
    if (m_pAllPriaseListInfo && m_pAllPriaseListInfo->size() > 0)
    {
        for (int i = 0; i < m_pAllPriaseListInfo->size(); i++)
        {
            friendId = m_pAllPriaseListInfo->at(i).fried_uid;
            friendIds.push_back(friendId);
        }
        m_bHasAllGanEn = true;
        onSendGanEnRequest(friendIds);

    }
     
            

}

void CPtPriaseListLayer::initData()
{
    m_pAllPriaseListInfo = NULL;
    m_pFriendManager = CFriendManager::getInstance();
    m_pContainer = NULL;
    m_pPriases = NULL;
    m_bHandlerHttp = false;
    m_nSelectIndex = -1;
    m_bHasAllGanEn = false;
}


void CPtPriaseListLayer::initUI()
{
    if (m_pContainer == NULL)
    {
        CC_SAFE_RELEASE_NULL(m_pPriases);
        m_pPriases = CCArray::create();
        m_pPriases->retain();
        if (m_pAllPriaseListInfo)
        {
            CPtTableItem *itemNode = NULL;
            for (int i = 0; i < m_pAllPriaseListInfo->size(); i++)
            {
                itemNode = CPtPriaseItem::create(m_pAllPriaseListInfo->at(i),i);
                itemNode->setLayerParent(this);
                ((CPtPriaseItem*)itemNode)->setHandler(this, callfuncO_selector(CPtPriaseListLayer::onClickGanEn));
                m_pPriases->addObject(itemNode);

            }
            
        }
       // CCLayerColor *bg = CCLayerColor::create(ccc4(125, 0, 0, 200));
        CPtListViewWidget *tempview = CPtListViewWidget::create(m_pPriases, CCSizeMake(600, 380));
        tempview->getTableView()->setTouchPriority(-300);
       // tempview->setBackGround(bg);
        m_pContainer = tempview; 
        CCPoint point = tempview->getTableView()->getContentOffset();
        ((TableView*)tempview->getTableView())->setDelayMode(false);
        m_pContainer->setPosition(ccp(220, 180));
        addChild(m_pContainer);
        
    }

}

void CPtPriaseListLayer::deletePraseListInfo()
{
    if(m_pAllPriaseListInfo) 
    {
        delete m_pAllPriaseListInfo;
        m_pAllPriaseListInfo = NULL;
    }
}
void CPtPriaseListLayer::onSendGetPraiseListRequest()
{
    m_bHandlerHttp = true;
    char buffer[200] = {0};
    sprintf(buffer, "&sig=%s",STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(
                           STR_URL_GETPRIASELIST(196), "CALLBACK_CPtPriaseListLayer::onSendGetPraiseListRequest",
                           "REQUEST_CPtPriaseListLayer::onSendGetPraiseListRequest", buffer, callfuncO_selector(CPtPriaseListLayer::onReceivePraiseListMsg));
}
void CPtPriaseListLayer::onReceivePraiseListMsg(CCObject *pObject)
{
    m_bHandlerHttp = false;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPtPriaseListLayer::onSendGetPraiseListRequest");
    char *buffer = (char *)pObject;
    if(buffer)
    {
        CCLog("the buffer: %s",buffer);
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        if(resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                deletePraseListInfo();
                resultDict = (CCDictionary *) resultDict->objectForKey("result");
                m_pAllPriaseListInfo = m_pFriendManager->getPraiseListByDict(resultDict);
                initUI();
            }
            
        }
      
        
    }
    
}

void CPtPriaseListLayer::onSendGanEnRequest(const vector<int> &ids)
{
    if (ids.size() > 0)
    {
        m_bHandlerHttp = true;
        char temp[150]={"0"};
        snprintf(temp,150,"&sig=%s&friend_uid=[%d",STR_USER_SIG,ids[0]);
        std::string buffer(temp);
        for (int i = 1; i < ids.size(); i++)
        {
            sprintf(temp, ",%d", ids[i]);
            buffer.append(temp);
        }
        buffer.append("]");
        CCLog("the buffer: %s", buffer.c_str());
        ADDHTTPREQUESTPOSTDATA(STR_URL_GANEN(196), "CALLBACK_CPtPriaseListLayer::onSendGanEnRequest", "REQUEST_CPtPriaseListLayer::onSendGanEnRequest", buffer.c_str(), callfuncO_selector(CPtPriaseListLayer::onReceiveGanEnMsg));
    }
    
}
void CPtPriaseListLayer::onReceiveGanEnMsg(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPtPriaseListLayer::onSendGanEnRequest");
    m_bHandlerHttp = false;
    char *buffer = (char*) pObject;
    CCLog("the buffer: %s", buffer);
//    // test:
//    updateData();
//    return;
//    // test end:
    if (buffer)
    {
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                CCLog("sucess");
                saveData(resultDict);
                updateData();
                
            }
            else
            {
                CCMessageBox(CCString::createWithFormat("the error code :%d", code)->getCString(), "");
            }
        }
    }

}


void CPtPriaseListLayer::onClickGanEn(CCObject *pObject)
{
    
    CPtPriaseItem * item = (CPtPriaseItem *)pObject;
    if(item)
    {
        vector<int> friendIds;
        int friendId;
        int index = item->getIndex();
        m_nSelectIndex = index;
        if (index >= 0 && index < m_pAllPriaseListInfo->size())
        {
            friendId = m_pAllPriaseListInfo->at(index).fried_uid;
            friendIds.push_back(friendId);
            onSendGanEnRequest(friendIds);
        }
    }

}

void CPtPriaseListLayer::updateData()
{
    if (m_bHasAllGanEn)
    {
        m_bHasAllGanEn = false;
        deletePraseListInfo();
        m_pPriases->removeAllObjects();
        ((CPtListViewWidget*)m_pContainer)->reload();
        return;
    }
    
    if(m_nSelectIndex != -1)
    {
        if (m_nSelectIndex >= 0 && m_nSelectIndex < m_pAllPriaseListInfo->size())
        {
            m_pAllPriaseListInfo->erase(m_pAllPriaseListInfo->begin()+m_nSelectIndex);
            m_pPriases->removeObjectAtIndex(m_nSelectIndex);
            ((CPtListViewWidget*)m_pContainer)->reload();
            for (int i = m_nSelectIndex; i < m_pPriases->count(); i++)
            {
                ((CPtPriaseItem*)m_pPriases->objectAtIndex(i))->setIndex(i);
            }
        }

        m_nSelectIndex = -1;
    }
    
    
}

void CPtPriaseListLayer::saveData(CCDictionary *inResDict)
{
    inResDict = (CCDictionary*) inResDict->objectForKey("result");
    if (inResDict)
    {
        CCDictionary * tmp = (CCDictionary*) inResDict->objectForKey("reward");
        if (tmp)
        {
            CReward *reward =NULL;
            if (tmp->objectForKey("add") &&((CCDictionary*)tmp->objectForKey("add"))->objectForKey("friend") )
            {
                reward = CReward::create((CCDictionary *)((CCDictionary*)tmp->objectForKey("add"))->objectForKey("friend"));
                reward->excuteReward(ADD);
                
            }
        }
        
    }

}
void CPtPriaseListLayer::testData()
{
    const char * testStr = "{\"code\": 0,\"result\":{\"2\":{\"friend_uid\":\"197\",\"username\":\"orlando\"},\"1\":{\"friend_uid\":\"196\",\"username\":\"pili\"},\"3\":{\"friend_uid\":\"187\",\"username\":\"orlando\"},\"5\":{\"friend_uid\":\"156\",\"username\":\"pili\"},\"36\":{\"friend_uid\":\"17\",\"username\":\"orlando\"},\"15\":{\"friend_uid\":\"1966\",\"username\":\"pili\"},\"24\":{\"friend_uid\":\"1997\",\"username\":\"orlando\"},\"221\":{\"friend_uid\":\"1963\",\"username\":\"pili\"},\"223\":{\"friend_uid\":\"1937\",\"username\":\"orlando\"},\"51\":{\"friend_uid\":\"1962\",\"username\":\"pili\"}}}";
    CCDictionary *resultDict = PtJsonUtility::JsonStringParse(testStr);
    if(resultDict)
    {
        int code = GameTools::intForKey("code", resultDict);
        if (code == 0)
        {
            deletePraseListInfo();
            resultDict = (CCDictionary*) resultDict->objectForKey("result");
            m_pAllPriaseListInfo = m_pFriendManager->getPraiseListByDict(resultDict);
            initUI();
        }
        
    }

}
