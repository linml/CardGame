//
//  CGamePropBufferTipLayer.h
//  91.cube
//
//  Created by phileas on 13-10-31.
//
//

#ifndef ___1_cube__CGamePropBufferTipLayer__
#define ___1_cube__CGamePropBufferTipLayer__

#include "cocos2d.h"
#include "CPlayerBufferManager.h"
#include "CBufferLogo.h"
using namespace cocos2d;


class CPropBufferLogo :public CBufferLogo
{
public:
    static CPropBufferLogo* create(PropBuffer &inBuffer);
public:
    CPropBufferLogo(PropBuffer &inBuffer);
    virtual ~CPropBufferLogo();
    bool initCPropBufferLogo();

};

class CGamePropBufferTipLayer : public CCLayer
{
public:
    CREATE_FUNC(CGamePropBufferTipLayer);
    
    CGamePropBufferTipLayer();
    virtual ~CGamePropBufferTipLayer();
    virtual bool init();

protected:
    void initCGamePropBufferTipLayer();
    void updateUI(float dt);

protected:
    CCNode *m_pIconNodeContainer;
    CCDictionary *m_pIconContainer;
    CPlayerBufferManager *m_pPlayerBufferManager;
};

#endif /* defined(___1_cube__CGamePropBufferTipLayer__) */
