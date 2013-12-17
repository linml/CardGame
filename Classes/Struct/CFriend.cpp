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

void CFriend::resetValue(int friendId, int level, int fightPower, const char *nickName,bool state)
{
    m_nFriendUid = friendId;
    m_nLevel = level;
    m_nFightPower = fightPower;
    m_sNickName = nickName;
    m_bState = state;
}

CFriendMM::CFriendMM()
{
    m_pFriendInfo = NULL;
    m_nApplyId = -1;
    m_bHaveHandler = false;
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
        m_pFriendList = m_pFriendList == NULL ? new vector<CFriend*>() : m_pFriendList;
        array = m_pFriendList;
        int count = array->size();
        int tempCount = 0;
        CCDictElement *element = NULL;
        CCDictionary *elementDict = NULL;
        CFriend *pFriend = NULL;
        CCDICT_FOREACH(inFriendInfoDict, element)
        {
          
            elementDict = (CCDictionary*) element->getObject();
            int friendId = atoi(element->getStrKey());
            const char *userName = GameTools::valueForKey("username", elementDict);
            int level = GameTools::intForKey("level", elementDict);
            bool state = GameTools::intForKey("haved_good", elementDict);
            int fight = 99999;
            if (tempCount < array->size())
            {
                array->at(tempCount)->resetValue(friendId, level,fight, userName, state);
                tempCount++;
                
            }else
            {
                pFriend = new CFriend(friendId, level,fight, userName, state);
                array->push_back(pFriend);
            }
            
            
        }
        if (tempCount < count)
        {
            relaseFriendList(tempCount);
            m_pFriendList->erase(m_pFriendList->begin()+tempCount, m_pFriendList->end());
        }
    }

    return array;
}

CFriendManager::CFriendManager()
{
    m_pActiveFriend = new vector<ActiveFriend *>(100, (ActiveFriend*)NULL);
    m_nStartIndex = MAXACTIVEFRIEND;
    m_pFriendList = NULL;
}

CFriendManager::~CFriendManager()
{
    releasActiveFriendContent();
    relaseFriendList();
    CC_SAFE_DELETE(m_pFriendList);
    CC_SAFE_DELETE(m_pActiveFriend);

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
        
        CCDICT_FOREACH(inResultDict, element)
        {
            elementDict = (CCDictionary*) element->getObject();
            int freindId =  GameTools::intForKey("friend_uid", elementDict); //atoi(element->getStrKey());
            const char *userName = GameTools::valueForKey("username", elementDict);
            array->push_back(Friend(freindId, userName));
        }
    }
    
    return array;
}

vector <ActiveFriend*>* CFriendManager:: getActiveFriend()
{
    vector<ActiveFriend *>* tmp = NULL;
    if (m_nStartIndex == MAXACTIVEFRIEND)
    {
        getActiveFriendFromServer();
    }
    else
    {
        vector<ActiveFriend*>::iterator it = m_pActiveFriend->begin();
        tmp = new vector<ActiveFriend*>(it+m_nStartIndex, it+m_nStartIndex+10);
        m_nStartIndex += 10;
        
        if (m_pFriendList)
        {
            for (int i = 0; i < tmp->size(); i++)
            {
                for (int j = 0; j < m_pFriendList->size(); j++)
                {
                    if(tmp->at(i)->friendInfo.fried_uid == m_pFriendList->at(j)->getFriendUid())
                    {
                        tmp->at(i)->isFriend = true;
                        break;
                    }
                }
                
            }
        }
    }
    return tmp;
  
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

void CFriendManager::releasActiveFriendContent()
{
    if (m_pActiveFriend)
    {
        ActiveFriend * tmp = NULL;
        for (int i = 0; i < m_pActiveFriend->size(); i++)
        {
            tmp = m_pActiveFriend->at(i);
            CC_SAFE_DELETE(tmp);
            tmp = NULL; 
        }
    }
}

void CFriendManager::relaseFriendList(int startIndex)
{
    if (m_pFriendList)
    {
        CFriend * tmp = NULL;
        for (int i = startIndex; i < m_pFriendList->size(); i++)
        {
            tmp = m_pFriendList->at(i);
            CC_SAFE_DELETE(tmp);
            tmp = NULL;
        }

    }
}

void CFriendManager::getActiveFriendFromServer()
{
    
}
void CFriendManager::getMsgActiveFriend(CCObject *pObject)
{
    
}



