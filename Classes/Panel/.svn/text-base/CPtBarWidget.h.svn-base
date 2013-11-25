//
//  CPtBarWidget.h
//  91.cube
//
//  Created by phileas on 13-7-4.
//
//

#ifndef ___1_cube__CPtBarWidget__
#define ___1_cube__CPtBarWidget__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class CPtBarWidget : public cocos2d::CCNode
{
public:
    
    static CPtBarWidget *create(const int& inCurrent, const int& inMax, const char* inForeground, const char* inBackground = NULL);
    void setValue(const int inValue);
    void setShowLabelEnable(const bool inFlag);
    void reset(const int inCurrentValue, const int inMaxValue);
    
    bool isFull(){return m_nValue == m_nMaxValue;};
    int getMaxValue(){return m_nMaxValue;};
    int getValue(){return m_nValue;};
    bool getShowLabelEnable(){return m_bShowLabel;};
    CCLabelTTF *getPercentLabel(){return m_pPercentLabel;};
    
protected:
    CPtBarWidget(const int& inCurrent, const int& inMax, const char* inForeground, const char* inBackground);
    virtual ~CPtBarWidget();
    void updateLabelValue();
    
protected:
    CCProgressTimer *m_pProgressTimer;
    CCLabelTTF *m_pPercentLabel;
    
    bool m_bShowLabel;
    
    int m_nValue;
    int m_nMaxValue;
    
    
};


#endif /* defined(___1_cube__CPtBarWidget__) */
