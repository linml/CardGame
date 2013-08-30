//
//  CMyDictionary.cpp
//  91.cube
//
//  Created by linminglu on 13-8-27.
//
//

#include "CMyDictionary.h"
void CMyDictionary::InsertItem(const char * pszKey, int nValue)
{
   m_cRoot[pszKey] =  Json::Value(nValue);
}
void CMyDictionary::InsertItem(const char * pszKey, double fValue)
{
     m_cRoot[pszKey] = Json::Value(fValue);
}
void CMyDictionary::InsertItem(const char * pszKey, const std::string  pszValue)
{
    m_cRoot[pszKey] = Json::Value(pszValue);
}

void CMyDictionary::InsertItem(const char * pszKey, const char * pszValue)
{
     m_cRoot[pszKey] = Json::Value(pszValue);
}
void CMyDictionary::InsertSubItem(const char * pszKey, CMyDictionary * pSubDictionary)
{
     m_cRoot[pszKey] = pSubDictionary->m_cRoot;
}
bool CMyDictionary::InsertArrayItem(const char * pszArrayKey, int nValue)
{
    Json::Value array;
    if(m_cRoot.isMember(pszArrayKey))
    {
        if (!m_cRoot[pszArrayKey].isArray() && !m_cRoot[pszArrayKey].isConvertibleTo(Json::arrayValue))
            return false;
        array = m_cRoot[pszArrayKey];
    }
    array.append(nValue);
    m_cRoot[pszArrayKey] = array;
    return true;
}
bool CMyDictionary::InsertArrayItem(const char * pszArrayKey, double fValue)
{
    Json::Value array;
    if(m_cRoot.isMember(pszArrayKey))
    {
        if (!m_cRoot[pszArrayKey].isArray() && !m_cRoot[pszArrayKey].isConvertibleTo(Json::arrayValue))
            return false;
        array = m_cRoot[pszArrayKey];
    }
    array.append(fValue);
    m_cRoot[pszArrayKey] = array;
    return true;
}
bool CMyDictionary::InsertArrayItem(const char * pszArrayKey, const char * pszValue)
{
    Json::Value array;
    if(m_cRoot.isMember(pszArrayKey))
    {
        if (!m_cRoot[pszArrayKey].isArray() && !m_cRoot[pszArrayKey].isConvertibleTo(Json::arrayValue))
            return false;
        array = m_cRoot[pszArrayKey];
    }
    array.append(pszValue);
    m_cRoot[pszArrayKey] = array;
    return true;
}

bool CMyDictionary::InsertArrayItem(const char * pszArrayKey, Json::Value value)
{
    Json::Value array;
    if(m_cRoot.isMember(pszArrayKey))
    {
        if (!m_cRoot[pszArrayKey].isArray() && !m_cRoot[pszArrayKey].isConvertibleTo(Json::arrayValue))
            return false;
        array = m_cRoot[pszArrayKey];
    }
    array.append(value);
    m_cRoot[pszArrayKey] = array;
    return true;
}

const char * CMyDictionary::GetDescriptionString()
{
    return m_cRoot.toStyledString().c_str();
}