//
//  CPtButtonWidget.cpp
//  91.cube
//
//  Created by phileas on 13-8-6.
//
//
#include "CPtTool.h"
#include "CPtButtonWidget.h"
#include "gameConfig.h"
CPtButtonWidget * CPtButtonWidget::create(const char * inText,const string& soundFile)
{
    CPtButtonWidget *button  = new CPtButtonWidget();
    if (button && button->init(inText,soundFile))
    {
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return NULL;

}
CPtButtonWidget::CPtButtonWidget()
{
    m_pNoraml = NULL;
    m_pPress = NULL;
    m_pSelect = NULL;
    m_pText = NULL;
    m_pMirrorIcon = NULL;
    m_bHaveMirror = false;
}
CPtButtonWidget::~CPtButtonWidget()
{
    
}

bool CPtButtonWidget::init(const char * inText, const string& soundFile)
{
    CCLog("CPtButtonWidget::init");
    bool bRet = false;
    do {
        
        CC_BREAK_IF(!CCSprite::init());
        
        m_strSoundFile = soundFile;
        
        m_pNoraml = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button_normal_bg.png"));
        m_pNoraml->setAnchorPoint(CCPointZero);
        
        m_pPress = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "button_press_bg.png"));
        m_pPress->setAnchorPoint(CCPointZero);
        m_pPress->setVisible(false);
        
        CCSize contentSize = m_pNoraml->getContentSize();
        setContentSize(contentSize);
        
        CCTextureCache *textureCach = CCTextureCache::sharedTextureCache();
        CCTexture2D *texture = textureCach->addImage(CSTR_FILEPTAH(g_mapImagesPath, "fangdajing_actived.png"));
        m_pMirrorIcon = CCSprite::createWithTexture(texture);
        m_pMirrorIcon->setPosition(ccp(contentSize.width*0.85, contentSize.height*0.5));
        
        addChild(m_pNoraml);
        addChild(m_pPress);
        addChild(m_pMirrorIcon);
        setMirrorVisiable(m_bHaveMirror);
        
        m_pText = CCLabelTTF::create(inText, "Arial", 14);
        m_pText->setAnchorPoint(CCPointZero);
        m_pText->setColor(ccc3(0xfe, 0xed, 0xb3));
        m_pText->setDimensions(CCSizeMake(contentSize.width, 0));
        addChild(m_pText);
        m_pText->setPosition(ccp(0, m_pNoraml->boundingBox().getMidY()));
        
    
        bRet= true;
    } while (0);
    return bRet;
    
}

void CPtButtonWidget::setNormal()
{
    m_pNoraml->setVisible(true);
    m_pPress->setVisible(false);
    m_pText->setColor(ccc3(0xfe, 0xed, 0xb3));
    
}
void CPtButtonWidget::setPress()
{

    m_pNoraml->setVisible(false);
    m_pPress->setVisible(true);
    m_pText->setColor(ccc3(0x1e, 0xf5, 0xf2));
}

void CPtButtonWidget::setMirrorVisiable(bool haveMirror)
{
    if (m_pMirrorIcon && m_bHaveMirror)
    {
        m_pMirrorIcon->setVisible(haveMirror);
    }
}

bool CPtButtonWidget::haveTouchMirror(CCTouch *pTouch)
{
    return CPtTool::isInNode(m_pMirrorIcon, pTouch);
}

void CPtButtonWidget::resetState()
{
    setHaveMirror(m_bHaveMirror);
}

void CPtButtonWidget::setHaveMirror(bool var)
{
    m_bHaveMirror = var;
    if (m_pMirrorIcon)
    {
        m_pMirrorIcon->setVisible(var);
    }
}

// 0:空事件 1:普通战斗 2: 精英战斗 3: Boss战斗 4: 商人 5: 拥兵 6:宝箱，陷阱 7: 神坛 8:好友是事件
void CPtButtonWidget::setEventType(int inType)
{
    char buffer[50]={0};
    switch (inType)
    {
        case 0:
            sprintf(buffer, "空事件");
            break;
        case 1:
             sprintf(buffer, "普通战斗");
            break;
        case 2:
             sprintf(buffer, "精英战斗");
            break;
        case 3:
             sprintf(buffer, "Boss战斗");
            break;
        case 4:
             sprintf(buffer, "商人");
            break;
        case 5:
             sprintf(buffer, "拥兵");
            break;
        case 6:
            sprintf(buffer, "宝箱");
            break;
        case 7:
            sprintf(buffer, "神坛");
            break;
        case 8:
            sprintf(buffer, "好友");
            break;
        default:
            break;
    }
    std::string tip (m_pText->getString());
    tip.append("---");
    tip.append(buffer);
    setText(tip.c_str());
}

void CPtButtonWidget:: setText(const char * inText)
{
    m_pText->setString(inText);
}

void CPtButtonWidget::setEnd()
{
    PtSoundTool::playSysSoundEffect(m_strSoundFile);
}
