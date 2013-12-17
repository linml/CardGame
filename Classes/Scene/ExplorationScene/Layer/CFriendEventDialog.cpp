//
//  CFriendEventDialog.cpp
//  91.cube
//
//  Created by phileas on 13-11-19.
//
//
#include "CFriendEventDialog.h"
#include "gameConfig.h"
#include "CPtTool.h"
#include "PtHttpClient.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"

CFriendEventDialog* CFriendEventDialog::create(Friend inFriend)
{
    CFriendEventDialog *dialog = new CFriendEventDialog();
    if (dialog)
    {
        dialog->setFriend(inFriend);
        if (dialog->init())
        {
            dialog->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(dialog);
            dialog = NULL;
        }
    }
    return dialog;
}

bool CFriendEventDialog::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(125, 125, 125, 200)));
        initCFriendEventDialog();
        bRet = true;
    } while (0);
    return bRet;
}

void CFriendEventDialog::initCFriendEventDialog()
{
    loadResource();
     CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_plistPath, "dialog_bg.plist"), CSTR_FILEPTAH(g_mapImagesPath, "dialog_bg.png"));
    // create bg:
    CCTexture2D * texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_mapImagesPath, "dialog_bg.png"));
    CCSpriteBatchNode * bathNode = CCSpriteBatchNode::createWithTexture(texture, 5);
 
    CCPoint point = ccp(280, 280);
    CCSprite * top = CCSprite::createWithSpriteFrameName("dialog_top.png");
    CCSprite * mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
    CCSprite * bottom = CCSprite::createWithSpriteFrameName("dialog_bottom.png");
    bottom->setPosition(point);
    bottom->setAnchorPoint(CCPointZero);
    top->setAnchorPoint(CCPointZero);
    mid->setAnchorPoint(CCPointZero);
    CCSize size1 = bottom->boundingBox().size;
    mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height-1));
    bathNode->addChild(mid);
    CCSize size2 = mid->boundingBox().size;
    int count = 2;
    for (int i = 1; i < 2; i++)
    {
        mid = CCSprite::createWithSpriteFrameName("dialog_mid.png");
        mid->setAnchorPoint(CCPointZero);
        mid->setPosition(ccp(bottom->getPositionX(), bottom->getPositionY()+size1.height+i*size2.height-i-1));
        bathNode->addChild(mid);
    }
    
    top->setPosition(ccp(mid->getPositionX(), mid->getPositionY()+size2.height-count));
    bathNode->addChild(top);
    bathNode->addChild(bottom);
    this->addChild(bathNode);

    
    // left btn:
    CCSprite *node = NULL;
    m_pLeft = CCLabelTTF::create("无视", "Arial", 18);
    
    CCSpriteFrameCache* cach = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame * frame  = cach->spriteFrameByName("Use_Normal.png");
    m_pFrame[0] = frame;
    CC_SAFE_RETAIN(m_pFrame[0]);
    node =CCSprite::createWithSpriteFrame(frame);
    node->setAnchorPoint(ccp(0,1));
    CCSize size =  node->boundingBox().size;
    m_pLeft->setPosition(ccp(size.width/2, size.height/2));
    m_pLeft->setColor(ccc3(126, 60, 30));
    node->addChild(m_pLeft);
    m_pBtn[0] = node;
    Utility::addTouchRect(LEFT_TAG, node, m_cTouches);
    addChild(node, 100);
    node->setPosition(ccpAdd(point,ccp(60, 55)));

    // right btn
    node=CCSprite::createWithSpriteFrame(frame);
    m_pRight = CCLabelTTF::create("加好友", "Arial", 18);
    node->addChild(m_pRight);
       node->setAnchorPoint(ccp(0,1));
    size =  node->boundingBox().size;
    m_pRight->setPosition(ccp(size.width/2, size.height/2));
    m_pRight->setColor(ccc3(126, 60, 30));
    frame  = cach->spriteFrameByName("Use_Pressed.png");
    m_pFrame[1] = frame;
    CC_SAFE_RETAIN(m_pFrame[1]);
    m_pBtn[1] = node;
    Utility::addTouchRect(RIGHT_TAG, node, m_cTouches);
    addChild(node, 100);
    node->setPosition(ccpAdd(point,ccp(300, 55)));
    
    CCNode *temp = createFriendInfo();
    temp->setPosition(ccpAdd(point,ccp(80, 120)));
    addChild(temp);
    // set touch
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-30000);
    setRightHandler(this, callfuncO_selector(CFriendEventDialog::onClickYes));
    setLeftHandler(this, callfuncO_selector(CFriendEventDialog::onClickNo));
}


bool CFriendEventDialog::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CPtTool::isInNode(m_pBtn[0], pTouch))
    {
        m_pBtn[0]->setDisplayFrame(m_pFrame[1]);
        m_nTouchTag = LEFT_TAG;
    }
    else if(CPtTool::isInNode(m_pBtn[1], pTouch))
    {
        m_pBtn[1]->setDisplayFrame(m_pFrame[1]);
        m_nTouchTag = RIGHT_TAG;
    }
    else
    {
        m_nTouchTag = -1;
    }
    
    
    return true;
}
void CFriendEventDialog::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_nTouchTag != -1)
    {
        if (m_nTouchTag == RIGHT_TAG)
        {
            m_pBtn[1]->setDisplayFrame(m_pFrame[0]);
            
        }else if(m_nTouchTag == LEFT_TAG)
        {
            m_pBtn[0]->setDisplayFrame(m_pFrame[0]);
        }
        
        if((m_nTouchTag== RIGHT_TAG && CPtTool::isInNode(m_pBtn[1], pTouch))||(m_nTouchTag == LEFT_TAG && CPtTool::isInNode(m_pBtn[0], pTouch)))
        {
            handlerTouch();
            PtSoundTool::playSysSoundEffect("UI_click.wav");
        }else
        {
            
        }
    }
}

CCNode* CFriendEventDialog::createFriendInfo()
{
    CCNode *node = CCNode::create();
    int iconIndex = (m_sFriend.fried_uid % 5) +1;
    char buffer[200] = {0};
    sprintf(buffer, "%d.png",iconIndex);
    CCSprite *icon = CCSprite::create(CSTR_FILEPTAH(g_friendIconPath, buffer));
    node->addChild(icon);
    CCSize size = icon->getContentSize();
    
    const char *userName = m_sFriend.username.c_str();
    CCLabelTTF *label = CCLabelTTF::create(userName, "Arial", 15);
    label->setColor(g_custom_color[15]);
  
    label->setPosition(ccp(0.5, -size.height/2));
    node->addChild(label);
    
    const char *pre = "在冒险中偶遇了";
    const char *last = ",多个朋友多条路，留下美好的回忆吧";
    snprintf(buffer,200,"%s%s%s", pre,userName, last);
    label = CCLabelTTF::create(buffer, "Arial", 18);
    label->setDimensions(CCSizeMake(300, 0));
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setPosition(ccp(size.width, size.height/2));
    label->setAnchorPoint(ccp(0, 1));
    label->setColor(g_custom_color[15]);
    node->addChild(label);
    return node;
    
}

void CFriendEventDialog::onClickYes(CCObject *pObject)
{
    char buffer[200]={0};
    snprintf(buffer,200, "%d",m_sFriend.fried_uid);
    string connectData="sig=";
    connectData += SinglePlayer::instance()->getUserSig();
    connectData +=string("&friend_uid=")+buffer;
    ADDHTTPREQUESTPOSTDATANOLOCK(STR_URL_SEARCHFRIEND(196), "CALLBACK_CFriendEventDialog::onClickYes", "CFriendEventDialog::onClickYes",connectData.c_str(),callfuncO_selector(CFriendEventDialog::onReceiveMsg));
}
void CFriendEventDialog::onClickNo(CCObject *pObject)
{
    closeDialog();
}



void CFriendEventDialog::onReceiveMsg(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CFriendEventDialog::onClickYes");
    if (!object )
    {
        CCMessageBox("服务端传输的是", "error");
        return ;
    }
    char * tempdata=(char *)object;
    //解析字符串。 赋值与vector中
    CCDictionary *dict=PtJsonUtility::JsonStringParse(tempdata);
    CCLog("tempdata:%s",tempdata);
    delete [] tempdata;
    tempdata = NULL;
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    CCDictionary *resultDict=(CCDictionary*)(dict->objectForKey("result"));
    if (GameTools::intForKey("info", resultDict)==1)
    {
        
    }
    else
    {
        CCMessageBox("没有该ID","");
    }
    closeDialog();
}

void CFriendEventDialog::setCloseCallBack(cocos2d::CCObject *pObject, SEL_CallFunc inSelector)
{
    m_pCloseHandler = pObject;
    m_pCloseSelector = inSelector;
}


void CFriendEventDialog::closeDialog()
{
    removeFromParentAndCleanup(true);
    if (m_pCloseHandler && m_pCloseSelector)
    {
        (m_pCloseHandler->*m_pCloseSelector)();
    }
}