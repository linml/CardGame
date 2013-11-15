//
//  CFriend.cpp
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#include "CFriend.h"
#include "gameTools.h"

CFriend::CFriend():m_nFriendUid(0), m_nFightPower(0), m_sNickName(""), m_nLevel(0),m_bState(false)
{
    
}
CFriend::CFriend(int friendId, int level, int fightPower, const char * nickName, bool state):m_nFriendUid(friendId), m_nFightPower(fightPower), m_sNickName(nickName), m_nLevel(level), m_bState(state)
{
    
}


CFriendMM::CFriendMM()
{
    m_pFriendInfo = NULL;
    m_nApplyId = -1;
}
CFriendMM::~CFriendMM()
{
    relaseFriendInfo();
}
void CFriendMM::setFriendInfo(int friendId, int level, int fightPower, const char * nickName)
{
    relaseFriendInfo();
    m_pFriendInfo = new CFriend(friendId, level, fightPower, nickName);
}

void CFriendMM::relaseFriendInfo()
{
    CC_SAFE_DELETE(m_pFriendInfo);
    m_pFriendInfo = NULL;
}



CFriendManager * CFriendManager::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new CFriendManager();
    }
    return s_pInstance;
}
void CFriendManager::releaseFriendManager()
{
    CC_SAFE_DELETE(s_pInstance);
    s_pInstance = NULL;
}


CFriendManager* CFriendManager::s_pInstance = NULL;

vector<CFriend*> * CFriendManager::getFriendListByDict(CCDictionary *inFriendInfoDict)
{
    vector<CFriend*> *array = NULL;
    if (isDictionary(inFriendInfoDict))
    {

        array = new vector<CFriend*>();
        CCDictElement *element = NULL;
        CCDictionary *elementDict = NULL;
        CFriend *pFriend = NULL;
        CCDICT_FOREACH(inFriendInfoDict, element)
        {
            elementDict = (CCDictionary*) element->getObject();
            int freindId = atoi(element->getStrKey());
            const char *userName = GameTools::valueForKey("username", elementDict);
            int level = GameTools::intForKey("level", elementDict);
            bool state = GameTools::intForKey("have_good", elementDict) == 1;
            int fight = 99999;
            pFriend = new CFriend(freindId, level,fight, userName, state);
            array->push_back(pFriend);
        }
    }

    return array;
}
vector<CFriendMM*>* CFriendManager::getApplyFriendListByDict(CCDictionary* inResultDict)
{
    vector<CFriendMM*> *array = NULL;
    if(isDictionary(inResultDict))
    {
        array =   new vector<CFriendMM*>();
        CCDictElement *element = NULL;
        CCDictionary *elementDict = NULL;
        CFriendMM* pFriendMM = NULL;
        CCDICT_FOREACH(inResultDict, element)
        {
            elementDict = (CCDictionary*) element->getObject();
            int applyId = atoi(element->getStrKey());
            int freindId = GameTools::intForKey("friend_uid", elementDict);
            const char *userName = GameTools::valueForKey("username", elementDict);
            int level = GameTools::intForKey("level", elementDict);
            int fight = 99999;
            pFriendMM = new CFriendMM();
            pFriendMM->setApplyId(applyId);
            pFriendMM->setFriendInfo(freindId, level, fight, userName);
            array->push_back(pFriendMM);
        }
    }
    return array;
}

vector<Friend> *CFriendManager::getPraiseListByDict(CCDictionary* inResultDict)
{
    vector<Friend>* array = NULL;
    if(isDictionary(inResultDict))
    {
        array =   new vector<Friend>();
        CCDictElement *element = NULL;
        CCDictionary *elementDict = NULL;
        
        CCDICT_FOREACH(elementDict, element)
        {
            elementDict = (CCDictionary*) element->getObject();
            int freindId = GameTools::intForKey("friend_uid", elementDict);
            const char *userName = GameTools::valueForKey("username", elementDict);
            array->push_back(Friend(freindId, userName));
        }
    }
    
    return array;
}


bool CFriendManager::isDictionary(CCDictionary *inDict)
{
    bool bRet = false;
    if (inDict)
    {
        std::string typeName = typeid(*inDict).name();
        bRet = typeName.find("CCDictionary") != std::string::npos ;
    }
    return bRet;
}




