//
//  LoginLayer.h
//  91.cube
//
//  Created by phileas on 13-6-5.
//
//

#ifndef ___1_cube__LoginScene__
#define ___1_cube__LoginScene__

#include <string>
#include <vector>

using namespace std;
#include "cocos2d.h"
#include "LayoutLayer.h"
#include "gameConfig.h"


// it should move ot one macro file
#define LOGO_ZORDER (5)
#define LOGIN_ZORDER (0)
#define BUTTON_PLAY_TAG (2001)



USING_NS_CC;


class CLoginScene : public CCLayer
{
public:
    CREATE_FUNC(CLoginScene);
    static  CCScene *scene();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void loginCallBack(CCNode * pSender);
    void setLogoOverCallBack();
    
protected:
    bool initLogin();
    bool handleTouchSpritePool(CCPoint point);
protected:
    bool m_bLogoOver;
    
    LayoutLayer *maps;
    vector<TouchRect> touchRect;
    
};

#endif /* defined(___1_cube__LoginScene__) */
