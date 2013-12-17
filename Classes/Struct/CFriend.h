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

#define MAXACTIVEFRIEND 100


struct Friend
{
    Friend():fried_uid(0),username(""){};
    Friend(int uid, const char *nickName):fried_uid(uid), username(nickName){};
    int fried_uid;
    std::string username;
};

struct ActiveFriend
{
    Friend friendInfo;
    bool isFriend = false;
};

class CFriend : public CCObject
{
public:
    CFriend();
    CFriend(int friendId, int level, int fightPower, const char * nickName, bool state = false);
    void resetValue(int friendId, int level, int fightPower, const char * nickName, bool state = false);
    CC_SYNTHESIZE(int, m_nFriendUid, FriendUid);
    CC_SYNTHESIZE(std::string, m_sNickName, NickName);
    CC_SYNTHESIZE(int, m_nLevel, Level);
    CC_SYNTHESIZE(int, m_nFightPower, FightPower);
    CC_SYNTHESIZE(bool, m_bState, State); //true 赞过
};


class CFriendMM
{
public:
    CFriendMM();
    ~CFriendMM();
    void setFriendInfo(int friendId, int level, int fightPower, const char * nickName);
    CFriend* getFriendInfo(){return m_pFriendInfo;};
protected:
    void relaseFriendInfo();
protected:
    CFriend * m_pFriendInfo;
    CC_SYNTHESIZE(int, m_nApplyId, ApplyId)
    CC_SYNTHESIZE(bool, m_bHaveHandler, HaveHandler);
};


class CFriendManager
{
public:
    static CFriendManager *getInstance();
    static void releaseFriendManager();
protected:
    static CFriendManager* s_pInstance;
public:
    CFriendManager();
    ~CFriendManager();
    /*
     * @breif: 以下三个工具类所产生的vector需要自己释放内存，包括元素内存
     */
    vector<CFriend*> *getFriendListByDict(CCDictionary *inFriendInfoDict);
    vector<CFriendMM*> *getApplyFriendListByDict(CCDictionary* inResultDict);
    vector<Friend> *getPraiseListByDict(CCDictionary* inResultDict);
    vector<ActiveFriend*>* getActiveFriend();
protected:
    bool isDictionary(CCDictionary *inDict);
    void releasActiveFriendContent();
    void relaseFriendList(int startIndex = 0);
    void getActiveFriendFromServer();
    void getMsgActiveFriend(CCObject *pObject);
protected:
    vector<ActiveFriend*>* m_pActiveFriend;
    vector<CFriend*> *m_pFriendList;
    int m_nStartIndex;
};
#endif /* defined(___1_cube__CFriend__) */
