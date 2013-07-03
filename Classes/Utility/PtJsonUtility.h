//
//  PtJsonUtility.h
//  91.cube
//
//  Created by xianbei1987 on 13-6-4.
//
//

#ifndef _1_cube_PtJsonUtility_h
#define _1_cube_PtJsonUtility_h

#include "json/reader.h"
#include "json/writer.h"
#include "json/elements.h"
#include "cocos2d.h"
#include "json.h"

using namespace cocos2d;
using namespace std;

namespace PtJsonUtility {
    
    //解析json文件 返回json对象
    json::Object getJsonObjectByFile(const char* sFileName, bool bWriteable = false);

    //解析json文件 返回Json
    Json::Value getJsonValueByFile(const char* sFileName, bool bWriteable = false);

    //解析json字符串 返回json对象
    json::Object getJsonObjectByString(const char* stringData);
    
    //json文件解析 返回字典对象
    CCDictionary* JsonFileParse(const char* fileName);
    
    //json字符串解析 返回字典对象
    CCDictionary* JsonStringParse(const char* data);
    
    //json对象解析
    void ParseDic(json::Object Object,CCDictionary* m_DicData);
    
    //json对象解析
    void ParseDic(Json::Value val,CCDictionary* m_DicData);
    void ParseArr(Json::Value val,CCArray* m_ArrData);

    
    //json对象 debug信息
    void printDebugInfOfJson(json::Object o);
    
    //保存json文件
    void writeJsonToFile(json::Object oJsonObject, const char* sFileName);
    
    void writeJsonToString(json::Object oJsonObject, string& sFileName);
    
    //从plist文件中获取字典对象
    CCDictionary* getDicFromPlist(const char* fileName);
}
#endif
