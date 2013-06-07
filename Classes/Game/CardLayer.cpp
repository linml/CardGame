//
//  CardLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-6-3.
//
//

#include "CardLayer.h"
#include "Utility.h"
#include "PtMapUtility.h"



CCardLayer::CCardLayer()
{
    m_cCardSpriteProperty=NULL;
}

CCardLayer::~CCardLayer()
{
    
}

bool CCardLayer::setSpriteProperty(CCardSprite *spriteProperty)
{
    if(spriteProperty==NULL)
    {
        return  false;
    }
    this->m_cCardSpriteProperty=spriteProperty;
    return true;
}

bool CCardLayer::initWithMapFile(const char *fileName)
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	CCLog("CCardLayer::initWithMapFile");
	bool bRet = false;
	do
	{
        setUserData((void *)fileName);
		char *tmp = NULL;
		CC_BREAK_IF(!CCLayer::init());
		tmp = (char *)this->getUserData();
        mapFileName=tmp;
		PtMapUtility::addChildFromScript(this,mapFileName.c_str());
        bRet = true;
	} while (0);
    showStarLevel(m_cCardSpriteProperty->m_cardData.m_unCardLevel);
    showName(m_cCardSpriteProperty->m_cardData.m_sPlayerCardName);
    return bRet;
}

CCSprite *CCardLayer::getHero()
{
    CCLayer *maplayer=getMapByTag();

    if (maplayer&&maplayer->getChildByTag(0)->getChildByTag(DEF_FIGHT_CARD_HERO))
    {
        return (CCSprite *)(maplayer->getChildByTag(0)->getChildByTag(DEF_FIGHT_CARD_HERO));
    }
    assert(maplayer->getChildByTag(0)->getChildByTag(DEF_FIGHT_CARD_HERO)!=NULL && "CCSprite Hero iSnULL");
    return NULL;
}

CCLayer *CCardLayer::getMapByTag()
{
    return (CCLayer *)getChildByTag(1);
}

void CCardLayer::showStarLevel(int level)
{
    CCLayer *maplayer=getMapByTag();
    if(m_cCardSpriteProperty &&m_cCardSpriteProperty>0)
    {
        for (int i=0; i<5; i++)
        {
            if(maplayer->getChildByTag(0))
            {
                if(maplayer->getChildByTag(0)->getChildByTag(DEF_FIGHT_CARD_STARLEVEL_BEGIN+i))
                {
                    maplayer->getChildByTag(0)->getChildByTag(DEF_FIGHT_CARD_STARLEVEL_BEGIN+i)->setVisible(false);  
                }
                else{
                    CCLog("%s:%d",__FILE__,__LINE__);
                }
                
            }
            else{
                CCLog("error1");
            }
         
        }
        for (int i=0; i<level; i++)
        {
          maplayer->getChildByTag(0)->getChildByTag(DEF_FIGHT_CARD_STARLEVEL_BEGIN+i)->setVisible(true);
        }
    }
}

void CCardLayer::showFightLayer()
{
    
}

void CCardLayer::showName(std::string str)
{
    CCNode *node=Utility::getNodeByTag(this, "1,0,0"); //get label
    if(node && node->getChildByTag(DEF_FIGHT_CARD_NAME))
    {
        CCLabelTTF *label=(CCLabelTTF *)(node->getChildByTag(DEF_FIGHT_CARD_NAME));
               label->setString(str.c_str());
    }
}

void CCardLayer::animationBeiGongji(int isRight)
{
     CCSprite *sprite=getHero();
     sprite->runAction(CCSequence::create(CCMoveBy::create(0.2f, ccp((25*isRight), 0)),CCMoveBy::create(0.1f, ccp(-25*isRight,0)),NULL));
}

void CCardLayer::animationHeroMove(int fangxiang)
{
    CCSprite *sprite=getHero();
    if(sprite)
    {
        sprite->runAction(CCSequence::create(CCMoveBy::create(0.2f, ccp((-25)*fangxiang, 0)),CCMoveBy::create(0.1f, ccp(50*fangxiang, 0)),CCMoveBy::create(0.1f, ccp(-25*fangxiang, 0)),NULL));
    }
}

void CCardLayer::animationHeroMoveLeft()
{
    animationHeroMove(-1);
}

void CCardLayer::animationHeroMoveRight()
{
    animationHeroMove(1);
}

void CCardLayer::showCardWholeLayer()
{
    
}

int CCardLayer::getHp()
{
    return 0;
}

int CCardLayer::getMp()
{
    return 0;    
}


