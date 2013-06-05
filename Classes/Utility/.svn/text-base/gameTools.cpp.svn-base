#include "gameTools.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "cocos2d.h"
#include "string.h"
#include "gameConfig.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

namespace GameTools {    

    vector<std::string> splitArgString(const char *str, const char *c)
	{
		vector<std::string> tmp;
        string ss(str);
		string sub_str;
        
        int start = 0;
        int end = 0;
        
        do {
            end = ss.find(c,start);
            if(end == string::npos) {
                sub_str = ss.substr(start);
            }
            else {
                sub_str = ss.substr(start,end - start);
            }
            
            //如果sub_str含有"{",寻找"}",忽略掉两者包围的"c" 
            if (sub_str.find("{") != string::npos) {
                int index = ss.find("}",start);
                end = ss.find(c,index);
                sub_str = ss.substr(start,end - start);
            }

            tmp.push_back(sub_str);
            start = end+1;
            if(end == string::npos) {
                break;
            }
        } while (true);
        
		return tmp;
	}

    //搜索s_str 截取从from到字符串find_str之间的字符串
    std::string getSubStr_endStr(const string& s_str, const char* find_str, int from)
    {
        int index = s_str.find_first_of(find_str);
        if (index != string::npos) {
            return s_str.substr(0,index);
        }
        return "";
    }
    
    vector<std::string> splitString(const char *str, const char *c)
	{
		vector<std::string> tmp;
		stringstream ss(str);
		string sub_str;
		while(getline(ss,sub_str,*c))
		{
			tmp.push_back(sub_str);
		}
		return tmp;
	}
    
    string getSubStr(const string& s_str, const char* from_c, const char* to_c)
    {
        int start = s_str.find_first_of(from_c);
        int end = s_str.find_last_of(to_c);
        if (start != string::npos && end != string::npos) {
            string substr = s_str.substr(start+1,end-(start+1));
            return substr;
        }
        return "";
    }
    
    string getSubStr(const string& s_str, const char* from_c, bool isFromEnd)
    {
        int start = s_str.find_first_of(from_c);
        if(isFromEnd) {
            start = s_str.find_last_of(from_c);
        }
        if (start != string::npos) {
            string substr = s_str.substr(start+1);
            return substr;
        }
        return "";
    }
    
    //将数字字符容器，装换为整型容器
    vector<int> transToIntVector(const vector<string>& vValue)
    {
        vector<int> vIntData;
        for (int i = 0; i < vValue.size(); i++) {
            vIntData.push_back(atoi(vValue[i].c_str()));
        }
        return vIntData;
    }
    
    //将长度为len的整型数组，转换为整型容器
    vector<int> transToIntVector(const int* value, int len)
    {
        vector<int> vIntData;
        //        int size = sizeof(*value)/sizeof(int) + 1;
        for (int i = 0; i < len; i++)
        {
            vIntData.push_back(value[i]);
        }
        return vIntData;
    }
    
    CCPoint pointFromString(const string& str)
	{
		float tmp[2];
		string value = str;
		stringstream ss1;
		ss1<<value.substr(1,value.size() - 1);
		string sub_str;
		int count = 0;
		while(getline(ss1,sub_str,','))
		{
			tmp[count] = atof(sub_str.c_str());
			count++;
		}
		return ccp(tmp[0],tmp[1]);
	}
    
    
    /**********************************/
    const char * valueForKey(const char *key, CCDictionary* dict)
	{
		if (dict)
		{
			CCString* pString = (CCString*)dict->objectForKey(std::string(key));
			return pString ? pString->m_sString.c_str() : "";
		}
		return "";
	}
    
    bool boolForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		return atoi(valueForKey(key, dict));
	}
    
	int intForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		return atoi(valueForKey(key, dict));
	}
    
	float floatForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		return atof(valueForKey(key, dict));
	}
    
	CCPoint ccPointForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		string value = string(valueForKey(key, dict));
		return pointFromString(value);
	}
    
	CCPoint pointForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		float tmp[2];
		stringstream ss1(valueForKey(key, dict));
		string sub_str;
		int count = 0;
		while(getline(ss1,sub_str,','))
		{
			tmp[count] = atof(sub_str.c_str());
			count++;
		}
		return ccp(tmp[0],tmp[1]);
	}
    
	CCRect rectForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		float tmp[4];
		stringstream ss1(valueForKey(key, dict));
		string sub_str;
		int count = 0;
		while(getline(ss1,sub_str,','))
		{
			tmp[count] = atof(sub_str.c_str());
			count++;
		}
		return CCRectMake(tmp[0],tmp[1],tmp[2],tmp[3]);
	}
    
	CCRect ccRectForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		float tmp[4];
		string value = string(valueForKey(key, dict));
		stringstream ss1;
		ss1<<value.substr(1,value.size() - 1);
		string sub_str;
		int count = 0;
		while(getline(ss1,sub_str,','))
		{
			tmp[count] = atof(sub_str.c_str());
			count++;
		}
		return CCRectMake(tmp[0],tmp[1],tmp[2],tmp[3]);
	}
    
	ccColor3B ccc3ForKey(const char *key, cocos2d::CCDictionary* dict)
	{
		float tmp[3];
		stringstream ss1(valueForKey(key, dict));
		string sub_str;
		int count = 0;
		while(getline(ss1,sub_str,','))
		{
			tmp[count] = atoi(sub_str.c_str());
			count++;
		}
		return ccc3(tmp[0],tmp[1],tmp[2]);
	}
}
