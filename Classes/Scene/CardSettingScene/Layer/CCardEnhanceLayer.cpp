

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
}

CCardEnhanceLayer::~CCardEnhanceLayer()
{
    if (m_cMaps)
    {
        m_cMaps->release();
    }
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
void CCardEnhanceLayer::addCard(const int& inIndex, CGamesCard * inCard)
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
    else if(inIndex >= 0 && inIndex < MATERIALCARDCOUNT)
    {
        removeCardAction(inIndex);
        m_pMaterialCards[inIndex] = NULL;
    }
    updateEnhance();
}

void CCardEnhanceLayer::addCardAction(const int& inIndex, CGamesCard * inCard)
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
    // get suport exp:
    for (int i = 0; i < MATERIALCARDCOUNT; i++)
    {
        CGamesCard * tmp = m_pMaterialCards[i];
        if (tmp)
        {
            m_nCurrentExp += tmp->getCardData()->getSupportValue(1);
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
//            for (int j = currentLevel+1; j <= level ; j++)
//            {
//                m_nAddAtak += tmp->getAddValue(j, 1);
//                m_nAddDef +=tmp->getAddValue(j, 2);
//                m_nAddHp += tmp->getAddValue(j, 3);
//                m_nAddRvc += tmp->getAddValue(j, 4);
//            }
           
           
                m_nAddAtak = tmp->getAddValue(level, 1) - tmp->m_attack;
                m_nAddDef =tmp->getAddValue(level, 2)- tmp->m_defend;
                m_nAddHp  = tmp->getAddValue(level, 3) - tmp->m_iHp;
                m_nAddRvc = tmp->getAddValue(level, 4) - 0;
        

           
        }
        m_nCurrentLevel = level;
    }else
    {
        
        m_nAddAtak  = 0;
        m_nAddDef  = 0;
        m_nAddHp = 0;
        m_nAddRvc = 0;
    }

}
void CCardEnhanceLayer::updateTexture()
{
    if (m_pSelectCard)
    {
        m_nCurrentAtk = m_pSelectCard->getCardData()->m_attack;
        m_nCurrentDef = m_pSelectCard->getCardData()->m_defend;
        m_nCurrentHp = m_pSelectCard->getCardData()->m_iCurrHp;
        m_nCurrentRv = m_pSelectCard->getCardData()->m_nLeadShip;
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
             sprintf(buff, "%d/100",m_nCurrentLevel);
             label->setString(buff);
        }
       
       
    }

    

}