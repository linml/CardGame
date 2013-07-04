#ifndef _GAME_TOOLS_H_
#define _GAME_TOOLS_H_ 

#include <string.h>
#include <iostream>
#include <vector>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

typedef float ccTime;

namespace GameTools {
    
    CCPoint pointFromString(const string& str);
    
    const char * valueForKey(const char *key, CCDictionary* dict);
    
    bool boolForKey(const char *key, cocos2d::CCDictionary* dict);
    
	int intForKey(const char *key, cocos2d::CCDictionary* dict);
    
	float floatForKey(const char *key, cocos2d::CCDictionary* dict);
    
	CCPoint ccPointForKey(const char *key, cocos2d::CCDictionary* dict);
    
	CCPoint pointForKey(const char *key, cocos2d::CCDictionary* dict);
    
	CCRect rectForKey(const char *key, cocos2d::CCDictionary* dict);
    
	CCRect ccRectForKey(const char *key, cocos2d::CCDictionary* dict);
    
	ccColor3B ccc3ForKey(const char *key, cocos2d::CCDictionary* dict);
    
    //add By Merlin
    CCArray *arrayForKey(const char *key, cocos2d::CCDictionary* dict);
    
    
    
    template<class T>
	std::string ConvertToString(T value)
	{
		std::stringstream ss;
		ss<<value;
		return ss.str();
	}


    /*************字符串解析**************/
    
    //解析字符串 获取“{}”里边的信息
    vector<string> splitArgString(const char *str, const char *c);
    
    //字符串解析 以 c 为分隔符，将 str 分割，提取相邻两个 c 之间的信息
    vector<std::string> splitString(const char *str, const char *c);
    vector<string> splitString(const string& s_str,const char* from_c,const char* to_c);

    
    //获取字串 搜索 s_str 截取从 from 到字符串 find_str 之间的字符串
    std::string getSubStr_endStr(const string& s_str, const char* find_str, int from = 0);
    
    //获取子串 截取 s_str 中 from_c 到 to_c 之间的子串
    string getSubStr(const string& s_str, const char* from_c, const char* to_c);
    
    //获取子串 截取 s_str 中 第一个或者最后一个(isFromEnd) from_c 之后的子串
    string getSubStr(const string& s_str, const char* from_c, bool isFromEnd = false);
    
    vector<std::string> splitStringSpecial(const char *str);
    
    int find_count_of(const char *str,const char *c);
    
    /*************容器类型转换**************/
    
    //将数字字符容器，装换为整型容器
    vector<int> transToIntVector(const vector<string>& vValue);
    
    //将长度为len的整型数组，转换为整型容器
    vector<int> transToIntVector(const int* value,int len);
    
    
    vector<std::string> splitStringByLength(const char *str,const char *c);
    //生成有颜色区分的文字
    CCLayer* createDiffColorWord(const string& word,const string& fontName,int fontSize,CCPoint anchor = ccp(0,0),int iChangeLineW = 0,bool isByLen=false);
    //生成文字
    CCLabelTTF* createWord(const string& word,const string& font,int intFontSize,ccColor3B color);



};

#endif