//
//  CGameEmailLayer.h
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#ifndef ___1_cube__CGameEmailLayer__
#define ___1_cube__CGameEmailLayer__

#include <iostream>
#include "cocos2d.h"
using namespace  std;
using namespace cocos2d;
class CUIEmailData  :public CCObject{
    
public:
    std::string CUIEmailDataTitle;
    std::string CUiEmailContext;
    
};
class CGameButtonControl;
class  CGameEmailLayer :public CCLayer 
{
public:
    CGameEmailLayer();
    ~CGameEmailLayer();
    static CGameEmailLayer *CreateEmailLayer();
private:
    bool initCreate();
    bool loadPlistFile();
    void createRecvAllButton();
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();
};

#endif /* defined(___1_cube__CGameEmailLayer__) */
