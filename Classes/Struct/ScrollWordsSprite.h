//
//  ScrollWordsSprite.h
//  91.cube
//
//  Created by phileas on 13-6-19.
//
//

#ifndef ___1_cube__ScrollWordsSprite__
#define ___1_cube__ScrollWordsSprite__

#include "cocos2d.h"
using namespace cocos2d;

class CScrollWordSprite : public CCLabelTTF
{
    
public:
    static CScrollWordSprite * create(const char *string, const char *fontName, float fontSize);
    static CScrollWordSprite * create(const char *string, const char *fontName, float fontSize,
                               const CCSize& dimensions, CCTextAlignment hAlignment);
    static CScrollWordSprite * create(const char *string, const char *fontName, float fontSize,
                               const CCSize& dimensions, CCTextAlignment hAlignment,
                               CCVerticalTextAlignment vAlignment);

    
public:
    CScrollWordSprite();
    ~CScrollWordSprite();
    void setScroll(const bool &inEnable);
    void setScrollDirection(const int &inType = 1);
    void setScrollRect(const CCRect & inRect);
    void setScrollStep(const float &inStep);
    void setScrollContentHeight(float inHeight);
    void runScroll(float dt);
    

protected:
    bool m_bScroll;
    bool m_bScrollOver;
    int  m_nScrollDirection;
    float m_fCurrentPosition;
    float m_fBottom;
    float m_fScrollStep;
    float m_fcontentHeight;
    CCRect m_sScrollRect;
    
    
};

#endif /* defined(___1_cube__ScrollWordsSprite__) */
