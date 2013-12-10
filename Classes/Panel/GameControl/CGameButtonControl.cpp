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

#define TAG_TEXTTAG 100

CGameButtonControl::CGameButtonControl():CButtonControl()
{
    m_cFontNormalColor=FONTNORMALCOLOR;
    m_bIsCanTouch=true;
    m_bIsEnabled=true;
}

CGameButtonControl::~CGameButtonControl()
{
    
    
}

CCRect CGameButtonControl::rect()
{
    return this->boundingBox();
}


CGameButtonControl *CGameButtonControl::createButton(TEXTPOSTION textPosion,const char *text,const char *normalPng,const char *selectPng, const char *disablePng/*=NULL*/)
{
    CGameButtonControl *game=new CGameButtonControl();
    if(!game || !game->init(textPosion, text, normalPng, selectPng, disablePng))
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
void CGameButtonControl::setEnable(bool flag)
{
    if (!flag) {
        if (getChildByTag(TAG_TEXTTAG)) {
            ((CCLabelTTF *)getChildByTag(TAG_TEXTTAG))->setColor(ccc3(150,150,150));
        }
        
        m_bIsCanTouch=false;
    }
    else
    {
        m_bIsCanTouch=true;
    }
}
void CGameButtonControl::setFontColor(ccColor3B csColor)
{
    m_cFontNormalColor=csColor;
    if (getChildByTag(TAG_TEXTTAG)) {
        ((CCLabelTTF *)getChildByTag(TAG_TEXTTAG))->setColor(m_cFontNormalColor);
    }

}

void CGameButtonControl::setText(const char * str)
{
    if (getChildByTag(TAG_TEXTTAG)) {
        ((CCLabelTTF *)getChildByTag(TAG_TEXTTAG))->setString(str);//(m_cFontNormalColor);
    }
}

CCLabelTTF *CGameButtonControl::getTextLabel()
{
    return (CCLabelTTF *)getChildByTag(TAG_TEXTTAG);
}

bool CGameButtonControl::init(TEXTPOSTION textPosion,const char *text,const char *normalPng,const char *selectPng,const char *disablePng/*=NULL*/)
{
    CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(normalPng);
    assert(frame!=NULL);
    setNormalFrame(frame);
    frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(selectPng);
    setSelectFrame(frame);
    if (disablePng)
    {
         frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(disablePng);
         setDisableFrame(frame);
    }
   
    
    setAnchorPoint(ccp(0.5,0.5));
    initWithSpriteFrame(m_pNormalFrame);
    CCLog("text:%s",text);
    if(strlen(text)>0)
    {
      CCLabelTTF  *  pLabel=CCLabelTTF::create(text, "Arial", 15);
        addChild(pLabel,1,TAG_TEXTTAG);
        pLabel->setColor(m_cFontNormalColor);
        CCSize contextSize=getContentSize();
        switch (textPosion)
        {
            case TEXTOP :
                pLabel->setPosition(ccp(contextSize.width/2,contextSize.height/2+10));
                break;
            case TEXTMID :
                pLabel->setPosition(ccp(contextSize.width/2,contextSize.height/2));
                break;
            case TEXTBOTTOM:
                pLabel->setPosition(ccp(contextSize.width/2,-contextSize.height/2-10));
                break;
            case TEXTLEFT:
                pLabel->setPosition(ccp(contextSize.width/2-20,contextSize.height/2));
                break;
            case TEXTRIGHT :
                break;
            default:
                break;
        }
    }
    //添加色
    return true;
}

void CGameButtonControl::selected()
{
    if(m_bIsCanTouch)
    {
        if(getChildByTag(TAG_TEXTTAG))
        {
        //CCLabelBMFont *label=(CCLabelBMFont *)getChildByTag(GAMEBUTTONLABELTAG);
            CCLabelTTF *label=(CCLabelTTF*)getChildByTag(TAG_TEXTTAG);
            label->setColor(FONTSELECTCOLOR);
        }
        setDisplayFrame(m_pSelectFrame);
    }
}

void CGameButtonControl::unselected()
{
    if (m_bIsCanTouch) {
        if(getChildByTag(TAG_TEXTTAG))
        {
            //CCLabelBMFont *label=(CCLabelBMFont *)getChildByTag(GAMEBUTTONLABELTAG);
            CCLabelTTF *label=(CCLabelTTF*)getChildByTag(TAG_TEXTTAG);
            label->setColor(m_cFontNormalColor);
        }
        setDisplayFrame(m_pNormalFrame );
    }
   
}

void CGameButtonControl::setDisable()
{
    if (m_bIsEnabled)
    {
        m_bIsEnabled = false;
        if (m_pDisableFrame)
        {
            setDisplayFrame(m_pDisableFrame);
        }
    }
}

bool CGameButtonControl::getEnable()
{
    return m_bIsEnabled;
}