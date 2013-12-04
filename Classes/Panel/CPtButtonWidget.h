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
#include "gameConfig.h"
using namespace cocos2d;
using namespace std;

class CPtButtonWidget : public CCSprite
{
public:
    static CPtButtonWidget *create(const char * inText, const string& soundFile = "UI_click.wav");
    virtual bool init(const char * inText, const string& soundFile = "");
    CPtButtonWidget();
    virtual ~CPtButtonWidget();
    
    void setText(const char * inText);
    void setNormal();
    void setPress();
    void setSelect();
    void setEnd();
    void setMirrorVisiable(bool haveMirror);
    bool haveTouchMirror(CCTouch *pTouch);
    bool getHaveMirror(){return m_bHaveMirror;};
    void resetState();
    void setHaveMirror(bool var);
    void setEventType(int inType);
protected:
    bool m_bHaveMirror;
    CCSprite * m_pMirrorIcon; // 探路镜
    CCSprite * m_pNoraml;
    CCSprite * m_pPress ;
    CCSprite * m_pSelect;
    CCLabelTTF * m_pText;
    string m_strSoundFile;
    // test:
    CC_SYNTHESIZE(int , m_nType, Type);

};

#endif /* defined(___1_cube__CPtButtonWidget__) */
