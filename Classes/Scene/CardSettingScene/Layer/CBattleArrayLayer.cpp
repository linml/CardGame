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
static CPtBattleArray * s_currentBattleArray = NULL;

// implement class of CPtBattleArrayItem:



bool CPtBattleArrayItem::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
 
    m_bMove = false;
 
    CCLog("CPtBattleArrayItem::ccTouchBegan");
    if(m_pDelegateLayer ==  NULL)
    {
        return false;
        
    }
    

    
    if (m_pDelegateLayer->getTableClickEnable() == false)
    {
        m_pDelegateLayer->m_bReplace = false;
        s_currentBattleArray = m_pDelegateLayer->panel->getBattleArray(pTouch);
    }
      
    // new add  or replace:
    
    CPtDisPlayCard * displace =(CPtDisPlayCard*)(this->getDisplayView());
    m_pDelegateLayer->m_pMoveCard = displace;

    if(displace)
    {
        if (CPtTool::isInNode(displace, pTouch))
        {
           
            // onclik mode:
            if (m_pDelegateLayer->getTableClickEnable())
            {
                m_pDelegateLayer->setTableClickMove(false);
                return true;
            }
            
            
            if(displace->getCardData()->getInWhichBattleArray()!=0)// (displace->getCardData()->getInBattleArray())
            {
                //
                CCLog("it's in battleArray");
                return false;
            }
            // ondrag mode:
            // create new sprite:
            CCPoint worldPostion = displace->getPosition();
            worldPostion = displace->getParent()->convertToWorldSpace(worldPostion);
            m_pDelegateLayer->m_pMoveCard = displace->getCopy();
            displace->setDead();
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


void CPtBattleArrayItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_pDelegateLayer)
    {
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
                (m_pDelegateLayer->m_pMoveCard)->getInCardBagPointer()->setLive();
                m_pDelegateLayer->m_pMoveCard->release();
                m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
                
                battles->updateBattleArray();
                return;
            }
            m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
            
            
        }
      

        
        //replace and add:
        
        int replace = battles->getReplaceCard(m_pDelegateLayer->m_pMoveCard);
        int index = battles->getInsertIndex();
        if (replace != -1)
        {
            CCLog("the replace: %d", replace);
          
            battles->replaceCard(m_pDelegateLayer->m_pMoveCard, replace);
            m_pDelegateLayer->m_pMoveCard->release();
            battles->updateBattleArray();
    
        }else if (battles->isAssistantCard(m_pDelegateLayer->m_pMoveCard, false))
        {
            battles->addCard(m_pDelegateLayer->m_pMoveCard, 4);
            m_pDelegateLayer->m_pMoveCard->release();
            battles->updateBattleArray();

        }
        else if (index != -1 )
        {
            
             battles->addCard(m_pDelegateLayer->m_pMoveCard, index);
             m_pDelegateLayer->m_pMoveCard->release();
        }else
        {
            (m_pDelegateLayer->m_pMoveCard)->getInCardBagPointer()->setLive();
            m_pDelegateLayer->m_pMoveCard->release();
        }
        
        battles->updateBattleArray();
        
    }else
        {
            (m_pDelegateLayer->m_pMoveCard)->getInCardBagPointer()->setLive();

            m_pDelegateLayer->m_pMoveCard->release();
            m_pDelegateLayer->m_pMoveCard->removeFromParentAndCleanup(true);
       
        }
    
    
    }
}

void CPtBattleArrayItem::onEnhanceBegin(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPtBattleArrayItem::onEnhanceEnd(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("---------%d",(int) getUserData());
    if (m_pDelegateLayer)
    {
        CCLog("onTableClick");
        // onclick:
        if (m_pDelegateLayer->m_pEnhancePanel && m_pDelegateLayer->getTableClickEnable())
        {
            if (m_pDelegateLayer->getTableClickMove())
            {
                return;
            }
            int i = m_pDelegateLayer->m_pEnhancePanel->getEmptyCardSlide();
            if (i != -1)
            {
                CPtDisPlayCard * displace =(CPtDisPlayCard *)(this->getDisplayView());
                
                
                if(displace)
                {
                    if (CPtTool::isInNode(displace, pTouch))
                    {
                        
                        
                        
                        if (displace->getCardData()->getInWhichBattleArray() != 0) // (displace->getCardData()->getInBattleArray())
                        {
                            //
                            CCLog("it's in battleArray");
                            
                        }
                        // ondrag mode:
                        // create new sprite:
                        
                        CPtDisPlayCard *node = displace->getCopy();
                        if ( m_pDelegateLayer->m_pEnhancePanel->canClickCard(node) && node->getCardData()->getEnConsume() == false)
                        {
                            node->setIndex((int)getUserData());
                            m_pDelegateLayer->m_pEnhancePanel->addCard(i, node);
                            node->setInCardBagPointer(displace);
                        }else
                        {
                            CCLog("该为阵容中的卡，不能用作材料卡");
                        }
                        
                        
                    }
                    
                }
                
                
            }
        }
        
    }
    

    
}

void CPtBattleArrayItem:: onSellEnd(CCTouch *pTouch, CCEvent *pEvent)
{

    if (m_pDelegateLayer)
    {
                // onclick:
        if (m_pDelegateLayer->m_pSellPanel && m_pDelegateLayer->getTableClickEnable())
        {
            if (m_pDelegateLayer->getTableClickMove())
            {
                return;
            }
            
            CPtDisPlayCard * displace =(CPtDisPlayCard *)(this->getDisplayView());
            
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
               
                return;
            }

                
            if(displace)
            {
                if (CPtTool::isInNode(displace, pTouch))
                {
                   
                    if (displace->getCardData()->getInWhichBattleArray() != 0)
                    {
                        CCLog("this card is in battle array can't be sell");
                        return;
                    }
                   
                    
                    CPtDisPlayCard *node = displace->getCopy();
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
                        if (m_pDelegateLayer->m_pSellPanel->getSellPackage()->getItems()->count() <= 3)
                        {
                         //   m_pDelegateLayer->m_pSellPanel->getSellPackage()->getTableView()->setContentOffset(ccp(0,244));
                        }else if(m_pDelegateLayer->m_pSellPanel->getSellPackage()->getItems()->count() <= 6)
                        {
                         //   m_pDelegateLayer->m_pSellPanel->getSellPackage()->getTableView()->setContentOffset(ccp(0,38));

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
                    
            }
                
       

        }
        
    }

    
}

void CPtBattleArrayItem::onEvolutionEnd(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if (m_pDelegateLayer)
    {
        // onclick:
        if (m_pDelegateLayer->m_pEvolutionPanel && m_pDelegateLayer->getTableClickEnable())
        {
            if (m_pDelegateLayer->getTableClickMove())
            {
                return;
            }
            
            CPtDisPlayCard * displace =(CPtDisPlayCard *)(this->getDisplayView());
            
            CCLog("onTableClick");
                       
            if(displace)
            {
                if (CPtTool::isInNode(displace, pTouch))
                {
                    
                    if (displace->getCardData()->getInWhichBattleArray() != 0)
                    {
                        CCLog("this card is in battle array should check rvc");
                      //  return;
                    }
                    
                    
                    CPtDisPlayCard *node = displace->getCopy();
                    if (node->getCardData()->getEnConsume() == false)
                    {
                        node->getCardData()->setEnConsume(true);
                        displace->setDead();
                        node->setInCardBagPointer(displace);
                        node->setIndex((int)getUserData());
                        node->setInCardBagPointer(displace);
                                              
                        m_pDelegateLayer->m_pEvolutionPanel->addCard(node);
                        
                    }else
                    {
                        CCLog("已经选中，不需要再选");
                    }
                    
                    
                }
                
            }
            
            
            
        }
        
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
    vector<CFightCard *> & bag =  m_pGamePlayer->m_vCardBag;
    CCArray * array = CCArray::create();
    for (int i = 0; i < bag.size() ; i++)
    {
        CFightCard * cardData = bag.at(i);
    
        
     
        CPtDisPlayCard * card = CPtDisPlayCard::Create(cardData);
        card->setAnchorPoint(CCPointZero);
        if (card)//(card)
        {
            if(cardData->getInWhichBattleArray() != 0) //(cardData->getInBattleArray())
            {
                card->setDead();
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
    vector<vector<CFightCard *> > &tmpFightArray = m_pGamePlayer->m_vvBattleArray;
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



bool CBattleArrayLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    s_currentBattleArray = NULL;
    return false;
}
void CBattleArrayLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    // onclik mode:
    if (m_bTableClikEnable)
    {
        setTableClickMove(true);
        return;
    }
    
    // ondrag mode:
    // move the copy Card:
    CPtTool::drag(m_pMoveCard, pTouch);
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
    m_nCurrentTab = 1;
    panel = CPtBattleArrayPanel::create(CCSizeMake(534, 640));
    panel->setPosition(ccp(10,10));
    addChild(panel, 30, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(true);
}

void CBattleArrayLayer::addEnhance()
{
    m_nCurrentTab = 2;
    m_pEnhancePanel = CCardEnhanceLayer::create();
    addChild(m_pEnhancePanel, 1, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(false);
    m_pEnhancePanel->setCardBag(m_pCards);

}

void CBattleArrayLayer::addEvolution()
{
    m_nCurrentTab = 3;
    m_pEvolutionPanel = CCardEvolutionLayer::create();
    addChild(m_pEvolutionPanel, 1, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(false);
    m_pEvolutionPanel->setCardBag(m_pCards);}

void CBattleArrayLayer::addSell()
{
    m_nCurrentTab = 4;
    m_pSellPanel = CCardSellLayer::create();
    addChild(m_pSellPanel, 1, 4000);
    ((TableView *)(m_pCards->getTableView()))->setDelayMode(false);
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



