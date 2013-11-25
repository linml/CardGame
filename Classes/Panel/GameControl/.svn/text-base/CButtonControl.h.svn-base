//
//  CButtonControl.h
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#ifndef ___1_cube__CButtonControl__
#define ___1_cube__CButtonControl__

#include <iostream>
#include "CCSprite.h"

using namespace std;
using namespace cocos2d;

enum TEXTPOSTION
{
    TEXTOP = 0,
    TEXTMID = 1 ,
    TEXTBOTTOM=2 ,
    TEXTLEFT=3,
    TEXTRIGHT
    
};
class CButtonControl : public cocos2d::CCSprite
{
public:
    CButtonControl();
    virtual ~CButtonControl();
    virtual void selected();
    virtual void unselected();
    virtual CCRect rect();
protected:
    bool m_bIsSelected;
    bool m_bIsEnabled;
    CC_PROPERTY(CCSpriteFrame*,m_pNormalFrame,NormalFrame);
    CC_PROPERTY(CCSpriteFrame* ,m_pSelectFrame,SelectFrame);
    CC_PROPERTY(CCSpriteFrame*, m_pDisableFrame, DisableFrame);
};

#endif /* defined(___1_cube__CButtonControl__) */
