//
//  PtJsonUtility.cpp
//  91.cube
//
//  Created by xianbei1987 on 13-6-4.
//
//

#include "PtJsonUtility.h"
#include "json.h"
#include "gameTools.h"

namespace PtJsonUtility {
    
    json::Object getJsonObjectByFile(const char* sFileName, bool bWriteable) {
        unsigned long size;
        const char* sFullName;
        string path;

        sFullName = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(sFileName);
        CCFileUtils::sharedFileUtils()->setPopupNotify(false);
        
        unsigned char* sData = CCFileUtils::sharedFileUtils()->getFileData(sFullName, "r", &size);
        json::Object oFullObj = getJsonObjectByString((char*)sData);
        return oFullObj;
    }
    
    Json::Value getJsonValueByFile(const char* sFileName, bool bWriteable)
    {
        unsigned long size;
        const char* sFullName;
        string path;
        
        sFullName = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(sFileName);
        CCFileUtils::sharedFileUtils()->setPopupNotify(false);
        
        unsigned char* sData = CCFileUtils::sharedFileUtils()->getFileData(sFullName, "r", &size);

        Json::Reader reader;
        Json::Value root;
        if (reader.parse((char*)sData, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
        {
            return root;
        }
        return Json::Value();
    }
    
    json::Object getJsonObjectByString(const char* stringData)
    {
        unsigned long size;
        json::Object oFullObj;
        CCFileUtils::sharedFileUtils()->setPopupNotify(true);
        
        if (stringData) {
            size = strlen(stringData);
            if(!size)
                return oFullObj;
        }
        
        try {
            std::stringstream ss;
            std::string str;
            str.append(stringData, size);
            
            ss << str;
            json::Reader::Read(oFullObj, ss);
            
            //		CCLOG("object is %s\n", ss.str().c_str());
        } catch (const json::Exception& e) {
            std::stringstream serror;
            serror << "Caught json::Exception: " << e.what() << std::endl << std::endl;
            
            CCLOG("%s\n", serror.str().c_str());
            //		free(sData);
            CC_SAFE_DELETE_ARRAY(stringData);
            return oFullObj;
        }
        
        //	free(sData);
        CC_SAFE_DELETE_ARRAY(stringData);
        return oFullObj;
    }
    
    void ParseDic(Json::Value val,CCDictionary* m_DicData)
    {
        Json::Value::iterator jsonIndex = val.begin();
        int i=0;
        while (jsonIndex != val.end()) {
            switch (val[jsonIndex.memberName()].type()) {
                case (Json::objectValue):
                {
                    CCDictionary* subDicData=new CCDictionary();
                    ParseDic(val[jsonIndex.memberName()],subDicData);
                    m_DicData->setObject(subDicData, jsonIndex.memberName());
                    subDicData->autorelease();                    
                    break;
                }
                case (Json::stringValue):
                {
                    CCString* data=new CCString(val[jsonIndex.memberName()].asString());
                    printf("\t%-5dkey %-20s value :%-20s\n",i,jsonIndex.memberName(),data->m_sString.c_str());
                    m_DicData->setObject(data, jsonIndex.memberName());
                    break;
                }
                case (Json::intValue):
                {
                    CCString* data=new CCString( GameTools::ConvertToString( val[jsonIndex.memberName()].asInt()));
                    printf("\t%-5dkey %-20s value :%-20s\n",i,jsonIndex.memberName(),data->m_sString.c_str());
                    m_DicData->setObject(data, jsonIndex.memberName());
                    break;
                }
                case (Json::arrayValue):
                {
                    
                    CCString* data=new CCString( GameTools::ConvertToString( val[jsonIndex.memberName()].asInt()));
                    printf("\t%-5dkey %-20s value :%-20s\n",i,jsonIndex.memberName(),data->m_sString.c_str());
                    m_DicData->setObject(data, jsonIndex.memberName());
                    break;
                }

                default:
                    break;
            }
            i++;
            jsonIndex++;
            
        }
    }
    
    void ParseDic(json::Object Object,CCDictionary* m_DicData)
    {
        json::Object::iterator jsonItr;
        json::Object ObjectStr=Object;
        json::Object::iterator jsonItrStr=ObjectStr.Begin();
        int i=0;
        for(jsonItr=Object.Begin();jsonItr!=Object.End();)
        {
            json::Object oFullObjectSub=jsonItr->element;
            if (oFullObjectSub.Size()) {
                CCDictionary* subDicData=new CCDictionary();
                //            printf("%-3ddic %s\n",i,jsonItr->name.c_str());
                ParseDic(oFullObjectSub,subDicData);
                m_DicData->setObject(subDicData, jsonItr->name.c_str());
                subDicData->autorelease();
            }
            else
            {
                std::string dataTemp=json::String(jsonItrStr->element);
                CCString* data=new CCString(dataTemp.c_str());
                printf("\t%-5dkey %-20s value :%-20s\n",i,jsonItr->name.c_str(),data->m_sString.c_str());
                m_DicData->setObject(data, jsonItr->name.c_str());
            }
            jsonItr++;
            jsonItrStr++;
            i++;
        }
    }
    
    CCDictionary* JsonFileParse(const char* fileName)
    {
        CCLog("xianbei FileName : %s",fileName);
//        json::Object oFullObject =  getJsonObjectByFile(fileName);
//        CCDictionary* m_DicData=new CCDictionary();
//        ParseDic(oFullObject,m_DicData);
//        oFullObject.Clear();
        CCDictionary* m_DicData=new CCDictionary();
        ParseDic(getJsonValueByFile(fileName), m_DicData);
        m_DicData->autorelease();
        return m_DicData;
    }
    
    CCDictionary* JsonStringParse(const char* stringData)
    {

//        json::Object oFullObject =  getJsonObjectByString(stringData);
//        CCDictionary* m_DicData=new CCDictionary();
//        ParseDic(oFullObject,m_DicData);
//        oFullObject.Clear();
        Json::Reader reader;
        Json::Value root;
        CCDictionary* m_DicData=new CCDictionary();
        if (reader.parse(stringData, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
        {
            ParseDic(root, m_DicData);
        }
        m_DicData->autorelease();
        return m_DicData;
    }
    
    void printDebugInfOfJson(json::Object o) {
        std::stringstream ss;
        json::Writer::Write(o, ss);
        std::cout << ss.str() << endl;
    }
    
    void writeJsonToFile(json::Object oJsonObject, const char* sFileName) {
        
    }
    
    void writeDictionaryToJsonString(CCDictionary* dic, string& sFileName)
    {
        
    }
    
    CCDictionary* getDicFromPlist(const char* fileName)
    {
        std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName);
        if( (access(fileName, 0 )) == -1 )
        {
            assert("file not exist");
        }
        CCDictionary* data = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
        
        return data;

    }
}