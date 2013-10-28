//
//  CPanelGameLog.cpp
//  91.cube
//
//  Created by linminglu on 13-9-27.
//
//

#include "CPanelGameLog.h"
#include <time.h>
#include "gameMiddle.h"

CPanelGameLog::CPanelGameLog(const char *szFileName)
{
    m_szFileName = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    pthread_mutex_init(&m_csLock, NULL);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    ::InitializeCriticalSection(&m_csLock);
#endif  
    char data[400];
    sprintf(data, "%s/%s",Middle::getAppDocumentPath().c_str(),szFileName);
    remove(data);
    SetFileName(data);
   
    //判断如果存在某个文件 就要删除。。
    
}

CPanelGameLog::~CPanelGameLog()
{
    Close();
    if (m_szFileName)
    {
        delete [] m_szFileName;
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    if(pthread_mutex_trylock(&m_csLock)==0)
    {
        pthread_mutex_unlock(&m_csLock);
    }
    pthread_mutex_destroy(&m_csLock);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    ::DeleteCriticalSection(&m_csLock);
#endif
}

void CPanelGameLog::SetFileName(const char *szFileName)
{
    if (szFileName == NULL) return;
    if (m_szFileName)
    {
        delete [] m_szFileName;
        m_szFileName = NULL;
    }
    Close();
    int mfilelength=strlen(szFileName)+1;
    m_szFileName = new char[strlen(szFileName) + 1];
    if (m_szFileName)
    {
        memset(m_szFileName, 0,mfilelength);
        sprintf(m_szFileName,"%s", szFileName);
    }
}
const char *CPanelGameLog::GetFileName()
{
    return m_szFileName;
}
void CPanelGameLog::Close()
{
    if (IsOpen())
    {
        m_file.close();
    }
}
bool CPanelGameLog::IsOpen()
{
    if (m_file)
    {
        if (m_file.is_open())
            return true;
    }
    return false;
    //return !m_file && m_file.is_open();
}
void CPanelGameLog::AddLog(const char  * lpBuffer, unsigned long dwLength)
{
    if (!lpBuffer)
        return;
    
    try {
        Lock();
        if (OpenFile())
        {
            WriteLog(lpBuffer, dwLength);
        }
        UnLock();
    } catch (...) {
         UnLock();
    }
}
void CPanelGameLog::Lock()
{
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    pthread_mutex_lock(&m_csLock);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    EnterCriticalSection(&m_csLock);
#endif
    
}

void CPanelGameLog::UnLock()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    pthread_mutex_unlock(&m_csLock);
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    LeaveCriticalSection(&m_csLock);
#endif
}

bool CPanelGameLog::OpenFile()
{
    if (IsOpen()) return true;
    if (!m_szFileName) return false;
    m_file.open(m_szFileName, ios::in | ios::out | ios::app);
    //m_file.seekp(0, ios::end);
    return IsOpen();
}
unsigned long CPanelGameLog::Write(const char  * lpBuffer, unsigned long dwLength)
{
    unsigned long ulWriteLength = 0;
    if (IsOpen())
    {
        m_file << lpBuffer << std::endl;
        ulWriteLength = dwLength;
    }
    return ulWriteLength;
}
void CPanelGameLog::WriteLog( const char * lpBuffer, unsigned long dwLength)
{
    time_t now;
    char temp[24];
    unsigned long ulWriteLength = 0;
    if (IsOpen())
    {
        time(&now);
        strftime(temp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
        m_file << temp << " ";
        ulWriteLength = dwLength;
        m_file << lpBuffer << std::endl;
        m_file.flush();
    }
}
void CPanelGameLog::Log(const char * szString, ...)
{
    char szEntry[2046];
    va_list args;
    va_start(args, szString);
    vsprintf(szEntry, szString, args);
    AddLog(szEntry, strlen(szEntry));
}

CPanelGameLog *m_gpGameLog;
void initLogCreate(const char *filename)
{
    m_gpGameLog=new CPanelGameLog(filename);
}

void closeLogCreate(const char *filename)
{
    if(m_gpGameLog)
    {
        delete m_gpGameLog;
        m_gpGameLog=NULL;
    }
    
}

void appendFileLog(const char *szString,...)
{
    if(m_gpGameLog)
    {
        unsigned long length=strlen(szString);
        char *szEntry=new char [length+1];
        va_list args;
        va_start(args, szString);
        //vsnprintf(<#char *#>, <#size_t#>, <#const char *#>, <#va_list#>)
        vsprintf(szEntry, szString, args);
        m_gpGameLog->AddLog(szEntry, strlen(szEntry));
        delete [] szEntry;
    }
}