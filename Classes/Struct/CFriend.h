//
//  CFriend.h
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#ifndef ___1_cube__CFriend__
#define ___1_cube__CFriend__

#include "cocos2d.h"
#include <vector>
using namespace std;
using namespace cocos2d;

struct Friend
{
    Friend():fried_uid(0),username(""){};
    Friend(int uid, const char *nickName):fried_uid(uid), username(nickName){};
    int fried_uid;
    std::string username;
};

class CFriend : public CCObject
{
public:
    CFriend();
    CFriend(int friendId, int level, int fightPower, const char * nickName, bool state = false);
    CC_SYNTHESIZE(int, m_nFriendUid, FriendUid);
    CC_SYNTHESIZE(std::string, m_sNickName, NickName);
    CC_SYNTHESIZE(int, m_nLevel, Level);
    CC_SYNTHESIZE(int, m_nFightPower, FightPower);
    CC_SYNTHESIZE(bool, m_bState, State);
};


class CFriendMM
{
public:
    CFriendMM();
    ~CFriendMM();
    void setFriendInfo(int friendId, int level, int fightPower, const char * nickName);
    const CFriend* getFriendInfo(){return m_pFriendInfo;};
protected:
    void relaseFriendInfo();
protected:
    CFriend * m_pFriendInfo;
    CC_SYNTHESIZE(int, m_nApplyId, ApplyId)
};


class CFriendManager
{
public:
    static CFriendManager *getInstance();
    static void releaseFriendManager();
protected:
    static CFriendManager* s_pInstance;
public:
    /*
     * @breif: 以下三个工具类所产生的vector需要自己释放内存，包括元素内存
     */
    vector<CFriend*> *getFriendListByDict(CCDictionary *inFriendInfoDict);
    vector<CFriendMM*> *getApplyFriendListByDict(CCDictionary* inResultDict);
    vector<Friend> *getPraiseListByDict(CCDictionary* inResultDict);
protected:
    bool isDictionary(CCDictionary *inDict);
};
#endif /* defined(___1_cube__CFriend__) */
