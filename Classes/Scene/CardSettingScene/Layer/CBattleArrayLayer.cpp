//
//  CBattleArrayLayer.cpp
//  91.cube
//
//  Created by phileas on 13-7-8.
//
//

#include "CBattleArrayLayer.h"
#include "gameConfig.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "gamePlayer.h"
#include "CGamesCard.h"
#include "CPtTableItem.h"
#include "CPtBatterArray.h"
#include "CPtTool.h"
#include "CCardEnhanceLayer.h"
#include "CCardSettingScene.h"
#include "CCardSellLayer.h"
#include "CCardInfoLayer.h"

static CPtBattleArray * s_currentBattleArray = NULL;
static void sortCards(CCArray *item, int inType);
// implement class of CPtBattleArrayItem:



bool CPtBattleArrayItem::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLog("CPtBattleArrayItem::ccTouchBegan");
    m_bMove = false;
    
    if (m_pDelegateLayer)
    {
        if (m_pDelegateLayer->getActionEnable())
        {
            CCLog("is action");
            return false;
           

        }
        m_pDelegateLayer->m_pMoveCard = NULL;
        m_pDelegateLayer->setTableClickMove(false);
        m_bClickManifier = false;
        // click fangdajing
        CPtDisPlayCard* tmp=  ((CPtDisPlayCard *)getDisplayView());
        if (tmp && tmp->isDisplay()&& tmp->isClickManifier(pTouch))
        {
            m_bClickManifier = true;
            tmp->setManifierPress();
            return true;
        }
        
        if (m_pDelegateLayer->getCurrentTab() == 2)
        {
            return onEnhanceBegin(pTouch, pEvent);
            
        }else if(m_pDelegateLayer->getCurrentTab() == 4)
        {
            
            return  onSellBegin(pTouch, pEvent);
            
        }else if(m_pDelegateLayer->getCurrentTab() == 3)
        {
            // onevelution:
      
            return onEvolutionBegin(pTouch, pEvent);

        }else if(m_pDelegateLayer->getCurrentTab() == 1)
        {
          return  onTeamArrayBegin(pTouch, pEvent);
        }else
        {
            return false;
        }
        
    }
    return  false;
}


void CPtBattleArrayItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_pDelegateLayer)
    {
        if (m_bClickManifier)
        {
            CPtDisPlayCard* tmp=  ((CPtDisPlayCard *)getDisplayView());
            if (tmp->isClickManifier(pTouch))
            {
                PtSoundTool::playSysSoundEffect("UI_click.wav");
                createInfoLayer();
               
            }
            tmp->setManifierNormal();
            return;
        }
        
        if (m_pDelegateLayer->getCurrentTab() == 2)
        {
            onEnhanceEnd(pTouch, pEvent);
            return;
        }else if(m_pDelegateLayer->getCurrentTab() == 4)
        {
            onSellEnd(pTouch, pEvent);
            return ;
        }else if(m_pDelegateLayer->getCurrentTab() == 3)
        {
            // onevelution:
            onEvolutionEnd(pTouch, pEvent);
            return;
        }else if(m_pDelegateLayer->getCurrentTab() == 1)
        {
            onTeamArrayEnd(pTouch, pEvent);
        }
      
    }
}

void CPtBattleArrayItem::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtBattleArrayItem::ccTouchCancelled");
    CPtBattleArray * battles =  s_currentBattleArray;
    m_bMove = false;
    s_currentBattleArray = NULL;
    
    if (m_pDelegateLayer)
    {
        if (m_bClickManifier)
        {
            CPtDisPlayCard* tmp=  ((CPtDisPlayCard *)getDisplayView());
            tmp->setManifierNormal();
            return;
        }
        
        if (m_pDelegateLayer && m_pDelegateLayer->m_pMoveCard)
        {
             m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
            m_pDelegateLayer->m_pMoveCard = NULL;
        }
        if (m_pDelegateLayer->getCurrentTab() == 2)
        {

        }else if(m_pDelegateLayer->getCurrentTab() == 4)
        {

           
        }else if(m_pDelegateLayer->getCurrentTab() == 3)
        {
         
        }else if(m_pDelegateLayer->getCurrentTab() == 1)
        {
            if (battles)
            {
               
                battles->updateBattleArray();
            }
        }
        
    }


}

bool CPtBattleArrayItem::onEnhanceBegin(CCTouch *pTouch, CCEvent *pEvent)
{
    bool bRet = false;
    bRet = getCopyCard(true, false);
    if (bRet == false)
    {
        displayManifier();
    }
    return bRet;
    
}
void CPtBattleArrayItem::onEnhanceEnd(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("---------%d",(int) getUserData());
    if (m_pDelegateLayer)
    {
        
        if (m_pDelegateLayer->m_pEnhancePanel)
        {
            if (m_pDelegateLayer->getTableClickMove())
            {
               // on move:

                int i = m_pDelegateLayer->m_pEnhancePanel->getEmptyCardSlide();
                if (i != -1)
                {
                    CPtDisPlayCard * displace = m_pDelegateLayer->m_pMoveCard; 
                    
                    if(displace)
                    {
                    
                        if (displace->getCardData()->getInWhichBattleArray() != 0)
                        {
                                //
                            if (i != -2)
                            {
                                CCLog("it's in battleArray");
                                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                                m_pDelegateLayer->m_pMoveCard = NULL;
                                return;
                            }
                               
                                
                        }
                            // ondrag mode:
                            // create new sprite:
                            
                        CPtDisPlayCard *node = m_pDelegateLayer->m_pMoveCard;
                        if ( m_pDelegateLayer->m_pEnhancePanel->canClickCard(node) && node->getCardData()->getEnConsume() == false)
                        {
                                node->setIndex((int)getUserData());
                                m_pDelegateLayer->m_pEnhancePanel->addCard(i, node);
                                
                        }else
                        {
                                CCLog("该为阵容中的卡，不能用作材料卡");
                                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                                m_pDelegateLayer->m_pMoveCard = NULL;
                                
                        }
                            
                        
                        
                    }
                }else
                {
                    m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                    m_pDelegateLayer->m_pMoveCard = NULL;
                }

            
            }else
            {
              // on click:
                CCLog("onclick");
                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                m_pDelegateLayer->m_pMoveCard = NULL;
                displayManifier();
            }
        
        }
        
    }
    

    
}

bool CPtBattleArrayItem::onSellBegin(CCTouch * pTouch, CCEvent *pEvent)
{
    bool bRet = false;
    if (m_bSellPanel)
    {
        return true;
    }else
    {
         bRet = getCopyCard(true);
        if (bRet == false)
        {
            displayManifier();
        }

    }
   
    return bRet;
}

void CPtBattleArrayItem:: onSellEnd(CCTouch *pTouch, CCEvent *pEvent)
{

    if (m_pDelegateLayer)
    {
                // onclick:
        if (m_pDelegateLayer->m_pSellPanel)
        {
            CPtDisPlayCard * displace =(CPtDisPlayCard *)(this->getDisplayView());
            if (m_pDelegateLayer->getTableClickMove())
            {
                // move:
                if (m_bSellPanel)
                {
                    return;
                }
                m_pDelegateLayer->m_pMoveCard->retain();
                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                if(displace)
                {
                        
                        if (displace->getCardData()->getInWhichBattleArray() != 0)
                        {
                            CCLog("this card is in battle array can't be sell");
                            m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                            m_pDelegateLayer->m_pMoveCard = NULL;
                            return;
                        }
                        
                        
                        CPtDisPlayCard *node = m_pDelegateLayer->m_pMoveCard;
                       
                 
                        if (node->getCardData()->getEnConsume() == false)
                        {
                            node->getCardData()->setEnConsume(true);
                            displace->setDead();
                            node->setInCardBagPointer(displace);
                            node->setIndex((int)getUserData());
                            node->setInCardBagPointer(displace);
                            CPtBattleArrayItem * item = CPtBattleArrayItem::create();
                            item->setSellPanel(true);
                            item->setDisplayView(node);
                            item->setTouchNode(node);
                            item->setDelegateLayer(m_pDelegateLayer);
                            m_pDelegateLayer->m_pSellPanel->getSellPackage()->getItems()->addObject(item);
                            m_pDelegateLayer->m_pSellPanel->getSellPackage()->reload();
                            CFightCard* priceCard =  displace->getCardData();
                            int coin = CPtTool::getSellPrice(priceCard->m_pCard->m_icard_price, priceCard->m_iCurrLevel, priceCard->m_pCard->m_nCard_star);
                            m_pDelegateLayer->m_pSellPanel->addCoin(coin);
                            if (m_pDelegateLayer->m_pSellPanel->getSellPackage()->getItems()->count() <= 3)
                            {
                              
                            }else if(m_pDelegateLayer->m_pSellPanel->getSellPackage()->getItems()->count() <= 6)
                            {
                                
                            }
                            else
                            {
                                m_pDelegateLayer->m_pSellPanel->getSellPackage()->getTableView()->setContentOffset(ccp(0,0));
                            }
                            
                            
                            
                        }else
                        {
                            CCLog("已经出售，不能再出售");
                           
                        }
                    
                                      
                }
                m_pDelegateLayer->m_pMoveCard->release();
                m_pDelegateLayer->m_pMoveCard = NULL;

                
                
            }else
            {
                // onclick:
                
                
                CCLog("onTableClick");
                if (m_bSellPanel && displace)
                {
                    if (m_bMove)
                    {
                        return;
                    }
                    displace->getCardData()->setEnConsume(false);
                    displace->getInCardBagPointer()->setLive();
                    // remove from sell package:
                    int sindex =(int)getUserData();
                    CCLog("index: %d", sindex);
                    
                    m_pDelegateLayer->m_pSellPanel->getSellPackage()->getItems()->removeObjectAtIndex(sindex);
                    m_pDelegateLayer->m_pSellPanel->getSellPackage()->reload();
                    CFightCard* priceCard =  displace->getCardData();
                    int coin = CPtTool::getSellPrice(priceCard->m_pCard->m_icard_price, priceCard->m_iCurrLevel, priceCard->m_pCard->m_nCard_star);
                    m_pDelegateLayer->m_pSellPanel->subCoin(coin);
                    
                    return;
                }
                
                if (m_bSellPanel == false)
                {
                    // onclick:
                    m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                    m_pDelegateLayer->m_pMoveCard = NULL;
                    displayManifier();
                }
                
            }
            
           
                
     
                
       

        }
        
    }

    
}

bool CPtBattleArrayItem::onEvolutionBegin(CCTouch *pTouch, CCEvent *pEvent)
{
    bool bRet = false;
    bRet = getCopyCard(true, false); //getCopyCard();
    return bRet;
    
}

void CPtBattleArrayItem::onEvolutionEnd(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_pDelegateLayer)
    {
        m_pDelegateLayer->m_pMoveCard->retain();
        m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
        // onclick:
        if (m_pDelegateLayer->m_pEvolutionPanel)
        {
            if (m_pDelegateLayer->getTableClickMove())
            {
                // on move:
                CPtDisPlayCard * displace =(CPtDisPlayCard *)(this->getDisplayView());
                
                CCLog("onTableMove");
                
                if(displace)
                {
                   
                    if (displace->getCardData()->getInWhichBattleArray() != 0)
                    {
                            CCLog("this card is in battle array should check rvc");
                            
                            //  return;
                    }
                        
                        
                    CPtDisPlayCard *node = m_pDelegateLayer->m_pMoveCard;
                    if (node->getCardData()->getEnConsume() == false)
                    {
                        node->getCardData()->setEnConsume(true);
                        if (node->getCardData()->getInWhichBattleArray() == 0)
                        {
                            displace->setDead();
                        }
                            
                        node->setInCardBagPointer(displace);
                        node->setIndex((int)getUserData());
                        node->setInCardBagPointer(displace);
                            
                        m_pDelegateLayer->m_pEvolutionPanel->addCard(node);
                        
                        m_pDelegateLayer->m_pMoveCard->release();
                        m_pDelegateLayer->m_pMoveCard = NULL;
                        return;
                            
                    }else
                    {
                            CCLog("已经选中，不需要再选");
                    }
                        
                   
                    
                    
                    
                }

            }else
            {
                // onclick:
                 displayManifier();
                
                
            }
            m_pDelegateLayer->m_pMoveCard->release();
            m_pDelegateLayer->m_pMoveCard = NULL;
            
            
            
        }
        
    }
    

}

bool CPtBattleArrayItem::onTeamArrayBegin(CCTouch *pTouch, CCEvent *pEvent)
{
    
    CCLog("CPtBattleArrayItem::onTeamArrayBegin");
    
    m_pDelegateLayer->m_bReplace = false;
    s_currentBattleArray = m_pDelegateLayer->panel->getBattleArray(pTouch);

    vector<vector<CFightCard *> > &fights = SinglePlayer::instance()->getCardBattleArray();
    // new add  or replace:
    
    CPtDisPlayCard * displace =(CPtDisPlayCard*)(this->getDisplayView());
    m_pDelegateLayer->m_pMoveCard = NULL; //displace;
    
    if(displace)
    {
        if (CPtTool::isInNode(displace, pTouch))
        {
            if(displace->getCardData()->getInWhichBattleArray()!=0)// (displace->getCardData()->getInBattleArray())
            {
                //
                CCLog("it's in battleArray");
               // displayManifier();
                return true;
            }
            int j =  m_pDelegateLayer->panel->getCurrentArray()->inTag-1;
            for (int i = 0; i < fights.size(); i++ )
            {
                if( j != i)
                {
                    vector<CFightCard*> &fight = fights.at(i);
                    for (int k = 0; k < fight.size(); k++)
                    {
                        if (fight.at(k))
                        {
                            if (fight.at(k)->m_User_Card_ID == displace->getCardData()->m_User_Card_ID)
                            {
                                
                                return false;
                            }
                        }
                    }
                }

            }
            // ondrag mode:
            // create new sprite:
            CCPoint worldPostion = displace->getPosition();
            worldPostion = displace->getParent()->convertToWorldSpace(worldPostion);
            m_pDelegateLayer->m_pMoveCard = displace->getCopy();
            m_pDelegateLayer->m_pMoveCard->setAnchorPoint(CCPointZero);
            m_pDelegateLayer->m_pMoveCard->setPosition(worldPostion);
            m_pDelegateLayer->addChild(m_pDelegateLayer->m_pMoveCard, 6000, 100);
            m_pDelegateLayer->m_cPrePoint = m_pDelegateLayer->m_pMoveCard->getPosition();
            (m_pDelegateLayer->m_pMoveCard)->setInCardBagPointer(displace);
            return true;
        }
        
    }
    return false;
}

void CPtBattleArrayItem::onTeamArrayEnd(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bMove== false)
    {
        // onclick
        if (m_pDelegateLayer->m_pMoveCard)
        {
            m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
            
        }
        displayManifier();
        return;

        
    }
    
    
    if (m_pDelegateLayer->m_pMoveCard)
    {
        m_pDelegateLayer->m_pMoveCard->retain();
        
    }
    
    
    CPtBattleArray * battles =  s_currentBattleArray;
    s_currentBattleArray = NULL;
    if (battles)
    {
        
        
        // remove:
        if (m_pDelegateLayer->m_pMoveCard)
        {
            CCRect rect1 = m_pDelegateLayer->m_pMoveCard->boundingBox();
            rect1.origin = battles->getParent()->convertToNodeSpace(rect1.origin);
            CCRect rect =  battles->boundingBox();
            
            
            if (!rect.intersectsRect(rect1))
            {
                
                m_pDelegateLayer->m_pMoveCard->release();
                
                //m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                removeAction();
                battles->updateBattleArray();
                return;
            }
            
          // 
            
            
        }
        
        
        
        
        //replace and add:
        
        int replace = battles->getReplaceCard(m_pDelegateLayer->m_pMoveCard);
        int index = battles->getInsertIndex();
        //同一个cardid 不能存在同一个阵容里面
        if (isSameCardId(battles->getBattleArray(), m_pDelegateLayer->m_pMoveCard->getCardData()->m_pCard->m_icard_id, replace))
        {
            // can't replace has same id:
            m_pDelegateLayer->m_pMoveCard->release();
            m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
            // tip:
            CCLog("tip:.....");
            addCardFail();
        }
        else
        {
            if (replace != -1)
            {
                 m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                CCLog("the replace: %d", replace);
                //
                battles->replaceCard(m_pDelegateLayer->m_pMoveCard, replace);
                m_pDelegateLayer->m_pMoveCard->release();
             //   battles->updateBattleArray();
                m_pDelegateLayer->m_pMoveCard->getInCardBagPointer()->setLogo(battles->inTag);
                
            }else if (battles->isAssistantCard(m_pDelegateLayer->m_pMoveCard, false))
            {
                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                battles->addCard(m_pDelegateLayer->m_pMoveCard, 4);
                m_pDelegateLayer->m_pMoveCard->getInCardBagPointer()->setLogo(battles->inTag);
                m_pDelegateLayer->m_pMoveCard->release();
               // battles->updateBattleArray();
                
            }
            else if (index != -1 )
            {
                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                battles->addCard(m_pDelegateLayer->m_pMoveCard, index);
                m_pDelegateLayer->m_pMoveCard->getInCardBagPointer()->setLogo(battles->inTag);
                m_pDelegateLayer->m_pMoveCard->release();
            }else
            {
                removeAction();
                //m_pDelegateLayer->m_pMoveCard->release();
            }

        }
               
        battles->updateBattleArray();
        
    }else
    {
      //  (m_pDelegateLayer->m_pMoveCard)->getInCardBagPointer()->setLive();
        if (m_pDelegateLayer->m_pMoveCard)
        {
             m_pDelegateLayer->m_pMoveCard->release();
            
        }
       
      //  m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
        removeAction();
    }
    

}

void CPtBattleArrayItem::createInfoLayer()
{
//    ((CPtDisPlayCard *)getDisplayView())->displayManifier();
    
    
    CFightCard * data = ((CPtDisPlayCard *)getDisplayView())->getCardData();
    CCardInfoLayer *layer = CCardInfoLayer::create(data);
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000);
}
void CPtBattleArrayItem::displayManifier()
{
   CPtDisPlayCard* current = ((CPtDisPlayCard *)getDisplayView());
    if (m_pDelegateLayer)
    {
        if (m_pDelegateLayer->getPreCardManifier())
        {
            if (m_pDelegateLayer->getPreCardManifier() != current)
            {
                m_pDelegateLayer->getPreCardManifier()->hideManifier();
                m_pDelegateLayer->setPreCardManifier(current);
                current->displayManifier();
            }
            
        }else
        {
            m_pDelegateLayer->setPreCardManifier(current);
            current->displayManifier();
        }
       
         PtSoundTool::playSysSoundEffect("UI_click.wav");
    }
  
}

bool CPtBattleArrayItem::getCopyCard(bool inEnsumeAble, bool inBattleArray)
{
    CPtDisPlayCard * displace =(CPtDisPlayCard*)(this->getDisplayView());
    m_pDelegateLayer->m_pMoveCard = NULL;
    if(displace)
    {
     
        {
            if(displace->getCardData()->getInWhichBattleArray()!=0)
            {
                if (inBattleArray)
                {
                    return false;
                }
                CCLog("it's in battleArray");
                
            }
            if (inEnsumeAble)
            {
                if (displace->getCardData()->getEnConsume())
                {
                    return false;
                }

            }
            // ondrag mode:
            // create new sprite:
            CCPoint worldPostion = displace->getPosition();
            worldPostion = displace->getParent()->convertToWorldSpace(worldPostion);
            m_pDelegateLayer->m_pMoveCard = displace->getCopy();
            m_pDelegateLayer->m_pMoveCard->setAnchorPoint(CCPointZero);
            m_pDelegateLayer->m_pMoveCard->setPosition(worldPostion);
            m_pDelegateLayer->addChild(m_pDelegateLayer->m_pMoveCard, 6000, 100);
            m_pDelegateLayer->m_cPrePoint = m_pDelegateLayer->m_pMoveCard->getPosition();
            (m_pDelegateLayer->m_pMoveCard)->setInCardBagPointer(displace);
            return true;
        }
        
    }
    return false;
}

bool CPtBattleArrayItem::isSameCardId(CPtDisPlayCard ** battleArray, const int &cardId, const int replaceIndex)
{
    bool bRet= false;
    if (battleArray)
    {
        for (int i = 0; i < CARDCOUNT; i++)
        {
            if (battleArray[i] && battleArray[i]->getCardData()->m_pCard->m_icard_id == cardId)
            {
                CCLog("the cardId:%d",battleArray[i]->getCardData()->m_pCard->m_icard_id  );
                if (replaceIndex != i)
                {
                    bRet = true;
                    break;
                }
             
           
            }
        }
    }
    return bRet;
}

void CPtBattleArrayItem::addCardFail()
{
    // tip:
    CCLabelTTF* label = CCLabelTTF::create("同CARDID的卡不能再一个阵容中出现", "Arial", 30);
    CCDirector::sharedDirector()->getRunningScene()->addChild(label, 1000);
    label->setPosition(ccp(512, 384));
    label->runAction(CCSequence::create(CCDelayTime::create(0.5f),CCCallFuncN::create(label, callfuncN_selector(CPtBattleArrayItem::removeCallBack)),NULL));
}

void CPtBattleArrayItem::removeCallBack(CCNode *pNode)
{
    pNode->removeFromParentAndCleanup(true);
}
void CPtBattleArrayItem::removeCallBack()
{
    if (m_pDelegateLayer && m_pDelegateLayer->m_pMoveCard)
    {
        m_pDelegateLayer->setActionEnable(false);
        m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
        m_pDelegateLayer->m_pMoveCard = NULL;
    }
    
}
void CPtBattleArrayItem::removeAction()
{
    if (m_pDelegateLayer && m_pDelegateLayer->m_pMoveCard)
    {
        m_pDelegateLayer->setActionEnable(true);
        CCPoint point = getDisplayView()->getPosition();
        point = getDisplayView()->getParent()->convertToWorldSpace(point);
        point = m_pDelegateLayer->m_pMoveCard->getParent()->convertToNodeSpace(point);
        m_pDelegateLayer->m_pMoveCard->runAction(CCSequence::create(CCMoveTo::create(0.1f, point),CCCallFunc::create(this, callfunc_selector(CPtBattleArrayItem::removeCallBack)),NULL));
    }
}
//implement class of CBattleArrayLayer

CBattleArrayLayer::CBattleArrayLayer()
{

    // 1. setting 2: enhance. 3: evolution 4: sell
    m_nCurrentTab = 1;
    m_pEvolutionPanel = NULL;
    m_pEnhancePanel = NULL;
    m_pSellPanel = NULL;
    panel = NULL;
    m_pMoveCard = NULL;
    m_pPreCardManifier = NULL;
    m_bActionEnable = false;
    
    m_bStarDec = false;
    m_bLevelDec = false;
    m_nSortType = 0; // default none sort
}
CBattleArrayLayer::~CBattleArrayLayer()
{
    
}

bool CBattleArrayLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());

        initCards();
        bRet = true;
    } while (0);
    return bRet ;
}



void CBattleArrayLayer::initCards()
{
    m_bTableClikEnable = false;
    m_pGamePlayer = SinglePlayer::instance();
    
    vector<CFightCard *> & bag =  m_pGamePlayer->getCardBagVector();
    
    CCArray * array = CCArray::create();
    for (int i = 0; i < bag.size() ; i++)
    {
        CFightCard * cardData = bag.at(i);
        CPtDisPlayCard * card = CPtDisPlayCard::Create(cardData);
        card->setAnchorPoint(CCPointZero);
        if (card)//(card)
        {
            int groupId = cardData->getInWhichBattleArray();
            if( groupId != 0) 
            {
                card->setLogo(groupId);
                if (CCardSettingScene::s_pBattleArrayCards)
                {
                    CCLog("user Id: %d", cardData->m_User_Card_ID);
                    CCardSettingScene::s_pBattleArrayCards->setObject(card, cardData->m_User_Card_ID);
                }
            }

            CPtBattleArrayItem * item = CPtBattleArrayItem::create();
            item->setDisplayView(card);
            item->setTouchNode(card);
            item->setDelegate(this);
            item->setDelegateLayer(this);
            array->addObject(item);
        }
    }

    
    m_pCards = CPtListViewWidget::create(array, CCSizeMake(476, 632), kCCScrollViewDirectionVertical , CCSizeMake(5, 2) , 3);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(true);
    
    m_pCards->setAnchorPoint(CCPointZero);
    m_pCards->setPosition(ccp(567, 10.0f));
    CCSprite *bg_header = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardBg_header.png"));
    bg_header->setAnchorPoint(CCPointZero);
    bg_header->setPosition(ccp(557, 635));
    addChild(bg_header, 2);
    addChild(m_pCards,1);
    
    //
    vector<vector<CFightCard *> > &tmpFightArray = m_pGamePlayer->getCardBattleArray();
    CCLog("%d",tmpFightArray.size());
    for (int i = 0; i < tmpFightArray.size(); i++)
    {
        vector<CFightCard * > &tmpFights = tmpFightArray.at(i);
        for (int j = 0; j < tmpFights.size(); j++)
        {
            if (tmpFights.at(j) == NULL)
            {
                CCLog("the fightArray: %d, %d is NULL",i,j);
            }
        
        }
        CCLog("the fightArray: size %d, %d",i,tmpFights.size());
    }
}

void CBattleArrayLayer::resetState()
{
    m_bActionEnable = false;
}

bool CBattleArrayLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    s_currentBattleArray = NULL;
    return false;
}
void CBattleArrayLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    setTableClickMove(true);
   
    if (m_pMoveCard == NULL)
    {
        return;
    }
    // ondrag mode:
    // move the copy Card:
    CPtTool::drag(m_pMoveCard, pTouch);
    if (getCurrentTab() == 1)
    {
        // onTeamArray:
        CPtBattleArray * battles = NULL;
        if (s_currentBattleArray)
        {
            battles =  s_currentBattleArray;
        }else
        {
            battles = panel->getCurrentArray();
        }
        
        if (m_pMoveCard)
        {
            if (battles)
            {
                if (s_currentBattleArray== NULL)
                {
                    s_currentBattleArray = battles;
                }
                
                battles->insertMoveCard(m_pMoveCard);
            }
        }

    }else if(getCurrentTab() == 2)
    {
        // onEnhance:
        
    }else if(getCurrentTab() == 3)
    {
        
    }else if(getCurrentTab() == 4)
    {
        
    }
   
}
void CBattleArrayLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
     
}
void CBattleArrayLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}



void CBattleArrayLayer::removeCallBack(CCNode *pNode)
{
    
  
}

void CBattleArrayLayer::addTeamArrayPanel()
{
    resetState();
    m_nCurrentTab = 1;
    panel = CPtBattleArrayPanel::create(CCSizeMake(534, 640),NULL, this);
    panel->setPosition(ccp(10,10));
    addChild(panel, 30, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(true);
}

void CBattleArrayLayer::addEnhance()
{
    resetState();
    m_nCurrentTab = 2;
    m_pEnhancePanel = CCardEnhanceLayer::create();
    addChild(m_pEnhancePanel, 1, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(true);
    m_pEnhancePanel->setCardBag(m_pCards);
   

}

void CBattleArrayLayer::addEvolution()
{
    resetState();
    m_nCurrentTab = 3;
    m_pEvolutionPanel = CCardEvolutionLayer::create();
    addChild(m_pEvolutionPanel, 1, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(true);
    m_pEvolutionPanel->setCardBag(m_pCards);
 
}

void CBattleArrayLayer::addSell()
{
    resetState();
    m_nCurrentTab = 4;
    m_pSellPanel = CCardSellLayer::create();
    addChild(m_pSellPanel, 1, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(true);
    m_pSellPanel->setCardBag(m_pCards);

}



void CBattleArrayLayer::removeLeft()
{
    CCLog("time begin: %d, %d", clock(), time(NULL));
    
    removeChildByTag(4000, true);
    panel = NULL;
    m_pEnhancePanel = NULL;
    m_pSellPanel = NULL;
    CCLog("time end: %d, %d", clock(), time(NULL ));
}

/*
 * @breif 排序
 * @param inType 1--> star 2--> level
 */

void CBattleArrayLayer::resort(int inType)
{
    if (m_pCards)
    {
        if (m_nSortType == 1)
        {
            if (inType == m_nSortType)
            {
                m_bStarDec = !m_bStarDec;
                m_pCards->setReverse(m_bStarDec);
                
            }else
            {
                m_nSortType = inType;
                m_pCards->setReverse(m_bLevelDec);
            }
        }else if(m_nSortType == 2)
        {
            if (inType == m_nSortType)
            {
                m_bLevelDec = !m_bLevelDec;
                m_pCards->setReverse(m_bLevelDec);
                
            }else
            {
                m_nSortType = inType;
                m_pCards->setReverse(m_bStarDec);
            }
            
        }else
        {
            m_nSortType = inType;
            if (inType == 1)
            {
                m_pCards->setReverse(m_bStarDec);
            }else
            {
                m_pCards->setReverse(m_bLevelDec);
            }
          
            
        }
      
        sortCards(m_pCards->getItems(), inType);
        m_pCards->reload();
    }
   
}

// static tool method:
// sort:升序排序

/*
 * @param inType = 1-->按升级排序  2-->按等级排序
 */
int compareWithStarLevel(const void* inCardOne, const void* inCardTwo, int const inType)
{
    int nRet = 0;
    CPtBattleArrayItem ** itemOne = (CPtBattleArrayItem**)(inCardOne);
    CFightCard * cardOne = ((CPtDisPlayCard*)(*itemOne)->getDisplayView())->getCardData();
    
    itemOne = (CPtBattleArrayItem**)inCardTwo;
    CFightCard * cardTwo = ((CPtDisPlayCard*)(*itemOne)->getDisplayView())->getCardData();
    if (inType == 1)
    {
        nRet = CPtTool::campare(cardOne->m_pCard->m_nCard_star, cardTwo->m_pCard->m_nCard_star,cardOne->m_iCurrExp,cardTwo->m_iCurrExp ,cardOne->m_User_Card_ID, cardTwo->m_User_Card_ID);
        
    }else if(inType == 2)
    {
        nRet = CPtTool::campare(cardOne->m_iCurrLevel,cardTwo->m_iCurrLevel,cardOne->m_pCard->m_nCard_star, cardTwo->m_pCard->m_nCard_star,cardOne->m_User_Card_ID, cardTwo->m_User_Card_ID);
    }
    return nRet;
}

int compareWithStar(const void* inCardOne, const void* inCardTwo)
{
    return compareWithStarLevel(inCardOne, inCardTwo, 1);
}


int compareWithLevel(const void*inCardOne, const void* inCardTwo)
{
    return compareWithStarLevel(inCardOne, inCardTwo, 2);

}

/*
 * @param inType = 1-->按升级排序  2-->按等级排序
 */
void sortCards(CCArray *item, int inType)
{
    if (inType == 1)
    {
         qsort(item->data->arr, item->data->num , sizeof(CPtBattleArrayItem*),compareWithStar);
        
    }else if(inType == 2)
    {
         qsort(item->data->arr, item->data->num , sizeof(CPtBattleArrayItem*),compareWithLevel);
    }
   
   
}

