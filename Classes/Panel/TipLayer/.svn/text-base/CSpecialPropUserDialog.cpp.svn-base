//
//  CSpecialPropUserDialog.cpp
//  91.cube
//
//  Created by phileas on 13-11-22.
//
//

#include "CSpecialPropUserDialog.h"
#include "PtJsonUtility.h"
#include "PtHttpClient.h"
#include "gameConfig.h"


CSpecialPropUserDialog * CSpecialPropUserDialog::create(USERTYPE inFunctionId, SPECIALPROPDATA specialPropData)
{
    CSpecialPropUserDialog *layer = new CSpecialPropUserDialog();
    if (layer)
    {
        layer->setFunctonId(inFunctionId);
        layer->setSpecialPropData(specialPropData);
        if (layer->init())
        {
            layer->autorelease();
        }else
        {
            CC_SAFE_DELETE(layer);
            layer = NULL;
        }
        
    }
    return layer;
}

CSpecialPropUserDialog::CSpecialPropUserDialog()
{
    
}
CSpecialPropUserDialog::~CSpecialPropUserDialog()
{
    
}
bool CSpecialPropUserDialog::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!CGameUserPropDialogLayer::init());
        initSpecialPropUserDialog();
        bRet = true;
    } while (0);
    return  bRet;
}

void CSpecialPropUserDialog::initSpecialPropUserDialog()
{
    setRightHandler(this, callfuncO_selector(CSpecialPropUserDialog::onClickUserOrRecharge));
}
/*
 *   sig=2ac2b1e302c46976beaab20a68ef95(用户标识码)&item_id=1(道具ID)&use_coin=1(是否使用道具)
 (1,回城 2探路,3消buff)
 &special_type=1&special_params={"chapter_id"(章id):1,"part_id"(节id):1,"shop_id"(商店id):80000}
 &special_type=3&special_params={"effect_id"(buff的效果id):1}
 &special_type=2
 */

void CSpecialPropUserDialog::onClickUserOrRecharge(CCObject *pObject)
{
    char buffer[300] = {0};
    int use_coin = m_bHaveProp ? 0:1;
    int specialType = 0;
    CCString *paramString = NULL;
    if (m_eFunctionId == HUICHENG)
    {
        paramString =  CCString::createWithFormat("&special_params={\"chapter_id\":%d,\"part_id\":%d,\"shop_id\":%d}", m_uSpecialPropData.saveData.chapterId, m_uSpecialPropData.saveData.partId, m_uSpecialPropData.saveData.shopId);
        specialType = 1;
    }
    else if(m_eFunctionId == XIAOBUFFER)
    {
        paramString =  CCString::createWithFormat("&special_params={\"effect_id\":%d,\"shop_id\":%d}",m_uSpecialPropData.effectId, 800000);
        specialType = 3;
    }else if(m_eFunctionId == TANLU)
    {
         paramString =  CCString::createWithFormat("&special_params={\"shop_id\":%d}", 800000);
        specialType = 2;
    }
    if (paramString)
    {
        sprintf(buffer, "sig=%s&item_id=%d&use_coin=%d&special_type=%d%s",STR_USER_SIG,m_nPropId,use_coin,specialType, paramString->getCString());
    }else
    {
         sprintf(buffer, "sig=%s&item_id=%d&use_coin=%d&special_type=%d",STR_USER_SIG,m_nPropId,use_coin,specialType);
    }
    CCLog("buffer: %s",buffer);
    ADDHTTPREQUESTPOSTDATA(STR_URL_USESPEACIALPROP(196), "CALLBACK_CSpecialPropUserDialog::onClickUserOrRecharge", "REQUEST_CSpecialPropUserDialog::onClickUserOrRecharge", buffer,callfuncO_selector( CSpecialPropUserDialog::onRecieveRequestMsg));
    
}
void CSpecialPropUserDialog::onRecieveRequestMsg(CCObject *pObject)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CSpecialPropUserDialog::onClickUserOrRecharge");
    char *buffer = (char *)pObject;
    if(buffer)
    {
        CCLog("the buffer: %s", buffer);
        CCDictionary * pResultDict = PtJsonUtility::JsonStringParse(buffer);
        delete [] buffer;
        int code = GameTools::intForKey("code", pResultDict);
        if (code == 0)
        {
            pResultDict = (CCDictionary*) pResultDict->objectForKey("result");
            if (m_bHaveProp)
            {
                handlerUserResult(pResultDict);
            }
            else
            {
                handlerBuyResult(pResultDict);
            }
            
            if (m_eFunctionId == XIAOBUFFER)
            {
                 CPlayerBufferManager::getInstance()->clearAltarBufferById(m_uSpecialPropData.effectId, false);
            }
            if (m_pSuccessCallBack && m_pSuccessHandler)
            {
                (m_pSuccessHandler->*m_pSuccessCallBack)();
            }
            
        }
        else
        {
            CCMessageBox(CCString::createWithFormat("error code :%d", code)->getCString(), "error tip");
        }
    }
    onClose();

}