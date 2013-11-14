//
//  CBackpackPageLayer.cpp
//  91.cube
//
//  Created by phileas on 13-8-21.
//
//

#include "CBackpackContainerLayer.h"
#include "CBackpackPageLayer.h"
#include "CPtTool.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "CDeletePropLayer.h"
#include "CGameDialogLayer.h"
#include "gameMiddle.h"
#include "CPlayerBufferManager.h"
#include "CPtPropUseEffectConfigData.h"
#include "gamePlayer.h"

CBackpackPageLayer * CBackpackPageLayer::create()
{
    CBackpackPageLayer *layer = new CBackpackPageLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(layer);
        layer = NULL;
    }
   
    
    return layer;
}

CBackpackPageLayer * CBackpackPageLayer::create(multimap<int, int> *inGridData, multimap<int, int>::iterator inBegin, multimap<int, int>::iterator inEnd,int inOpenGridCount)
{
    CBackpackPageLayer *layer = new CBackpackPageLayer();
    if (layer && layer->init())
    {
        layer->autorelease();
        layer->setGridData(inGridData);
        layer->updatePageContent(inBegin, inEnd, inOpenGridCount);
    }
    else
    {
        CC_SAFE_DELETE(layer);
        layer = NULL;
    }
    
    
    return layer;
}


CBackpackPageLayer::CBackpackPageLayer()
{
    m_nCurrentPageTag = 0;
    m_bTouchLock = false;
    m_cMaps = NULL;
    m_pItemNums = NULL;
    m_pMasks = NULL;
    m_pContainerHandler = NULL;
    m_nTouchTag = -1;
    m_nOpenGridNumber = 0;
    m_nUserGridNumber = 0;
    m_bLockRequest = false;
    
    for (int i = 0;  i < 9; i++)
    {
        m_pDeleteButtons[i] = NULL;
        m_pUseButton[i] = NULL;
    }
    
}

CBackpackPageLayer::~CBackpackPageLayer()
{
    //relase resouce:
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    CC_SAFE_RELEASE(m_pItemNums);
    CC_SAFE_RELEASE(m_pMasks);
}

bool CBackpackPageLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initCBackpackPageLayer();
        bRet = true;
    } while (0);
    
    return bRet;
}
bool CBackpackPageLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    // note: if touch lock sprite: set m_bTouchLock = true & return false
    m_bTouchLock = false;
    m_nTouchTag = -1;
    CCLog("CBackpackPageLayer::touchBegin");
    bool bRect = false;
    
    // detect lock buttons:
    int lockCount = m_pMasks->count();
    CCLog("touch lock count : %d",lockCount);
    CCSprite * sprite = NULL;
    for (int i = 0;  i < lockCount; i++)
    {
        sprite = (CCSprite*) m_pMasks->objectAtIndex(i);
        if (CPtTool::isInNode(sprite, pTouch))
        {
            m_bTouchLock = true;
            return bRect;
        }
    }
    CCSprite * tmpSprite = NULL;
    
    // detect use and delete button:
    for (int i = 0; i < m_nOpenGridNumber; i++)
    {
        
        // test:
        // test end:
        if (CPtTool::isInNode(m_pDeleteButtons[i], pTouch))
        {
            m_nTouchTag =(int) m_pDeleteButtons[i]->getUserData();
            tmpSprite = m_pDeleteButtons[i];
            
            CCLog("delete begin: %d, %d ", m_nTouchTag, i);
            bRect = true;
            break;
        }
        if (CPtTool::isInNode(m_pUseButton[i], pTouch))
        {
            m_nTouchTag = (int) m_pUseButton[i]->getUserData();
            tmpSprite = m_pUseButton[i];
            CCLog("use begin: %d", m_nTouchTag);
            bRect = true;
            break;
        }
    }
    if (m_nTouchTag == -1)
    {
        bRect = false;
    }else
    {
        tmpSprite->setTextureRect(m_aButtonRect[1]);
        
    }
    
    return bRect;
    
}
void CBackpackPageLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CBackpackPageLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    // two click event:
    // 1. use and delete:
    // 2. unlock grid:
    
    if (m_bTouchLock)
    {
        // do open the grid:
        int lockCount = m_pMasks->count();
        CCSprite * sprite = NULL;
        for (int i = 0;  i < lockCount; i++)
        {
            sprite = (CCSprite*) m_pMasks->objectAtIndex(i);
            if (CPtTool::isInNode(sprite, pTouch))
            {
                CCLog("open the grid:");
               // openGrid();
                onClickOpenGrid();
                break;
            }
        }

       
        return;
    }
    
    
    // reset button state:
    if (m_nTouchTag!= -1)
    {
        if (m_nTouchTag >= UseTag)
        {
            m_pUseButton[m_nTouchTag-UseTag]->setTextureRect(m_aButtonRect[0]);
        }else
        {
            m_pDeleteButtons[m_nTouchTag-DeleteTag]->setTextureRect(m_aButtonRect[0]);
        }
        
    }
    int tag = -1;
    for (int i = 0; i < m_nOpenGridNumber; i++)
    {
        if (CPtTool::isInNode(m_pDeleteButtons[i], pTouch))
        {
            tag =(int) m_pDeleteButtons[i]->getUserData();
            break;
        }
        if (CPtTool::isInNode(m_pUseButton[i], pTouch))
        {
            tag =(int) m_pUseButton[i]->getUserData();
            break;
        }
    }
    if (tag != -1)
    {
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        if (m_nTouchTag == tag)
        {
            // delete event:
           
            handlerTouch();
            
        }
        
    }

    
}
void CBackpackPageLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CBackpackPageLayer::initCBackpackPageLayer()
{
    
    
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "daoju.plist"));
    
    m_pMasks = CCArray::createWithCapacity(9);
    m_pMasks->retain();
    
    m_pItemNums = CCArray::createWithCapacity(9);
    m_pItemNums->retain();
    
    initPanel(true);
}

void CBackpackPageLayer::handlerTouch()
{
    
    
    if(m_nTouchTag >= UseTag)
    {
       
        m_cPropItem.propCount = 0;
        m_cPropItem.propId = 0;
        PropItem *item = &m_cPropItem;
        CCLog("use end: %d", m_nTouchTag);
        int index = m_nTouchTag-UseTag;
        CCAssert(index <= m_cGridDataIterator.size(), "click error");
        multimap<int, int>::iterator it = m_cGridDataIterator.at(index);
        item->propId = it->first;
        item->propCount = 1;
        if (it->second == 0)
        {
            return;
        }
        char buffer[100] = {0};
        std::string s = SinglePropConfigData::instance()->getPropById(item->propId)->getPropName();
        sprintf(buffer, "你确定要使用%s道具吗?", s.c_str());
        CPtDialog * dialog = CPtDialog::create(buffer, NULL,this, NULL ,callfuncO_selector(CBackpackPageLayer::onClickUseProp),NULL, item);

        CCDirector::sharedDirector()->getRunningScene()->addChild(dialog, 20000, 10000);
        
    }else if (m_nTouchTag >= DeleteTag)
    {
        CCLog("delete end: %d", m_nTouchTag);
        m_cPropItem.propCount = 0;
        m_cPropItem.propId = 0;
        PropItem *item = &m_cPropItem;
        int index = m_nTouchTag-DeleteTag;
        CCAssert(index <= m_cGridDataIterator.size(), "click error");
        multimap<int, int>::iterator it = m_cGridDataIterator.at(index);
        item->propId = it->first;
        item->propCount = it->second;
        CDeletePropLayer * layer = CDeletePropLayer::create(item);
        layer->setHanlder(this, callfuncO_selector(CBackpackPageLayer::onClickDeleteProp), NULL);
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 20000, 10000);
    }
    
}

void CBackpackPageLayer::addItems(int inUseNum)
{
    inUseNum = inUseNum > AllGridInPageNumber ? AllGridInPageNumber : inUseNum;

    CCNode * node = NULL;
    int array[]={1,0};
    for (int i = 0 ; i < inUseNum; i++)
    {
        array[1] = i;
        node =  m_cMaps->getElementByTags(array,2);
        
        // test:
        CPtProp *prop = SinglePropConfigData::instance()->getPropById(200001+i);
        if (prop)
        {
            setItem(node, prop);
        }
        
    }
}
void CBackpackPageLayer::addMask(int inCloseNum)
{
    int array[]={1,0,0};
    inCloseNum  = inCloseNum >= AllGridInPageNumber ? AllGridInPageNumber : inCloseNum;
    inCloseNum = inCloseNum /3;
    CCNode * node = NULL;
    CCSprite * sprite = NULL;

    if (inCloseNum > 0)
    {
        for (int i = (3-inCloseNum)*3; i < AllGridInPageNumber; i++)
        {
            array[1]= i;
            node =  m_cMaps->getElementByTags(array, 2);
            sprite = CCSprite::createWithSpriteFrameName("lock.png");
            node->addChild(sprite, 1000);
            node = m_cMaps->getElementByTags(array, 3);
            sprite->setAnchorPoint(node->getAnchorPoint());
            sprite->setPosition(node->getPosition());
            
            m_pMasks->addObject(sprite);
        }
    }
    
}

void CBackpackPageLayer::initPanel(bool inResetTexture)
{
    CCTexture2D * texture = NULL;
    if (inResetTexture)
    {
       
        CCSpriteFrame * frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("normal.png");
        texture = frame->getTexture();
        m_aButtonRect[0] = frame->getRect();
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("pressed.png");
        m_aButtonRect[1] = frame->getRect();
        
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("disabled.png");
        m_aButtonRect[2] = frame->getRect();

    }
      
    CCNode *mapLayer = NULL;
    CCLabelTTF * label = NULL;
    int array[]={1,0,0};
    for (int i = 0; i < AllGridInPageNumber; i++)
    {
        array[1] = i;
        mapLayer = m_cMaps->getElementByTags(array, 2);
    
        // set use button:
        CCSprite * tmp = (CCSprite *) mapLayer->getChildByTag(19) ;
        tmp->setUserData((void*)(-1));
        if (inResetTexture)
        {
             tmp->setTexture(texture);
        }
       
        label = CCLabelTTF::create("Use", "Arial", 18);
        label->setPosition(ccp(60,18));
        tmp->addChild(label,2, 1);
        if (i < m_nUserGridNumber)
        {
            label->setColor(ccGREEN);
            tmp->setTextureRect(m_aButtonRect[0]);

        }else
        {
            label->setColor(ccGREEN);
            tmp->setTextureRect(m_aButtonRect[2]);
            tmp->setVisible(false);
        }
        m_pUseButton[i] = tmp;
        
        // set delete button:
        tmp = (CCSprite *) mapLayer->getChildByTag(20) ;
        tmp->setUserData((void*)(-1));
        if (inResetTexture)
        {
            tmp->setTexture(texture);
        }
        label = CCLabelTTF::create("Delete", "Arial", 18);
        label->setPosition(ccp(60,18));
        tmp->addChild(label,2, 1);
        if (i < m_nUserGridNumber)
        {
            label->setColor(ccRED);
            tmp->setTextureRect(m_aButtonRect[0]);
            
        }else
        {
            label->setColor(ccRED);
            tmp->setTextureRect(m_aButtonRect[0]);
            tmp->setVisible(false);
        }
        m_pDeleteButtons[i] = tmp;
        
        // number:
        tmp = (CCSprite *) mapLayer->getChildByTag(18) ;
        label = CCLabelTTF::create("0", "Arial", 18);
        label->setPosition(ccp(15,15));
        label->setAnchorPoint(ccp(0.5,0.5));
        tmp->addChild(label,0,1);
        m_pItemNums->addObject(label);

    }
    
}

void CBackpackPageLayer::clearItem(CCNode *node)
{
    
    CCNode * parent;
    if (node)
    {
        for (int i = 16;  i<21 ; i++)
        {
            parent = node->getChildByTag(i);
            if (i == 19 || i == 20)
            {
                ((CCLabelTTF *)parent->getChildByTag(1))->setColor(ccGRAY);
                ((CCSprite *)parent)->setTextureRect(m_aButtonRect[2]);
                parent->setVisible(false);
            }else
            {
                if (i == 18)
                {
                    parent->getChildByTag(1)->setVisible(false);
                }else
                {
                    parent->removeAllChildrenWithCleanup(true);
                }
                
            }
        }
        
        int tag = node->getTag();
        CCLog("clearItem: %d", tag);
        parent = m_pUseButton[tag];
        node = m_pDeleteButtons[tag];
        if (parent)
        {
            parent->setUserData((void*)-1);
            parent->setVisible(false);
        }if (node)
        {
            node->setUserData((void*)-1);
            node->setVisible(false);
        }
        
    }
    
}
void CBackpackPageLayer::resetPanel()
{
    for (int i = 0; i < m_pMasks->count(); i++)
    {
        ((CCSprite*)m_pMasks->objectAtIndex(i))->removeFromParentAndCleanup(true);
    }
    m_pMasks->removeAllObjects();
    CCNode *mapLayer = NULL;
    int array[]={1,0};
    for (int i = 0; i < AllGridInPageNumber; i++)
    {
        array[1] = i;
        mapLayer = m_cMaps->getElementByTags(array, 2);
        CCNode *node = NULL;
        if (mapLayer)
        {
            mapLayer->setVisible(true);
            for (int j = 16; j < 21; j++)
            {
                if (j == 18)
                {
                    ((CCLabelTTF *)m_pItemNums->objectAtIndex(i))->setString("");
                    continue;
                }else if(j==19 || j == 20)
                {
                    // use and delete button

                    continue;
                }
                node = mapLayer->getChildByTag(j);
                if (node)
                {
                    node->removeAllChildrenWithCleanup(true);
                }
            }
            
        }
        
        //reset button tag:
        m_pDeleteButtons[i]->setUserData((void*)-1);
        m_pUseButton[i]->setUserData((void*)-1);
        m_pDeleteButtons[i]->setVisible(false);
        m_pUseButton[i]->setVisible(false);
    }

  

}
/*
 * 1.可使用，可删除
 * 2.可使用不可删除
 * 3.不可使用可删除
 * 4.不可使用不可删除
 */
void CBackpackPageLayer::setItem(CCNode *node, CPtProp *inData)
{
    if (node && inData)
    {
        int tag = node->getTag();
        // tag: 19-->use, 20-->delete:
        CCSprite *useButton =(CCSprite *) node->getChildByTag(19);
        CCSprite *deleteButton =(CCSprite*)node->getChildByTag(20);
       
        CCNode * parent = NULL;
        CCLabelTTF *label = NULL;
        CCRect tmpRect[2] = {m_aButtonRect[0], m_aButtonRect[0]};
        ccColor3B tmpColor[2] = {ccGREEN , ccRED};
        bool tmpFlag[2] = {true,true};
        int useDelete = inData->getUseDelete();
        
        switch (useDelete)
        {
            case 1:
                deleteButton->setUserData((void *)(tag+DeleteTag));
                useButton->setUserData((void*)(tag+UseTag));
                break;
            case 3:
                deleteButton->setUserData((void *)(tag+DeleteTag));
                tmpRect[0] = m_aButtonRect[2];
                tmpColor[0] = ccGRAY;
                tmpFlag[0] = false;
                break;
            case 2:
                useButton->setUserData((void*)(tag+UseTag));
                tmpColor[1] = ccGRAY;
                tmpRect[1] = m_aButtonRect[0];
                break;
            case 4:
                
                tmpRect[0] = m_aButtonRect[2];
                tmpColor[0] = ccGRAY;
                tmpColor[1] = ccGRAY;
                tmpRect[1] = m_aButtonRect[0];
                tmpFlag[0] = false;
                break;
            default:
                break;
        }
        
        deleteButton->setVisible(tmpFlag[1]);
        useButton->setVisible(tmpFlag[0]);
        deleteButton->setTextureRect(tmpRect[1]);
        useButton->setTextureRect(tmpRect[0]);
        
        label = (CCLabelTTF*)deleteButton->getChildByTag(1);
        label->setColor(tmpColor[1]);
        label = (CCLabelTTF*)useButton->getChildByTag(1);
        label->setColor(tmpColor[0]);
        CCSize size;
        
        
        // set name:
        parent = node->getChildByTag(16);
        label = CCLabelTTF::create(inData->getPropName().c_str(), "arial", 13);
        size = parent->getContentSize();
        label->setPosition(ccp(size.width/2, size.height/2));
        parent->addChild(label, 1);
        
        // set tips:
        parent = node->getChildByTag(17);
        label = CCLabelTTF::create(inData->getTips().c_str(), "arial", 13);
        label->setPosition(ccp(116, 90));
        label->setAnchorPoint(ccp(0,1));
        label->setDimensions(CCSizeMake(139, 0));
        parent->addChild(label, 1);
        CCSprite *icon = CCSprite::create(CSTR_FILEPTAH(g_propImagesPath, inData->getIconName().c_str()));
        icon->setPosition(ccp(60,60));
        parent->addChild(icon);
        
        int tmpTag = (int)deleteButton->getUserData();
        CCLog("delet Tag: %d", tmpTag);
        tmpTag = (int) useButton->getUserData();
         CCLog("use Tag: %d", tmpTag);

    }

}

bool CBackpackPageLayer::canOpenGrid()
{
    bool bRet = false;
    //
    CGamePlayer * player = SinglePlayer::instance();
    int openGrid =  player->getOpenGridCount() +3;
    if (player->addGridBySys())
    {
        openGrid -= 3;
    }
    
    int price = ((openGrid - 9)/3 -1) * 9 + 6;
    if (price < player->getPlayerCash())
    {
        bRet = true;
    }
    else
    {
        Middle::showAlertView("钱不够");
    }
    return bRet;
}

void CBackpackPageLayer::openGrid()
{
    if (m_nOpenGridNumber >= AllGridInPageNumber)
    {
        return;
    }
    CGamePlayer * player = SinglePlayer::instance();
    player->AddOpenGridCount(3);
    // handler event:
    // updateData:
    m_nOpenGridNumber += 3;
    // more todo :
   
    int openGrid =  player->getOpenGridCount() +3;
    if (player->addGridBySys())
    {
        openGrid -= 3;
    }
    int price = ((openGrid - 9)/3 -1) * 9 + 6;
    player->subPlayerCash(price);
    
    // update ui
    openGridUI();
    
    // play sound effect:
    PtSoundTool::playSysSoundEffect("UI_click.wav");
    char buffer[300] = {0};
    sprintf(buffer, "消耗金币: %d", price);
    CCMessageBox( buffer, "开格子");
   
}
void CBackpackPageLayer::openGridUI()
{
    if (m_pMasks->count() < 3)
    {
        CCLog("errror:CBackpackPageLayer::openGridUI()");
        return;
    }
    CCSprite* sprite = NULL;
    for (int i = 0; i < 3; i++)
    {
        sprite = (CCSprite*)m_pMasks->objectAtIndex(0);
        sprite->removeFromParentAndCleanup(true);
        m_pMasks->removeObjectAtIndex(0);
    }
    if(m_pMasks->count() == 0)
    {
        m_pContainerHandler->appendPage();
    }
}

void CBackpackPageLayer::onClickOpenGrid()
{
    // 是否可以开启格子：
    bool canOpen = canOpenGrid();
    if (canOpen == false || m_bLockRequest == true)
    {
        //
        CCLog("can't open grid");
        return;
    }
    m_bLockRequest = true;
    int openGridCount = SinglePlayer::instance()->getOpenGridCount();
        openGridCount += 3;
    if (openGridCount > 45)
    {
        CCLog("error");
        return;
    }
  //  sig=2ac2b1e302c46976beaab20a68ef95(标识码),bag_type_id=101(背包类型),grid_num=12(背包格子) unlock_way=1 (解锁方式  1：现金币解锁 2:系统赠送)
    char buffer[100] = {};
//xianbei modify    sprintf(buffer, "sig=2ac2b1e302c46976beaab20a68ef95&bag_type_id=101&grid_num=%d&unlock_way=1",openGridCount);
    sprintf(buffer, "sig=%s&bag_type_id=101&grid_num=%d&unlock_way=1",STR_USER_SIG,openGridCount);
    ADDHTTPREQUESTPOSTDATA(STR_URL_ADD_GRID(196),"addGrid", "addGrid", buffer,callfuncO_selector(CBackpackPageLayer::onReceiveOpenGridMsg));

}
void CBackpackPageLayer::onReceiveOpenGridMsg(CCObject *pOject)
{
    m_bLockRequest = false;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "addGrid");
    char *buff =(char*) pOject;
    
    if (buff)
    {
       CCDictionary *tmp= PtJsonUtility::JsonStringParse(buff);
        if (tmp)
        {
            int result  = GameTools::intForKey("code", tmp);
            if (result == 0)
            {
                openGrid();
            }else
            {
                // error tip:
                CCLog("add grid error....");
            }
        }
    }else
    {
        CCLog("server error");
    }
}

bool CBackpackPageLayer::canUseItemById(int inPropId)
{
    CGamePlayer *player = SinglePlayer::instance();
    CPtPropConfigData *propConfigData = SinglePropConfigData::instance();
    bool bRet = true;
    if (propConfigData->isCanGetCardById(inPropId) && player->cardBagIsMoreThanConfig())
    {
        CCMessageBox("卡包满了", "产生卡片的道具");
        return false;
    }
    int itemEffectid = 0;
    CPtProp* prop =  propConfigData->getPropById(inPropId);
    if (prop)
    {
        itemEffectid = prop->getUseId();
    }
    if (prop->getUnLockLevel() > player->getPlayerLevel())
    {
        CCMessageBox("等级不够，不能使用，快去找王老板开后门", "");
        bRet = false;
        return bRet;
    }
    PtPropUseEffectData * effectData = SinglePropUseEffectConfig::instance()->getPropUseDataById(itemEffectid);
    if (effectData)
    {
        if (effectData->getItemType() == 1)
        {
            if (effectData->getItemTarget() == 1)
            {
                if(player->hasFullAP())
                {
                    CCMessageBox("当前的体力满了，不需要服用大力丸","");
                    bRet = false;
                }
            }
            else if (effectData->getItemTarget() == 2)
            {
                if (player->hasFullGP()) {
                    CCMessageBox("当前的神力满了，不需要喝十全大补汤了","");
                    bRet = false;
                }
            }
        }
    }
    
    return bRet;
}

void CBackpackPageLayer::onClickUseProp(CCObject *object)
{
    if (object)
    {
        PropItem * item = (PropItem*)object;
        if(canUseItemById(item->propId))
        {
            onClickUseProp(item->propId, item->propCount);
        }

    }
}


void CBackpackPageLayer::onClickUseProp(int inPropId, int inPropNum)
{
    //sig=2ac2b1e302c46976beaab20a68ef95(用户标识码) item_id=1(道具ID) num=1(数量)
    char buffer[100] = {0};
//xianbei modify    sprintf(buffer, "sig=2ac2b1e302c46976beaab20a68ef95&item_id=%d&num=%d", inPropId, inPropNum);
    sprintf(buffer, "sig=%s&item_id=%d&num=%d",STR_USER_SIG , inPropId, inPropNum);
    ADDHTTPREQUESTPOSTDATA(STR_URL_USE_GRID(196), "useProp", "useProp", buffer, callfuncO_selector(CBackpackPageLayer::onReceiveUsePropMsg));
}

void CBackpackPageLayer::onClickDeleteProp(CCObject *object)
{
    if (object)
    {
        PropItem * item = (PropItem*)object;
        m_cPropItem.propCount = item->propCount;
        m_cPropItem.propId = item->propId;

        onClickDeleteProp(item->propId, item->propCount);
    }
}
void CBackpackPageLayer::onClickDeleteProp(int inPropId, int inPropNum)
{
    //sig=2ac2b1e302c46976beaab20a68ef95(用户标识码) item_id=1(道具ID) num=1(数量)
    char buffer[100] = {0};
//xianbei modify    sprintf(buffer, "sig=2ac2b1e302c46976beaab20a68ef95&item_id=%d&num=%d", inPropId, inPropNum);
    sprintf(buffer, "sig=%s&item_id=%d&num=%d",STR_USER_SIG, inPropId, inPropNum);
    ADDHTTPREQUESTPOSTDATA(STR_URL_DELETE_PROP(196), "deleteProp", "deleteProp", buffer, callfuncO_selector(CBackpackPageLayer::onReceiveDeletProp));
    
}

void CBackpackPageLayer::onReceiveUsePropMsg(CCObject *pOject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "useProp");
    char *buff = (char*)pOject;
    if (buff)
    {
        CCLog("the buffer: %s", buff);
        CCDictionary * tmp = PtJsonUtility::JsonStringParse(buff);
        delete [] buff;
        if(tmp)
        {
            int resultCode = GameTools::intForKey("code", tmp);
            if (resultCode == 0)
            {
                CCLog("use end: %d", m_nTouchTag);
                int index = m_nTouchTag-UseTag;
                CCAssert(index <= m_cGridDataIterator.size(), "click error");
                multimap<int, int>::iterator it = m_cGridDataIterator.at(index);
                it->second --;
                SinglePlayer::instance()->subProp(it->first, 1); // m_vProps.find(it->first)->second --;
                char tmpbuff[10]={0};
                sprintf(tmpbuff, "%d",it->second);
                ((CCLabelTTF*) m_pItemNums->objectAtIndex(index))->setString(tmpbuff);
                CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(10000, true);
                if (it->second == 0)
                {
//                  
                    reloadPage(it, m_cGridDataIterator.begin()+index);
                }

                // success:
                
                CCDictionary *result = (CCDictionary*) tmp->objectForKey("result");
                CReward *reward =  onUsePropWithDict(result);
                if (reward)
                {
                    char buffer[300]={0};
                    reward->getRewardContent(buffer, 300);
                    CCMessageBox(buffer, "PropUse");
                }
            }
            else
            {
                // failure:
                  CCLog("CBackpackPageLayer::onReceiveUsePropMsg-->server error code: %d", resultCode);
            }
        }
       
    }else
    {
        // server no response:
        CCLog("CBackpackPageLayer::onReceiveUsePropMsg-->server no response");
    }
}

CReward* CBackpackPageLayer::onUsePropWithDict(cocos2d::CCDictionary *inDict)
{
    CReward *addReward = NULL;
    CReward *addItemReward = NULL;
    CCDictionary *rewardDict = (CCDictionary*) inDict->objectForKey("reward");
    CCDictionary* tmpDict = NULL;
    if(rewardDict)
    {
       CCDictionary* addDict = (CCDictionary*)  rewardDict->objectForKey("add");
       if (addDict)
       {
           // get card:
            tmpDict = (CCDictionary*) addDict->objectForKey("card_item");
            addReward= CReward::create(tmpDict);
            if (addReward)
            {
                addReward->excuteReward(ADD);
            }
           
           // get item effect:
           tmpDict = (CCDictionary*) addDict->objectForKey("item");
           addItemReward= CReward::create(tmpDict);
           if (addItemReward)
           {
               addItemReward->excuteReward(ADD);
           }
           
           addReward = CReward::create(addReward, addItemReward);
           
       }
      
    }
    handlerPropBuffer(inDict->objectForKey("buffs"));

    return addReward;
    
}
void CBackpackPageLayer::handlerPropBuffer(CCObject * inEffect)
{
    if (inEffect)
    {
        std::string typeName = typeid(*inEffect).name();
        if (typeName.find("CCDictionary") != std::string::npos)
        {
    
            CPlayerBufferManager::getInstance()->resetPropBufferByDict((CCDictionary*)inEffect);
        }else
        {
            CPlayerBufferManager::getInstance()->setPropBufferZero();
        }
    }
}


void CBackpackPageLayer::onReceiveDeletProp(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "deleteProp");
    char *buff = (char*) pObject;
    
    if (buff)
    {
        CCDictionary * tmp = PtJsonUtility::JsonStringParse(buff);
        if(tmp)
        {
            int resultCode = GameTools::intForKey("code", tmp);
            if (resultCode == 0)
            {
                // success:
                CCLog("use end: %d", m_nTouchTag);
                int index = m_nTouchTag-DeleteTag;
                CCAssert(index <= m_cGridDataIterator.size(), "click error");
                multimap<int, int>::iterator it = m_cGridDataIterator.at(index);
                it->second -= m_cPropItem.propCount;
                SinglePlayer::instance()->subProp(it->first, m_cPropItem.propCount); //m_vProps.find(it->first)->second-=m_cPropItem.propCount;
                char tmpbuff[10]={0};
                sprintf(tmpbuff, "%d",it->second);
                ((CCLabelTTF*) m_pItemNums->objectAtIndex(index))->setString(tmpbuff);
                CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(10000, true);
                if (it->second == 0)
                {
                    int array[2] = {1,index};
                    clearItem(m_cMaps->getElementByTags(array,2));
                    reloadPage(it, m_cGridDataIterator.begin()+index);
                }

            }
            else
            {
                // failure:
                CCLog("CBackpackPageLayer::onReceiveDeletePropMsg-->server error code: %d", resultCode);
            }
        }
        delete [] buff;
    }else
    {
        // server no response:
        CCLog("CBackpackPageLayer::onReceiveDeletePropMsg-->server no response");
    }

}


void CBackpackPageLayer::updatePageContent(multimap<int, int>::iterator inBegin, multimap<int, int>::iterator inEnd, int inOpenGridCount)
{

    // update data:
    m_cGridDataIterator.clear();
    for (multimap<int, int>::iterator i = inBegin; i != inEnd; i++)
    {
        m_cGridDataIterator.push_back(i);
    }
    
    m_nOpenGridNumber = inOpenGridCount;
    // update UI:
    updatePageContentUI(true);
    
}
void CBackpackPageLayer::updatePageContent(vector<multimap<int, int>::iterator> inDataIterators)
{
    // update data:
    m_cGridDataIterator.clear();
    m_cGridDataIterator.assign(inDataIterators.begin(), inDataIterators.end());
    m_nOpenGridNumber = m_cGridDataIterator.size();
    // update UI:
    updatePageContentUI(false);

}

void CBackpackPageLayer::initGridPage()
{
    m_cGridDataIterator.clear();
    m_nOpenGridNumber = m_cGridDataIterator.size();
    // update UI:
    resetPanel();
    if (AllGridInPageNumber - m_nOpenGridNumber > 0)
    {
        addMask(AllGridInPageNumber-m_nOpenGridNumber);
    }
}

void CBackpackPageLayer::updatePageContentUI(bool inAllProps /*= true*/)
{
    resetPanel();
    CPtPropConfigData * propData = SinglePropConfigData::instance();
    int useGridCount = m_cGridDataIterator.size();
    multimap<int, int>::iterator mapIterator;
    CCAssert(useGridCount <= 9, "CBackpackPageLayer::updatePageContentUI-->error");
    CCNode * node = NULL;
    CPtProp *prop = NULL;
    int array[]={1,0};
    char buff[20]={0};
    if (useGridCount == 0)
    {
        
    }
    for (int i = 0 ; i < useGridCount; i++)
    {
        mapIterator = m_cGridDataIterator.at(i);
        array[1] = i;
        node =  m_cMaps->getElementByTags(array,2);
        if (mapIterator->second == 0)
        {
            CCLog("the zero--------%d", mapIterator->first);
            node->getChildByTag(19)->setVisible(false);
            node->getChildByTag(20)->setVisible(false);
            continue;
        }
        CCLog("the number: %d", mapIterator->second);
        sprintf(buff, "%d", mapIterator->second);
        CCLabelTTF * label = ((CCLabelTTF*)m_pItemNums->objectAtIndex(i));
        if (label)
        {
            if (label->retainCount() == 1 && label->getParent() == NULL)
            {
                node->getChildByTag(18)->addChild(label);
            }
            label->setVisible(true);
            label->setString(buff);
        }
        
        // test:
        prop = propData->getPropById(mapIterator->first);
        if (prop)
        {
            setItem(node, prop);
        }
    }

    if (inAllProps)
    {
        // Lock
        if (AllGridInPageNumber - m_nOpenGridNumber > 0)
        {
            addMask(AllGridInPageNumber-m_nOpenGridNumber);
        }
        
    }else
    {
        // unLock:
        CCLog("");
        for (int i = useGridCount; i < AllGridInPageNumber; i++)
        {
            array[1] = i;
            node =  m_cMaps->getElementByTags(array,2);
            node->setVisible(false);
        }
    }
    
    
}

void CBackpackPageLayer::reloadPage(multimap<int, int>::iterator inZeroInterator,vector<multimap<int, int>::iterator>::iterator inVectorIterator)
{
    
    
    if (inZeroInterator->second == 0)
    {
        m_pGridData->erase(inZeroInterator);

    }
    if(inVectorIterator != m_cGridDataIterator.end())
    {
        m_cGridDataIterator.erase(inVectorIterator);
    }
    if (m_pContainerHandler)
    {
        m_pContainerHandler->reLoadPage(m_nCurrentPageTag);
    }
}
