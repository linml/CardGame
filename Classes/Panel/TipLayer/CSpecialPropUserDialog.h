//
//  CSpecialPropUserDialog.h
//  91.cube
//
//  Created by phileas on 13-11-22.
//
//

#ifndef ___1_cube__CSpecialPropUserDialog__
#define ___1_cube__CSpecialPropUserDialog__

#include "CGameUserPropDialogLayer.h"

struct SAVESECTIONDATA
{
    int chapterId;
    int partId;
    int shopId;
};

union SPECIALPROPDATA
{
    SAVESECTIONDATA saveData;
    int effectId;
};

class CSpecialPropUserDialog : public CGameUserPropDialogLayer
{
public:
    static CSpecialPropUserDialog * create(USERTYPE inFunctionId, SPECIALPROPDATA specialPropData);
    CSpecialPropUserDialog();
    virtual ~CSpecialPropUserDialog();
    bool init();
protected:
    void initSpecialPropUserDialog();
    void onClickUserOrRecharge(CCObject *pObject);
    void onRecieveRequestMsg(CCObject *pObject);
  

    CC_SYNTHESIZE(SPECIALPROPDATA, m_uSpecialPropData, SpecialPropData);
};

#endif /* defined(___1_cube__CSpecialPropUserDialog__) */
