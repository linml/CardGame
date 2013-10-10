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
#include "CBattleArrayLayer.h"
#include "LayoutLayer.h"

#include <vector>

using namespace cocos2d;
using namespace std;

class CCardSettingScene : public CCLayer
{
public:
    static CCScene *scene(int inLastSceneTag = 0);
    CREATE_FUNC(CCardSettingScene);
    static CCDictionary *s_pBattleArrayCards;
    
public:
    CCardSettingScene();
    virtual ~CCardSettingScene ();
    void setLastSceneTag(int inLastSceneTag);
    
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    int getCurrentTableId(){ return m_nCurrentTableId;};
protected:
    void initCCardSetting();
    void handlerTouch();
    void onClickBack();
    
protected:
    int m_nTouchTag;
    int m_nLastSceneTag; // 0--> hall 1--> exploration
    LayoutLayer *m_cMaps;
    vector<TouchRect> m_cTouches;
    int m_nCurrentTableId;
    CBattleArrayLayer *m_pCardSetting;
    CCSprite * m_pBackBtn;
    CCSprite * m_pTabButton[4];
// test: network;
    void callback(float dt);
};


#endif /* defined(___1_cube__CCardSettingScene__) */
