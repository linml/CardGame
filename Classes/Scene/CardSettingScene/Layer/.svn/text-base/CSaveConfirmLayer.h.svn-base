//
//  CSaveConfirmLayer.h
//  91.cube
//
//  Created by phileas on 13-7-29.
//
//

#ifndef ___1_cube__CSaveConfirmLayer__
#define ___1_cube__CSaveConfirmLayer__


#include <vector>

#include "cocos2d.h"
#include "LayoutLayer.h"
#include "Utility.h"

using namespace cocos2d;
using namespace std;
//保存的对话框。 准备做过滤触控事件的问题。 
class CSaveConfirmLayer : public CCLayerColor {
    
public:
    CREATE_FUNC(CSaveConfirmLayer);
    
public:
    CSaveConfirmLayer();
    virtual ~CSaveConfirmLayer();
    
    void setResultCode(int inCode, bool delay = false);
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initConfirm();
    void handlerTouch();
    void updateText();
    
protected:
    bool m_bFight;
    int m_nTouchTag;
    LayoutLayer * m_cMaps;
    vector<TouchRect> m_cTouches;
    CCLabelTTF* pLabelTip;
    static int m_nResult;
    bool isDelayToTouch;
};




#endif /* defined(___1_cube__CSaveConfirmLayer__) */
