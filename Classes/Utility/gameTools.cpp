#include "gameTools.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "cocos2d.h"
#include "string.h"
#include "gameConfig.h"
#include "SimpleAudioEngine.h"
#include "HBLabelPanel.h"

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
    
    vector<string> splitString(const string& s_str,const char* from_c,const char* to_c)
    {
        string substr = getSubStr(s_str,from_c,to_c);
        return splitArgString(substr.c_str(),",");
    }
    
    vector<std::string> splitStringSpecial(const char *str)
	{
		string mainStr = str;
		vector<std::string> tmp;
		int pos = 0;
		while((pos = mainStr.find(";",pos+1,1))!=string::npos)
		{
			string str1 = mainStr.substr(0,pos);
			int count1 = find_count_of(str1.c_str(),"(");
			int count2 = find_count_of(str1.c_str(),")");
			if (count1 == count2)
			{
				tmp.push_back(mainStr.substr(0,pos));
				mainStr = mainStr.substr(pos+1);
				pos = 0;
			}
		}
		tmp.push_back(mainStr);
		return tmp;
	}
    
    int find_count_of(const char *str,const char *c)
	{
		string tmp = str;
		int pos = 0;
		int count = 0;
		while((pos = tmp.find(c))!=string::npos)
		{
			tmp = tmp.substr(pos+1);
			count++;
		}
		return count;
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
    
    CCArray  *arrayForKey(const char *key,cocos2d::CCDictionary *dict)
    {
        return (CCArray*)dict->objectForKey(key);
    }
    
    vector<std::string> splitStringByLength(const char *str,const char *c)
	{
		vector<std::string> tmp;
		stringstream ss(str);
		string sub_str;
        int bufLen=100;
		while(getline(ss,sub_str,*c))
		{
            if(sub_str.size()>bufLen)
            {
                const char* obj=sub_str.c_str();
                //                printf("xianbei NOTICE obj : %s\n",obj);
                char col_and_size_info[16]="";
                memset(col_and_size_info,0,16);
                short cpyLen=memchr(obj, '$', 10)==NULL?6:9;
                strncpy(col_and_size_info,obj,cpyLen);
                //                printf("xianbei NOTICE col_and_size_info : %s\n",col_and_size_info);
                string tmp_str;
                for(int i=cpyLen;i<sub_str.size();)
                {
                    int step = 0;
                    while(true)
                    {
                        if (sub_str[i+step] == 0 || !(sub_str[i+step]&0x80) || (sub_str[i+step] & 0xc0)==0xc0)
                        {
                            if (step >= bufLen || sub_str[i+step] == 0) {
                                break;
                            }
                        }
                        step++;
                    }
                    tmp_str = col_and_size_info + sub_str.substr(i, step);
                    tmp.push_back(tmp_str);
                    i+=step;
                }
            }
            else {
                tmp.push_back(sub_str);
            }
		}
		return tmp;
	}
    
    CCLabelTTF* createWord(const string& word,const string& font,int intFontSize,ccColor3B color)
    {
        CCLabelTTF* lab = CCLabelTTF::labelWithString(word.c_str(), font.c_str(), intFontSize);
        lab->setAnchorPoint(ccp(0.5,0.5));
        lab->setColor(color);
        return lab;
    }


    
    //生成有颜色区分的文字
    CCLayer* createDiffColorWord(const string& word,const string& fontName,int fontSize,CCPoint anchor,int iChangeLineW,bool isByLen)
    {
        //解析文字 sscanf(str,"%x",&i);
        //modify xianbei splitStringByLength 功能：避免因为文字太长导致程序退出
        vector<string> vSubWord;
        if(isByLen)
        {
            vSubWord = splitStringByLength(word.c_str(), "#");
        }
        else
        {
            vSubWord = splitString(word.c_str(), "#");
        }
        CCLayer* layer = CCLayer::create();
        if(vSubWord.size() <= 1) {
            if(iChangeLineW == 0) {
                CCLabelTTF* ttf = createWord(word, fontName, fontSize, ccc3(255, 255, 255));
                ttf->setAnchorPoint(anchor);
                layer->addChild(ttf);
                layer->setContentSize(ttf->getContentSize());
            }
            else {
                //换行
                HBLabelPanel* ttf = new HBLabelPanel(iChangeLineW);
                ttf->addLabelTTF(word.c_str(), fontSize, ccc3(255, 255, 255),0);
                layer->addChild(ttf);
                layer->setContentSize(CCSizeMake(ttf->getWidth(), ttf->getHeight()));
            }
            
        }
        else {
            //        CCLOG("vSubWord size = %d",vSubWord.size());
            //        CCSprite* sprite = new CCSprite();
            //        sprite->autorelease();
            string sColor;
            string sSzie;
            int R,G,B;
            int fSize=fontSize;
            int wordStart=6;
            int x = 0;
            int w = 0;
            HBLabelPanel* ttf;
            if(iChangeLineW != 0) {
                ttf = new HBLabelPanel(iChangeLineW);
                ttf->setTag(0);
                layer->addChild(ttf);
            }
            for(int i = 1; i < vSubWord.size(); i++) {
                if(vSubWord.at(i) == "") {
                    continue;
                }
                //头六个字符是颜色值
                sColor = vSubWord.at(i).substr(0,2);
                //16进制转十进制
                sscanf(sColor.c_str(),"%x", &R);
                //                CCLOG("colorR = %s,R = %d", sColor.c_str(),R);
                //头六个字符是颜色值
                sColor = vSubWord.at(i).substr(2,2);
                //16进制转十进制
                sscanf(sColor.c_str(),"%x", &G);
                //                CCLOG("colorG = %s,G = %d", sColor.c_str(),G);
                //头六个字符是颜色值
                sColor = vSubWord.at(i).substr(4,2);
                //16进制转十进制
                sscanf(sColor.c_str(),"%x", &B);
                //                CCLOG("colorB = %s,B = %d", sColor.c_str(),B);
                //                CCLog("subWord = %s", vSubWord.at(i).c_str());
                //第七第八位为字体大小
                if(*(vSubWord.at(i).substr(6,1).begin())=='$')
                {
                    sSzie = vSubWord.at(i).substr(7,2);
                    sscanf(sSzie.c_str(),"%x", &fSize);
                    wordStart=9;
                    //bq add
//                    #if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//                    if(iPlatfrom == PLATFORM_IPHONE) {
//                        fSize /= 2;
//                    }
//                    #endif
                }
                if(iChangeLineW == 0) {
                    CCLabelTTF* subttf = createWord(vSubWord.at(i).substr(wordStart), fontName, fSize, ccc3(R, G, B));
                    subttf->setAnchorPoint(ccp(0,0));
                    subttf->setPosition(ccp(x,-anchor.y * subttf->getContentSize().height));
                    subttf->setTag(i);
                    layer->addChild(subttf);
                    x += subttf->getContentSize().width;
                }
                else {
                    //判断是否有“\n”
                    //                    string
                    string tmpWord = vSubWord.at(i).substr(wordStart);
                    //                    string tmpWord = "啊大是大/%非阿德发生/%11111的发";
                    //                    CCLog("tmpWord = %s",tmpWord.c_str());
                    int index = 0;
                    if ((index = tmpWord.find("/%"))!=string::npos) {
                        //换行
                        string tmpSubWord = tmpWord;
                        string tmpLeftSubWord = tmpSubWord;
                        while ((index = tmpLeftSubWord.find("/%"))!=string::npos) {
                            tmpSubWord = tmpLeftSubWord.substr(0,index);
                            //                            CCLog("tmpSubWord 1111 = %s",tmpSubWord.c_str());
                            ttf->addLabelTTF(tmpSubWord.c_str(), fSize, ccc3(R, G, B),0);
                            ttf->addEnter();
                            tmpLeftSubWord = tmpLeftSubWord.substr(index + 2);
                            //                            CCLog("index = %d",index);
                            //                            CCLog("tmpSubWord 2222 = %s",tmpLeftSubWord.c_str());
                        }
                        
                        ttf->addLabelTTF(tmpLeftSubWord.c_str(), fSize, ccc3(R, G, B),0);
                    }
                    else {
                        //换行
                        ttf->addLabelTTF(vSubWord.at(i).substr(wordStart).c_str(), fSize, ccc3(R, G, B),0);
                    }
                }
            }
            if(iChangeLineW == 0) {
                w = x;
                //根据anchor调整位置
                for(int i = 1; i < vSubWord.size();i++) {
                    CCLabelTTF* subttf = (CCLabelTTF*)layer->getChildByTag(i);
                    if(subttf) {
                        CCPoint pos = subttf->getPosition();
                        subttf->setPosition(ccp(pos.x - w * anchor.x,pos.y));
                    }
                }
                layer->setContentSize(CCSizeMake(w,fontSize));
            }
            else {
                //换行
                HBLabelPanel* ttf = (HBLabelPanel*)layer->getChildByTag(0);
                ttf->setAnchorPoint(anchor);
                layer->setContentSize(CCSizeMake(ttf->getWidth(), ttf->getHeight()));
            }
        }
        
        return layer;
    }

}
