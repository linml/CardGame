//
//  CPtApplyListLayer.cpp
//  91.cube
//
//  Created by phileas on 13-11-15.
//
//

#include "CPtApplyListLayer.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CPtListViewWidget.h"
#include "CPtTableItem.h"
#include "gamePlayer.h"
// implement class of CPtApplyItem

CPtApplyItem::CPtApplyItem()
{
    initApplayItemData();
}
CPtApplyItem::~CPtApplyItem()
{
    
}
bool CPtApplyItem::init()
{
   
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CPtTableItem::init());
        bRet = true;
    } while (0);
    return bRet;
}

bool CPtApplyItem::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (parent)
    {
        if(((CPtApplyListLayer*)parent)->getHandlerHttp())
        {
            CCLog("send_3333");
            return false;
        }
    }
    if(m_bHasConsent)
    {
        return !m_bHasConsent;
    }
    m_nTag = -1;
    bool bRet = CPtTableItem::ccTouchBegan(pTouch, pEvent);
    if(bRet)
    {
        CGameFriedItem *m_pItem = (CGameFriedItem*)m_pDisplayView;
        m_nTag = m_pItem->getTouchTag(pTouch);
        m_pItem->setSelect(m_nTag);
        bRet = m_nTag != -1;
    }
    return bRet;
}
void CPtApplyItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CPtTableItem::ccTouchEnded(pTouch, pEvent);
    CGameFriedItem *m_pItem = (CGameFriedItem*)m_pDisplayView;
    m_pItem->setNormal(m_nTag);
    if(m_nTag == m_pItem->getTouchTag(pTouch))
    {
       
        if (m_pHanlder)
        {
            /*
             #define FRIENDLEFTTAG 1
             #define FRIENDRIGHTTAG 2
             #define FRIENDTOPTAG 3
             class CGameFriedItem : p
             */
            if (m_nTag == FRIENDLEFTTAG && m_pConsentSelector)
            {
                (m_pHanlder->*m_pConsentSelector)(m_pItem);
            }
            else if (m_nTag == FRIENDRIGHTTAG && m_pCancelSelector)
            {
                 (m_pHanlder->*m_pCancelSelector)(m_pItem);
            }
        }

    }
  

}

void CPtApplyItem::initApplayItemData()
{
    m_pHanlder = NULL;
    m_pCancelSelector = NULL;
    m_pConsentSelector = NULL;
    m_bHasConsent = false;
}

void CPtApplyItem::setHandler(CCObject *pTarget,SEL_CallFuncO cancel, SEL_CallFuncO consent)
{
    m_pHanlder = pTarget;
    m_pCancelSelector = cancel;
    m_pConsentSelector = consent;
}

void CPtApplyItem::setIndex(int inIndex)
{
    ((CGameFriedItem*)getDisplayView())->setIndex(inIndex);
}


// implement class of CPtApplyListLayer

CPtApplyListLayer::CPtApplyListLayer()
{
    initData();
}
CPtApplyListLayer::~CPtApplyListLayer()
{
    releaseResource();
}
bool CPtApplyListLayer::init()
{
    bool bRet =false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCPtApplyListLayer();
        bRet = true;
    } while (0);
    return bRet;
}

void CPtApplyListLayer::initData()
{
    m_nSelectIndex = -1;
    m_pApplyList = NULL;
    m_bHandlerHttp = false;
    m_pApplyItems = NULL;
    m_pContainer = NULL;
    m_bAllOperator = false;
    m_pFriendManager = CFriendManager::getInstance();
}

void CPtApplyListLayer::initCPtApplyListLayer()
{
    onSendGetApplyListData();
   // testData();
}

void CPtApplyListLayer::initUI()
{
    if (m_pContainer == NULL)
    {
        m_pApplyItems = CCArray::create();
        m_pApplyItems->retain();
        if (m_pApplyList)
        {
            CGameFriedItem *itemNode = NULL;
            CPtApplyItem* item = NULL;
            for (int i = 0; i < m_pApplyList->size(); i++)
            {
                itemNode = CGameFriedItem::create(m_pApplyList->at(i)->getFriendInfo(), i);
                itemNode->setButtonText("同意", "忽略");
                itemNode->setItemType(E_APPLY);
                item = CPtApplyItem::create();
                item->setDisplayView(itemNode);
                item->setLayerParent(this);
                m_pApplyItems->addObject(item);
                item->setTouchNode(itemNode);
                item->setHandler(this, callfuncO_selector(CPtApplyListLayer::onClickCancle) ,callfuncO_selector(CPtApplyListLayer::onClickConsent));
            }
            
        }
       // CCLayerColor *bg = CCLayerColor::create(ccc4(125, 0, 0, 200));
        CPtListViewWidget *tempview = CPtListViewWidget::create(m_pApplyItems, CCSizeMake(600, 380), kCCScrollViewDirectionVertical,  CCSizeMake(5, 2.5), 2);
        tempview->getTableView()->setTouchPriority(-300);
       
        m_pContainer = tempview; //CCTableView::create(this, CCSizeMake(200, 300), bg);
        CCPoint point = tempview->getTableView()->getContentOffset();
        ((TableView*)tempview->getTableView())->setDelayMode(false);
        m_pContainer->setPosition(ccp(220, 180));
        addChild(m_pContainer);

    }
}

void CPtApplyListLayer::releaseResource()
{
    CC_SAFE_RELEASE_NULL(m_pApplyItems);
    deleteApplyListData();
}

void CPtApplyListLayer::deleteApplyListData()
{
    if (m_pApplyList)
    {
        for (int i = 0;  i < m_pApplyList->size(); i++)
        {
            delete m_pApplyList->at(i);
        }
        delete m_pApplyList;
        m_pApplyList = NULL;
    }
}
void CPtApplyListLayer::onSendGetApplyListData()
{
    m_bHandlerHttp = true;
    char buffer[200] = {0};
    sprintf(buffer, "&sig=%s",STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETAPPLYLIST(196), "CALLBACK_CPtApplyListLayer::onSendGetApplyListData",
                           "REQUEST_CPtApplyListLayer::onSendGetApplyListData", buffer, callfuncO_selector(CPtApplyListLayer::onReceiveGetApplyMsg));
    
}

void CPtApplyListLayer::onReceiveGetApplyMsg(CCObject *pObject)
{
    m_bHandlerHttp = false;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPtApplyListLayer::onSendGetApplyListData");
    char *buffer = (char*) pObject;
    deleteApplyListData();
    if (buffer)
    {
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        buffer=NULL;
        int code = GameTools::intForKey("code", resultDict);
        if (code == 0)
        {
           resultDict = (CCDictionary*) resultDict->objectForKey("result");
           m_pApplyList = m_pFriendManager->getApplyFriendListByDict(resultDict);
           initUI();
        }
    }
    
}

// callBack

void CPtApplyListLayer::onClickCancle(CCObject *pObject)
{
    CGameFriedItem * item = (CGameFriedItem *)pObject;
    if(item)
    {
        vector<int> applyIds;
        int applyId;
        int index = item->getIndex();
        m_nSelectIndex = index;
        if (index >= 0 && index < m_pApplyList->size())
        {
            applyId = m_pApplyList->at(index)->getApplyId();
            applyIds.push_back(applyId);
            onSendApplyRequest(applyIds, 0);
        }
    }
    
}
void CPtApplyListLayer::onClickConsent(CCObject *pObject)
{
    CGameFriedItem * item = (CGameFriedItem *)pObject;
    if(item)
    {
        vector<int> applyIds;
        int applyId;
        int index = item->getIndex();
        m_nSelectIndex = index;
        if (index >= 0 && index < m_pApplyList->size())
        {
            applyId = m_pApplyList->at(index)->getApplyId();
            applyIds.push_back(applyId);
            onSendApplyRequest(applyIds, 1);
        }
    }

}

/*
 * id
 */
void CPtApplyListLayer::onSendApplyRequest(const vector<int> &array, int inType)
{
    
    if (array.size() > 0)
    {
        m_bHandlerHttp = true;
        CCLog("send_111");
        char temp[150]={"0"};
        snprintf(temp,150,"&sig=%s&id=[%d",STR_USER_SIG,array[0]);
        std::string buffer(temp);
        for (int i = 1; i < array.size(); i++)
        {
            sprintf(temp, ",%d", array[i]);
            buffer.append(temp);
        }
        snprintf(temp, 150, "]&type=%d", inType);
        buffer.append(temp);
        CCLog("the buffer: %s", buffer.c_str());
        ADDHTTPREQUESTPOSTDATA(STR_URL_HANDLERAPPLY(196), "CALLBACK_CPtApplyListLayer::onSendApplyRequest", "REQUEST_CPtApplyListLayer::onSendApplyRequest", buffer.c_str(), callfuncO_selector(CPtApplyListLayer::onReceiveApplyMsg));
    }
    
}

void CPtApplyListLayer::onReceiveApplyMsg(CCObject *pObject)
{
    m_bHandlerHttp = false;
//    
//    // test
//    updateData();
//    return;
//    // test end
    
    CCLog("send_2222");
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this,  "CALLBACK_CPtApplyListLayer::onSendApplyRequest");
    char *buffer = (char*) pObject;
    CCLog("the buffer: %s", buffer);
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
                updateData();
            }
            else
            {
                CCMessageBox(CCString::createWithFormat("the error code :%d", code)->getCString(), "");
            }
        }
    }
}

void CPtApplyListLayer::updateData()
{
    if(m_bAllOperator)
    {
        m_bAllOperator = false;
        deleteApplyListData();
        m_pApplyItems->removeAllObjects();
        ((CPtListViewWidget*)m_pContainer)->reload();
        return;
    }
    if (m_nSelectIndex != -1)
    {
        if (m_nSelectIndex >= 0 && m_nSelectIndex < m_pApplyList->size())
        {
            CC_SAFE_DELETE(m_pApplyList->at(m_nSelectIndex));
            m_pApplyList->erase(m_pApplyList->begin()+m_nSelectIndex);
            m_pApplyItems->removeObjectAtIndex(m_nSelectIndex);
            ((CPtListViewWidget*)m_pContainer)->reload();
            for (int i = m_nSelectIndex; i <m_pApplyItems->count(); i++)
            {
                ((CPtApplyItem*) m_pApplyItems->objectAtIndex(i))->setIndex(i);
            }
        }

        m_nSelectIndex = -1;
    }
}


void CPtApplyListLayer::onClickAllConsent()
{
    if (!SinglePlayer::instance()->getPlayrHadRecharged())
    {
        CCMessageBox("土豪不差钱", "充值福利");
        return;
    }
    vector<int> applyIds;
    int applyId;
    m_nSelectIndex = -1;
    if (m_pApplyList && m_pApplyList->size() > 0)
    {
        for (int i = 0; i < m_pApplyList->size(); i++)
        {
            applyId = m_pApplyList->at(i)->getApplyId();
            applyIds.push_back(applyId);
        }
        m_bAllOperator = true;
        onSendApplyRequest(applyIds, 1);
    }
    

}
void CPtApplyListLayer::onClickAllCancel()
{
    if (!SinglePlayer::instance()->getPlayrHadRecharged())
    {
        CCMessageBox("土豪不差钱", "充值福利");
        return;
    }
    vector<int> applyIds;
    int applyId;
    m_nSelectIndex = -1;
    if (m_pApplyList && m_pApplyList->size() > 0)
    {
        m_bAllOperator = true;
        for (int i = 0; i < m_pApplyList->size(); i++)
        {
            applyId = m_pApplyList->at(i)->getApplyId();
            applyIds.push_back(applyId);
        }
        onSendApplyRequest(applyIds, 0);
    }
   
}


// test:
void CPtApplyListLayer::testData()
{
    const char * str = "{\"code\": 0,\"result\":{\"2\":{\"friend_uid\": \"197\",\"username\": \"apple\",\"level\": \"6\"},\"3\":{\"friend_uid\": \"182\",\"username\": \"orlando\",\"level\": \"4\"},\"6\":{\"friend_uid\": \"256\",\"username\": \"orlando\",\"level\": \"4\"},\"4\":{\"friend_uid\": \"962\",\"username\": \"orlando\",\"level\": \"4\"}}}";
   
    CCDictionary *resultDict = PtJsonUtility::JsonStringParse(str);
    int code = GameTools::intForKey("code", resultDict);
    if (code == 0)
    {
        resultDict = (CCDictionary*) resultDict->objectForKey("result");
        m_pApplyList = m_pFriendManager->getApplyFriendListByDict(resultDict);
        initUI();
    }

}
