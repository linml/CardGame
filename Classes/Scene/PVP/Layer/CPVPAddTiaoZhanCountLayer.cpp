//
//  CPVPAddTiaoZhanCountLayer.cpp
//  91.cube
//
//  Created by phileas on 14-1-15.
//
//

#include "CPVPAddTiaoZhanCountLayer.h"
#include "gamePlayer.h"
#include "CPtTool.h"
#include "PtHttpClient.h"
#include "PtJsonUtility.h"
#include "CReward.h"
#include "CGameRechargeTip.h"
CPVPAddTiaoZhanCountLayer::CPVPAddTiaoZhanCountLayer()
{
    
}
CPVPAddTiaoZhanCountLayer::~CPVPAddTiaoZhanCountLayer()
{
    
}
bool CPVPAddTiaoZhanCountLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        initCPVPAddTiaoZhanCountLayer();
        setTouchEnabled(true);
        setTouchMode(kCCTouchesOneByOne);
        bRet =true;
    } while (0);
    return bRet;
    
}

bool CPVPAddTiaoZhanCountLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(CPtTool::isInNode(m_pBtn, pTouch))
    {
        return true;
    }
    return false;
}
void CPVPAddTiaoZhanCountLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
void CPVPAddTiaoZhanCountLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(CPtTool::isInNode(m_pBtn, pTouch))
    {
        onClickAddChangeNum();
    }
}
void CPVPAddTiaoZhanCountLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CPVPAddTiaoZhanCountLayer::initCPVPAddTiaoZhanCountLayer()
{
    CCSize size = CCSizeMake(140, 40);
//    CCLayerColor* layer = CCLayerColor::create(ccc4(125, 0, 0, 125));
//    layer->setContentSize(size);
//    addChild(layer);
    setContentSize(size);

    ignoreAnchorPointForPosition(false);
    setAnchorPoint(CCPointZero);
    int currentCount = SinglePlayer::instance()->getPlayerPVPCount();
    char buffer[10] = {0};
    sprintf(buffer, "%d/%d",currentCount,SinglePlayer::instance()->getPVPTotalNum());
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial", 13);
    CCPoint point(0,size.height*0.5);
    label->setAnchorPoint(ccp(0,0.5));
    label->setPosition(point);
    addChild(label);
    m_pNumLabel = label;
    const char * title = "增加";
    CCSprite *tmpSprite = CCSprite::create(CSTR_FILEPTAH(g_mapImagesPath, "jieshouanniu_Normal.png"));
    CCLabelTTF *tmpLabel = CCLabelTTF::create(title, "Arial", 14);
    point.x += 30;
    tmpLabel->setPosition(ccp(tmpSprite->getContentSize().width*0.5, tmpSprite->getContentSize().height*0.5));
    tmpSprite->addChild(tmpLabel);
    tmpSprite->setPosition(point);
    tmpSprite->setAnchorPoint(ccp(0,0.5));
    addChild(tmpSprite);
    m_pBtn = tmpSprite;
}

void CPVPAddTiaoZhanCountLayer::onClickAddChangeNum()
{
    if (SinglePlayer::instance()->getPlayerPVPCount() < SinglePlayer::instance()->getPVPTotalNum())
    {
        if (SinglePlayer::instance()->getPlayerCash() < SinglePlayer::instance()->getCostCashPerAddPVPCount())
        {
           CCLayer* layer = CGameRechargeTip::create();
           CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 100);

        }else
        {
             sendAddChangeNumRequest();
        }
       
    }else
    {
        CCMessageBox("挑战次数已满", "tip");
    }
}
void CPVPAddTiaoZhanCountLayer::updateNumState()
{
    int currentCount = SinglePlayer::instance()->getPlayerPVPCount();
    char buffer[10] = {0};
    sprintf(buffer, "%d/%d",currentCount,SinglePlayer::instance()->getPVPTotalNum());
    m_pNumLabel->setString(buffer);
}
void CPVPAddTiaoZhanCountLayer::sendAddChangeNumRequest()
{
    retain();
    char buffer[100]={0};
    snprintf(buffer, sizeof(buffer),"sig=%s",STR_USER_SIG);
    ADDHTTPREQUESTPOSTDATA(STR_URL_ADDPVPFIHGHTCOUNT(194), "CALLBACK_CPVPAddTiaoZhanCountLayer::sendAddChangeNumRequest", "REQUEST_CPVPAddTiaoZhanCountLayer::sendAddChangeNumRequest", buffer, callfuncO_selector(CPVPAddTiaoZhanCountLayer::receiveAddChangeNumMsg));

}
void CPVPAddTiaoZhanCountLayer::receiveAddChangeNumMsg(CCObject *pObject)
{
    release();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CPVPAddTiaoZhanCountLayer::sendAddChangeNumRequest");
    char *buffer = (char*) pObject;
    if(buffer)
    {
        CCLog("the buff:%s", buffer);
        CCDictionary *resultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        int code = GameTools::intForKey("code", resultDict);
        if (code == 0)
        {
            const char * names[] = {"result","reward", "dec","buy_pvp_num"};
            for (int i = 0; i < 4; i++)
            {
                resultDict = (CCDictionary*) resultDict->objectForKey(names[i]);
                if(resultDict== NULL)
                {
                    break;
                }
            }
            if (resultDict)
            {
                SinglePlayer::instance()->addPlayerPVPCount();
                CReward *reward = CReward::create(resultDict);
                reward->excuteReward(DEC);
                updateNumState();
            }
        }
        
    }
}