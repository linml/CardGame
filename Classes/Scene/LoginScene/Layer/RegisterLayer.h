//
//  RegisterLayer.h
//  91.cube
//
//  Created by xianbei1987 on 13-8-29.
//
//

#ifndef _1_cube_RegisterLayer_h
#define _1_cube_RegisterLayer_h

#include "gameConfig.h"
#include "PtHttpClient.h"

class CRegisterLayer : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuRegisterCallback(CCObject* pSender);
    void menuCancelCallback(CCObject* pSender);
    void menuLoginCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CRegisterLayer);
    
    void doRegiter();
    
    static CRegisterLayer* create(void* parent);
    
    void onReceiveRegiterMsg(CCObject *pOject);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void* m_Parent;
private:
    CCEditBox* m_pEditName;
    CCEditBox* m_pEditEMail;
    CCEditBox* m_pEditPassword;
    CCEditBox* m_pEditPassword_re;
    
    CCLabelTTF* m_pErrInf;
    

};

class CSubLoginLayer : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuLoginCallback(CCObject* pSender);
    void menuNoAccountCallback(CCObject* pSender);
    void menuCancelCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CSubLoginLayer);
    
    void doLogin();
    
    static CSubLoginLayer* create(void* parent);
    static CSubLoginLayer* create(void* parent,const char* pchName ,const char* pchPassword );
    
    void onReceiveRegiterMsg(CCObject *pOject);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void* m_Parent;

    CCEditBox* m_pEditEMail;
    CCEditBox* m_pEditPassword;
    
    CCLabelTTF* m_pErrInf;
    
    
};

class CRegisterNickNameLayer : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    CREATE_FUNC(CRegisterNickNameLayer);
    static CRegisterNickNameLayer* create(void* parent);
    void* m_Parent;
    CCEditBox* m_pEditName;
    CCSprite* m_hero;
    
    CCSprite* m_roleMale;
    CCSprite* m_roleFemale;
    CCSprite* m_selectSprite;
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool handleTouchSpritePool(CCPoint point);
    vector<TouchRect> touchRect;
    
    int m_nGender;
};

#endif
