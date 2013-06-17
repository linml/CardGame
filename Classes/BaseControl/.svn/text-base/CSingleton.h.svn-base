//
//  CSingleton.h
//  en.cube
//
//  Created by linminglu on 13-5-23.
//
//

#ifndef __en_cube__CSingleton__
#define __en_cube__CSingleton__

#include <iostream>
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
#include <pthread.h>
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32

#endif

using namespace std;
class CResGuard {
public:
    CResGuard() ;
    ~CResGuard() ;
    bool IsGuarded() const;
    
public:
    class CGuard {
    public:
        CGuard(CResGuard& rg) : m_rg(rg) { m_rg.Guard(); }
        ~CGuard() { m_rg.Unguard(); }
        
    private:
        CResGuard & m_rg;
    };
    
private:
    void Guard() ;
    void Unguard() ;
    friend class CResGuard::CGuard;
private:
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID
    pthread_mutex_t m_cs;
#elif CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
    CRITICAL_SECTION m_cs;
#endif
    long m_lGrdCnt;
};


template<class T>
class Singleton
{
public:
    static inline T * instance();
private:
    Singleton(){}
    ~Singleton(){}
    Singleton(const Singleton &){}
    Singleton &operator =(const Singleton &){}
    
    static auto_ptr<T> _instance;
    static CResGuard _rs;
};

template <class T>
auto_ptr<T> Singleton<T>::_instance;

template <class T>
CResGuard Singleton<T>::_rs;

template <class T>
inline T* Singleton<T>::instance()
{
    if( 0 == _instance.get() )
    {
        CResGuard::CGuard gd(_rs);
        if( 0== _instance.get())
        {
            _instance.reset ( new T);
        }
    }
    return _instance.get();
}

#define DECLARE_SINGLETON_CLASS( type ) \
friend class auto_ptr< type >;\
friend class Singleton< type >;


#endif /* defined(__en_cube__CSingleton__) */
