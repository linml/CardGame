//
//  CCardSettingScene.h
//  91.cube
//
//  Created by phileas on 13-7-18.
//
//

#ifndef ___1_cube__CCardSettingScene__
#define ___1_cube__CCardSettingScene__

#include "cocos2d.h"
#include "LayoutLayer.h"

#include <vector>

using namespace cocos2d;
using namespace std;

class CCardSettingScene : public CCLayer
{
public:
    static CCScene *scene();
    CREATE_FUNC(CCardSettingScene);
    
public:
    CCardSettingScene();
    virtual ~CCardSettingScene ();
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    void initCCardSetting();
    void handlerTouch();
    

    
    
protected:
    int m_nTouchTag;
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    
};


#endif /* defined(___1_cube__CCardSettingScene__) */
