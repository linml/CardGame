//
//  PVPDataManager.h
//  91.cube
//
//  Created by phileas on 14-1-10.
//
//

#ifndef ___1_cube__PVPDataManager__
#define ___1_cube__PVPDataManager__
#include "cocos2d.h"
#include "CPVPPublicStruct.h"
using namespace cocos2d;

class PVPDataManager
{
public:
    static PVPDataManager*getInstance();
    static void releasManager();
protected:
    static PVPDataManager* s_pIntance;
    static bool s_bIsHandlerNetwork;
protected:
    PVPDataManager();
    ~PVPDataManager();
    
    //get pvp main ui info:
    void getPVPMainUIInfoFromServer();
    void parsePVPMainUIInfo(CCObject *pObject);
};

#endif /* defined(___1_cube__PVPDataManager__) */
