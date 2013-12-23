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

static bool compareLevel(ActivePlayer * param1, ActivePlayer *param2);

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
    
        CCDictionary *maxDict = (CCDictionary*) pRandomFriend->objectForKey("max");
        if (isDictionary(maxDict))
        {
            count += setActivePlayerInfoesByDict(maxDict);
        }
      
        m_pActivePlayerListInfo[0].setValue(0, count, MAXPLAYERCOUNT);
        CCDictionary *nowDict = (CCDictionary*) pRandomFriend->objectForKey("now");
        if(isDictionary(nowDict))
        {
            count += setActivePlayerInfoesByDict(nowDict,m_pActivePlayerListInfo[0].endIndex);
        }
 
        m_pActivePlayerListInfo[1].setValue(m_pActivePlayerListInfo[0].endIndex, count,NOWPLAYERCOUNT);
        
        CCDictionary *minDict = (CCDictionary*) pRandomFriend->objectForKey("min");
        if (isDictionary(minDict))
        {
            count += setActivePlayerInfoesByDict(minDict,m_pActivePlayerListInfo[1].endIndex);
        }
    
        m_pActivePlayerListInfo[2].setValue(m_pActivePlayerListInfo[1].endIndex, count, MINPLAYERCOUNT);
        
        if (count < m_pActivePlayeres->size())
        {
            releasActivePlayerContent(count);
            m_pActivePlayeres->erase(m_pActivePlayeres->begin()+count, m_pActivePlayeres->end());
        }
        
        sortLevelData();
        
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
                tmpPlayer->friend_uid = friendId;
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
        sort(outActivePlayer->begin(), outActivePlayer->end(), compareLevel);
//        if (m_pFriendList)
//        {
//            for (int i = 0; i < outActivePlayer->size(); i++)
//            {
//                for (int j = 0; j < m_pFriendList->size(); j++)
//                {
//                    if(outActivePlayer->at(i)->friend_uid == m_pFriendList->at(j)->getFriendUid())
//                    {
//                        outActivePlayer->at(i)->isFriend = true;
//                        break;
//                    }
//                }
//                
//            }
//        }

    }
        

}

void CFriendManager::gernerActivePlayerList(vector<ActivePlayer *>*outActivePlayer)
{    
    if (m_pActivePlayeres->size() <= 10)
    {
        outActivePlayer->assign(m_pActivePlayeres->begin(), m_pActivePlayeres->end());
        return;
    }

    
    int addArray[3] = {0};
    int j = 0;
    for (int i = 0; i < 3; i++)
    {
        j = index[i];
        PlayerLevelData &temp = m_pActivePlayerListInfo[j];
        addArray[i] = fillActivePlayer(outActivePlayer, temp.startIndex, temp.orginStartIndex, temp.endIndex, temp.moveCount);
        temp.startIndex = (temp.startIndex-temp.orginStartIndex+addArray[i]) %(temp.count) + temp.orginStartIndex;
        addArray[i] -= temp.moveCount;
    }
    
    
    if (addArray[2] < 0)
    {
        if (addArray[1] <= 0)
        {
            int count = abs((addArray[2]-addArray[1]));
            PlayerLevelData &temp = m_pActivePlayerListInfo[index[0]];
            count = fillActivePlayer(outActivePlayer, temp.startIndex, temp.orginStartIndex, temp.endIndex, count);
            temp.startIndex = (temp.startIndex-temp.orginStartIndex+count) %(temp.count) + temp.orginStartIndex;
        }else
        {
            int tmpCount = 0;
            int count = abs(addArray[2]);
            PlayerLevelData &temp = m_pActivePlayerListInfo[index[0]];
            tmpCount = fillActivePlayer(outActivePlayer, temp.startIndex, temp.orginStartIndex, temp.endIndex, count);
            temp.startIndex = (temp.startIndex-temp.orginStartIndex+tmpCount) %(temp.count) + temp.orginStartIndex;
            
            if (tmpCount < count)
            {
                count -= tmpCount;
                PlayerLevelData &temp = m_pActivePlayerListInfo[index[1]];
                tmpCount = fillActivePlayer(outActivePlayer, temp.startIndex, temp.orginStartIndex, temp.endIndex, count);
                temp.startIndex = (temp.endIndex-temp.orginStartIndex+tmpCount) %(temp.count) + temp.orginStartIndex;
            }
        }
    }
}

int CFriendManager:: fillActivePlayer(vector<ActivePlayer*> *outActivePlayeres, int nowstartIndex, int orignstartIndex, int endIndex, int count)
{
    
    int hasCount = endIndex - orignstartIndex;
    if (hasCount == 0)
    {
        return 0;
    }
    if (hasCount > count)
    {
        
        for (int i = nowstartIndex; i < endIndex && i < (nowstartIndex+count) && i < m_pActivePlayeres->size(); i++)
        {
            outActivePlayeres->push_back(m_pActivePlayeres->at(i));
        }
        if ((endIndex - nowstartIndex) < count)
        {
            for (int i = 0; i < (count - (endIndex- nowstartIndex)); i++)
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

void CFriendManager::sortLevelData()
{
    int sum = 0;
    int data[3]={0};
    for (int i = 0; i < 3; i++)
    {
        sum += m_pActivePlayerListInfo[i].count;
        data[i] = m_pActivePlayerListInfo[i].count - m_pActivePlayerListInfo[i].moveCount;
        index[i] = 0;
    }
    if (sum <= 10)
    {
        return;
    }
    
    if (data[0]  < data[2])
    {
        index[2] = 0;
        index[0] = 2;
    }
    
    if (data[1] > data[index[0]])
    {
        index[1] = index[0];
        index[0] = 1;
    }else if(data[1] < data[index[2]])
    {
        index[1] = index[2];
        index[2] = 1;
    }
}
bool compareLevel(ActivePlayer * param1, ActivePlayer *param2)
{
    int result = 0;
    if (param1 && param2)
    {
        result = param1->level-param2->level;
    }
    if(result > 0)
    {
        return  true;
    }
    else
    {
        return false;
    }
}

