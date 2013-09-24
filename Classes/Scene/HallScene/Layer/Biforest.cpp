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

CBiforestLayer::CBiforestLayer()
{
    m_nCurrentChapterId = -1;
    m_nCurrentChaptetIndex = -1;
    m_cMaps = NULL;
    m_pTouchSprite = NULL;
    m_pChapters = NULL;
    m_pSections = NULL;
    m_pTips = NULL;
    m_pBg = NULL;
    m_pGoBtn = NULL;
    m_pLeftPanel = NULL;
    
    m_sPicName = "a.png";
    m_sTips = "";
    m_bGoneVisiable = false;
    m_bChapterMode = true;
    
    m_nMaxChapterId = SinglePlayer::instance()->getMaxChapterId();
    CCLog("m_nMaxChapterId==%d",m_nMaxChapterId);
    m_nMaxSectionId = SinglePlayer::instance()->getMaxSectionid();
    selectNode = NULL;
    m_pListView = NULL;
}

CBiforestLayer::~CBiforestLayer()
{
    CC_SAFE_RELEASE(m_cMaps);
    CC_SAFE_RELEASE(m_pChapters);
    CC_SAFE_RELEASE(m_pSections);
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
    if(m_nTouchTag != -1)
    {
        return true;
    }
    return  true;
}

void CBiforestLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CBiforestLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
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
    
    initPanel();
    
}




void CBiforestLayer::initRightPanel(int inMaxChapterId)
{
    int array[]={2,0,7};
    
    CPtChapter * chapter = (CPtChapter*)m_pChapters->objectAtIndex(inMaxChapterId);
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


CCArray * CBiforestLayer::getChapterItem()
{
    CCArray * array = CCArray::create();
    CPtChapter * chapter = NULL;
    CPtTableItem * item = NULL;

    for (int i = 0; i <m_pChapters->count(); i++)
    {
        item = CPtTableItem::create();
        chapter = (CPtChapter *) m_pChapters->objectAtIndex(i);
        if (chapter)
        {
              item->setDisplayView(createItemView(chapter->getChapterName().c_str()));
        }
        array->addObject(item);
    }
    return array;
}

CCArray * CBiforestLayer::getSectionItem()
{
    CCArray* sections = CCArray::create();
    CPtSection * section = NULL;
    
    for (int i = 0; i <m_pSections->count(); i++)
    {
       CPtExpandableListViewItem*  item = CPtExpandableListViewItem::create();
        section = (CPtSection *) m_pSections->objectAtIndex(i);
        if (section)
        {
            item->setDisplayView(createItemViewByType(section->getSectionName().c_str(), CHILDRENT_TYPE));
            item->setHandler(this, callfuncO_selector(CBiforestLayer::setNormal), callfuncO_selector(CBiforestLayer::setSelected), NULL);
            item->setIndex(i);
            item->setItemType(CHILDRENT_TYPE);
            if (i == m_pSections->count()-1)
            {
                item->setSelected();
                m_nCurrentSectionIndex = i;
            }
        }
        sections->addObject(item);
    }
 
    
    return sections;
    
}

CCNode * CBiforestLayer::createItemView(const char* const inTitle)
{
    CCSprite *sprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "chapter_bg.png"));
    CCLabelTTF *title = CCLabelTTF::create(inTitle, "arial", 15);
    title->setPosition(ccp(119,16));
    sprite->addChild(title,0,1);
    return sprite;
}


void CBiforestLayer::updatePanel(int inChapterIndex, int inSectionId)
{
    if (inSectionId == -1)
    {
        CPtChapter * chapter = (CPtChapter*)m_pChapters->objectAtIndex(inChapterIndex);
        m_sPicName =  chapter->getChapterPicName();
        m_sTips = Utility::getWordWithFile("dictionary.plist",chapter->getChapterTipid().c_str());
        m_bGoneVisiable = false;

    }
    else
    {
        CPtSection* section =  (CPtSection*) ((CPtChapter*)(m_pChapters->objectAtIndex(inChapterIndex)))->getSections()->getSectionByIndex(inSectionId);
        
        m_sPicName =  section->getSectionPicName();
        m_sTips = Utility::getWordWithFile("dictionary.plist",section->getTipId().c_str());
        m_bGoneVisiable = true;
        m_nCurrentSectionId =  section->getSectionId();
        
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



void CBiforestLayer::getChapters(int inMaxChapterId)
{
     CPtChapterConfigData * chapters =  SingleCPtChapters::instance();
     CC_SAFE_RELEASE(m_pChapters);
     m_pChapters = chapters->getOpenChapter(inMaxChapterId);
     m_pChapters->retain();
    
}

void CBiforestLayer::getSections(int inSelectedChapterIndex, int inMaxSectionId)
{
    if (inMaxSectionId == -1)
    {
        CC_SAFE_RELEASE(m_pSections);
        m_pSections = ((CPtChapter*)m_pChapters->objectAtIndex(inSelectedChapterIndex))->getSections()->getSectionByOrder();
        m_nCurrentChaptetIndex = inSelectedChapterIndex;
        m_pSections->retain();
        return;
    }
    
    if (inSelectedChapterIndex >= 0 && inSelectedChapterIndex <= m_pChapters->count())
    {
        CC_SAFE_RELEASE(m_pSections);
        m_pSections = ((CPtChapter*)m_pChapters->objectAtIndex(inSelectedChapterIndex))->getSections()->getSectionsBeforeId(inMaxSectionId);
        m_pSections->retain();
        m_nCurrentChaptetIndex = inSelectedChapterIndex;
    }
    
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
            onClickGoSection();
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
        bRet = true;
    } while (0);
    return bRet;
}
void CBiforestLayer::onClickGoSection()
{
    // constructor post data:
    //api.php?m=Part&a=getPartEvent&uid=194(用户ID)&sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&chapter_id=1(章)&part_id=1(节)
    
    m_nCurrentChapterId = m_nCurrentChaptetIndex*INTERVALVALUE + BASEVALUE;
    // first detected can go:
    if (!canGoSection())
    {
        CCLog("error:can go section:");
    }
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
   
}
void CBiforestLayer::onParseGoSectionMsgByDictionary(CCDictionary * inDataDictionary)
{
    int result = GameTools::intForKey("code", inDataDictionary);
    if(result == 0)
    {
         // init:
         CCDictionary *tmp = (CCDictionary*)inDataDictionary->objectForKey("result");
         CExploration::setCurrentStep(GameTools::intForKey("step", tmp));
         EVENTDATA eventData =  dispatchEventWithType((CCDictionary*)tmp->objectForKey("event_info"));
         CExploration::setEvents(eventData);
         CExploration::setExplorationInfo((CPtSection*) ((CPtChapter*)m_pChapters->objectAtIndex(m_nCurrentChaptetIndex))->getSections()->getSectionById(m_nCurrentSectionId));
         SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_EXPLORATIONSCENE);
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
void CBiforestLayer::initPanel()
{
    m_nCurrentChapterId = m_nMaxChapterId;
    m_nCurrentChaptetIndex = (m_nMaxChapterId - BASEVALUE )/INTERVALVALUE;
    m_nCurrentSectionId = m_nMaxSectionId;;
    getChapters(m_nMaxChapterId);
    getSections(m_nCurrentChaptetIndex, m_nMaxSectionId);
    // get items:
    CCArray * array = CCArray::create();
    CPtChapter * chapter = NULL;
    CPtExpandableListViewItem * item = NULL;
    
    CCArray * sections = getSectionItem();

    for (int i = 0; i <m_pChapters->count(); i++)
    {
        item = CPtExpandableListViewItem::create();
        chapter = (CPtChapter *) m_pChapters->objectAtIndex(i);
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
    
     CPtExpandableListViewItemList * expandList = CPtExpandableListViewItemList::create();
     expandList->setListViewItems(array);
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
    if (items && items->getItemType() == PARENT_TYPE)
    {
        int chapterId = items->getIndex();
        if(chapterId >= 0 && chapterId < m_pChapters->count())
        {
            getSections(chapterId);
            
            CCArray * sections = CCArray::create();
            CPtSection * section = NULL;
            
            for (int i = 0; i <m_pSections->count(); i++)
            {
               CPtExpandableListViewItem* item = CPtExpandableListViewItem::create();
                section = (CPtSection *) m_pSections->objectAtIndex(i);
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
