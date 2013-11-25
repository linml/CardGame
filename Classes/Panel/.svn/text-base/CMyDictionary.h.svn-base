//
//  CMyDictionary.h
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#ifndef ___1_cube__CMyDictionary__
#define ___1_cube__CMyDictionary__

#include <iostream>
#include "json.h"
class CMyDictionary
{
public:
    void InsertItem(const char * pszKey, int nValue);
    void InsertItem(const char * pszKey, double fValue);
    void InsertItem(const char * pszKey, const char * pszValue);
    void InsertItem(const char * pszKey, const std::string  pszValue);
    void InsertSubItem(const char * pszKey, CMyDictionary * pSubDictionary);
    bool InsertArrayItem(const char * pszArrayKey, int nValue);
    bool InsertArrayItem(const char * pszArrayKey, double fValue);
    bool InsertArrayItem(const char * pszArrayKey, const char * pszValue);
    bool InsertArrayItem(const char * pszArrayKey, Json::Value value);
    const char * GetDescriptionString();
public:
    Json::Value  m_cRoot;
};


#endif /* defined(___1_cube__CMyDictionary__) */
