//
//  CGameButtonControl.h
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#ifndef ___1_cube__CGameButtonControl__
#define ___1_cube__CGameButtonControl__

#include <iostream>
#include "CButtonControl.h"
#include "cocos2d.h"
using namespace cocos2d;
//对话框上的button
class CGameButtonControl:public CButtonControl
{
public:
    CGameButtonControl();
    ~CGameButtonControl();
    CCRect rect();
    static CGameButtonControl *createButton(TEXTPOSTION textPosion,const char *text,const char *normalPng,const char *selectPng, const char *disablePng=NULL);
    //static CGameButtonControl *createButton(TEXTPOSTION textPosion,cocos2d::CCLabelTTF *labelttf,const char *normalPng,const char *selectPng);
    virtual void selected();
    virtual void unselected();
    CCLabelTTF *getTextLabel();
    void setFontColor(ccColor3B csColor);
    void setEnable(bool flag);
    void setText(const char * str);    
    void setDisable();
    bool getEnable();
protected:
    bool m_bIsCanTouch;
private:
    ccColor3B  m_cFontNormalColor;
protected:
    bool init(TEXTPOSTION textPosion,const char *text,const char *normalPng,const char *selectPng,const char *disablePng=NULL);
};
#endif /* defined(___1_cube__CGameButtonControl__) */
