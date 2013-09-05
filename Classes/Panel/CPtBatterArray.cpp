#include "CPtBatterArray.h"
#include "CPtTool.h"
#include "gameConfig.h"
#include "CGamesCard.h"
#include <cmath>
#include "CCard.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CCardSettingScene.h"
#include "CSaveConfirmLayer.h"
#include "PtHttpURL.h"

using namespace CPtTool;

// implement CPtDisPlayCard:
CPtDisPlayCard  * CPtDisPlayCard::Create(CFightCard *card)
{
    CPtDisPlayCard *cardSprite=new CPtDisPlayCard();
    if(cardSprite ==NULL|| !cardSprite->initCreate(card))
    {
        delete cardSprite;
        cardSprite=NULL;
    }
    cardSprite->autorelease();
    return cardSprite;

}

CPtDisPlayCard * CPtDisPlayCard::getCopy()
{
   return  Create(m_pCardData);
}

void CPtDisPlayCard::createManifier()
{
    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "fangdajing_normal.png"));
    m_pMagnifier = CCSprite::createWithTexture(texture);
    m_pMagnifier->setScale(0.75f);
    m_pMagnifier->setPosition(ccp(100, 40));
    addChild(m_pMagnifier, 200);
}
bool CPtDisPlayCard::isDisplay()
{
    if (m_pMagnifier&& m_pMagnifier->isVisible())
    {
        return true;
    }
    return false;
}
void CPtDisPlayCard::displayManifier()
{
    if (m_pMagnifier)
    {
        m_pMagnifier->setVisible(true);
    }else
    {
        createManifier();
    }
}
void CPtDisPlayCard::hideManifier()
{
        
    if (m_pMagnifier)
    {
        m_pMagnifier->setVisible(false);
    }
}

void CPtDisPlayCard::setManifierNormal()
{
    if (m_pMagnifier)
    {
        CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "fangdajing_normal.png"));
        m_pMagnifier->initWithTexture(texture);
    }

    
}
void CPtDisPlayCard::setManifierPress()
{
    if (m_pMagnifier)
    {
        CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "fangdajing_actived.png"));
        m_pMagnifier->initWithTexture(texture);
    }

}
bool CPtDisPlayCard::isClickManifier(CCTouch *pTouch)
{
    bool bRet = CPtTool::isInNode(m_pMagnifier, pTouch);
    return bRet;
}

// implement CPtBattleArray:
CPtBattleArray* CPtBattleArray::create(vector<CFightCard *> &fightArray, const cocos2d::CCSize &size, const cocos2d::CCPoint &point, int Tag /*= 1*/)
{
	CPtBattleArray * batterarray = new CPtBattleArray();
    
	if (batterarray)
	{
        batterarray->inTag = Tag;
        batterarray->initMap();
        batterarray->setTouchEnabled(true);
        batterarray->setTouchMode(kCCTouchesOneByOne);
		batterarray->autorelease();
        batterarray->initSize(size, point);
        batterarray->initBattleArrayFromServer(fightArray);
    
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

void CPtBattleArray::initBattleArrayFromServer(vector<CFightCard *> &fightArray)
{
    CCDictionary * cardBagPointArray = CCardSettingScene::s_pBattleArrayCards;

    if (fightArray.size() == 0)
    {
        return;
    }

    CFightCard * tmp = fightArray.at(fightArray.size()-1);
    CCLog("size: %d", fightArray.size());
    m_nCardCount = 0;
    if (tmp)
    {

        CCLog("id, %d", tmp->m_pCard->m_icard_id);
        CPtDisPlayCard * card = CPtDisPlayCard::Create(tmp);
        card->setInCardBagPointer((CPtDisPlayCard*)cardBagPointArray->objectForKey(card->getCardData()->m_User_Card_ID));
        addCard(card, 4);
    }
    int index = 0;
    for (int i = 3; i >= 0 ;i--)
    {
        tmp = NULL;
        tmp = fightArray.at(i);
        if (tmp)
        {
             CCLog("fight: %d",i);
             CCLog("position:%d,id, %d", i,tmp->m_pCard->m_icard_id);
            CPtDisPlayCard * card = CPtDisPlayCard::Create(tmp);
            card->setInCardBagPointer((CPtDisPlayCard*)cardBagPointArray->objectForKey(card->getCardData()->m_User_Card_ID));
            addCard(card, index++);
            
        }else
        {
            CCLog("fight: %d null",i);
        }
    }
    updateBattleArray();
}

void CPtBattleArray::createSuitLogo(const int& inSuit, const int &inSequence, const int &inPositionIndex)
{
    char buff[10] ={0};
    sprintf(buff, "suit%d.png", inSuit);
    CCSprite *suitSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, buff));
    suitSprite->setAnchorPoint(CCPointZero);
    suitSprite->setPosition(ccp(5,5));
    m_pSuitLogo[inPositionIndex]->cocos2d::CCNode::removeAllChildrenWithCleanup(true);
    m_pSuitLogo[inPositionIndex]->addChild(suitSprite,300);
   
    CCLabelTTF * label = NULL;
    if (inSequence >1 && inSequence < 11)
    {
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%2d", inSequence);
        label=CCLabelTTF::create(buff, "Arial", 6 );
    }else
    {
        char c[2]={'\n'};
        switch (inSequence)
        {
            case 1:
                c[0]='A';
                break;
            case 11:
                c[0]='J';
                break;
            case 12:
                c[0]='Q';
                break;
            case 13:
                c[0]='K';
                break;
            default:
                break;
        }
        label=CCLabelTTF::create(c, "Arial", 6);
    }
    if (label)
    {
        label->setPosition(ccp(4,4));
        suitSprite->addChild(label);
    }

    
    m_aSuitArray[inPositionIndex] = inSuit;
    m_aSequenceArray[inPositionIndex] = inSequence;
}

/*
 * @function: bool addCard(CCObject *inCard, const int & inCardType)
 * @brief: add or replace card to the batterarray.
 * @param inCard : the card to the batterarray.
 * @param inCardType: the card type: from 0 - 4, when the value is 4, it means the card act as intervene role.
 * @return ture | false 
 */

bool CPtBattleArray::addCard(CPtDisPlayCard *inCard, const int & inCardType , const bool &inAppendEnable /*= false*/)
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
            inCard->getCardData()->setInBattleArray(inTag); //(true);
            inCard->setAnchorPoint(CCPointZero);
            inCard->setPosition(m_cPositions[inCardType]);
            this->addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
            
		}


	}

    // append:
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
            (inCard)->getCardData()->setInBattleArray(inTag);//(true);
        }
		
	}
	else
	{
		// the illegality card
	}

	updateBattleArray();

	return bRet;
}

bool CPtBattleArray::replaceCard(CPtDisPlayCard *inCard, const int & inCardType)
{
    bool bRet = false;


	if(inCardType == 4)
	{
		// add the intervene card
		// 1. first distinguish the card's kind , if it'kind is intervene. continue, else return false.
        
        // update
        if (m_pCardArray[inCardType])
		{
            
            CPtDisPlayCard * tmpCard = (m_pCardArray[inCardType]);
            if (tmpCard && tmpCard->getCardData())
            {
                tmpCard->getCardData()->setInBattleArray(0);//(false);
               // tmpCard->getInCardBagPointer()->setLive();
                tmpCard->getInCardBagPointer()->setLogo(0);
            }

            removeChild(m_pCardArray[inCardType], true);
            m_pCardArray[inCardType] = NULL;
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        inCard->setPosition(m_cPositions[inCardType]);
        inCard->setAnchorPoint(CCPointZero);
        
        if (m_pCardArray[inCardType])
		{
            (inCard)->getCardData()->setInBattleArray(inTag);//(true);
            addChild(m_pCardArray[inCardType], 1000+inCardType, inCardType);
           
        }

        
	}
    
	if (inCardType >= 0 && inCardType < 4)
	{
		if (m_pCardArray[inCardType])
		{
			// remove the former card, set the former card state.
            CPtDisPlayCard * tmpCard = (m_pCardArray[inCardType]);
            if (tmpCard && tmpCard->getCardData())
            {
                tmpCard->getCardData()->setInBattleArray(0);//(false);
                if (tmpCard)
                {
                   // tmpCard->getInCardBagPointer()->setLive();
                    tmpCard->getInCardBagPointer()->setLogo(0);
                }
            }
            
            removeChild(m_pCardArray[inCardType], true);
            m_pCardArray[inCardType] = NULL;
            
		}
		// add the  card
		m_pCardArray[inCardType] = inCard;
        inCard->setPosition(m_cPositions[inCardType]);
        inCard->setAnchorPoint(CCPointZero);
        
        if (m_pCardArray[inCardType])
		{
            (inCard)->getCardData()->setInBattleArray(inTag);//(true);
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

int CPtBattleArray::getReplaceCard(CPtDisPlayCard *inCard,bool inSamePanel)
{
    CCRect rect = inCard->boundingBox();
    if (inSamePanel == false)
    {
         rect.origin = this->convertToNodeSpace(rect.origin);
    }
   

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

int CPtBattleArray::getReplaceCard(CPtDisPlayCard *inCard, const int &inSelfIndex , bool inSamePanel)
{
    CCRect rect = inCard->boundingBox();
    if (inSamePanel == false)
    {
        rect.origin = this->convertToNodeSpace(rect.origin);
    }
    
    
    for (int i = 0; i < CARDCOUNT; i++)
    {
        CCNode *tmp = m_pCardArray[i];
        if (i != inSelfIndex && tmp)
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

bool CPtBattleArray::isAssistantCard(CPtDisPlayCard *inCard, bool inRemove, bool inSamePanel)
{
    CCRect rect ;
    rect.origin = m_cPositions[4];
    rect.size = CCSizeMake(139, 200);
    CCPoint point = inCard->boundingBox().origin;
    if (inSamePanel == false)
    {
        point = this->convertToNodeSpace(point);
    }
  
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

            CPtDisPlayCard * tmpCard = (m_pCardArray[inCardType]);
            if (tmpCard && tmpCard->getCardData())
            {
                if (tmpCard)
                {
                    tmpCard->getCardData()->setInBattleArray(0);//(false);
                    if (tmpCard->getInCardBagPointer())
                    {
                        tmpCard->getInCardBagPointer()->setLogo(0);
                    }
                 
                }
            }

          //  removeChild(m_pCardArray[inCardType], true);
			m_pCardArray[inCardType] = NULL;
            m_pSuitLogo[inCardType]->removeAllChildrenWithCleanup(true);
            m_aSequenceArray[inCardType] = -1;
            m_aSuitArray[inCardType] = -1;
			updateBattleArray();
            
		}
		
	}

	return bRet;
}

/*
 * @brief 删除卡牌时的action：
 */

void CPtBattleArray::removeAction(CCNode *pNode)
{
    CCLog("%d, removeAction:", inTag);
//    if (pNode)
//    {
//        m_bActionEnable = true;
//        CPtDisPlayCard * tmp = ((CPtDisPlayCard*)pNode)->getInCardBagPointer();
//        if(tmp && tmp->getParent())
//        {
//            CCPoint point = tmp->getPosition();
//            point = tmp->getParent()->convertToWorldSpace(point);
//            CCLog("point: %f, %f",point.x, point.y);
//            pNode->runAction(CCSequence::create(CCMoveTo::create(0.1f, point), CCCallFunc::create(this, callfunc_selector(CPtBattleArray::deletNode)),NULL));
//        }else
//        {
//            deletNode();
//        }
//
//      
//    }
    deletNode();
    
}
void CPtBattleArray::deletNode()
{
     m_bActionEnable = false;
    if (selectNode)
    {
        selectNode->removeFromParentAndCleanup(true);
        selectNode = NULL;
    }
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
        m_nCardCount = next;
    }
    if (m_pCardArray[4])
    {
        m_pCardArray[4]->setPosition(m_cPositions[4]);
        if (m_pCardArray[4]->getCardData()->hasAssistantSkill() == false)
        {
            m_pCardArray[4]->setDead();
        }
    }
    return next;
}

int CPtBattleArray::getInsertIndex()
{
   return insertWhichNode((CCNode**)m_pCardArray, this, CARDCOUNT);
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
       ((CCLabelTTF *)(m_cMaps->getElementByTags("2,0,0,1")))->setString("Team 2 Arrage") ;
    }else if(inTag == 3)
    {
        ((CCLabelTTF *)(m_cMaps->getElementByTags("2,0,0,1")))->setString("Defens Team") ;
    }
    
    
    m_pSaveBtn =(CCSprite*)(m_cMaps->getElementByTags("2,0,8"));
//    m_pSaveBtn->setTexture();
    
   CCPoint point = m_pSaveBtn->getAnchorPoint();
    CCTexture2D * normal =   CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_normal.png"));
    m_pSaveBtn->initWithTexture(normal);
    m_pSaveBtn->setAnchorPoint(point);
   
}
void CPtBattleArray::initData()
{
    m_bActionEnable = false;
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
        m_aSuitArray[i] = -1;
        m_aSequenceArray[i] = -1;
    }
    selectNode  = NULL;
    selectIndex = -1;
    
}


void CPtBattleArray::updateData()
{
    m_nLeadership = 0;
	m_nFightPower = 0;
    m_nDefense = 0;
    m_nHP = 0;
    
    if (m_pCardArray[CARDCOUNT-1])
    {
        m_nLeadership = (m_pCardArray[CARDCOUNT-1])->getCardData()->m_pCard->m_icard_leadership;
        m_nFightPower = (m_pCardArray[CARDCOUNT-1])->getCardData()->m_attack;

        m_nDefense = (m_pCardArray[CARDCOUNT-1])->getCardData()->m_defend;
        m_nHP = (m_pCardArray[CARDCOUNT-1])->getCardData()->m_iHp;
    }
    
    for (int i = 0; i < CARDCOUNT-1 ;i++ )
    {
        
        if (m_pCardArray[i])
        {
           CFightCard *cardData = (m_pCardArray[i])->getCardData();
           m_nLeadership += cardData-> m_pCard->m_icard_leadership;
           m_nFightPower += cardData-> m_attack; //攻击力
           m_nDefense    += cardData-> m_defend; //防御力
           m_nHP    += cardData-> m_iHp;    //卡牌
        }
        else
        {
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
    
    int array[5]={0};
    int len = 0;
    getSuitArray(array, len);
    int outSuit, outSequence;
    for (int i = 0; i < 5; i++)
    {
        if (i >= len && m_pSuitLogo[i])
        {
            m_aSuitArray[i] = -1;
            m_aSequenceArray[i] = -1;
            m_pSuitLogo[i]->removeAllChildrenWithCleanup(true);
            return;
        }
        if(CPtTool::getSuit(array[i], outSuit, outSequence))
        {
            m_aSuitArray[i] = outSuit;
            m_aSequenceArray[i] = outSequence;
           createSuitLogo(outSuit, outSequence, i);
        }
    }
  
//    for (int i = 0; i < CARDCOUNT-1; i++)
//    {
//        if (m_pCardArray[i])
//        {
//            if ((m_pCardArray[i])->getSuit(outSuit, outSequence))
//                createSuitLogo(outSuit, outSequence, i+1);
//        }else
//        {
//             m_pSuitLogo[i+1]->removeAllChildrenWithCleanup(true);
//             m_aSuitArray[i+1] = -1;
//             m_aSequenceArray[i+1] = -1;
//        }
//       
//    }
//    if (m_pCardArray[CARDCOUNT-1])
//    {
//        if ((m_pCardArray[CARDCOUNT-1])->getSuit(outSuit, outSequence))
//            createSuitLogo(outSuit, outSequence, 0);
//    }else
//    {
//        m_pSuitLogo[0]->removeAllChildrenWithCleanup(true);
//        m_aSuitArray[0] = -1;
//        m_aSequenceArray[0] = -1;
//    }
    
    // test
    
    m_nSuitAtk = 10;
    m_nSuitDef = 5;
}

void CPtBattleArray::getSuitArray(int array[], int &len)
{
    len = 0;
    for (int i = 0; i < CARDCOUNT; i++)
    {
        if (m_pCardArray[i])
        {
            array[len++]= m_pCardArray[i]->getCardData()->m_iSuit;
        }
    }
    int tmpValue;
    int minValue = 0;
    for (int i = 0; i < len; i++)
    {
        minValue = i;
        for (int j = i+1; j < len; j++)
        {
            if (array[minValue] > array[j])
            {
                minValue = j;
            }
        }
        tmpValue = array[minValue];
        array[minValue] = array[i];
        array[i] = tmpValue;
    }

}

void CPtBattleArray::updateLabel()
{
    CCLabelTTF *label = NULL;
    char buff[40]={0};
    
    // atk:
    sprintf(buff, "ATK: %d (+%d)",m_nFightPower, m_nSuitAtk);
    label = (CCLabelTTF *)(m_cMaps->getElementByTags("2,0,1,3"));
    label->setString(buff);
    
    // hp
    sprintf(buff, "HP: %d",m_nHP);
    label = (CCLabelTTF *)(m_cMaps->getElementByTags("2,0,1,4"));
    label->setString(buff);
    
    // def
    sprintf(buff, "DEF: %d (-%d)",m_nDefense,m_nSuitDef);
    label = (CCLabelTTF *)(m_cMaps->getElementByTags("2,0,1,5"));
    label->setString(buff);
    
    // rvc
    sprintf(buff, "RVC: %d",m_nLeadership);
    label = (CCLabelTTF *)(m_cMaps->getElementByTags("2,0,1,6"));
    label->setString(buff);
//    if (m_nLeadership > m_pPanelCntainer->getGamePlayer()->getRVC())
//    {
//        label->setColor(ccRED);
//    }else
//    {
//        label->setColor(ccWHITE);
//    }
    
    // suit
    sprintf(buff, "ATK +%d DEF -%d",m_nSuitAtk, m_nSuitDef);
    label = (CCLabelTTF *)(m_cMaps->getElementByTags("2,0,2,2"));
    label->setString(buff);
    
    
}

//static CCNode * selectNode  = NULL;
//static int selectIndex = -1;


bool CPtBattleArray::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bActionEnable)
    {
        // is action
        CCLog("is action");
        return false;
    }
    
    m_bOnClick = false;
    m_bTouchEnable = false;
    selectAssistant = false;
    CCLog("CPtBattleArray %d ::ccTouchBegan",inTag+1);
   
    if (CPtTool::isInNode(m_pSaveBtn, pTouch))
    {
        m_bOnClick = true;
       // m_bTouchEnable = true;
        CCPoint point = m_pSaveBtn->getAnchorPoint();
        CCTexture2D * press = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_pressed.png"));
        m_pSaveBtn->setTexture(press);
        m_pSaveBtn->setAnchorPoint(point);
        
        return true;
    }
    
    if (isAssistantCard(pTouch))
    {
        selectIndex = 4;
        selectNode = m_pCardArray[4];
        CCPoint point = selectNode->getPosition();
        point = selectNode->getParent()->convertToWorldSpace(point);
        point = m_pMoveLayer->convertToNodeSpace(point);
        selectNode->setPosition(point);
        selectNode->retain();
        selectNode->removeFromParentAndCleanup(true);
        m_pMoveLayer->addChild(selectNode);
        selectNode->release();
        if (selectNode->getCardData()->hasAssistantSkill() == false)
        {
            selectNode->setLive();
        }
        
        CCLog("%d", m_nCardCount);
        if (m_nCardCount < 4)
        {
           
            selectIndex = m_nCardCount;
            m_pCardArray[4] = NULL;
            m_pCardArray[m_nCardCount] = (CPtDisPlayCard *)selectNode;
            selectAssistant = true;
        }

        
        m_bTouchEnable = true;
        return true;
    }

    CCPoint point = this->getParent()->convertTouchToNodeSpace(pTouch);
    if (this->boundingBox().containsPoint(point))
    {
       
        selectIndex = whichNode((CCNode**)m_pCardArray, this, pTouch, resortFightCard());
        CCLog("the selectIndex: %d", selectIndex);
        if (selectIndex == -1)
        {
            return false;
        }
        selectNode = m_pCardArray[selectIndex];
     
        if (m_pMoveLayer)
        {
            CCPoint point = selectNode->getPosition();
            point = selectNode->getParent()->convertToWorldSpace(point);
            point = m_pMoveLayer->convertToNodeSpace(point);
            selectNode->setPosition(point);
            selectNode->retain();
            selectNode->removeFromParentAndCleanup(true);
            m_pMoveLayer->addChild(selectNode);
            selectNode->release();
        }
        else
        {
            this->reorderChild(selectNode, 1020);
        }
     
        m_bTouchEnable = true;
        return true;
        
    }
    
      return false;
}
void CPtBattleArray::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtBattleArray::ccTouchMoved");
    
    if(m_bOnClick == true)
    {
        return;
    }
    
    
    
    if (selectNode)
    {
        //
        if (m_pMoveLayer)
        {
            CCLog("move in CPtBattleArrayLayer");
            drag(selectNode, pTouch);
            
            // 移动援护牌, 满四张不跳动卡牌
            if (selectIndex ==4 && m_nCardCount == 4)
            {
                return;
            }
            
            
            CCNode *right = selectIndex+1 < 4? m_pCardArray[selectIndex+1] : NULL;
            CCNode *left = selectIndex -1 >= 0? m_pCardArray[selectIndex-1]:NULL;
            if (isNeedMove(selectNode, right, 1))
            {
                moveHalfStep(right, 2);
                m_pCardArray[selectIndex] = static_cast<CPtDisPlayCard*>(right);
                selectIndex++;
                m_pCardArray[selectIndex] = static_cast<CPtDisPlayCard*>(selectNode);
            }else if(isNeedMove(selectNode, left, 2))
            {
                moveHalfStep(left, 1);
                m_pCardArray[selectIndex] = static_cast<CPtDisPlayCard*>(left);
                selectIndex--;
                m_pCardArray[selectIndex] = static_cast<CPtDisPlayCard*>(selectNode);
            }

            return;
        }
        
    }

}
void CPtBattleArray::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtBattleArray::ccTouchEnded");
    
 
    if (m_bOnClick)
    {
        CCPoint point = m_pSaveBtn->getAnchorPoint();
        CCTexture2D *noraml = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "save_normal.png"));
        m_pSaveBtn->initWithTexture(noraml);
        m_pSaveBtn->setAnchorPoint(point);
        if (m_pPanelCntainer->getPanelMove())
        {
           
            return;
        }
        PtSoundTool::playSysSoundEffect("UI_click.wav");
        saveOnClick();
        return;
    }
    CCPoint orignPoint;
    
    if (selectNode)
    {
        if (m_pMoveLayer)
        {
            CCRect rect1 = selectNode->boundingBox();
            rect1.origin = selectNode->getParent()->convertToWorldSpace(rect1.origin);
            CCRect rect = this->boundingBox();
            rect1.origin = this->convertToNodeSpace(rect1.origin);
            rect.origin = CCPointZero;
        
            CCPoint point = selectNode->getPosition();
            orignPoint = point;
            point = selectNode->getParent()->convertToWorldSpace(point);
            point = convertToNodeSpace(point);
            selectNode->retain();
            selectNode->removeFromParentAndCleanup(true);
           // selectNode->setPosition(m_cPositions[selectIndex]);
            selectNode->setPosition(point);
            addChild(selectNode, 1000+selectIndex);
            selectNode->release();
            
            int replace = getReplaceCard((CPtDisPlayCard *)selectNode, selectIndex, true);
            if (replace != -1 && replace != selectIndex)
            {
                if (selectAssistant && m_pCardArray[4] == NULL)
                {
                    m_pCardArray[4] = selectNode;
                    m_pCardArray[selectIndex] = NULL;
                    swapCard(4, replace);
                }
                else if (selectIndex == 4 || replace == 4)
                {
                     swapCard(selectIndex, replace);
                }
               
                // replace card: selectIndex & replace!
                
            }
            else
            {
                if (m_pCardArray[4] == NULL &&  isAssistantCard((CPtDisPlayCard*)selectNode, false, true))
                {
                    // add card to assistant:
                    swapCard(4, selectIndex);
                }
                else if (selectIndex == 4 && !isAssistantCard((CPtDisPlayCard*)selectNode,true, true))
                {
                    selectNode->retain();
                    selectNode->removeFromParentAndCleanup(true);
                    m_pMoveLayer->addChild(selectNode,200);
                    selectNode->release();
                    removeCard(4);
                    removeAction(selectNode);
                    
                }
                else if(!rect.intersectsRect(rect1))
                {
                    
                    selectNode->retain();
                    selectNode->removeFromParentAndCleanup(true);
                    m_pMoveLayer->addChild(selectNode,200);
                    selectNode->release();
                    removeCard(selectIndex);
                    removeAction(selectNode);
                }

            }
            
            
            updateBattleArray();
        }
        
    }
   // updateBattleArray();
    
}
void CPtBattleArray::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLayer::ccTouchCancelled(pTouch, pEvent);
    ccTouchEnded(pTouch, pEvent);
}

void CPtBattleArray::insertMoveCard(CCNode *inCard)
{
    insertMoveNode((CCNode**)m_pCardArray, this, inCard, CARDCOUNT-1);
}

void CPtBattleArray::save()
{
    
    CCLog("save");
    // save data:
    int count = 0;
    for (int i = 0; i < CARDCOUNT; i++)
    {
        if (m_aSuitArray[i] != -1)
        {
            count++;
        }
        CCLog("i%d, %d", m_aSequenceArray[i], m_aSuitArray[i]);
    }
    int suit = getSuitResult(m_aSuitArray, m_aSequenceArray, count);
    int array[5]={0};
    CPtDisPlayCard * tmp = NULL;
    if (m_pCardArray[4] != NULL)
    {
        CPtDisPlayCard * tmp = (m_pCardArray[4]) ;
        array[4] =tmp->getCardData()->m_User_Card_ID;
    }
    int index = 0;
   
    for (int i = 3; i >= 0 ;i--)
    {
        tmp = NULL;
        tmp = (m_pCardArray[i]) ;
        if (tmp)
        {
            array[index++] = tmp->getCardData()->m_User_Card_ID;
        }
    }
    
    char buffer[200]={0};
//xianbei modify    sprintf(buffer, "sig=2ac2b1e302c46976beaab20a68ef95&troops=%d&card_color_id=%d&card_team_param={"
//            ,inTag,suit);
    sprintf(buffer, "sig=%s&troops=%d&card_color_id=%d&card_team_param={",STR_USER_SIG, inTag,suit);
            string std = buffer;
    bool flag = true;
    for (int i = 0; i < 5; i++)
    {
        CCLog("the index: %d", array[i]);
        if (array[i] != 0)
        {
            if (flag)
            {
                sprintf(buffer, "\"%d\":\"%d\"", i+1, array[i]);
                flag = false;
            }
            else
            {
                sprintf(buffer, ",\"%d\":\"%d\"", i+1, array[i]);
            }

            
            std = std + buffer;
        }
        
    }
    std = std+"}";
    

    ADDHTTPREQUESTPOSTDATA(STR_URL_SAVE_TEAM(194), "helloworld1","ehll", std.c_str(), callfuncO_selector(CPtBattleArray::callBack));
    CCLog("suit: %d, %s, %s", suit, buffer, std.c_str());
   // define STR_URL_SAVE_TEAM(UID)      URL_FACTORY("api.php?m=Card&a=saveCardTeam&uid=",UID)
}

/*
 * 阵容设置的条件:
 * 1.领导力足够
 * 2.第一阵容必须有主将牌
 */
void CPtBattleArray::saveOnClick()
{
    CSaveConfirmLayer * layer = CSaveConfirmLayer::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000, 2000);
    if (hasMainAttacker() == false)
    {
        // 第一阵容没有主将牌
        layer->setResultCode(1);
        if (inTag == 1)
        {
            return;
        }
    }
    else if(m_pCardArray[4] == NULL)
    {
        layer->setResultCode(2);
    }
    else if (isAssistantCard() == false)
    {
        // 有援护牌且没有援护功能
        layer->setResultCode(3);
       // return ;
    }
    else if (isOverRVC())
    {
        layer->setResultCode(4);
        // 领导力不够
        return;
    }
    save();
    

}
void CPtBattleArray::callBack(CCObject *pSender)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "helloworld1");
 
    char * buffer = (char *)pSender;
   
    CSaveConfirmLayer * layer = (CSaveConfirmLayer*) CCDirector::sharedDirector()->getRunningScene()->getChildByTag(2000);
    if (!buffer)
    {
        static int badResualt=200;
        layer->setResultCode(badResualt);
        return;
    }
    CCDictionary* dic = PtJsonUtility::JsonStringParse(buffer);
   
    int result = GameTools::intForKey("code", dic);
    layer->setResultCode(result, true);
    if (result == 0)
    {
        save(SinglePlayer::instance()->m_vvBattleArray.at(inTag-1));
        
    }else
    {
        CCLog("error");
    }
    
    delete [] buffer;
    CCLog("callback: %s", buffer);
   
}

void CPtBattleArray::save(vector<CFightCard *> & infightArray)
{
    CCDictionary * cardBagPointArray = CCardSettingScene::s_pBattleArrayCards;
    CPtDisPlayCard * tmp = (m_pCardArray[4]);
    
    if (tmp)
    {
        if (infightArray.at(4))
        {
            cardBagPointArray->removeObjectForKey(infightArray.at(4)->m_User_Card_ID);
        }
         
         infightArray.at(4) = tmp->getCardData() ;        
    }
    else
    {
        if (infightArray.at(4))
        {
            cardBagPointArray->removeObjectForKey(infightArray.at(4)->m_User_Card_ID);
            infightArray.at(4) = NULL;
        }
    }
    
    int j = 0;
    for (int i =  3; i >= 0; i--)
    {
        if (m_pCardArray[i])
        {
            if (infightArray.at(j))
            {
                cardBagPointArray->removeObjectForKey(infightArray.at(j)->m_User_Card_ID);
            }
            
            tmp = (m_pCardArray[i]);
            infightArray.at(j++) = tmp->getCardData();
            
        }
        else
        {
            if (infightArray.at(i))
            {
                cardBagPointArray->removeObjectForKey(infightArray.at(i)->m_User_Card_ID);
                infightArray.at(i) = NULL;
            }
           
            
        }

    }
    
    for (int i =  0; i < CARDCOUNT; i++)
    {
        if (m_pCardArray[i])
        {
            if (m_pCardArray[i]->getInCardBagPointer())
            {
                cardBagPointArray->setObject(m_pCardArray[i]->getInCardBagPointer(), m_pCardArray[i]->getCardData()->m_User_Card_ID);
            }
            
        }
    }
    
    // test:
    for (int i = 0; i < infightArray.size(); i++)
    {
        if (infightArray.at(i))
        {
            CCLog("infight: %d, %d", inTag-1, infightArray.at(i)->m_User_Card_ID);
        }else
        {
            CCLog("%d null",i);
        }
    }
    
    CCDictElement * element;
    CCDICT_FOREACH(cardBagPointArray, element)
    {
        CCLog("%d",element->getIntKey()) ;
    }
    
}


void CPtBattleArray::resetBattleArray()
{
   
    vector<CFightCard*> fightArray = SinglePlayer::instance()->m_vvBattleArray.at(inTag-1);
    CCDictionary* dir =     CCardSettingScene::s_pBattleArrayCards;
    CPtDisPlayCard * tmp = NULL;
    CFightCard * tmpFightCard = NULL;
  
     CCLog("inTag: %d", inTag);

    for (int i = 0; i < CARDCOUNT ; i++)
    {
               
        tmp = (m_pCardArray[i]);
        if (tmp)
        {
            if (tmp->getInCardBagPointer())
            {
                //tmp->getInCardBagPointer()->setLive();
                tmp->getInCardBagPointer()->setLogo(0);
            }
            tmp->getCardData()->setInBattleArray(0);//(false);
            m_pCardArray[i] = NULL;
        }
    }
    for (int i = 0; i < CARDCOUNT; i++)
    {
        tmpFightCard = fightArray.at(i);
        if (tmpFightCard)
        {
            if (dir)
            {
                tmp =  ((CPtDisPlayCard*)(dir->objectForKey(tmpFightCard->m_User_Card_ID)));
                if (tmp)
                {
                    //tmp ->setDead();
                   // tmp->getCardData()->setInBattleArray(inTag);
                    tmp->setLogo(inTag);
                    tmp = NULL;
                }
            }
            tmpFightCard->setInBattleArray(inTag);
           
        }
    }
    

}

/*
 * @brief: 对调卡牌
 *
 */
void CPtBattleArray::swapCard(const int & inSrc, const int &inDes)
{
    CPtDisPlayCard * tmp = NULL;
    
    if (inSrc < 5 && inDes < 5 && inSrc >= 0 && inDes >= 0)
    {
        tmp = m_pCardArray[inSrc];
        m_pCardArray[inSrc] =  m_pCardArray[inDes];
        m_pCardArray[inDes] = tmp;
    }
}

/*
 * @brief : 判断当前阵容是否超过领导力
 */
bool CPtBattleArray::isOverRVC()
{
    bool bRet = false;
   
    if (m_pPanelCntainer->getGamePlayer()->getRVC() < m_nLeadership)
    {
        bRet = true;
    }
    
    return bRet;
    
}
/*
 * @breif:判别是否有援护技能
 */
bool CPtBattleArray:: isAssistantCard()
{
    bool bRet = true;
    if (m_pCardArray[4] && m_pCardArray[4]->getCardData()->m_pCard->m_iskillHelp <= 0)
    {
        bRet = false;
    }
    return bRet;
}
// implement class of CPtBatterArrayPanel

CPtBattleArrayPanel* CPtBattleArrayPanel::create(CCSize size, CCNode* container ,CCLayer* moveLayer)
{
    
    CPtBattleArrayPanel* pRet = new CPtBattleArrayPanel();
    if (pRet && pRet->initWithViewSize(size, container))
    {
        pRet->setMoveLayer(moveLayer);
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
    
    gamePlayer = SinglePlayer::instance();
}


bool CPtBattleArrayPanel::init()
{
    
    CCLayer *layer = CCLayer::create();
    //CCLayerColor *layer = CCLayerColor::create(ccc4(125, 0, 0, 125));
    layer->ignoreAnchorPointForPosition(false);
    

  
    for (int i = 2; i >= 0; i--)
    {
        CPtBattleArray * battleArray = CPtBattleArray::create (gamePlayer->m_vvBattleArray.at(i) ,CCSizeMake(139, 200), ccp(175, 150) ,i+1);
        battleArray->setMoveLayer(m_pMoveLayer);
        layer->addChild(battleArray);
        battleArray->setPosition(ccp(0, (2-i) * 520 + 85));
        m_pBatterArrays[i] = battleArray;
        battleArray->setPanelContainer(this);
       // battleArray->initSize(CCSizeMake(139, 200), ccp(175, 150));
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

void CPtBattleArrayPanel::resetBattleArrays()
{
    for (int i = 0; i < 3; i++)
    {
        m_pBatterArrays[i]->resetBattleArray();

    }


}

// test:
static CCPoint prePoint;
bool CPtBattleArrayPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bMove = false;
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
    m_bMove = true;
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
    m_bMove = false;
    CCScrollView::ccTouchCancelled(pTouch, pEvent);
    m_pMoveLayer = NULL;
}

CPtBattleArrayPanel::~CPtBattleArrayPanel()
{
    resetBattleArrays();

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