//
//  CGameFriedItem.cpp
//  91.cube
//
//  Created by phileas on 13-11-14.
//
//

#include "CGameFriedItem.h"
#include "gameConfig.h"
#include "CPtTool.h"

CGameFriedItem* CGameFriedItem::create(CFriend *friendInfo, int index)
{
    CGameFriedItem *item = new CGameFriedItem();
    if (item)
    {
        item->setFriendInfo(friendInfo);
        item->setIndex(index);
        if (item->init())
        {
            item->autorelease();
        }else
        {
            CC_SAFE_DELETE(item);
            item = NULL;
        }
    }
    return item;
}
CGameFriedItem::CGameFriedItem()
{
    initData();
}
CGameFriedItem::~CGameFriedItem()
{
    for (int i = 0; i < 4; i++)
    {
        CC_SAFE_RELEASE_NULL(m_pTexture[i]);
    }
  
}
bool CGameFriedItem::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!initFriendItem());
        bRet = true;
    } while (0);
    return bRet;
}

void CGameFriedItem::setButtonText(const char* leftText, const char *rightText)
{
    if (m_pBtnText[0]&&leftText)
    {
        m_pBtnText[0]->setString(leftText);
    }
    if (m_pBtnText[1]&&rightText)
    {
        m_pBtnText[1]->setString(rightText);
    }
}

int CGameFriedItem::getTouchTag(CCTouch *pTouch)
{
    int touchTag = -1;
    if (CPtTool::isInNode(m_pLeftBtn, pTouch))
    {
        touchTag = FRIENDLEFTTAG;
    }else if(CPtTool::isInNode(m_pLeftBtn, pTouch))
    {
        touchTag = FRIENDRIGHTTAG;
    }else if(m_eItemType == E_FRIENDED && CPtTool::isInNode(m_pCloseBtn, pTouch))
    {
        touchTag = FRIENDTOPTAG;
    }
    return touchTag;
}

void CGameFriedItem::setItemType(EFriendItemType itemType)
{
    if (itemType != m_eItemType)
    {
        m_eItemType = itemType;
        if (m_eItemType == E_FRIENDED && m_pCloseBtn)
        {
            m_pCloseBtn->setVisible(true);
        }else if(m_eItemType ==E_APPLY && m_pCloseBtn)
        {
            m_pCloseBtn->setVisible(false);
        }
    }
}

void CGameFriedItem::setSelect(int tag)
{
    if (tag == FRIENDLEFTTAG)
    {
        m_pLeftBtn->setTexture(m_pTexture[1]);
    }
    else if (tag == FRIENDRIGHTTAG)
    {
        m_pRightBtn->setTexture(m_pTexture[1]);
    }
    else if (tag == FRIENDTOPTAG)
    {
        m_pCloseBtn->setTexture(m_pTexture[3]);
    }
}
void CGameFriedItem::setNormal(int tag)
{
    if (tag == FRIENDLEFTTAG)
    {
        m_pLeftBtn->setTexture(m_pTexture[0]);
    }
    else if (tag == FRIENDRIGHTTAG)
    {
        m_pRightBtn->setTexture(m_pTexture[0]);
    }
    else if (tag == FRIENDTOPTAG)
    {
        m_pCloseBtn->setTexture(m_pTexture[2]);
    }

}

void CGameFriedItem::initData()
{
    m_pLeftBtn = NULL;
    m_pRightBtn = NULL;
    m_pCloseBtn = NULL;
    m_nIndex = 0;
    m_eItemType = E_FRIENDED;
    m_pFriendInfo = NULL;
    for (int i = 0; i < 2; i++)
    {
        m_pTexture[i] = NULL;
        m_pBtnText[i] = NULL;
    }
    for (int i = 2; i < 4; i++)
    {
        m_pTexture[i] = NULL;
    }
}

bool CGameFriedItem::initFriendItem()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(m_pFriendInfo==NULL);
        CCTextureCache* cache = CCTextureCache::sharedTextureCache();
        CCTexture2D* texture = cache->addImage(CSTR_FILEPTAH(g_mapImagesPath, "shangchengzixiang.png"));
        CC_BREAK_IF(!initWithTexture(texture));
        
        CCSize size = getContentSize();
        
        // add icon
        char buffer[30]={0};
        const char * iconName = "1.png";
        int uid = m_pFriendInfo->getFriendUid()%5+1;
        sprintf(buffer, "%d.png",uid);
        texture = cache->addImage(CSTR_FILEPTAH(g_friendIconPath, buffer));
        CCSprite *icon = CCSprite::createWithTexture(texture);
        addChild(icon);
        icon->setPosition(ccp(size.width*0.25, size.height*0.5));
      
        const char *friendname = m_pFriendInfo->getNickName().c_str(); // "燃烧的胸毛";
        int level = m_pFriendInfo->getLevel(); //999;
        int fight = m_pFriendInfo->getFightPower();        
        
      
        sprintf(buffer, "昵称: %s", friendname);
        CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial", 13);
        label->setAnchorPoint(ccp(0,1));
        addChild(label);
        label->setPosition(ccp(size.width*0.5, size.height*0.7));
    
        sprintf(buffer, "等级: %d", level);
        label = CCLabelTTF::create(buffer, "Arial", 13);
        label->setAnchorPoint(ccp(0,1));
        addChild(label);
        label->setPosition(ccp(size.width*0.5, size.height*0.6));
        sprintf(buffer, "战力: %d", fight);
        label = CCLabelTTF::create(buffer, "Arial", 13);
        label->setAnchorPoint(ccp(0,1));
        addChild(label);
        label->setPosition(ccp(size.width*0.5, size.height*0.5));
        
        // add btn:
        m_pTexture[0] = cache->addImage(CSTR_FILEPTAH(g_mapImagesPath, "Use_Normal.png"));
        m_pTexture[1] = cache->addImage(CSTR_FILEPTAH(g_mapImagesPath, "Use_Pressed.png"));
        m_pTexture[2] = cache->addImage(CSTR_FILEPTAH(g_mapImagesPath, "QuitButton_Normal.png"));
        m_pTexture[3] = cache->addImage(CSTR_FILEPTAH(g_mapImagesPath, "QuitButton_Actived.png"));
        
        for (int i = 0;  i < 4; i++)
        {
            CC_SAFE_RETAIN(m_pTexture[i]);
        }
        
        m_pLeftBtn = CCSprite::createWithTexture(m_pTexture[0]);
        m_pRightBtn = CCSprite::createWithTexture(m_pTexture[0]);
        m_pCloseBtn = CCSprite::createWithTexture(m_pTexture[2]);
        m_pLeftBtn->setPosition(ccp(size.width*0.25, size.height*0.1));
        m_pRightBtn->setPosition(ccp(size.width*0.75, size.height*0.1));
        m_pCloseBtn->setPosition(ccp(size.width*0.9, size.height*0.9));
        addChild(m_pLeftBtn);
        addChild(m_pRightBtn);
        addChild(m_pCloseBtn);
        
        
        size = m_pLeftBtn->getContentSize();
        m_pBtnText[0] = CCLabelTTF::create("资料","Arial", 15);
        m_pBtnText[1] = CCLabelTTF::create("赞美","Arial", 15);
        CCPoint point(size.width/2, size.height/2);
       
        for (int i = 0; i <2; i++)
        {
            m_pBtnText[i]->setPosition(point);
            m_pBtnText[i]->setColor(ccc3(0, 0, 0));
        }
        m_pLeftBtn->addChild(m_pBtnText[0]);
        m_pRightBtn->addChild(m_pBtnText[1]);
        
        
        
        bRet = true;
    } while (0);
       return bRet;
}