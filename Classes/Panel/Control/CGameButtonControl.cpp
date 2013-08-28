//
//  CGameButtonControl.cpp
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#include "CGameButtonControl.h"
#include "cocos2d.h"
using namespace std;

#define FONTSELECTCOLOR ccc3(255,255,255)
#define FONTNORMALCOLOR ccc3(255,228,120)
CGameButtonControl::CGameButtonControl():CButtonControl()
{
    m_cFontNormalColor=FONTNORMALCOLOR;
}

CGameButtonControl::~CGameButtonControl()
{
    
    
}

CCRect CGameButtonControl::rect()
{
    return this->boundingBox();
}


CGameButtonControl *CGameButtonControl::createButton(TEXTPOSTION textPosion,const char *text,const char *normalPng,const char *selectPng)
{
    CGameButtonControl *game=new CGameButtonControl();
    if(!game || !game->init(textPosion, text, normalPng, selectPng))
    {
        delete  game;
        return  NULL;
    }
    game->autorelease();
    
    return game;
}
//CGameButtonControl *CGameButtonControl::createButton(TEXTPOSTION textPosion,cocos2d::CCLabelTTF *labelttf,const char *normalPng,const char *selectPng)
//{
//    
//}

void CGameButtonControl::setFontColor(ccColor3B csColor)
{
    m_cFontNormalColor=csColor;
}

CCLabelTTF *CGameButtonControl::getTextLabel()
{
    return (CCLabelTTF *)getChildByTag(100);
}

bool CGameButtonControl::init(TEXTPOSTION textPosion,const char *text,const char *normalPng,const char *selectPng)
{
    CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(normalPng);
    assert(frame!=NULL);
    setNormalFrame(frame);
    frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selectPng);
    setSelectFrame(frame);
    setAnchorPoint(ccp(0.5,0.5));
    initWithSpriteFrame(m_pNormalFrame);
    CCLabelTTF *label=CCLabelTTF::create(text, "Arial", 15);
    addChild(label,1,100);
    label->setColor(m_cFontNormalColor);
    CCSize contextSize=getContentSize();
    switch (textPosion) {
        case TEXTOP :
            label->setPosition(ccp(contextSize.width/2,contextSize.height/2+10));
            break;
        case TEXTMID :
            label->setPosition(ccp(contextSize.width/2,contextSize.height/2));
            break;
        case TEXTBOTTOM:
            label->setPosition(ccp(contextSize.width/2,-contextSize.height/2-10));
            break;
        case TEXTLEFT:
            label->setPosition(ccp(contextSize.width/2-20,contextSize.height/2));
            break;
        case TEXTRIGHT :
            break;
        default:
            break;
    }
    //添加色
    return true;
}

void CGameButtonControl::selected()
{
    if(getChildByTag(100))
    {
        //CCLabelBMFont *label=(CCLabelBMFont *)getChildByTag(GAMEBUTTONLABELTAG);
        CCLabelTTF *label=(CCLabelTTF*)getChildByTag(100);
        label->setColor(FONTSELECTCOLOR);
    }
    setDisplayFrame(m_pSelectFrame);
}

void CGameButtonControl::unselected()
{
    if(getChildByTag(100))
    {
        //CCLabelBMFont *label=(CCLabelBMFont *)getChildByTag(GAMEBUTTONLABELTAG);
        CCLabelTTF *label=(CCLabelTTF*)getChildByTag(100);
        label->setColor(m_cFontNormalColor);
    }
    setDisplayFrame(m_pNormalFrame );
}