//
//  CPtButtonWidget.h
//  91.cube
//
//  Created by phileas on 13-8-6.
//
//

#ifndef ___1_cube__CPtButtonWidget__
#define ___1_cube__CPtButtonWidget__

#include "cocos2d.h"
using namespace cocos2d;

class CPtButtonWidget : public CCSprite
{
public:
    static CPtButtonWidget *create(const char * inText);
    CPtButtonWidget();
    virtual ~CPtButtonWidget();
    virtual bool init(const char * inText);
    void setText(const char * inText);
    void setNormal();
    void setPress();
protected:
    CCSprite * m_pNoraml;
    CCSprite * m_pPress ;
    CCLabelTTF * m_pText;
    
    // test:
    CC_SYNTHESIZE(int , m_nType, Type);
};

#endif /* defined(___1_cube__CPtButtonWidget__) */
