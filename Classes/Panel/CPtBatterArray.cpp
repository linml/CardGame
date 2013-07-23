#include "CPtBatterArray.h"
#include "CPtTool.h"
#include "gameConfig.h"
#include "CGamesCard.h"
#include <cmath>
#include "CCard.h"
using namespace CPtTool;

// implement CPtBattleArray:
CPtBattleArray* CPtBattleArray::create(int Tag)
{
	CPtBattleArray * batterarray = new CPtBattleArray();
	if (batterarray)
	{
        batterarray->inTag = Tag;
        batterarray->initMap();
        batterarray->setTouchEnabled(true);
        batterarray->setTouchMode(kCCTouchesOneByOne);
		batterarray->autorelease();
	}
	return batterarray;
}

CPtBattleArray::CPtBattleArray()
{
	initData();
}

CPtBattleArray::~CPtBattleArray()
{
}

void CPtBattleArray::initSize(const cocos2d::CCSize &size, const cocos2d::CCPoint &point)
{
    float width = size.width;
    m_cPositions[0] = point;
    for (int i = 1; i < CARDCOUNT-1; i++)
    {
        m_cPositions[i] = ccp(m_cPositions[i-1].x+width/2, m_cPositions[i-1].y);
    }
    m_cPositions[4] = ccp(25, 150);
    
}

void CPtBattleArray::createSuitLogo(const int& inSuit, const int &inSequence, const int &inPositionIndex)
{
    char buff[10] ={0};
    sprintf(buff, "suit%d.png", inSuit);
    CCSprite *suitSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, buff));
    
//    CCLabelTTF* label = NULL;
//    if (inSuit >1 && inSuit < 11)
//    {
//        memset(buff, 0, sizeof(buff));
//        sprintf(buff, "%d", inSuit);
//        label=CCLabelTTF::create(buff, "Arial", 6 );
//    }else
//    {
//        char c[2]={'\n'};
//        switch (inSuit)
//        {
//            case 1:
//                c[0]='A';
//                break;
//            case 11:
//                c[0]='J';
//                break;
//            case 12:
//                c[0]='Q';
//                break;
//            case 13:
//                c[0]='K';
//                break;
//            default:
//                break;
//        }
//        label=CCLabelTTF::create(c, "Arial", 6);
//    }
    //label->setAnchorPoint(CCPointZero);
    //label->setPosition(ccp(6,6));
    //suitSprite->addChild(label);
    suitSprite->setAnchorPoint(CCPointZero);
    suitSprite->setPosition(ccp(5,5));
    m_pSuitLogo[inPositionIndex]->cocos2d::CCNode::removeAllChildrenWithCleanup(true);
    m_pSuitLogo[inPositionIndex]->addChild(suitSprite,300);
}

/*
 * @function: bool addCard(CCObject *inCard, const int & inCardType)
 * @brief: add or replace card to the batterarray.
 * @param inCard : the card to the batterarray.
 * @param inCardType: the card type: from 0 - 4, when the value is 4, it means the card act as intervene role.
 * @return ture | false 
 */

bool CPtBattleArray::addCard(CCNode *inCard, const int & inCardType)
{
    
	bool bRet = false;

	if(inCardType == 4)
	{
        if (m_pCardArray[inCardType] != NULL)
        {
           return replaceCard(inCard, inCardType);
        }
		// add the intervene card
		// 1. first distinguish the card's kind , if it'kind is intervene. continue, else return false.
        
        // update ui:
        m_pCardArray[inCardType] = inCard;
        
        if (m_pCardArray[inCardType])
		{
            
            inCard->setAnchorPoint(CCPointZero);
            inCard->setPosition(m_cPositions[inCardType]);
            this->addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
            
		}


	}

	if (inCardType >= 0 && inCardType < 4)
	{
		if (m_pCardArray[inCardType])
		{
			// remove the former card, set the former card state.
            CCLog("have");
		
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        
        if (m_pCardArray[inCardType])
		{
            inCard->setAnchorPoint(CCPointZero);
            inCard->setPosition(m_cPositions[inCardType]);
            this->addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
		}
		
	}
	else
	{
		// the illegality card
	}

	updateBattleArray();

	return bRet;
}

bool CPtBattleArray::replaceCard(CCNode *inCard, const int & inCardType)
{
    bool bRet = false;


	if(inCardType == 4)
	{
		// add the intervene card
		// 1. first distinguish the card's kind , if it'kind is intervene. continue, else return false.
        
        // update
        if (m_pCardArray[inCardType])
		{
            removeChild(m_pCardArray[inCardType], true);
            m_pCardArray[inCardType] = NULL;
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        inCard->setPosition(m_cPositions[inCardType]);
        inCard->setAnchorPoint(CCPointZero);
        
        if (m_pCardArray[inCardType])
		{
            addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
           
        }

        
	}
    
	if (inCardType >= 0 && inCardType < 4)
	{
		if (m_pCardArray[inCardType])
		{
			// remove the former card, set the former card state.
            removeChild(m_pCardArray[inCardType], true);
            m_pCardArray[inCardType] = NULL;
            
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        inCard->setPosition(m_cPositions[inCardType]);
        inCard->setAnchorPoint(CCPointZero);
        
        if (m_pCardArray[inCardType])
		{
            addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
		}
		
	}
	else
	{
		// the illegality card
	}
    
	updateBattleArray();
    
	return bRet;

    
}

int CPtBattleArray::getReplaceCard(CCNode *inCard)
{
    CCRect rect = inCard->boundingBox();
    rect.origin = this->convertToNodeSpace(rect.origin);
    

    for (int i = 0; i < CARDCOUNT; i++)
    {
        CCNode *tmp = m_pCardArray[i];
        if (tmp)
        {
            float dt = abs(ccpDistance(ccp(rect.getMidX(), rect.getMidY()), ccp(tmp->boundingBox().getMidX(), tmp->boundingBox().getMidY())));
            if (dt < 8.0f)
            {
                return i;
            }
        }
    }
    return -1;
}

bool CPtBattleArray::isAssistantCard(CCNode *inCard, bool inRemove)
{
    CCRect rect ;
    rect.origin = m_cPositions[4];
    rect.size = CCSizeMake(139, 200);
    CCPoint point = inCard->getPosition();
    point = this->convertToNodeSpace(point);
    if (rect.intersectsRect (CCRectMake(point.x, point.y, 139, 200)))
    {
        if (inRemove)
        {
            return true;
        }
        else
        {
            if ((point.x-rect.origin.x) < 100)
            {
                return true;
            }
        }

    }
    return false;
}

bool CPtBattleArray::isAssistantCard(CCTouch *pTouch)
{
   
    if (m_pCardArray[CARDCOUNT-1])
    {
        CCRect rect = m_pCardArray[CARDCOUNT-1]->boundingBox();
        CCPoint point =  this->convertTouchToNodeSpace(pTouch);
        if (rect.containsPoint(point))
        {
            return true;
        }
    }
    return false;

}

bool CPtBattleArray::removeCard(const int& inCardType)
{
	bool bRet = false;
	if (inCardType >= 0 && inCardType < 5)
	{
		if (m_pCardArray[inCardType])
		{
			// set card state:            
			// set null
            removeChild(m_pCardArray[inCardType], true);
			m_pCardArray[inCardType] = NULL;
            m_pSuitLogo[inCardType]->removeAllChildrenWithCleanup(true);
			updateBattleArray();
		}
		
	}

	return bRet;
}

int CPtBattleArray::getFightPower()
{
	return m_nFightPower;
}

int CPtBattleArray::getLeaderShip()
{
	return m_nLeadership;
}

int CPtBattleArray::getGrowthValue()
{
	return m_nGrowthValue;
}

CardSuitKind CPtBattleArray::getCardSuitKind()
{
	return m_SuitKind;
}


CCArray * CPtBattleArray::getCardArray()
{
	CCArray * array =  CCArray::create();
	for (int i = 0; i < CARDCOUNT; ++i)
	{
		if (m_pCardArray[i])
		{
			array->addObject(m_pCardArray[i]);
		}
	}
	return array;
	
}

int  CPtBattleArray::resortFightCard()
{
    int next = 0;
    for (int i  = 0;  i < CARDCOUNT -1; i++)
    {
       
        
        if (m_pCardArray[i])
        {
            if (next != i)
            {
                m_pCardArray[next] = m_pCardArray[i];
                m_pCardArray[i] = NULL;
               
            }
            m_pCardArray[next]->setPosition(m_cPositions[next]);
            this->reorderChild(m_pCardArray[next], 1000+next);
            next++;
            CCLog("the card:%d", i);
        }
    }
    if (m_pCardArray[4])
    {
        m_pCardArray[4]->setPosition(m_cPositions[4]);
    }
    return next;
}

int CPtBattleArray::getInsertIndex()
{
   return insertWhichNode(m_pCardArray, this, CARDCOUNT);
}

void CPtBattleArray::initMap()
{
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "zhenxingfudong.plist"));
    CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardsetting_bg.png"));
    bg->setAnchorPoint(CCPointZero);
    this->addChild(bg, -1);
    this->setContentSize(bg->getContentSize());
    int tags[3]={2,0,3};
    for (int i = 0 ; i < CARDCOUNT; i++)
    {
        tags[2] = 3+ i;
        m_pSuitLogo[i] =(CCSprite *) m_cMaps->getElementByTags(tags, 3);
    }
    
    if (inTag == 2)
    {
       dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,0,1"))->setString("Team 2 Arrage") ;
    }else if(inTag == 3)
    {
        dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,0,1"))->setString("Defens Team") ;
    }
  
}
void CPtBattleArray::initData()
{
    m_bTouchEnable = false;
	m_nLeadership = 0;
	m_nFightPower = 0;
    m_nDefense = 0;
    m_nHP = 0;
    m_nSuitAtk = 0;
    m_nSuitDef = 0;
    
	m_nGrowthValue = 0;
    // init logo:
    for (int i = 0; i < CARDCOUNT; i++)
    {
        m_pSuitLogo[i] = NULL;
        m_pCardArray[i] = NULL;
    }
}


void CPtBattleArray::updateData()
{
    m_nLeadership = 0;
	m_nFightPower = 0;
    m_nDefense = 0;
    m_nHP = 0;
    
    if (m_pCardArray[CARDCOUNT-1])
    {
        m_nLeadership = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_icard_leadership;
        m_nFightPower = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_icard_attack;
        m_nDefense = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_icard_defend;
        m_nHP = dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getCardData()->m_icardhp;
    }
    
    for (int i = 0; i < CARDCOUNT-1 ;i++ )
    {
        
        if (m_pCardArray[i])
        {
            CCard *cardData = dynamic_cast<CGamesCard *>(m_pCardArray[i])->getCardData();
           m_nLeadership += cardData->m_icard_leadership;
           m_nFightPower += cardData-> m_icard_attack; //攻击力
           m_nDefense    += cardData-> m_icard_defend; //防御力
           m_nHP    += cardData-> m_icardhp;    //卡牌
        }
        else{
            break;
        }
    }
    
}

void CPtBattleArray::updateBattleArray()
{
  
    
    resortFightCard();
    updateCardSuitKind();
    updateData();
    updateLabel();
}

void CPtBattleArray::updateCardSuitKind()
{
    //
    
    m_nSuitAtk = 0;
    m_nSuitDef = 0;
    
    int outSuit, outSequence;
    for (int i = 0; i < CARDCOUNT-1; i++)
    {
        if (m_pCardArray[i])
        {
            if (dynamic_cast<CGamesCard *>(m_pCardArray[i])->getSuit(outSuit, outSequence))
                createSuitLogo(outSuit, outSequence, i+1);
        }else
        {
             m_pSuitLogo[i+1]->removeAllChildrenWithCleanup(true);
        }
       
    }
    if (m_pCardArray[CARDCOUNT-1])
    {
        if (dynamic_cast<CGamesCard *>(m_pCardArray[CARDCOUNT-1])->getSuit(outSuit, outSequence))
            createSuitLogo(outSuit, outSequence, 0);
    }else
    {
        m_pSuitLogo[0]->removeAllChildrenWithCleanup(true);
    }
    
    // test
    
    m_nSuitAtk = 10;
    m_nSuitDef = 5;
}

void CPtBattleArray::updateLabel()
{
    CCLabelTTF *label = NULL;
    char buff[40]={0};
    
    // atk:
    sprintf(buff, "ATK: %d (+%d)",m_nFightPower, m_nSuitAtk);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,3"));
    label->setString(buff);
    
    // hp
    sprintf(buff, "HP: %d",m_nHP);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,4"));
    label->setString(buff);
    
    // def
    sprintf(buff, "DEF: %d (-%d)",m_nDefense,m_nSuitDef);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,5"));
    label->setString(buff);
    
    // rvc
    sprintf(buff, "RVC: %d",m_nLeadership);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,1,6"));
    label->setString(buff);
    
    // suit
    sprintf(buff, "ATK +%d DEF -%d",m_nSuitAtk, m_nSuitDef);
    label = dynamic_cast<CCLabelTTF *>(m_cMaps->getElementByTags("2,0,2,2"));
    label->setString(buff);
    
    
}

static CCNode * selectNode  = NULL;
static int selectIndex = -1;

bool CPtBattleArray::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    m_bTouchEnable = false;
    CCLog("CPtBattleArray %d ::ccTouchBegan",inTag+1);
   
    if (isAssistantCard(pTouch))
    {
        selectIndex = 4;
        selectNode = m_pCardArray[4];
        m_bTouchEnable = true;
        return true;
    }

    CCPoint point = this->getParent()->convertTouchToNodeSpace(pTouch);
    if (this->boundingBox().containsPoint(point))
    {
       
        selectIndex = whichNode(m_pCardArray, this, pTouch, resortFightCard());
        CCLog("the selectIndex: %d", selectIndex);
        if (selectIndex == -1)
        {
            return false;
        }
        selectNode = m_pCardArray[selectIndex];
        this->reorderChild(selectNode, 1020);
        m_bTouchEnable = true;
        return true;
        
    }
    
      return false;
}
void CPtBattleArray::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtBattleArray::ccTouchMoved");
    
    if (selectNode)
    {
        drag(selectNode, pTouch);
       
        CCNode *right = selectIndex+1 < 4? m_pCardArray[selectIndex+1] : NULL;
        CCNode *left = selectIndex -1 >= 0? m_pCardArray[selectIndex-1]:NULL;
        if (isNeedMove(selectNode, right, 1))
        {
            moveHalfStep(right, 2);
            m_pCardArray[selectIndex] = right;
            selectIndex++;
            m_pCardArray[selectIndex] = selectNode;
        }else if(isNeedMove(selectNode, left, 2))
        {
            moveHalfStep(left, 1);
            m_pCardArray[selectIndex] = left;
            selectIndex--;
            m_pCardArray[selectIndex] = selectNode;
        }
    }

}
void CPtBattleArray::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtBattleArray::ccTouchEnded");
    
    if (selectNode)
    {
        CCRect rect1 = selectNode->boundingBox();
        rect1.origin = this->getParent()->convertToNodeSpace(rect1.origin);
        CCRect rect = this->boundingBox();
        
        //rect.origin = this->convertToNodeSpace(rect.origin);
        
        if (selectIndex == 4 && isAssistantCard(selectNode,true))
        {
            removeCard(4);
        }
        else if(!rect.intersectsRect(rect1))
        {
            removeCard(selectIndex);
        }
     
    }
    updateBattleArray();
    
    for (int i = 0; i < CARDCOUNT-1; i++)
    {
        if (m_pCardArray[i])
        {
            CCLog("the card: %d",i);
        }
    }
    
}
void CPtBattleArray::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void CPtBattleArray::insertMoveCard(CCNode *inCard)
{
    insertMoveNode(m_pCardArray, this, inCard, CARDCOUNT-1);
}

// implement class of CPtBatterArrayPanel

CPtBattleArrayPanel* CPtBattleArrayPanel::create(CCSize size, CCNode* container)
{
    
    CPtBattleArrayPanel* pRet = new CPtBattleArrayPanel();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->init();
        pRet->autorelease();
       
        
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;

}

CPtBattleArrayPanel::CPtBattleArrayPanel()
{
    
    
}


bool CPtBattleArrayPanel::init()
{
    
    CCLayer *layer = CCLayer::create();
    //CCLayerColor *layer = CCLayerColor::create(ccc4(125, 0, 0, 125));
    layer->ignoreAnchorPointForPosition(false);
    

  
    for (int i = 2; i >= 0; i--)
    {
        CPtBattleArray * battleArray = CPtBattleArray::create(i+1);
        layer->addChild(battleArray);
        battleArray->setPosition(ccp(0, (2-i) * 520 + 85));
        m_pBatterArrays[i] = battleArray;
        battleArray->initSize(CCSizeMake(139, 200), ccp(175, 150));
    }
    setContainer(layer);
    setDirection(kCCScrollViewDirectionVertical);
    setContentSize(CCSizeMake(534, 1560 +85));
    m_pContainer->setPosition(ccp(0, -1000));
    m_pCurrentBatterArray = m_pBatterArrays[0];
    return true;
}

CPtBattleArray * CPtBattleArrayPanel::getBattleArray(CCTouch *pTouch)
{
    CPtBattleArray * tmp ;
    for (int i = 0; i < BATTERARRAYCOUNT; i++)
    {
        tmp = m_pBatterArrays[i];
        if (tmp)
        {
            if(CPtTool::isInNode(tmp, pTouch))
            {
                return tmp;
            }
        }
    }
    return NULL;
}


// test:
static CCPoint prePoint;
bool CPtBattleArrayPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    prePoint = m_pContainer->getPosition();
    CCLog("logo");
    for (int i = 0; i < BATTERARRAYCOUNT ; i++)
    {
        CPtBattleArray * tmp =  m_pBatterArrays[i];
        if (tmp)
        {
            if (tmp->m_bTouchEnable)
            {
                return  false;
            }
        }
    }
    
    m_cBeginOffset = getContentOffset();
    
 return   CCScrollView::ccTouchBegan(pTouch, pEvent);
 
}
void CPtBattleArrayPanel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchMoved(pTouch, pEvent);
}
void CPtBattleArrayPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
    CCScrollView::ccTouchEnded(pTouch, pEvent);
    
    CCPoint m_EndOffset=getContentOffset();
//    if (m_cBeginOffset.equals(m_EndOffset))
//    {
//       // onclick:
//        return ;
//    }
    adjustScrollView(m_cBeginOffset,m_EndOffset);
    updatePanel();

    
    
}
void CPtBattleArrayPanel::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchCancelled(pTouch, pEvent);
}

CPtBattleArrayPanel::~CPtBattleArrayPanel()
{


}

/*
 * just the view poistion
 */

void CPtBattleArrayPanel::adjustScrollView(const CCPoint& begin, const CCPoint &end)
{
    CCLog("begin: %f, %f, end: %f, %f", begin.x, begin.y, end.x, end.y);
    
  //   int nPage = abs(begin.x / (int)m_CellSize.width);
    int nPage = abs((begin.y) / (int)500);
    int nAdjustPage = 0;
    int nDis = end.y-begin.y;

    if (nDis<-getViewSize().height/3)
    {
        nAdjustPage=nPage+1;
    }
    else if (nDis > getViewSize().height/3)
    {
        nAdjustPage=nPage-1;
    }
    else
    {
        nAdjustPage=nPage;
    }

    nAdjustPage=MIN(nAdjustPage,3-1);
    nAdjustPage=MAX(nAdjustPage,0);
    scrollToPage(nAdjustPage);
    CCLog("the page: %d, %d", nAdjustPage, nPage);
}



void CPtBattleArrayPanel::scrollToPage( int nPage )
{
    m_nCurrentPage = 2-nPage;
        // 关闭CCScrollView中的自调整
    unscheduleAllSelectors();
    CCPoint adjustPos=ccp(0,-500*nPage);
    // 调整位置
    setContentOffsetInDuration(adjustPos, 0.2f);
 
}




void CPtBattleArrayPanel::initData()
{
	// initData
	for (int i = 0; i < BATTERARRAYCOUNT; ++i)
	{
		m_pBatterArrays[i] = NULL;
		
	}
	m_nPlayerLeadership = 0;
	m_nPlayerFightPower = 0;
	m_nCurrentPage = -1;
	m_pCurrentBatterArray = NULL;

}



void CPtBattleArrayPanel::updatePanel()
{
	// refesh
    if (m_nCurrentPage <= 2 && m_nCurrentPage >=0)
    {
        m_pCurrentBatterArray = m_pBatterArrays[m_nCurrentPage];
    }
}