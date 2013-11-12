//
//  Biforest.cpp
//  91.cube
//
//  Created by phileas on 13-6-13.
//
//

#include "Biforest.h"
#include "AsgardLayer.h"
#include "gameConfig.h"
#include "ExplorationScene.h"
#include "SceneManager.h"
#include "CPtSectionConfigData.h"
#include "CPtListViewWidget.h"
#include "CPtTableItem.h"
#include "PtHttpURL.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CPlayerBufferManager.h"

// implement class of CPtBookItem

/*
 * @param inType: 1(TYPE_CHAPTER)---> chapter; 2(TYPE_CHAPTER)---> section:
 */
CPtBookItem * CPtBookItem::create(const char *inName, int inType)
{
    CPtBookItem * bookItem = NULL;
    bookItem = new CPtBookItem();
    bookItem->init();
    bookItem->autorelease();
    
    if (inType == TYPE_CHAPTER)
    {
        
    }else if(inType == TYPE_SECTION)
    {
        
    }
    return bookItem;
}




// implement class of CBiforestLayer:

CBiforestLayer * CBiforestLayer::create(EXPLORATIONTYPE inType /*= NORMALEXPLORATION*/, int inSelectSectionId)
{
    CBiforestLayer *layer = new CBiforestLayer(inType);
    if (layer)
    {
        layer->setSelectSectionId(inSelectSectionId);
        if(layer->init())
        {
            layer->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(layer);
            layer = NULL;
        }
        
    }else
    {
        CC_SAFE_DELETE(layer);
        layer = NULL;
    }
    return layer;
}

CBiforestLayer::CBiforestLayer(EXPLORATIONTYPE inType /*= NORMALEXPLORATION*/): m_eExploartionType(inType)
{
    m_nCurrentChapterId = -1;
    m_nCurrentChaptetIndex = -1;
    m_nCurrentSectionId = -1;
    m_nCurrentSectionIndex = -1;
    
    m_cMaps = NULL;
    m_pTouchSprite = NULL;
    
    m_pNormalChapters = NULL;
    m_pActivityChapters = NULL;
    m_pNormalSections = NULL;
    m_pActivitySections = NULL;
    
    m_pTips = NULL;
    m_pBg = NULL;
    m_pGoBtn = NULL;
    
    m_sPicName = "a.png";
    m_sTips = "";
    m_bGoneVisiable = false;
    m_bSendRequest = false;
    
    m_nMaxChapterId = SinglePlayer::instance()->getMaxChapterId();
    m_nMaxSectionId = SinglePlayer::instance()->getMaxSectionid();
    
    
    m_pActivityEncounterManager = SingleActivityEncounterManager::instance();
    m_pSwtichBtn = NULL;
    m_pListView = NULL;
    m_pActivityEncounterInfo = NULL;
    m_bStartActivity = false;
}

CBiforestLayer::~CBiforestLayer()
{
    CC_SAFE_RELEASE(m_cMaps);
    CC_SAFE_RELEASE(m_pNormalChapters);
    CC_SAFE_RELEASE(m_pActivityChapters);
    CC_SAFE_RELEASE(m_pNormalSections);
    CC_SAFE_RELEASE(m_pActivitySections);
    CC_SAFE_RELEASE(m_pActivityEncounterInfo);
}



bool CBiforestLayer::init()
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 125)));
        initBiforest();
        bRet = true;
    } while (0);
    return  bRet;
}

bool CBiforestLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
   
    CCPoint touchPoint = pTouch->getLocation();
    m_nTouchTag = TouchRect::SearchTouchTag(touchPoint, m_cTouches);
    return  true;
}

void CBiforestLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CBiforestLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_bSendRequest)
    {
        return;
    }
    CCPoint touchPoint = pTouch->getLocation();
    if (m_nTouchTag ==  TouchRect::SearchTouchTag(touchPoint, m_cTouches, &m_pTouchSprite))
    {
        if (m_nTouchTag != -1)
        {
            Utility::handleBtnCallBack(m_pTouchSprite, this, NULL);
        }
     
        handlerTouch();
    }

}
void CBiforestLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}


void CBiforestLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{    
    int index = cell->getIdx();
    CCLog("the id: %d", index);
    int before =  m_pListView->getItems()->getWhichOneOpen();
    m_pListView->setOpenItemByIndex(index);
    int afeter = m_pListView->getItems()->getWhichOneOpen();
     CPtExpandableListViewItem* item = (CPtExpandableListViewItem*)(m_pListView->getItems()->getListViewItems()->objectAtIndex(before));
    if (before == afeter)
    {
       int count =   index - before;
        if (count != 0)
        {
            if( m_nCurrentSectionIndex != count-1)
            {
                if (m_nCurrentSectionIndex != -1)
                {
                    ((CPtExpandableListViewItem*) item->getChildren()->objectAtIndex(m_nCurrentSectionIndex))->setNormal();
                }
                m_nCurrentSectionIndex = count-1;
                ((CPtExpandableListViewItem*) item->getChildren()->objectAtIndex(m_nCurrentSectionIndex))->setSelected();
             
            }
            
        }
        
    }else
    {
       
        item->setNormal();
        if (m_nCurrentSectionIndex != -1)
        {
            ((CPtExpandableListViewItem*) item->getChildren()->objectAtIndex(m_nCurrentSectionIndex))->setNormal();
        }
        
        m_nCurrentSectionIndex = -1;
        item = (CPtExpandableListViewItem*)(m_pListView->getItems()->getListViewItems()->objectAtIndex(afeter));
        item->setSelected();
    }
   
    m_nCurrentChaptetIndex = afeter;
    m_nCurrentChapterId = afeter*INTERVALVALUE + BASEVALUE;
    CCLog("the current: chapter: %d, section: %d selectId: %d", m_nCurrentChaptetIndex, m_nCurrentSectionIndex, index);

    updatePanel(m_nCurrentChaptetIndex, m_nCurrentSectionIndex);
}

void CBiforestLayer::initBiforest()
{
    // load resource:
    loadResource();
 
    // init data:
    m_nTouchTag  = -1;
    m_bSectionTouchEnable = false;
    
    // init map
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "biforest.plist"));
    m_cMaps->getTouchRects(m_cTouches);
    
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-7);
    
    
    int array[] = {2,0,1};
    for (int i = 1; i< 8; i++)
    {
        if (i == 4)
        {
            continue;
        }
        array[2] = i;
        CCNode *node = m_cMaps->getElementByTags(array, 3);
        node->setVisible(false);
    }
    
    // add switchBtn
    m_pSwtichBtn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button.png"));
    m_pSwtichBtn->setAnchorPoint(CCPointZero);
    m_pSwtichBtn->setPosition(ccp(650, 580));
    CCNode *node = m_cMaps->getElementByTags(array, 2);
    node->addChild(m_pSwtichBtn, 100,2006);

    Utility::addTouchRect(2006, m_pSwtichBtn, m_cTouches);
    
    if (m_eExploartionType == NORMALEXPLORATION)
    {
        initPanel();
    }
    else
    {
        goToAcitivityEncounter();
    }
    
}


CCArray* CBiforestLayer::getDispLayerData()
{
    CCArray* chapters = getChaptersByType(m_eExploartionType, m_nMaxChapterId);
    
    
    // get items:
    CCArray * array = CCArray::create();
    CPtChapter * chapter = NULL;
    CPtExpandableListViewItem * item = NULL;
    
    CCArray * sections = getSectionItem(getSections(m_nCurrentChaptetIndex, m_nCurrentSectionId));
    
    for (int i = 0; i <chapters->count(); i++)
    {
        item = CPtExpandableListViewItem::create();
        chapter = (CPtChapter *) chapters->objectAtIndex(i);
        if (chapter)
        {
            item->setDisplayView(createItemViewByType(chapter->getChapterName().c_str()));
            item->setHandler(this, callfuncO_selector(CBiforestLayer::setNormal), callfuncO_selector(CBiforestLayer::setSelected), callfuncO_selector(CBiforestLayer::getChildren));
            item->setIndex(i);
        }
        if (i == m_nCurrentChaptetIndex)
        {
            item->setChildrenItem(sections);
            item->setSelected();
        }
        array->addObject(item);
    }

    return array;
}

void CBiforestLayer::initPanel()
{
    if (m_eExploartionType == NORMALEXPLORATION)
    {
        initNormalPanel();
    }else
    {
        initActivityPanel();
    }
    
    CPtExpandableListViewItemList * expandList = CPtExpandableListViewItemList::create();
    expandList->setListViewItems(getDispLayerData());
    expandList->setWhichOneOpen(m_nCurrentChaptetIndex);
    
    CPtExpandableListView*  leftPanel = CPtExpandableListView::create(expandList, CCSizeMake(246, 515),  CCSizeMake(0.0f, 0.0f));
    leftPanel->getTabelView()->setDelegate(this);
    leftPanel->getTabelView()->setTouchPriority(-300);
    leftPanel->setPosition(ccp(220, 135));
    leftPanel->getTabelView()->setTouchPriority(-200);
    m_cMaps->getElementByTags("2,0")->addChild(leftPanel, 10);
    m_pListView = leftPanel;
    
    initRightPanel(m_nCurrentChaptetIndex);
    updatePanel(m_nCurrentChaptetIndex, m_nCurrentSectionIndex);
    
}


void CBiforestLayer::initActivityPanel()
{
    m_nCurrentChapterId = ACTIVITYCHAPTERID;
    m_nCurrentChaptetIndex = 0;
    m_nCurrentSectionIndex = -1;
    
}

void CBiforestLayer::initNormalPanel()
{
    m_nCurrentChapterId = m_nMaxChapterId;
    m_nCurrentChaptetIndex = (m_nMaxChapterId - BASEVALUE )/INTERVALVALUE;
    m_nCurrentSectionId = m_nMaxSectionId;

}


void CBiforestLayer::initRightPanel(int inCurrentChapterIndex)
{
    int array[]={2,0,7};
    CPtChapter * chapter = getChapterByIndex(inCurrentChapterIndex);
    m_sPicName =  chapter->getChapterPicName();
    CCLog("---%s",chapter->getChapterTipid().c_str());
    m_sTips = Utility::getWordWithFile("dictionary.plist", chapter->getChapterTipid().c_str());
    m_bGoneVisiable = false;
    
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_chapterPath, m_sPicName.c_str()));
    m_pBg = CCSprite::createWithTexture(texture);
    m_pTips = CCLabelTTF::create(m_sTips.c_str(), "arial", 15,CCSizeMake(300,0),kCCTextAlignmentLeft);
    m_pBg->setPosition(ccp(640, 550));
    m_pTips->setAnchorPoint(ccp(0, 1));
    m_pTips->setPosition(ccp(490, 400));
    m_pTips->setColor(ccBLACK);
    CCNode *node = m_cMaps->getElementByTags(array, 2);
    node->addChild(m_pBg, 10);
    node->addChild(m_pTips, 10);
    m_pGoBtn = (CCSprite*) (m_cMaps->getElementByTags(array, 3));
}



CCNode * CBiforestLayer::createItemView(const char* const inTitle)
{
    CCSprite *sprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "chapter_bg.png"));
    CCLabelTTF *title = CCLabelTTF::create(inTitle, "arial", 15);
    title->setPosition(ccp(119,16));
    sprite->addChild(title,0,1);
    return sprite;
}


void CBiforestLayer::updatePanel(int inChapterIndex, int inSectionIndex)
{
    CCArray *chapters = getChaptersByType(m_eExploartionType);
    CCAssert(inChapterIndex < chapters->count(), "chapter two more");
    if (inSectionIndex == -1)
    {
        CPtChapter * chapter = (CPtChapter*)chapters->objectAtIndex(inChapterIndex);
        m_sPicName =  chapter->getChapterPicName();
        m_sTips = Utility::getWordWithFile("dictionary.plist",chapter->getChapterTipid().c_str());
        m_bGoneVisiable = false;

    }
    else
    {
        CPtSection* section  = NULL;
        if (m_eExploartionType == ACTIVITYEXPLORATION)
        {
            section =(CPtSection*) getActivitySections()->objectAtIndex(inSectionIndex);
        }else
        {
          section  =  (CPtSection*) ((CPtChapter*)(chapters->objectAtIndex(inChapterIndex)))->getSections()->getSectionByIndex(inSectionIndex);
        }
        
        m_sPicName =  section->getSectionPicName();
        m_sTips = Utility::getWordWithFile("dictionary.plist",section->getTipId().c_str());
        m_bGoneVisiable = true;
        m_nCurrentSectionId = section->getSectionId();
        
    }
    
    updateRightPanelUI();
    
}



void CBiforestLayer::updateRightPanelUI()
{
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_chapterPath, m_sPicName.c_str()));
    m_pBg->setTexture(texture);
    m_pTips->setString(m_sTips.c_str());
    m_pGoBtn->setVisible(m_bGoneVisiable);
}




void CBiforestLayer::handlerTouch()
{
    CCLog("CBiforestLayer: %d", m_nTouchTag);
    switch (m_nTouchTag)
    {
        case 2004:
            removeFromParentAndCleanup(true);
            break;
        case 2005:
            //如果是场景切换 请调用scenemanage
            if (m_pGoBtn && m_pGoBtn->isVisible())
            {
                onClickGoSection();
            }

            break;
        case 2006:
            if (m_eExploartionType == NORMALEXPLORATION)
            {
                switchExplorationType(ACTIVITYEXPLORATION);
            }
            else
            {
                switchExplorationType(NORMALEXPLORATION);
            }
            break;
        default:
            break;
    }
    
}



// server communication:

bool CBiforestLayer::canGoSection()
{
    bool bRet = false;
    do
    {
        if (m_nCurrentChaptetIndex == -1 || m_nCurrentSectionId == -1)
        {
            break;
        }
        if (m_eExploartionType == ACTIVITYEXPLORATION && m_pActivitySections)
        {
           
            if (m_nCurrentSectionIndex < m_pActivitySections->count())
            {
                CActivityEncounter *encounter = (CActivityEncounter*) m_pActivityEncounterInfo->objectAtIndex(m_nCurrentSectionIndex);
                if (encounter && encounter->haveTime()==false)
                {
                  
                    CCMessageBox(CCString::createWithFormat("活动：%d :近日的次数已经用完了", encounter->getSectionInfo()->getSectionId())->getCString(), "活动副本");
                    break;
                }
            }
        }
        bRet = true;
    } while (0);
    return bRet;
}
void CBiforestLayer::onClickGoSection()
{
    // constructor post data:
    //api.php?m=Part&a=getPartEvent&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&chapter_id=1(章)&part_id=1(节)
    if (m_eExploartionType == NORMALEXPLORATION)
    {
        m_nCurrentChapterId = m_nCurrentChaptetIndex*INTERVALVALUE + BASEVALUE;
    }else
    {
        m_nCurrentChapterId = ACTIVITYCHAPTERID;
    }

    // first detected can go:
    if (!canGoSection())
    {
        CCLog("error:can go section:");
        return;
    }
    
    m_bSendRequest = true;
    CCLog("the chapter id : %d, the section Id: %d", m_nCurrentChapterId, m_nCurrentSectionId);
    char buffer[200]={0};
    sprintf(buffer, "sig=%s&chapter_id=%d&part_id=%d",STR_USER_SIG, m_nCurrentChapterId, m_nCurrentSectionId);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GOSECTION(196),"goSection", "goSection",buffer, callfuncO_selector(CBiforestLayer::onReceiveGoSectionMsg));
    
}
void CBiforestLayer::onReceiveGoSectionMsg(CCObject *pObject)
{
    char * buffer = (char *)pObject;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "goSection");
    if(buffer)
    {
         CCDictionary * tmp = PtJsonUtility::JsonStringParse(buffer);
         delete []buffer;
        if (tmp)
        {
            onParseGoSectionMsgByDictionary(tmp);
        }
    }else
    {
        CCLog("error: connect server");
    }
    m_bSendRequest = false;
   
}
void CBiforestLayer::onParseGoSectionMsgByDictionary(CCDictionary * inDataDictionary)
{
    int result = GameTools::intForKey("code", inDataDictionary);
    if(result == 0)
    {
         // init:
         CCDictionary *tmp = (CCDictionary*)inDataDictionary->objectForKey("result");
         if (tmp)
         {
             CExploration::setExplorationType(m_eExploartionType);
             CExploration::setCurrentStep(GameTools::intForKey("step", tmp));
             EVENTDATA eventData =  dispatchEventWithType((CCDictionary*)tmp->objectForKey("event_info"));
             CExploration::setEvents(eventData);
             CPtSection* tempPtSection= getChapterByIndex(m_nCurrentChaptetIndex)->getSections()->getSectionById(m_nCurrentSectionId);
             CExploration::setExplorationInfo(tempPtSection);
             CPlayerBufferManager::getInstance()->clearAllAltarBufferes();
             SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_EXPLORATIONSCENE);
         }
         
        // success:
    }else
    {
        CCLog("error:CBiforestLayer::onReceiveGoSectionMsg error code: %d ", result);
    }
}

/*
 * @param inType: 0--> commnon event, 1---> special event:
 */

EVENTDATA CBiforestLayer:: dispatchEventWithType(CCDictionary *inDict)
{
    EVENTDATA eventData;
    int type = 0;
    if (inDict)
    {
        CCDictionary * tmpValue = (CCDictionary*) inDict->objectForKey("special_id");
        if (tmpValue)
        {
            type = 1;
            eventData.eventId[0] = GameTools::intForKey("left", tmpValue);
            eventData.eventId[1] = GameTools::intForKey("mid", tmpValue);
            eventData.eventId[2] = GameTools::intForKey("right", tmpValue);
            eventData.storyId =GameTools::intForKey("story_id", inDict);
            eventData.specialEventId = GameTools::intForKey("special_event_id", inDict);

        }else
        {
            CCArray *array =  (CCArray*) inDict->objectForKey("event_id");
            if (array)
            {
                for (int i = 0; i < array->count(); i++)
                {
                    eventData.eventId[i] = ((CCString*) array->objectAtIndex(i))->intValue();
                }
            }
        }
        eventData.type = type;
    }
    
    return  eventData;
}

// load image resource:
void CBiforestLayer::loadResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "tansuo.plist"), CSTR_FILEPTAH(g_mapImagesPath, "tansuo.png"));
}


// test:

void CBiforestLayer::setNormal(CCObject *pObject)
{
    CPtExpandableListViewItem *item = (CPtExpandableListViewItem*) pObject;
    CCSprite *sprite = NULL;
    if (item)
    {
        sprite = (CCSprite*) item->getDisplayView();
        if (item->getItemType() == PARENT_TYPE)
        {
            sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tansuo2.png"));
        }else if(item->getItemType() == CHILDRENT_TYPE)
        {
            ((CCLabelTTF*) sprite->getChildByTag(1))->setColor(ccc3(252, 245, 191));
            sprite->getChildByTag(2)->setVisible(false);
        }
    }
}
void CBiforestLayer::setSelected(CCObject *pObject)
{
    CPtExpandableListViewItem *item = (CPtExpandableListViewItem*) pObject;
    CCSprite *sprite = NULL;
    if (item)
    {
        sprite = (CCSprite*) item->getDisplayView();
        if (item->getItemType() == PARENT_TYPE)
        {
            sprite->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tansuo1.png"));
        }else if(item->getItemType() == CHILDRENT_TYPE)
        {
            ((CCLabelTTF*) sprite->getChildByTag(1))->setColor(ccc3(35, 255, 1));
            sprite->getChildByTag(2)->setVisible(true);
        }
    }
    
}
void CBiforestLayer::getChildren(CCObject* pObject)
{
    CPtExpandableListViewItem* items = (CPtExpandableListViewItem*) pObject;
    CCArray *sectionInfo = NULL;
    if (items && items->getItemType() == PARENT_TYPE)
    {
        int chapterIndex = items->getIndex();
        
        if(chapterIndex >= 0 && chapterIndex < getChaptersByType(m_eExploartionType, m_nMaxChapterId)->count())
        {
           sectionInfo = getSections(chapterIndex);
            
            CCArray * sections = CCArray::create();
            CPtSection * section = NULL;
            
            for (int i = 0; i <sectionInfo->count(); i++)
            {
                CPtExpandableListViewItem* item = CPtExpandableListViewItem::create();
                section = (CPtSection *) sectionInfo->objectAtIndex(i);
                if (section)
                {
                    item->setDisplayView(createItemViewByType(section->getSectionName().c_str(), CHILDRENT_TYPE));
                    item->setHandler(this, callfuncO_selector(CBiforestLayer::setNormal), callfuncO_selector(CBiforestLayer::setSelected), NULL);
                    item->setIndex(i);
                    item->setItemType(CHILDRENT_TYPE);
                }
                sections->addObject(item);
            }
            
            items->setChildrenItem(sections);
            
        }
    }
}

CCNode* CBiforestLayer::createItemViewByType(const char* inTitle,ITEM_TYPE inType)
{
    CCSprite *sprite = NULL;
    CCLabelTTF *title = CCLabelTTF::create(inTitle, "arial", 15);
  
    CCSize size ;
    if (inType == PARENT_TYPE)
    {
       sprite = CCSprite::createWithSpriteFrameName("tansuo2.png");
       size = sprite->getContentSize();
       
    }else if(inType == CHILDRENT_TYPE)
    {
        sprite = CCSprite::createWithSpriteFrameName("tansuo3.png");
        title->setColor(ccc3(252, 245, 191));
        CCSprite* icon = CCSprite::createWithSpriteFrameName("tansuo7.png");
        size = sprite->getContentSize();
        sprite->addChild(icon, 0,2);
        sprite->setPosition(ccp(120, size.height/2));
        icon->setAnchorPoint(CCPointZero);
        icon->setVisible(false);
    }
    else
    {
        return sprite;
    }
    
    title->setPosition(ccp(size.width/2,size.height/2));
    sprite->addChild(title,0,1);
    CCLog("the sprite: size: %f, %f", sprite->boundingBox().size.width, sprite->boundingBox().size.height);
    
    return sprite;
 
}


void CBiforestLayer::switchExplorationType(EXPLORATIONTYPE inType)
{
    if (!m_bStartActivity)
    {
        goToAcitivityEncounter();
        return;
    }
    m_eExploartionType = inType;
    if (m_eExploartionType == NORMALEXPLORATION)
    {
        initNormalPanel();
    }else
    {
        initActivityPanel();
    }
    CPtExpandableListViewItemList * expandList = CPtExpandableListViewItemList::create();
    expandList->setListViewItems(getDispLayerData());
    expandList->setWhichOneOpen(m_nCurrentChaptetIndex);
    
    bool first = m_pListView == NULL;
    if (first)
    {
        m_pListView = CPtExpandableListView::create(expandList, CCSizeMake(246, 515),  CCSizeMake(0.0f, 0.0f));
        m_pListView->getTabelView()->setDelegate(this);
        m_pListView->getTabelView()->setTouchPriority(-300);
        m_pListView->setPosition(ccp(220, 135));
        m_pListView->getTabelView()->setTouchPriority(-200);
        m_cMaps->getElementByTags("2,0")->addChild(m_pListView, 10);
    }
    CCSize size1 =  m_pListView->getTabelView()->getContentSize();
    m_pListView->setItems(expandList);
    m_pListView->getTabelView()->reloadData();
    CCSize size2 =  m_pListView->getTabelView()->getContentSize();
    CCPoint offset = m_pListView->getTabelView()->getContentOffset();
    offset.y -= (size2.height- size1.height);
    m_pListView->getTabelView()->setContentOffset(offset);
    if (first)
    {
        initRightPanel(m_nCurrentChaptetIndex);
       
    }
    updatePanel(m_nCurrentChaptetIndex, m_nCurrentSectionIndex);

}
CCArray * CBiforestLayer::getChaptersByType(EXPLORATIONTYPE inType, int inMaxChapterId /*=0*/)
{
    if (inType == NORMALEXPLORATION)
    {
        return getNormalChapters(inMaxChapterId);
    }else
    {
        return getActivityChapters();
    }
}


/*
 *  @breif : 获得当前开启得所有章
 */

CCArray* CBiforestLayer::getNormalChapters(int inMaxChapterId)
{
    if (m_pNormalChapters == NULL)
    {
        CPtChapterConfigData * chapters =  SingleCPtChapters::instance();
        m_pNormalChapters = chapters->getOpenChapter(inMaxChapterId);
        m_pNormalChapters->retain();
    }
    return m_pNormalChapters;
}

CCArray* CBiforestLayer::getActivityChapters()
{
    if (m_pActivityChapters == NULL)
    {
        m_pActivityChapters =  CCArray::create();
        m_pActivityChapters->retain();
        CPtChapterConfigData * chapters =  SingleCPtChapters::instance();
        m_pActivityChapters->addObject(chapters->getAcitivityChapter());
    }
    return m_pActivityChapters;
}

CCArray * CBiforestLayer::getSections(int inSelectedChapterIndex, int inMaxSectionId)
{
    if (m_eExploartionType == NORMALEXPLORATION)
    {
        return getNormalSections(inSelectedChapterIndex, inMaxSectionId);
    }
    else
    {
        return getActivitySections();
    }
    return NULL;
}

/*
 * @breif:获取所选章的节
 */
CCArray* CBiforestLayer::getNormalSections(int inSelectedChapterIndex, int inMaxSectionId)
{
    if (inMaxSectionId == -1)
    {
        CC_SAFE_RELEASE(m_pNormalSections);
        m_pNormalSections = ((CPtChapter*)m_pNormalChapters->objectAtIndex(inSelectedChapterIndex))->getSections()->getSectionByOrder();
        m_nCurrentChaptetIndex = inSelectedChapterIndex;
        m_pNormalSections->retain();
        
    }else
    {
        if (inSelectedChapterIndex >= 0 && inSelectedChapterIndex <= m_pNormalChapters->count())
        {
            CC_SAFE_RELEASE(m_pNormalSections) ;
            m_pNormalSections = ((CPtChapter*)m_pNormalChapters->objectAtIndex(inSelectedChapterIndex))->getSections()->getSectionsBeforeId(inMaxSectionId);
            m_pNormalSections->retain();
            m_nCurrentChaptetIndex = inSelectedChapterIndex;
        }
    }
    
   
    return m_pNormalSections;
}

CCArray* CBiforestLayer::getActivitySections()
{

    if (m_pActivitySections == NULL)
    {
        m_pActivitySections = CCArray::create();
        if (m_pActivityEncounterInfo)
        {
            CCObject *pObject = NULL;
            CCARRAY_FOREACH(m_pActivityEncounterInfo, pObject)
            {
                m_pActivitySections->addObject(((CActivityEncounter*)pObject)->getSectionInfo());
            }
        }
       // m_pActivitySections = ((CPtChapter*)m_pActivityChapters->objectAtIndex(0))->getSections()->getSectionByOrder();
        m_nCurrentChaptetIndex = 0;
        m_pActivitySections->retain();
    }
    return m_pActivitySections;
    
}

CCArray * CBiforestLayer::getChapterItem()
{
    CCArray * array = CCArray::create();
    CPtChapter * chapter = NULL;
    CPtTableItem * item = NULL;
    CCArray *chapters = getChaptersByType(m_eExploartionType);
    for (int i = 0; i <chapters->count(); i++)
    {
        item = CPtTableItem::create();
        chapter = (CPtChapter *) chapters->objectAtIndex(i);
        if (chapter)
        {
            item->setDisplayView(createItemView(chapter->getChapterName().c_str()));
        }
        array->addObject(item);
    }
    return array;
}

CCArray * CBiforestLayer::getSectionItem(CCArray *inSectionInfo)
{
    if (inSectionInfo == NULL)
    {
        return NULL;
    }
    CCArray* sections = CCArray::create();
    CPtSection * section = NULL;
    bool selected = false;
    for (int i = 0; i <inSectionInfo->count(); i++)
    {
        CPtExpandableListViewItem*  item = CPtExpandableListViewItem::create();
        section = (CPtSection *) inSectionInfo->objectAtIndex(i);
        if (section)
        {
            item->setDisplayView(createItemViewByType(section->getSectionName().c_str(), CHILDRENT_TYPE));
            item->setHandler(this, callfuncO_selector(CBiforestLayer::setNormal), callfuncO_selector(CBiforestLayer::setSelected), NULL);
            item->setIndex(i);
            item->setItemType(CHILDRENT_TYPE);
            if (!selected)
            {
                if(m_nCurrentSectionId != -1 && section->getSectionId() == m_nCurrentSectionId)
                {
                    item->setSelected();
                    m_nCurrentSectionIndex = i;
                    selected = true;
                    sections->addObject(item);
                    continue;
                }
                if (i == inSectionInfo->count()-1)
                {
                    item->setSelected();
                    m_nCurrentSectionIndex = i;
                    selected = true;
                }

                
               
            }
            
        }
        sections->addObject(item);
    }
    return sections;
    
}

CPtChapter * CBiforestLayer::getChapterByIndex(int inIndex)
{
    if(m_eExploartionType == NORMALEXPLORATION)
    {
        return (CPtChapter*)m_pNormalChapters->objectAtIndex(inIndex);
    }else
    {
        return (CPtChapter*)m_pActivityChapters->objectAtIndex(inIndex);
    }
}


// connect with server:

void CBiforestLayer::onSendRequestGetActivity()
{
    m_bSendRequest = true;
    char buffer[150] = {0};
    sprintf(buffer, "&sig=%s", STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_GETACTIVITY(196),  "CALLBACK_CBiforestLayer::onSendRequestGetActivity","REQUEST_CBiforestLayer::onSendRequestGetActivity",buffer, callfuncO_selector(CBiforestLayer::onReceiveMsgGetActivity));

}

void CBiforestLayer::onReceiveMsgGetActivity(CCObject* pObject)
{
    m_bStartActivity = true;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CBiforestLayer::onSendRequestGetActivity");
    CC_SAFE_RELEASE_NULL(m_pActivityEncounterInfo);
    char *buffer = (char*) pObject;
    if (buffer)
    {
        CCLog("CBiforestLayer::onReceiveMsgGetActivity: %s",buffer);
        CCDictionary * resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        if (resultDict)
        {
            int code = GameTools::intForKey("code", resultDict);
            if (code == 0)
            {
                resultDict = (CCDictionary*)resultDict->objectForKey("result");
                if (resultDict)
                {
                   m_pActivityEncounterInfo = m_pActivityEncounterManager->getCurrentActivityByDict((CCDictionary*)resultDict->objectForKey("activity_part"));
                   m_pActivityEncounterInfo->retain();
                   switchExplorationType(ACTIVITYEXPLORATION);
                }
            }
        }
    }
    m_bSendRequest = false;

}


void CBiforestLayer::goToAcitivityEncounter()
{
    onSendRequestGetActivity();
}

