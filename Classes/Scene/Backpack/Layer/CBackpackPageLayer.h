//
//  CBackpackPageLayer.h
//  91.cube
//
//  Created by phileas on 13-8-21.
//
//

#ifndef ___1_cube__CBackpackPageLayer__
#define ___1_cube__CBackpackPageLayer__

#include "cocos2d.h"
#include "CBattleArrayLayer.h"
#include "CPtPropConfigData.h"
#include "LayoutLayer.h"

#include <vector>

using namespace cocos2d;
using namespace std;

class CBackpackPageLayer : public CCLayer
{
public:
    static CBackpackPageLayer *create(int inOpenNumber = 0);

    
public:
    CBackpackPageLayer();
    virtual ~CBackpackPageLayer ();
public:
    virtual bool init(int inOpenNumber);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    bool getTouchLock(){return m_bTouchLock;};
protected:
    void initCBackpackPageLayer(int inOpenNumber);
    void handlerTouch();
    
    void addItems(int inUseNum);
    void addMask(int inCloseNum);
    
    void clearItem(CCNode *node);
    void setItem(CCNode *node, CPtProp *inData);
    
    void initPanel(bool inResetTexture=false);
    void resetPanel();
    
    void openGrid();
    void openGridUI();
    
protected:
    bool m_bTouchLock;
    
    int m_nTouchTag;
    int m_nUserGridNumber;
    int m_nOpenGridNumber;
    
    CCArray * m_pItemNums;
    CCArray * m_pMasks;
    
    CCSprite *m_pDeleteButtons[9];
    CCSprite *m_pUseButton[9];
    
    // button rect:[0-2] 0: normal, 1: pressed: 2: disabled
    CCRect m_aButtonRect[3];
    
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    
    static const int AllGridInPageNumber = 9;
    static const int DeleteTag = 2000;
    static const int UseTag = 3000;

};



#endif /* defined(___1_cube__CBackpackPageLayer__) */
