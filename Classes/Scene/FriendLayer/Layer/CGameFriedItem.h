//
//  CGameFriedItem.h
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#ifndef ___1_cube__CGameFriedItem__
#define ___1_cube__CGameFriedItem__

#include "CFriend.h"
#include "cocos2d.h"
using namespace cocos2d;
typedef enum
{
    E_FRIENDED = 1,
    E_APPLY = 2
    
} EFriendItemType;

#define FRIENDLEFTTAG 1
#define FRIENDRIGHTTAG 2
#define FRIENDTOPTAG 3
class CGameFriedItem : public CCSprite
{
public:
    static CGameFriedItem* create(CFriend *friendInfo, int index);
public:
    CGameFriedItem();
    virtual ~CGameFriedItem();
    virtual bool init();
    void setButtonText(const char* leftText, const char *rightText);
    int getTouchTag(CCTouch *pTouch);
    void setItemType(EFriendItemType itemType);
    
    void setSelect(int tag);
    void setNormal(int tag);
protected:
    void initData();
    bool initFriendItem();
protected:
    CCSprite *m_pLeftBtn;
    CCSprite *m_pRightBtn;
    CCSprite *m_pCloseBtn;
    CCLabelTTF* m_pBtnText[2];
    CCTexture2D* m_pTexture[4];
    EFriendItemType m_eItemType;
    CC_SYNTHESIZE(int, m_nIndex, Index);
   
    CC_SYNTHESIZE(CFriend*, m_pFriendInfo, FriendInfo);
};

#endif /* defined(___1_cube__CGameFriedItem__) */
