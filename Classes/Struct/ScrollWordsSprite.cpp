//
//  ScrollWordsSprite.cpp
//  91.cube
//
//  Created by phileas on 13-6-19.
//
//

#include "ScrollWordsSprite.h"


CScrollWordSprite * CScrollWordSprite::create(const char *string, const char *fontName, float fontSize)
{
    return CScrollWordSprite::create(string, fontName, fontSize,
                              CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    
}
CScrollWordSprite * CScrollWordSprite::create(const char *string, const char *fontName, float fontSize,
                                  const CCSize& dimensions, CCTextAlignment hAlignment)
{
    return CScrollWordSprite::create(string, fontName, fontSize, dimensions, hAlignment, kCCVerticalTextAlignmentTop);
    
}
CScrollWordSprite * CScrollWordSprite::create(const char *string, const char *fontName, float fontSize,
                                  const CCSize& dimensions, CCTextAlignment hAlignment,
                                  CCVerticalTextAlignment vAlignment)
{
    CScrollWordSprite *pRet = new CScrollWordSprite();

    if(pRet && pRet->initWithString(string, fontName, fontSize, CCSizeZero, hAlignment, vAlignment))
    {

        pRet->setDimensions(CCSizeMake(dimensions.width, 0.0f));
        pRet->setScrollContentHeight(pRet->getContentSize().height);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;

}




CScrollWordSprite::CScrollWordSprite()
{
    m_bScroll = false;
    m_fScrollStep = 0.2f;
    m_bScrollOver = false;
    m_nScrollDirection = 1;
}

CScrollWordSprite::~CScrollWordSprite()
{
   
}

void CScrollWordSprite::setScroll(const bool &inEnable)
{
    if (inEnable !=  m_bScroll)
    {
        if (inEnable)
        {
            m_fCurrentPosition = 0.0f;
            m_fBottom = 0;
            this->schedule(schedule_selector(CScrollWordSprite::runScroll));
        }
        else
        {
            this->unschedule(schedule_selector(CScrollWordSprite::runScroll));
        }
        m_bScroll = inEnable;
        
    }
}

void CScrollWordSprite::setScrollContentHeight(float inHeight)
{
    m_fcontentHeight = inHeight;
}

/*
 * @param inType : 1 horizontal, other is vertical
 */
void CScrollWordSprite::setScrollDirection(const int &inType)
{
    m_nScrollDirection = inType;
}

void CScrollWordSprite::setScrollRect(const cocos2d::CCRect &inRect)
{
    m_sScrollRect = inRect;
    this->setTextureRect(inRect);
}

void CScrollWordSprite::setScrollStep(const float &inStep)
{
    m_fScrollStep = inStep;
}

void CScrollWordSprite::runScroll(float dt)
{

    switch (m_nScrollDirection)
    {
        case 1:
            
            if (m_bScrollOver)
            {
              //  CCLog("%f", m_fBottom);
                if (m_fBottom > m_sScrollRect.size.height)
                {
                    m_fBottom = 0.0f;
                    m_bScrollOver = false;
                }
                else
                {
                    m_fBottom += m_fScrollStep;
                    this->setTextureRect(CCRectMake(0, 0, m_sScrollRect.size.width, m_fBottom));
                    
                }
                
            }
            else
            {
                if ( m_fCurrentPosition > m_fcontentHeight )
                {
                    m_fCurrentPosition = 0.0f;
                    m_fBottom = 0.0f;
                    m_bScrollOver = true;
                }
                else
                {
                    m_fCurrentPosition +=  m_fScrollStep;
                    this->setTextureRect(CCRectMake(0, m_fCurrentPosition, m_sScrollRect.size.width, m_sScrollRect.size.height));
                    
                }
            }
        
            
            break;
        default:
            
            break;
    }
    
}


