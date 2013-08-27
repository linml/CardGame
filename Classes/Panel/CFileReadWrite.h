//
//  CFileReadWrite.h
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#ifndef ___1_cube__CFileReadWrite__
#define ___1_cube__CFileReadWrite__

#include <iostream>
#include <string>
#include "cocos2d.h"
using namespace  cocos2d;
using namespace std;
class CFileReadWrite {
public:
    /** 读取本地文件，返回数据 */
    static string getFileByName(string pFileName);
    
    /** 储存内容到文件 */
    static bool saveFile(char* pContent,string pFileName);
    
};
#endif /* defined(___1_cube__CFileReadWrite__) */
