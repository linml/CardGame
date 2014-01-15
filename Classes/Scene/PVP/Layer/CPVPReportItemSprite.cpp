//
//  CPVPReportItemSprite.cpp
//  91.cube
//
//  Created by linminglu on 14-1-13.
//
//

#include "CPVPReportItemSprite.h"
#include "CPVPReportItemData.h"
#include "gameConfig.h"
#include "gameTools.h"
#include "CPtTool.h"
#include "CPVPPublicStruct.h"
#define  TAG_PVPREPORT_COLORLAYER 1
CPVPReportItemSprite::CPVPReportItemSprite()
{
    
}

CPVPReportItemSprite::~CPVPReportItemSprite()
{
    
}

CPVPReportItemSprite *CPVPReportItemSprite::CreateByPvpData(CPVPReportItemData *pvp)
{
    CPVPReportItemSprite *sprite=new CPVPReportItemSprite;
    if (! sprite || !sprite->init(pvp) ) {
        delete sprite;
        sprite=NULL;
        return NULL;
    }
    sprite->autorelease();
    return sprite;
}
bool CPVPReportItemSprite::init(CPVPReportItemData *pvp)
{
    this->m_pCPVPReportItemData=pvp;
    createBackgroud();
    createItemIcon();
    createName();
    createTitle();
    createContext();
    createShiJian();
    setContentSize( CCSizeMake(250,100));
    return true;
}

void CPVPReportItemSprite::createBackgroud()
{
    //状态 0 失败 1成功
    CCLayerColor *pColor=CCLayerColor::create();
    pColor->setContentSize(CCSizeMake(250, 100));

    switch (this->m_pCPVPReportItemData->status)
    {
        case 0:
            pColor->setColor(ccc3(255, 0, 0));
            break;
        case 1:
            pColor->setColor(ccc3(0, 255, 0));
            break;
        case 2:
            pColor->setColor(ccc3(125, 125, 125)); //灰色
            break;
        default:
            break;
       
    }
    pColor->setOpacity(255);
   addChild(pColor,1,TAG_PVPREPORT_COLORLAYER);
    
}

void CPVPReportItemSprite::createItemIcon()
{
    char buffer[200] = {0};
    sprintf(buffer, "%d.png",rand()%2+1);
    CCSprite *icon = CCSprite::create(CSTR_FILEPTAH(g_friendIconPath, buffer));
    addChild(icon,1);
    icon->setPosition(CCPointMake(50, 50));
    
}
void CPVPReportItemSprite::createName()
{

    CCLabelTTF *pLabelTTF=CCLabelTTF::create(m_pCPVPReportItemData->m_pvpRankData.name.c_str(), "Arial", 15);
    addChild(pLabelTTF,1);
    pLabelTTF->setPosition(ccp(50,20));
}

void CPVPReportItemSprite::createTitle()
{
    string value;
    if (m_pCPVPReportItemData->status==1) {
        value="你战胜了对方获得XX积分";
    }
    else if(m_pCPVPReportItemData->status==0){
        value="菊花受伤 失去XX积分";
    }
    CCLabelTTF *pLabelTTF=CCLabelTTF::create(value.c_str(), "Arial", 15);
    addChild(pLabelTTF,1);
    pLabelTTF->setAnchorPoint(CCPointMake(0, 0));
    pLabelTTF->setPosition(ccp(80,80));
}

void CPVPReportItemSprite::createContext()
{
    vector<string>strlist;
    strlist.push_back(string("等级:")+ConvertToString(m_pCPVPReportItemData->m_pvpRankData.level));
    strlist.push_back(string("战斗力:")+ConvertToString(m_pCPVPReportItemData->m_pvpRankData.fightpointer));
    for (int i=0; i<strlist.size(); i++) {
        CCLabelTTF *pLabelTTF=CCLabelTTF::create(strlist[i].c_str(), "Arial", 15);
        addChild(pLabelTTF,1);
        pLabelTTF->setAnchorPoint(CCPointMake(0, 0));
        pLabelTTF->setPosition(ccp(80+50*i,50));

    }

}

void CPVPReportItemSprite::createShiJian()
{
    string value;
    CPtTool::getDataShijianChai((time_t)m_pCPVPReportItemData->time,time(NULL),value);
    CCLabelTTF *pLabelTTF=CCLabelTTF::create(value.c_str(), "Arial", 15);
    addChild(pLabelTTF,1);
//    pLabelTTF->setAnchorPoint(CCPointMake(0, 0));
    pLabelTTF->setPosition(ccp(125,20));
}


void CPVPReportItemSprite::updateSetFuChou()
{
    if(getChildByTag(TAG_PVPREPORT_COLORLAYER))
    {
        ((CCLayerColor *)getChildByTag(TAG_PVPREPORT_COLORLAYER))->setColor(ccc3(125, 125, 125));
    }
    
}