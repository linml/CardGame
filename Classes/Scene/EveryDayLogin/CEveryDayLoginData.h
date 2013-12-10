//
//  CEveryDayLoginData.h
//  91.cube
//
//  Created by linminglu on 12/9/13.
//
//

#ifndef ___1_cube__CEveryDayLoginData__
#define ___1_cube__CEveryDayLoginData__

#include <vector>
#include <iostream>
#include <string>
using namespace std;

class CEveryDayLoginData
{
public:
    CEveryDayLoginData()
    {
        m_nServerTime=0;
        memset(m_nQianDaoArray, 0, 31);
    }
    bool getIsLogin(); //判断今天是否签到
    int m_nServerTime; //保存成时间cuo
    bool m_nQianDaoArray[31];
};

struct  CEveryDayItemValue
{
    string m_sItemValueStr; //获取的道具名称
    int  count;             //获取的道具数目
};
//成就每天的拥有的数据
class CEveryDayLoginDataStruct
{
public:
    CEveryDayLoginDataStruct();
    ~CEveryDayLoginDataStruct();
    
    // getter && setter:
    void setSignIn(bool flag){m_bIsSignIn = flag;};
    void setIconType(int iconType){m_nIconType = iconType;};
    void setDropId(int dropId){m_nDropId = dropId;};
    bool IsSignIn(){return m_bIsSignIn;};
    int  getIconType(){return m_nIconType;};
    int  getDayInMonth(){return  m_nDay;};
    void setDayInMonth(int day){m_nDay = day;};
    vector<CEveryDayItemValue *> *getDropData();
    
    std::string getIconFullPathByType(int inType);// inType : 0-1: 0-->格子中的图标，1-－>详细信息中的图标 当m_nIconType == 5时，返回空字符串
protected:
    void initData();
    void loadDropData();
    void deleteDropData();
protected:
    bool m_bIsSignIn;      // 是否签到
    int  m_nIconType;      // icon 图片的类型 1-5:1表示道具包外形，2表示金币包外形，3表示钻石包外形，4表示卡包外形，若填5则日历框不显示图标，四种类型的图标写死
    int  m_nDropId;        // 掉落包ID
    int  m_nDay;
    vector<CEveryDayItemValue *> *m_pItemList; //获取的道具列表 --> 当为null时表示该vector 数据没有加载
    
    
};

class CEveryDayLoginRewardDataContainer
{
public:
    CEveryDayLoginRewardDataContainer();
    ~CEveryDayLoginRewardDataContainer();
    void initWithLoginData(const CEveryDayLoginData * loginData);
    CEveryDayLoginDataStruct* getLoginDataStructByIndex(int index);
    int getDataSize();
protected:
    void loadData();
    void relaseData();
protected:
    vector<CEveryDayLoginDataStruct*> m_pLoginDataContainer;
};



#endif /* defined(___1_cube__CEveryDayLoginData__) */
