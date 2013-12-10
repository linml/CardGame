//
//  CEveryDayLoginRewardInfoLayer.cpp
//  91.cube
//
//  Created by phileas on 13-12-9.
//
//

#include "CEveryDayLoginRewardInfoLayer.h"
#include "gameConfig.h"
CEveryDayLoginRewardInfoLayer *CEveryDayLoginRewardInfoLayer::create(CEveryDayLoginDataStruct * data)
{
    CEveryDayLoginRewardInfoLayer* layer = new CEveryDayLoginRewardInfoLayer();
    if (layer)
    {
        if (layer->init())
        {
            layer->autorelease();
            layer->updateView(data);
        }else
        {
            delete layer;
            layer = NULL;
        }
    }
    return layer;
}

CEveryDayLoginRewardInfoLayer::CEveryDayLoginRewardInfoLayer()
{
    m_pDropIcon = NULL;
    m_pItemLabels = NULL;
}
CEveryDayLoginRewardInfoLayer::~CEveryDayLoginRewardInfoLayer()
{
    CC_SAFE_DELETE(m_pItemLabels);
}

bool CEveryDayLoginRewardInfoLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCEveryDayLoginRewardInfoLayer();
        bRet = true;
    } while (0);
    return bRet;
}
void CEveryDayLoginRewardInfoLayer::updateView(CEveryDayLoginDataStruct * data)
{
    if (data)
    {
        CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(data->getIconFullPathByType(1).c_str());
        if (texture)
        {
            m_pDropIcon->setTexture(texture);
        }
        vector<CEveryDayItemValue *> *items = data->getDropData();
        int itemCount = items==NULL ? 0 : items->size();
        itemCount = itemCount > 5 ? 5 : itemCount;
        char buffer[100] = {0};
        
        for (int i = 0; i < itemCount; i++)
        {
            snprintf(buffer, sizeof(buffer), "%s * %d", items->at(i)->m_sItemValueStr.c_str(), items->at(i)->count);
            ((CCLabelTTF*) m_pItemLabels->objectAtIndex(i))->setString(buffer);
        }
        for (int i = itemCount; i < 5; i++)
        {
            ((CCLabelTTF*) m_pItemLabels->objectAtIndex(i))->setString("");
        }
    }
}

void CEveryDayLoginRewardInfoLayer::initCEveryDayLoginRewardInfoLayer()
{
    
    CCSize size = CCSizeMake(256, 384);
    CCPoint point = ccp(size.width/2, size.height-50);
    ignoreAnchorPointForPosition(false);
    setContentSize(size);
    m_pItemLabels = new CCArray(5);
    m_pDropIcon = CCSprite::create(CSTR_FILEPTAH(g_loginRewardIconPath, "2_2.png"));
    m_pDropIcon->setPosition(point);
    addChild(m_pDropIcon);
    CCLabelTTF * tmpTips = NULL;
    tmpTips = CCLabelTTF::create("签到后可从宝箱中领取一个奖励", "Arial", 16);
    point.y -= 50;
    tmpTips->setPosition(point);
    addChild(tmpTips);
    
    for (int i = 0; i < 5; i++)
    {
        tmpTips = CCLabelTTF::create("", "Arial", 13);
        point.y -= 40;
        tmpTips->setPosition(point);
        addChild(tmpTips);
        m_pItemLabels->addObject(tmpTips);
    }
    
}
