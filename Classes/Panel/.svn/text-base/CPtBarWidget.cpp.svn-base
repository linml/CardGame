//
//  CPtBarWidget.cpp
//  91.cube
//
//  Created by phileas on 13-7-4.
//
//

#include "CPtBarWidget.h"

#include "CptBarWidget.h"

/*
 * @param: inForeground, the foreground sprite's filename. (it's fullname)
 * @parem: inBackGround, the background sprite's filename. (it's fullname)
 */

CPtBarWidget::CPtBarWidget(const int& inCurrent, const int& inMax, const char* inForeground, const char* inBackground /*= NULL*/)
{
    // init data:
    m_pPercentLabel = NULL;
    m_nValue = inCurrent;
    m_nMaxValue = inMax;
    m_bShowLabel = true;
    
    // init sprite:
    float percent = 100.0f * m_nValue/m_nMaxValue;
    percent = percent > 100.0f ? 100.0f : percent;
    CCSprite *background = NULL;
    
    if (inBackground)
    {
        background = CCSprite::create(inBackground);
        background->setAnchorPoint(ccp(0, 0));
        background->setPosition(ccp(0, 0));
        
        addChild(background, 0);
    }
    
    CCSprite *sprite = cocos2d::CCSprite::create(inForeground);
    m_pProgressTimer = CCProgressTimer::create(sprite);
    m_pProgressTimer->setType(kCCProgressTimerTypeBar);
    m_pProgressTimer->setMidpoint(ccp(0,1));
    m_pProgressTimer->setBarChangeRate(ccp(1,0));
    CCProgressTo *pProgressTo = CCProgressTo::create(0.01f, percent);
    m_pProgressTimer->runAction(pProgressTo);
    m_pProgressTimer->setPercentage(percent);
    
    addChild(m_pProgressTimer, 1);
    
    m_pProgressTimer->setPosition(ccp(0, 0));
    m_pProgressTimer->setAnchorPoint(ccp(0, 0));
    
    if (inBackground)
    {
        this->setContentSize(background->getContentSize());
    }else
    {
        this->setContentSize(sprite->getContentSize());
    }
}

CPtBarWidget::~CPtBarWidget()
{
    
}

CPtBarWidget* CPtBarWidget::create(const int& inCurrent, const int& inMax, const char* inForeground, const char* inBackground)
{
    CPtBarWidget * bar = new CPtBarWidget(inCurrent, inMax, inForeground, inBackground);
    bar->autorelease();
    bar->updateLabelValue();
    return bar;
}




void CPtBarWidget::setValue(const int inValue)
{
    float fromPercent = 100.0f * (float)m_nValue / m_nMaxValue;
    float toPercent = 100.0f * (float)inValue / m_nMaxValue;
    float currentPercent = toPercent;
    m_nValue = inValue;
    
    m_nValue = m_nValue > m_nMaxValue ? m_nMaxValue : m_nValue;
    
    toPercent = toPercent > 100.0f ? 100.0f : toPercent;
    
    CCProgressFromTo *pProgressFromTo = CCProgressFromTo::create(0.5f, fromPercent, toPercent);
    m_pProgressTimer->runAction(pProgressFromTo);
    
    if (currentPercent <= 0)
    {
        currentPercent = 0;
    }
    else if(currentPercent > 0 && currentPercent <= 1)
    {
        currentPercent = 1;
    }
    else if(currentPercent > 100)
    {
        currentPercent = 100;
    }
    
    updateLabelValue();
}

/*
 * @breif: whether the percent label 's showed.
 */

void CPtBarWidget::setShowLabelEnable(const bool inFlag)
{
    m_bShowLabel = inFlag;
    updateLabelValue();
    
}

/*
 * @breif : reset the bar maxvalue and the current value
 */

void CPtBarWidget::reset(const int inCurrentValue, const int inMaxValue)
{
    m_nValue = inCurrentValue;
    m_nMaxValue = inMaxValue;
    
    float percent = 100 * inCurrentValue / inMaxValue;
    
    percent = percent > 100.0f ? 100.0f : percent;
    CCProgressTo *pProgressTo = CCProgressTo::create(0.1f, percent);
    m_pProgressTimer->runAction(pProgressTo);
    
    updateLabelValue();
}

/*
 * @breif: update the percent label 's state
 */

void CPtBarWidget::updateLabelValue()
{
    if (m_bShowLabel == false)
    {
        if (m_pPercentLabel && m_pPercentLabel->isVisible())
        {
            m_pPercentLabel->setVisible(false);
        }
        return;
    }
    
    if (m_pPercentLabel && m_pPercentLabel->isVisible() == false)
    {
        m_pPercentLabel->setVisible(true);
    }
    
//    float percent = 100 * m_nValue / m_nMaxValue;
//    percent = percent > 100.0f ? 100.0f : percent;
//    
    char buffer[125];
    
    snprintf(buffer, sizeof(buffer), "%d/%d", m_nValue, m_nMaxValue);
    
    if (m_pPercentLabel == NULL)
    {
        m_pPercentLabel = CCLabelTTF::create(buffer, "Artial", (int)m_pProgressTimer->getContentSize().height);
        addChild(m_pPercentLabel, 4);
        m_pPercentLabel->setPosition(ccp(m_pProgressTimer->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
    }
    else
    {
        m_pPercentLabel->setString(buffer);
    }
    
    
}
