//
//  CSceneShowOtherUidTeam.h
//  91.cube
//
//  Created by linminglu on 11/18/13.
//
//

#ifndef ___1_cube__CSceneShowOtherUidTeam__
#define ___1_cube__CSceneShowOtherUidTeam__

#include <iostream>
#include "cocos2d.h"
class CFightCard;
class CGamePlayer;
using namespace std;
using namespace cocos2d;

//显示团队信息
class CGameButtonControl;

class CSceneShowOtherUidTeam :public CCLayer
{
public:
    bool initCreate(int Uid);
    static CSceneShowOtherUidTeam *CreateLayer(int Uid);
    ~CSceneShowOtherUidTeam();
    CSceneShowOtherUidTeam();
protected:
    //触控问题
public:
    void createFightButton();
    void createQuitButton();
    void createOtherUidText();
    void createFightText();
    void createTeamText();
    void sendGetTeamInfo();
    void createBackGroud();
    void createFightCardContext();
    void decodeGetTeamInfo(CCObject *object);
    CCPoint getPostionCard(int row ,int cloumn);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void registerWithTouchDispatcher(void);
    void onExit();
    void handTouchTag(int tag);
    void clearVectorList();
private:
    void initCardTeam(int index,CCDictionary *cardTeam,CCDictionary *cardItem);
private:
    CC_SYNTHESIZE(int , m_nUid, OtherUid);
    vector<CGameButtonControl *>m_vTouchVector;
    vector<CFightCard *>m_vFightCardList[3];
    CGamePlayer *m_tempGamePlayer;
};

#endif /* defined(___1_cube__CSceneShowOtherUidTeam__) */
