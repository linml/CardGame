

//
//  CCardEvolutionLayer.cpp
//  91.cube
//
//  Created by phileas on 13-7-23.
//
//

#include "CCardEnhanceLayer.h"
#include "gameConfig.h"
#include "CPtTool.h"
#include "CPtLevelConfigData.h"
#include "gamePlayer.h"
#include "PtJsonUtility.h"
#include "PtHttpClient.h"
#include "CSaveConfirmLayer.h"
CCardEnhanceLayer::CCardEnhanceLayer()
{
    m_pSelectCard = NULL;
    m_nCurrentExp = 0;
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        m_pMaterialCards[i] = NULL;
    }
    m_nAddAtak = 0;
    m_nAddDef = 0;
    m_nAddHp = 0;
    m_nAddRvc = 0;
    m_nCurrentLevel = -1;
    m_pLevelConfig = SingleLevleConfigData::instance();
    m_nCostConin = 0;
    m_pPlayer = SinglePlayer::instance();
    m_pCardBag = NULL;
}

CCardEnhanceLayer::~CCardEnhanceLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
    resetMaterialCard();
}

bool CCardEnhanceLayer::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        initCCardEvolution();
        bRet = true;
    } while (0);
    return bRet;
}

bool CCardEnhanceLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CCardEnhanceLayer::ccTouchBegan");
    if(m_pSelectCard && CPtTool::isInNode(m_pSelectCard, pTouch))
    {
        return true;
    }
    
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        CCNode * tmp = m_pMaterialCards[i];
        if (tmp && CPtTool::isInNode(tmp, pTouch))
        {
            return true;
        }
    }
    CCNode * node = m_cMaps->getElementByTags("1,0,0");
    if (node&& CPtTool::isInNode(node, pTouch))
    {
        return true;
    }

    return  false;
}
void CCardEnhanceLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
        
}
void CCardEnhanceLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    handlerTouch(pTouch);
    
}
void CCardEnhanceLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLayer::ccTouchCancelled(pTouch, pEvent);
}

void CCardEnhanceLayer::initCCardEvolution()
{
    CCSprite * bg = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "cardsetting_bg.png"));
    bg->setScaleY(1.4f);
    
    addChild(bg, -1);
    bg->setPosition(ccp(10, 10));
    bg->setAnchorPoint(CCPointZero);
  
    m_nTouchTag = -1;
    m_cMaps = LayoutLayer::create();
    m_cMaps->retain();
    m_cMaps->initWithFile(this, CSTR_FILEPTAH(plistPath, "qianghua.plist"));
    
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-1);
    m_cMaps->getTouchRects(m_cTouches);
    
    
    updateTexture();
}

void CCardEnhanceLayer::handlerTouch(CCTouch * pTouch)
{
    CCLog("CCardEnhanceLayer::ccTouchBegan");
    if(m_pSelectCard && CPtTool::isInNode(m_pSelectCard, pTouch))
    {
        removeCard(-2);
        return;
    }
    
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        CCNode * tmp = m_pMaterialCards[i];
        if (tmp && CPtTool::isInNode(tmp, pTouch))
        {
            removeCard(i);
            return;
        }
    }
    
    CCNode * node = m_cMaps->getElementByTags("1,0,0");
    if (node&& CPtTool::isInNode(node, pTouch))
    {
        CCLog("save");
       // save();
        saveOnClick();
    }

}

int CCardEnhanceLayer::getEmptyCardSlide()
{
    if (m_pSelectCard == NULL)
    {
        return  -2;
    }
    
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i] == NULL)
        {
            return  i;
        }
    }
    return  -1;
}
void CCardEnhanceLayer::addCard(const int& inIndex, CPtDisPlayCard * inCard)
{
    
    
    if (inCard != NULL )
    {
        if ((inIndex >= 0 && inIndex < MATERIALCARDCOUNT) || inIndex == -2)
        {
            if (inIndex == -2)
            {
                addCardAction(inIndex, inCard);
                m_pSelectCard = inCard;
                m_nCurrentLevel = inCard->getCardData()->m_iCurrLevel;

            }else if(m_pMaterialCards[inIndex] == NULL)
            {
                // data change:
                m_pMaterialCards[inIndex] = inCard;
                inCard->getCardData()->setEnConsume(true);
                // action:
                addCardAction(inIndex, inCard);
            }
            updateEnhance();
        }
    
    }
}

void CCardEnhanceLayer::removeCard(const int &inIndex)
{
    if (inIndex == -2)
    {
        removeCardAction(inIndex);
        m_pSelectCard = NULL;
        m_nCurrentLevel = -1;
    }
    else if(inIndex >= 0 && inIndex < MATERIALCARDCOUNT && m_pMaterialCards[inIndex])
    {
        m_pMaterialCards[inIndex]->getCardData()->setEnConsume(false);
        removeCardAction(inIndex);
        m_pMaterialCards[inIndex] = NULL;
    }
    updateEnhance();
}
bool CCardEnhanceLayer::canClickCard(CPtDisPlayCard * inCard)
{
    bool bRet = true;
    if (m_pSelectCard)
    {
        if (m_pSelectCard->getCardData()->m_User_Card_ID == inCard->getCardData()->m_User_Card_ID)
        {
            bRet =  false;
        }
        if (inCard->getCardData()->getInWhichBattleArray() != 0)
        {
           bRet = false;
        }

    }

    
    return bRet;
}

void CCardEnhanceLayer::addCardAction(const int& inIndex, CPtDisPlayCard * inCard)
{
    if (inIndex == -2)
    {
        inCard->setPosition(ccp(10, 10));
        inCard->setAnchorPoint(CCPointZero);
        m_cMaps->getElementByTags("1,0, 137")-> addChild(inCard);
        return;
    }
    
    int i = (6+inIndex)%8 + 18;
    int array[3] = {1,0,0};
    array[2] = i;
    CCNode * node = m_cMaps->getElementByTags(array,3);
    if (node)
    {
        CCPoint point = node->boundingBox().origin;
        point.x += 3;
        point.y += 4;
        inCard->setPosition(point);
        inCard->setAnchorPoint(CCPointZero);
        inCard->setScale(0.71f);
        addChild(inCard, 22, 8000+inIndex);
    }
    
}
void CCardEnhanceLayer::removeCardAction(const int& inIndex)
{
    if (inIndex == -2)
    {
        m_cMaps->getElementByTags("1,0, 137")->removeChild(m_pSelectCard, true);
        return;
    }
    else if(inIndex >= 0 && inIndex < MATERIALCARDCOUNT)
    {
        removeChildByTag(8000+inIndex, true);
    }
        
}

void CCardEnhanceLayer::updateEnhance()
{
    updateData();
    updateTexture();
}
void CCardEnhanceLayer::updateData()
{

    m_nCurrentExp = 0;
    m_nAddAtak  = 0;
    m_nAddDef  = 0;
    m_nAddHp = 0;
    m_nAddRvc = 0;
    m_nCostConin = 0;
    // get suport exp:
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        CGamesCard * tmp = m_pMaterialCards[i];
        if (tmp)
        {
            m_nCurrentExp += tmp->getCardData()->getSupportValue(1);
            m_nCostConin += tmp->getCardData()->getCostConin();
        }
    }
    
    
    if (m_pSelectCard)
    {
        int  exp = m_pSelectCard->getCardData()->m_iCurrExp;
        exp += m_nCurrentExp;
        int currentLevel = m_pSelectCard->getCardData()->m_iCurrLevel;
        int level = m_pLevelConfig->getCurrentLevel(exp, currentLevel);
        CCLog("current-----: %d, level: %d, currentLevel: %d", exp, level, m_nCurrentLevel);
        if (level > currentLevel && level > 1)
        {
            CCLog("can qianghua");
            CFightCard * tmp = m_pSelectCard->getCardData();
            m_nAddAtak = tmp->getAddValue(level, 1) - tmp->m_attack;
            m_nAddDef =tmp->getAddValue(level, 2)- tmp->m_defend;
            m_nAddHp  = tmp->getAddValue(level, 3) - tmp->m_iHp;
            m_nAddRvc = tmp->getAddValue(level, 4) - 0;
            m_nCurrentLevel = level;
        }
       
    }else
    {
        
        m_nAddAtak  = 0;
        m_nAddDef  = 0;
        m_nAddHp = 0;
        m_nAddRvc = 0;
        m_nCostConin = 0;
    }

}
void CCardEnhanceLayer::updateTexture()
{
    int maxLevel = 0;
    if (m_pSelectCard)
    {
        m_nCurrentAtk = m_pSelectCard->getCardData()->m_attack;
        m_nCurrentDef = m_pSelectCard->getCardData()->m_defend;
        m_nCurrentHp = m_pSelectCard->getCardData()->m_iCurrHp;
        m_nCurrentRv = m_pSelectCard->getCardData()->m_nLeadShip;
        maxLevel = m_pSelectCard->getCardData()->m_pCard->m_ileve_max;
    }
    else
    {
        m_nCurrentAtk = 0;
        m_nCurrentDef = 0;
        m_nCurrentHp = 0;
        m_nCurrentRv = 0;
       
    }
        //init zero

   
    int array[4]={1,0,1,4001};
    char buff[30] = {0};
    CCLabelTTF * label = NULL;
    // atak
    array[3]=4001;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
       
        sprintf(buff, "ATK: %d (+ %d)",m_nCurrentAtk, m_nAddAtak);
        label->setString(buff);
    }
    
    // hp
  
    array[3]=4002;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
        
        sprintf(buff, "HP: %d (+ %d)",m_nCurrentHp, m_nAddHp);
        label->setString(buff);
    }

    // def:
    array[3]=4003;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
        
        sprintf(buff, "DEF: %d (+ %d)",m_nCurrentDef , m_nAddDef);
        label->setString(buff);
    }

    // rvc:
     array[3]=4004;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
       
        sprintf(buff, "RVC: %d (+ %d)",m_nCurrentRv, m_nAddRvc);
        label->setString(buff);
    }
    
    array[2]=160;
    array[3]= 4007;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label)
    {
        if (m_nCurrentLevel == -1)
        {
             label->setString("");
        }else
        {
             sprintf(buff, "%d/%d",m_nCurrentLevel, maxLevel);
             label->setString(buff);
        }
       
       
    }

    // conin:
    array[2]=129;
    array[3]=4005;
    label = (CCLabelTTF *)m_cMaps->getElementByTags(array, 4);
    if (label && maxLevel != -1)
    {
        sprintf(buff, "%d",m_nCostConin);
        label->setString(buff);
    }
    

}


void CCardEnhanceLayer::clearMaterialCard()
{
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i])
        {
            m_pMaterialCards[i]->removeFromParentAndCleanup(true);
            m_pMaterialCards[i] = NULL;
        }
    }
    if (m_pSelectCard)
    {
        
    }
    updateEnhance();
    
}
void CCardEnhanceLayer::resetMaterialCard()
{
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i])
        {
            m_pMaterialCards[i]->getCardData()->setEnConsume(false);
        }
    }

}
bool CCardEnhanceLayer::verifyConin()
{
    int conin = m_pPlayer->getCoin();
    bool bRet = false;
    if (m_nCostConin <= conin)
    {
        bRet = true;
    }
    return bRet;
}



void CCardEnhanceLayer:: save()
{
       // save data:
    m_pPlayer->ReduceCoin(m_nCostConin);
    m_nCostConin = 0;
    
    vector<CFightCard *> &r_CardBag = m_pPlayer->m_vCardBag;
    CCArray * array = m_pCardBag->getItems();
    list<int> ids;

    int index = -1;
    for (int i = 0; i < MATERIALCARDCOUNT; i ++)
    {
        if (m_pMaterialCards[i])
        {
            index = m_pMaterialCards[i]->getIndex();
            ids.push_back(index);
        }
    }
    ids.sort();
    for (std::list<int>::reverse_iterator i = ids.rbegin(); i != ids.rend(); i++)
    {
        index = *i;
        CCLog("index:%d",index);
        array->removeObjectAtIndex(index);
        std::vector<CFightCard*>::iterator j = r_CardBag.begin()+index;
        r_CardBag.erase(j);
    }
    
    
    if(m_pSelectCard)
    {
        m_pSelectCard->updateToLevel(m_nCurrentLevel);
        m_pSelectCard->getInCardBagPointer()->updateToLevel(m_nCurrentLevel);
    
    }
    m_pCardBag->reload();
 
    clearMaterialCard();
}

void CCardEnhanceLayer::saveOnClick()
{
    if (m_pSelectCard == 0 && m_nCostConin == 0)
    {
        CCLog("没有材料卡或被强化的卡");
        return;
    }
    bool bRet =verifyConin();
    if (bRet == false)
    {
        CCLog("金币不足");
        return ;
    }
    char buff[200]={0};
    char param[200]={0};
    sprintf(buff, "sig=2ac2b1e302c46976beaab20a68ef95&type=%d&info={\"card_id\":\"%d\",\"other\":[",0,m_pSelectCard->getCardData()->m_User_Card_ID);
    char tmp [20];
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        if (m_pMaterialCards[i])
        {
            sprintf(tmp, "\"%d_%d\",",m_pMaterialCards[i]->getCardData()->m_User_Card_ID, m_pMaterialCards[i]->getCardData()->getInWhichBattleArray());
            strncat(param, tmp, strlen(tmp));
        }
    }
    CCLog("%s", param);
    strncat(buff, param, strlen(param)-1);
    strncat(buff, "]}", strlen(param)-1);
    CCLog("%s",buff);
   
    ADDHTTPREQUESTPOSTDATA("http://cube.games.com/api.php?m=Card&a=cardUpGrade&uid=194", "cardenhance","enhancel", buff, callfuncO_selector(CCardEnhanceLayer::receiveCallBack));
    
}
void CCardEnhanceLayer::receiveCallBack(CCObject *pSender)
{

    char *buffer = (char *) pSender;
    CCLog("callback: %s", buffer);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "cardenhance");

    CCDictionary* dic = PtJsonUtility::JsonStringParse(buffer);
    CSaveConfirmLayer * layer = new CSaveConfirmLayer();
    int result = GameTools::intForKey("code", dic);
   
  
    if (result == 0)
    {
        save();
        layer->setUserData((void *) 30001);
    }else
    {
        layer->setUserData((void *) 30002);
        CCLog("error");
    }
    
    delete [] buffer;
  
    layer->init();
    layer->autorelease();
    CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 2000);

}