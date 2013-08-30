//
//  CGameEmailManager.h
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#ifndef ___1_cube__CGameEmailManager__
#define ___1_cube__CGameEmailManager__

#include <iostream>
#include "cocos2d.h"
#include "CSingleton.h"
#include "gameStruct.h"
using namespace std;
using namespace cocos2d;
class CGameEmailData;
class CMyDictionary;
class CGameEmailManager :public cocos2d::CCObject
{
public:
    CGameEmailManager();
    ~CGameEmailManager();
    void decodeEmap(std::string str);
    void decodeEmap(CCDictionary *dict);
    void postHttpRequest();
    void getHttpReponse(CCObject *object);
    void changeEmailStatus();
public:
    int  getCurrentTotalEmail();
    int  getUpdateTotalEmail();
    int  getCurrentEmailMapMaxMsgId();
    int  getCurrentEmailUnreadCount();
    void setCurrentTotalEmail(int value);
    void setUpdateTotalEmail(int value);
    void loadLocalEmail();
    void deleteAllEmail();
    void writeToFile();
    void deleteEmailByEmailId(int msgID);
    void deleteEmailData(vector<int >emilIdList);
    std::string getJsonData();
    CMyDictionary * createMydict();
    void cleareMyDictionaryList();
    void testData();
    int getMailCount();
    CGameEmailData *getGameDataByIndex(int index);
    void removeGameEmailData(list<int >vlistEmailList);
    void copyDataTovectory(vector<EMAIL_DATA>&vEmaildata,int data=-1);
public:
    int m_iGetHttpStatus;
    map<int , CGameEmailData *>m_gameEmail;
    list<CMyDictionary *>m_ldataManagerMydict;
};

typedef Singleton<CGameEmailManager> G_GAMESINGEMAIL;

#endif /* defined(___1_cube__CGameEmailManager__) */
