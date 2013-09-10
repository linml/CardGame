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
    m_nMaxSectionId = SinglePlayer::instance()->getMaxSectionid();
    selectNode = NULL;
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
    m_nSelectIndex = cell->getIdx();
    CCLog("touch which one: %d", cell->getIdx());
    CCArray *array = cell->getChildren();
    CCSprite * tmp = (CCSprite*) array->objectAtIndex(0);
    if (tmp == selectNode)
    {
    }else
    {
        if (selectNode!= NULL)
        {
            ((CCLabelTTF*) selectNode->getChildByTag(1))->setColor(ccWHITE);
            
        }
        
         selectNode = tmp;
        ((CCLabelTTF*) selectNode->getChildByTag(1))->setColor(ccRED);
        
    }
    updatePanel(cell->getIdx(), m_bChapterMode);
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
    setTouchPriority(GOLDPLACE_TOUCH_PRORITY-1);
    
    
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
    
    initLeftPanel(m_nMaxChapterId);
    initRightPanel(m_nMaxChapterId-1);
    // add image and tip to right panel:
  
    m_pBackBtn = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "back_normal.png"));
    CCNode * node = m_cMaps->getElementByTags(array, 2);
    node->addChild(m_pBackBtn, 100, 2008);
    m_pBackBtn->setPosition(ccp(200, 650));
    Utility::addTouchRect(2008, m_pBackBtn, m_cTouches);

    
}

void CBiforestLayer::initLeftPanel(int inMaxChapterId)
{
    getChapters(inMaxChapterId);
    if (m_pChapters)
    {
        CCArray *items = getChapterItem();
        setSelect(((CPtTableItem*)items->objectAtIndex(m_nMaxChapterId-1))->getDisplayView());
        m_pLeftPanel = CPtListViewWidget::create(items, CCSizeMake(246, 515), kCCScrollViewDirectionVertical, CCSizeMake(5.0, 8.0));
        m_pLeftPanel->setItemHandlerEnable(false);
        m_pLeftPanel->setTableItemDelegate(this);
        m_pLeftPanel->setPosition(ccp(220, 135));
        m_pLeftPanel->getTableView()->setTouchPriority(-200);
        m_cMaps->getElementByTags("2,0")->addChild(m_pLeftPanel, 10);
    }else
    {
        CCLog("assert: error!");
    }

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
    CCArray * array = CCArray::create();
    CPtSection * section = NULL;
    CPtTableItem * item = NULL;
    
    CPtChapter *chapter = (CPtChapter*) m_pChapters->objectAtIndex(m_nCurrentChapterId);
    
    if (chapter)
    {
        item = CPtTableItem::create();
        item->setDisplayView(createItemView(chapter->getChapterName().c_str()));
        array->addObject(item);
        
    }
    
    for (int i = 0; i <m_pSections->count(); i++)
    {
        item = CPtTableItem::create();
        section = (CPtSection *) m_pSections->objectAtIndex(i);
        if (section)
        {
            item->setDisplayView(createItemView(section->getSectionName().c_str()));
        }
        array->addObject(item);
    }
    
    // add chapter:
 
    
    return array;
    
}

CCNode * CBiforestLayer::createItemView(const char* const inTitle)
{
    CCSprite *sprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "chapter_bg.png"));
    CCLabelTTF *title = CCLabelTTF::create(inTitle, "arial", 15);
    title->setPosition(ccp(119,16));
    sprite->addChild(title,0,1);
    return sprite;
}

void CBiforestLayer::updatePanel(int inCurrentId, bool inChapterEnable)
{
    updateLeftPanel(inCurrentId, inChapterEnable);
    
    updateRightPanel(inCurrentId, m_bChapterMode);
 
    updateLeftPanelUI();
    updateRightPanelUI();
}

/*
 * @param inCurrentId start from 0
 */
void CBiforestLayer::updateRightPanel(int inCurrentId, bool inChapterEnable)
{        // update chapter:
       if(inChapterEnable)
       {
           CPtChapter * chapter = (CPtChapter*)m_pChapters->objectAtIndex(inCurrentId);
           m_sPicName =  chapter->getChapterPicName();
           m_sTips = Utility::getWordWithFile("dictionary.plist",chapter->getChapterTipid().c_str());
           m_bGoneVisiable = false;
       }else
       {
           if (inCurrentId == 0)
           {
               if (inCurrentId == 0)
               {
                   CPtChapter * chapter = (CPtChapter*)m_pChapters->objectAtIndex(inCurrentId);
                   m_sPicName =  chapter->getChapterPicName();
                   m_sTips = Utility::getWordWithFile("dictionary.plist", chapter->getChapterTipid().c_str());
                   m_bGoneVisiable = false;
                   
                   return;
               }
               return;
           }
           
           CPtSection *section = NULL;
           if (inCurrentId == -1)
           {
               section = (CPtSection*)m_pSections->lastObject();
           }else
           {
               section = (CPtSection*)m_pSections->objectAtIndex(inCurrentId-1);
           }
           m_nCurrentSectionId = section->getSectionId();
           m_nCurrentChapterId = section->getChapterId();
           
           m_sPicName =  section->getSectionPicName();
           m_sTips = Utility::getWordWithFile("dictionary.plist",section->getTipId().c_str());
           m_bGoneVisiable = true;
       }
}

void CBiforestLayer::updateLeftPanel(int inCurrentId, bool inChapterEnable)
{
    if(inChapterEnable)
    {
        getSections(inCurrentId);
   
    }

}

void CBiforestLayer::updateRightPanelUI()
{
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_chapterPath, m_sPicName.c_str()));
    m_pBg->setTexture(texture);
    m_pTips->setString(m_sTips.c_str());
    m_pGoBtn->setVisible(m_bGoneVisiable);
}

void CBiforestLayer::updateLeftPanelUI()
{
    if (m_bChapterMode)
    {
        CCArray * items = getSectionItem();
        setSelect(((CPtTableItem*)items->objectAtIndex(0))->getDisplayView());
        m_nCurrentSectionId = -1;
        m_pLeftPanel->setItems(items);
        m_pLeftPanel->reload();
        m_bChapterMode = false;
        
    }
    
}

void CBiforestLayer::getChapters(int inMaxChapterId)
{
     CPtChapterConfigData * chapters =  SingleCPtChapters::instance();
     CC_SAFE_RELEASE(m_pChapters);
     m_pChapters = chapters->getOpenChapter(inMaxChapterId);
     m_pChapters->retain();
    
}

void CBiforestLayer::getSections(int inSelectedChapterId, int inMaxSectionId)
{
    if (inMaxSectionId == -1)
    {
        CC_SAFE_RELEASE(m_pSections);
        m_pSections = ((CPtChapter*)m_pChapters->objectAtIndex(inSelectedChapterId))->getSections()->getSectionByOrder();
        m_nCurrentChapterId = inSelectedChapterId;
        m_pSections->retain();
        return;
    }
    
    if (inSelectedChapterId > 0 && inSelectedChapterId <= m_pChapters->count())
    {
        CC_SAFE_RELEASE(m_pSections);
        m_pSections = ((CPtChapter*)m_pChapters->objectAtIndex(inSelectedChapterId))->getSections()->getSectionsBeforeId(inMaxSectionId);
        m_pSections->retain();
        m_nCurrentChapterId = inSelectedChapterId;
    }
    
}

void CBiforestLayer::backChapter()
{
    if (m_bChapterMode)
    {
        return;
    }
    CCArray *items = getChapterItem();
    setSelect(((CPtTableItem*)items->objectAtIndex(m_nMaxChapterId-1))->getDisplayView());
    m_pLeftPanel->setItems(items);
    m_pLeftPanel->reload();
    m_bChapterMode = true;
    updateRightPanel(m_nMaxChapterId-1, m_bChapterMode);
    updateRightPanelUI();
    selectNode = NULL;
   
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
        case 2008:
            backChapter();
            break;
            
        default:
            break;
    }
    
}

void CBiforestLayer::setSelect(CCNode *node)
{
    ((CCLabelTTF*)node->getChildByTag(1))->setColor(ccRED);
    selectNode =(CCSprite*) node;
}

// server communication:

bool CBiforestLayer::canGoSection()
{
    bool bRet = false;
    do
    {
        if (m_nCurrentChapterId == -1 || m_nCurrentSectionId == -1)
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
         CExploration::s_SectionData.currentStep = GameTools::intForKey("step", tmp);
         CExploration::s_SectionData.sectionInfo = (CPtSection*) m_pSections->objectAtIndex(m_nSelectIndex-1);
         CExploration::s_SectionData.eventId[0] = EMPTY_EVENT;
         CExploration::s_SectionData.eventId[1] = EMPTY_EVENT;
         CExploration::s_SectionData.eventId[2] = EMPTY_EVENT;
         SingleSceneManager::instance()->runSceneSelect(EN_CURRSCENE_EXPLORATIONSCENE);
        // success:
    }else
    {
        CCLog("error:CBiforestLayer::onReceiveGoSectionMsg error code: %d ", result);
    }
}


// load image resource:
void CBiforestLayer::loadResource()
{
   // CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "tansuo.plist"));
}
