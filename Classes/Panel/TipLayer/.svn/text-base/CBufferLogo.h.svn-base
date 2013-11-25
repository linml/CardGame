//
//  CBufferLogo.h
//  91.cube
//
//  Created by phileas on 13-11-4.
//
//

#ifndef ___1_cube__CBufferLogo__
#define ___1_cube__CBufferLogo__

#include <string>
#include "cocos2d.h"
#include "CPlayerBufferManager.h"

using namespace cocos2d;
using std::string;

class CBufferLogo : public CCNode
{
public:
    CBufferLogo(const Buffer& inBuffer);
    ~CBufferLogo();
    bool initCBufferLog(const std::string &inIcon);
    void updateTime(int inTime);
    void updateTime();
    const Buffer &getBufferData(){return m_rBuffer;};
protected:
    CCSprite * m_pLogo;
    CCLabelTTF *m_pTime;
    const Buffer &m_rBuffer;
};


#endif /* defined(___1_cube__CBufferLogo__) */
