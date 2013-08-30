//
//  CGameEmailLayer.h
//  91.cube
//
//  Created by linminglu on 13-8-28.
//
//

#ifndef ___1_cube__CGameEmailLayer__
#define ___1_cube__CGameEmailLayer__

#include <iostream>
#include "cocos2d.h"
using namespace  std;
using namespace cocos2d;
class CUIEmailData  :public CCObject{
    
public:
    std::string CUIEmailDataTitle;
    std::string CUiEmailContext;
    
};
enum EN_EMAILHTTPREQUEST {
    EN_EMAILHTTPREQUEST_NONE = 0,
    EN_EMAILHTTPREQUEST_CHANEGSTATUS =1,
    EN_EMAILHTTPREQUEST_GETSINGLEITEM,
    EN_EMAILHTTPREQUEST_GETALLEMAIL
    };


class CGameButtonControl;
class  CGameEmailLayer :public CCLayer 
{
public:
    CGameEmailLayer();
    ~CGameEmailLayer();
    static CGameEmailLayer *CreateEmailLayer();
private:
    bool initCreate();
    bool loadPlistFile();
    void createRecvAllButton();
    void creaetEmailTableView();
    void getAllEmailItem();
    //发送已经读取的表示
    void sendPostHttpChangeEmailStatus();
    void sendPostHttpGetAllItem();
    void sendPostHttpGetSingleItem();
    void recvBockHttpCallBack(CCObject *object);
    bool decodeRecvBackStr(char *data); //true 邮件校验成功
    void showDialogBagFull(CCObject *obect);
    
    void dialogOkButtonSetFunc(CCObject *object);
    void dialogCancelButtonSetFunc(CCObject *object);
private:
    CCNode *m_tempTouchNode;
    EN_EMAILHTTPREQUEST m_enhttpStatus;
    vector<int>canereadList;
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
    void onExit();
};

#endif /* defined(___1_cube__CGameEmailLayer__) */
