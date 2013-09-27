//
//  CPanelGameLog.h
//  91.cube
//
//  Created by linminglu on 13-9-27.
//
//

#ifndef ___1_cube__CPanelGameLog__
#define ___1_cube__CPanelGameLog__

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;
class CPanelGameLog
{
public :
    CPanelGameLog (const char *szFileName ); // set the filename of logfile
    ~CPanelGameLog();
    void SetFileName(const char *szFileName); // close last logfile and rename the filename
    const char * GetFileName();
    void Close();
    bool IsOpen();
    void AddLog(const char  *lpBuffer, unsigned long dwLength); // append log
    void Log(const char * szString, ...);
protected :
    fstream m_file;
    char *m_szFileName;
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    pthread_mutex_t m_csLock;
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    CRITICAL_SECTION m_csLock;
#endif
    
    long m_lGrdCnt;
    bool OpenFile(); // open file and move the point to the end of file
    void Lock();
    void UnLock();
    unsigned long Write(const char *lpBuffer, unsigned long dwLength);
    virtual  void WriteLog(const char *lpBuffer, unsigned long dwLength);// writeLog
private://屏蔽函数
    CPanelGameLog (const CPanelGameLog &);
    CPanelGameLog &operator = (const CPanelGameLog &);
};

extern void initLogCreate(const char *filename);
extern void closeLogCreate(const char *filename);
extern void appendFileLog(const char *pStr,...);

#endif /* defined(___1_cube__CPanelGameLog__) */
