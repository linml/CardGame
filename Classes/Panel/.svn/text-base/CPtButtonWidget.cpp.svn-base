//
//  CPtButtonWidget.cpp
//  91.cube
//
//  Created by phileas on 13-8-6.
//
//

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
        
        
        addChild(m_pNoraml);
        addChild(m_pPress);
        CCSize contentSize = m_pNoraml->getContentSize();
        setContentSize(contentSize);
        
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

void CPtButtonWidget:: setText(const char * inText)
{
    m_pText->setString(inText);
}

void CPtButtonWidget::setEnd()
{
    PtSoundTool::playSysSoundEffect(m_strSoundFile);
}
