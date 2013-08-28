//
//  CGameErrorConfig.cpp
//  91.cube
//
//  Created by linminglu on 13-8-15.
//
//

#include "CGameErrorConfigLayer.h"

#include "gameConfig.h"
#include "LayoutLayer.h"
#include "PtMapUtility.h"
CErrorDialog::CErrorDialog()
{
    this->m_enDialogType=EN_DIALOGTYPE_NONE;
}
CErrorDialog::~CErrorDialog()
{
    
}
void CErrorDialog::CreateByMsgId(std::string strKeyId,EN_DIALOGTYPE entypeButton)
{
    CCScene *scene=CCDirector::sharedDirector()->getRunningScene();
    if(scene&&scene->getChildByTag(10000))
    {
        
    }
    else
    {
        CErrorDialog * dialog=new CErrorDialog();
        if(dialog && dialog->initCreate(strKeyId,entypeButton))
        {
            dialog->autorelease();
            scene ->addChild(dialog,100,10000);
        }else
        {
            delete dialog;
            dialog=NULL;
        }
    }

}
void CErrorDialog::removeTagArray(string str)
{
    removeChild(Utility::getNodeByTag(this, str),true);
}
bool CErrorDialog::initCreate(std::string strKeyId,EN_DIALOGTYPE enDialogType)
{
    string word = Utility::getWordWithFile("errormsg.plist", strKeyId.c_str());
    m_enDialogType=enDialogType;
    //加载map
    LayoutLayer *tempLayerout=LayoutLayer::create();
    tempLayerout->initWithFile(this, CSTR_FILEPTAH(plistPath, "cuowutishi.plist"));
    tempLayerout->getTouchRects(m_cTouches);
    CCLabelTTF *label=CCLabelTTF::create(word.c_str(), "Arial", 20, CCSizeMake(600, 200), kCCTextAlignmentCenter);
    addChild(label,5,100);
    switch (enDialogType) {
        case EN_DIALOGTYPE_ONLYCANCEL:
        {
            removeTagArray("1,2,1");
            CCSize size=Utility::getNodeByTag(this, "1,1")->getContentSize();
            CCLog("%f,%f",size.width,size.height);
        }
            break;
        case EN_DIALOGTYPE_ONLYOK:
        {
            removeTagArray("1,2,0");
            CCSize size=Utility::getNodeByTag(this, "1,1")->getContentSize();
            CCLog("%f,%f",size.width,size.height);

        }
            break;
        case EN_DIALOGTYPE_ONLYOKANDCANCEL:
        {
            CCSize size=Utility::getNodeByTag(this, "1,1")->getContentSize();
            CCLog("%f,%f",size.width,size.height);
        }
            break;
        default:
            break;
    }
    return true;
}