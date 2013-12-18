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

#define MAXPLAYERCOUNT 3
#define NOWPLAYERCOUNT 4
#define MINPLAYERCOUNT 3


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
    void resetValue(int friendId, int level, int fightPower, const char * nickName, bool state = false);
    CC_SYNTHESIZE(int, m_nFriendUid, FriendUid);
    CC_SYNTHESIZE(std::string, m_sNickName, NickName);
    CC_SYNTHESIZE(int, m_nLevel, Level);
    CC_SYNTHESIZE(int, m_nFightPower, FightPower);
    CC_SYNTHESIZE(bool, m_bState, State); //true 赞过
};

struct ActivePlayer
{
    ActivePlayer(int inFriendId, int inLevel, const char *inUserName, bool inFriend =false ):friend_uid(inFriendId),level(inLevel), username(inUserName), isFriend(inFriend)
    {
        
    };
    int friend_uid;
    int level;
    std::string username;
    bool isFriend = false;
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
    void getActivePlayerListFromLocal(vector<ActivePlayer*> *outActivePlayer);
    void paraseActivePlayerListByDict(CCDictionary *pRandomFriend);
    
protected:
    bool isDictionary(CCDictionary *inDict);
    void releasActivePlayerContent(int startIndex = 0);
    void relaseFriendList(int startIndex = 0);
    void gernerActivePlayerList(vector<ActivePlayer *>*outActivePlayer);
    int  setActivePlayerInfoesByDict(CCDictionary *pDict, int startIndex = 0);
    int  fillActivePlayer(vector<ActivePlayer*> *outActivePlayeres, int nowstartIndex, int orignstartIndex, int endIndex, int count);
   
protected:
    vector<ActivePlayer*>* m_pActivePlayeres;
    vector<CFriend*> *m_pFriendList;
    
    int m_nMaxStartIndex;
    int m_nEqualStartIndex;
    int m_nMinStartIndex;
    
    int m_nMaxEndIndex;
    int m_nEqualEndIndex;
    int m_nMinEndIndex;
};
#endif /* defined(___1_cube__CFriend__) */
