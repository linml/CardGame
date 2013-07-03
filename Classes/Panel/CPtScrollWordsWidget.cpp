//
//  CPtScrollWordsWidget.cpp
//  91.cube
//
//  Created by phileas on 13-6-19.
//
//

#include "CPtScrollWordsWidget.h"


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

        pRet->setDimensions(CCSizeMake(dimensions.width, dimensions.height));
        pRet->setScrollContentHeight(pRet->getContentSize().height);
        pRet->setScrollContentWidth(pRet->getContentSize().width);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;

}




CScrollWordSprite::CScrollWordSprite()
{
    m_bScroll = false;
    m_bLoop = true;
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
            if (m_nScrollDirection == 1)
            {
                this->setAnchorPoint(CCPointZero);
                m_fCurrentPosition = 0.0f;
                m_fBottom = 0;
            }else if( m_nScrollDirection == 2)
            {
                this->setAnchorPoint(ccp(1,0));
                m_fCurrentPosition = 0.0f;
                m_fRight = 0.0f;
            }
                
          
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

void CScrollWordSprite::setScrollContentWidth(float inWidth)
{
    m_fcontentWidth = inWidth;
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
                if (m_bLoop == false)
                {
                    this->unschedule(schedule_selector(CScrollWordSprite::runScroll));
                }
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
        
        case 2:
            
            if (m_bScrollOver)
            {
                if (m_bLoop == false)
                {
                    this->unschedule(schedule_selector(CScrollWordSprite::runScroll));
                }
                //  CCLog("%f", m_fBottom);
                if (m_fRight > m_sScrollRect.size.width)
                {
                    m_fRight = 0.0f;
                    m_bScrollOver = false;
                }
                else
                {
                    m_fRight += m_fScrollStep;
                    this->setTextureRect(CCRectMake(0, 0, m_fRight, m_sScrollRect.size.height));
                    
                }
                
            }
            else
            {
                if ( m_fCurrentPosition > m_fcontentWidth )
                {
                    m_fCurrentPosition = 0.0f;
                    m_fRight = 0.0f;
                    m_bScrollOver = true;
                }
                else
                {
                    m_fCurrentPosition +=  m_fScrollStep;
                    this->setTextureRect(CCRectMake(m_fCurrentPosition, 0.0f , m_sScrollRect.size.width, m_sScrollRect.size.height));
                    
                }
            }

            
            break;
        default:
            
            break;
    }
    
}


