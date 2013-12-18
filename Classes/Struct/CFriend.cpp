//
//  CFriend.cpp
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#include "CFriend.h"
#include "gameTools.h"
#include "gamePlayer.h"

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
    m_pActivePlayeres = new vector<ActivePlayer *>(110, (ActivePlayer*)NULL);
    m_pFriendList = NULL;
    
    m_nMaxStartIndex = 0;
    m_nEqualStartIndex = 0;
    m_nMinStartIndex = 0;
    
    m_nMaxEndIndex = 0;
    m_nEqualEndIndex = 0;
    m_nMinEndIndex = 0;
}

CFriendManager::~CFriendManager()
{
    releasActivePlayerContent();
    relaseFriendList();
    CC_SAFE_DELETE(m_pFriendList);
    CC_SAFE_DELETE(m_pActivePlayeres);

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

void CFriendManager::releasActivePlayerContent(int startIndex)
{
    if (m_pActivePlayeres)
    {
        ActivePlayer * tmp = NULL;
        for (int i = startIndex; i < m_pActivePlayeres->size(); i++)
        {
            tmp = m_pActivePlayeres->at(i);
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

void CFriendManager::paraseActivePlayerListByDict(CCDictionary *pRandomFriend)
{
    if (pRandomFriend)
    {
        int count = 0;
        m_nMaxStartIndex = 0;
        
        CCDictionary *maxDict = (CCDictionary*) pRandomFriend->objectForKey("max");
        if (isDictionary(maxDict))
        {
            count += setActivePlayerInfoesByDict(maxDict);
        }
        m_nMaxEndIndex = count;
        m_nEqualStartIndex = count;
        
        CCDictionary *nowDict = (CCDictionary*) pRandomFriend->objectForKey("now");
        if(isDictionary(nowDict))
        {
            count += setActivePlayerInfoesByDict(nowDict,m_nMaxEndIndex);
        }
        m_nEqualEndIndex = count;
        m_nMinStartIndex  = count;
        
        CCDictionary *minDict = (CCDictionary*) pRandomFriend->objectForKey("min");
        if (isDictionary(minDict))
        {
            count += setActivePlayerInfoesByDict(minDict,m_nEqualEndIndex);
        }
        m_nMinEndIndex = count;
        
        if (count < m_pActivePlayeres->size())
        {
            releasActivePlayerContent(count);
            m_pFriendList->erase(m_pFriendList->begin()+count, m_pFriendList->end());
        }

        
    }
    
}
int CFriendManager::setActivePlayerInfoesByDict(CCDictionary *pDict, int startIndex)
{
    int myuid = atoi(SinglePlayer::instance()->getUserId());
    m_pActivePlayeres = m_pActivePlayeres == NULL ? new vector<ActivePlayer*>() : m_pActivePlayeres;
    int elementCount = 0;
    int count = m_pActivePlayeres->size();
    int tempCount = startIndex;
    CCDictElement *element = NULL;
    CCDictionary *elementDict = NULL;
    ActivePlayer *tmpPlayer = NULL;
    CCDICT_FOREACH(pDict, element)
    {
        
        elementDict = (CCDictionary*) element->getObject();
        int friendId = atoi(element->getStrKey());
        if (friendId == myuid)
        {
            continue;
        }

        const char *userName = GameTools::valueForKey("username", elementDict);
        int level = GameTools::intForKey("level", elementDict);
               
        if (tempCount < count)
        {
            tmpPlayer =  m_pActivePlayeres->at(tempCount);
            if (tmpPlayer)
            {
                tmpPlayer->isFriend = friendId;
                tmpPlayer->username = userName;
                tmpPlayer->level = level;
                tmpPlayer->isFriend = false;
               
            }
            else
            {
                tmpPlayer = new ActivePlayer(friendId, level, userName);
                m_pActivePlayeres->at(tempCount) = tmpPlayer;
                
            }
            tempCount++;
            
            
        }else
        {
            tmpPlayer = new ActivePlayer(friendId, level, userName);
            m_pActivePlayeres->push_back(tmpPlayer);
        }
        elementCount++;
    }
    
    return elementCount;
}
void CFriendManager::getActivePlayerListFromLocal(vector<ActivePlayer*> *outActivePlayer)
{
    if (outActivePlayer)
    {
        outActivePlayer->clear();
        gernerActivePlayerList(outActivePlayer);
        sort(outActivePlayer->begin(), outActivePlayer->end());
        if (m_pFriendList)
        {
            for (int i = 0; i < outActivePlayer->size(); i++)
            {
                for (int j = 0; j < m_pFriendList->size(); j++)
                {
                    if(outActivePlayer->at(i)->friend_uid == m_pFriendList->at(j)->getFriendUid())
                    {
                        outActivePlayer->at(i)->isFriend = true;
                        break;
                    }
                }
                
            }
        }

    }
        

}

void CFriendManager::gernerActivePlayerList(vector<ActivePlayer *>*outActivePlayer)
{
   int sum = 0;
   int count =  fillActivePlayer(outActivePlayer, m_nMaxStartIndex, 0, m_nMaxEndIndex, MAXPLAYERCOUNT);
   sum += count;
   m_nMaxStartIndex = (m_nMaxStartIndex+count)%(MAXPLAYERCOUNT);

   count = fillActivePlayer(outActivePlayer, m_nEqualStartIndex, m_nMaxEndIndex, m_nEqualEndIndex, NOWPLAYERCOUNT+(MAXPLAYERCOUNT-sum));
   sum += count;
   m_nEqualStartIndex = (m_nEqualStartIndex+count)%(NOWPLAYERCOUNT);
    
   count = fillActivePlayer(outActivePlayer, m_nMinStartIndex, m_nEqualEndIndex, m_nMinEndIndex, MINPLAYERCOUNT+(MAXPLAYERCOUNT+NOWPLAYERCOUNT - sum));
    m_nMinStartIndex = (m_nMinStartIndex+count) %(MINPLAYERCOUNT);
}

int CFriendManager:: fillActivePlayer(vector<ActivePlayer*> *outActivePlayeres, int nowstartIndex, int orignstartIndex, int endIndex, int count)
{
    
    int hasCount = endIndex - orignstartIndex;
    if (hasCount == 0)
    {
        return 0;
    }
    if (hasCount < count)
    {
        
        for (int i = nowstartIndex; i < endIndex && i < m_pActivePlayeres->size(); i++)
        {
            outActivePlayeres->push_back(m_pActivePlayeres->at(i));
        }
        if ((endIndex - nowstartIndex) < count)
        {
            for (int i = 0; i < count - endIndex- nowstartIndex; i++)
            {
                outActivePlayeres->push_back(m_pActivePlayeres->at(i+orignstartIndex));
            }
        }
       
    }
    else
    {
        for (int i = orignstartIndex ; i < endIndex && i < m_pActivePlayeres->size(); i++)
        {
            outActivePlayeres->push_back(m_pActivePlayeres->at(i));
        }
    }
    
    return hasCount <= count ? hasCount : count;
}


