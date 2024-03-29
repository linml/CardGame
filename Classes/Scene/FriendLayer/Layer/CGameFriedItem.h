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
    //添加定时器刷新的方法
    void upDateCountdown(float t);
    void startUpdate();
    void stopUpdate();
    void checkAndStartUpdateText(const char *sRightText);
private:
    void setRightButton(const char *sRightText);
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
    CC_SYNTHESIZE(int, m_nCountDown, CountDown);
    CC_SYNTHESIZE(std::string, m_sRightText, RightText);
};

#endif /* defined(___1_cube__CGameFriedItem__) */
