//
//  CScreenHandBookLayer.cpp
//  91.cube
//
//  Created by linminglu on 11/7/13.
//
//

#include "CScreenHandBookLayer.h"
#include "CGamesCard.h"
#include "PtHttpURL.h"
#include "PtJsonUtility.h"
#include "Utility.h"
#include "gamePlayer.h"
#include "PtHttpClient.h"
#include <map>
const int handbookheight=650;   //最上面的一点
const int handbookwidth=200;   //起点
const int cardWidth=100;
const int cardHeight=200;

#define DELETE_POINT_VECTOR(VECTORARRAY,VECTORITETYPE,__TYPECLASSNAME__) \
{\
for (VECTORITETYPE::iterator it=VECTORARRAY.begin(); it!= VECTORARRAY.end(); it++) { \
__TYPECLASSNAME__ *temp=*it; \
delete temp; \
temp=NULL; \
} \
VECTORARRAY.erase(VECTORARRAY.begin(),VECTORARRAY.end()); \
CEmrysClearVectorMemory< __TYPECLASSNAME__ *> tempClear(VECTORARRAY) ; \
tempClear.clearVector(); \
}
CHandBookLayerCell::CHandBookLayerCell()
{
    
}

CHandBookLayerCell::~CHandBookLayerCell()
{
    
}

CCPoint CHandBookLayerCell::getCardPostion(int nIndex)
{
        int  nRow=nIndex/5;
        int  nColomn=nIndex%5;
        return CCPointMake(200+nColomn *(cardWidth+5),650-(cardHeight+10)*nRow);
}

CHandBookLayerCell *CHandBookLayerCell::Create(vector<CFightCard *>CardList,int begin,int end)
{
    CHandBookLayerCell *handcell=new CHandBookLayerCell;
    if (!handcell|| !handcell->initCreate(CardList, begin, end))
    {
        CC_SAFE_DELETE(handcell);
        return NULL;
    }
    handcell->autorelease();
    return handcell;
    
    
}
bool CHandBookLayerCell::initCreate(vector<CFightCard *>CardList,int begin,int end)
{
    for (int i=begin; i<CardList.size() && i<end; i++) {
        CGamesCard *card =CGamesCard::Create(CardList[i]);
        addChild(card, 1,i);
        card->setPosition(getCardPosition(i-begin));
    }
    return true;
}

int  CHandBookLayerCell::checkTouchCell(CCTouch *pTouch)
{
    // 遍历子节点 判断是哪个 item被点击并获得该ID 然后new出一个layer 去显示
    CCArray *arrayNode=getChildren();
    for (int i=0; i<arrayNode->count(); i++) {
        if (((CCNode *)arrayNode->objectAtIndex(i))->boundingBox().containsPoint(pTouch->getLocation())){
            return ((CCNode *)arrayNode->objectAtIndex(i))->getTag();
        }
    }
    return -1;
}

CScreenHandBookLayer::CScreenHandBookLayer()
{
    
}

CScreenHandBookLayer::~CScreenHandBookLayer()
{
    DELETE_POINT_VECTOR(m_vCardList,vector<CFightCard *>,   CFightCard );
}

bool CScreenHandBookLayer::init(){
    if (CCLayer::init())
    {

    }
    return true;
}


void CScreenHandBookLayer::createBackGroud()
{
    
}

void CScreenHandBookLayer::createTableView()
{
    
    
}

void CScreenHandBookLayer::loadCardVector()
{
        for (std::map<int ,CCard *>::iterator it=m_tempPlayer->getCardMap().begin();  it!=m_tempPlayer->getCardMap().end(); it++) {
            this->m_vCardList.push_back(new CFightCard(it->second,1));
        }
}

void CScreenHandBookLayer::sendGetTuJian()
{
    string postStrdata="sig=";
    postStrdata+=SinglePlayer::instance()->getUserSig();
    ADDHTTPREQUESTPOSTDATA(STR_URL_ATLAS(194), "CALLBACK_CScreenHandBookLayer_sendGetTuJian", "REQUEST_CScreenHandBookLayer_sendGetTuJian",postStrdata.c_str(),callfuncO_selector(CScreenHandBookLayer::decodeGetData));
}

void CScreenHandBookLayer::decodeGetData(CCObject *object)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "CALLBACK_CScreenHandBookLayer_sendGetTuJian");
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
    tempdata=NULL;
    
    if (GameTools::intForKey("code",dict)!=0)
    {
        CCMessageBox("code错误", "error");
        return ;
    }
    //解析result 数据
    loadCardVector();
    CCArray *exsitArray= (CCArray*) (((CCDictionary *)(dict->objectForKey("result")))->objectForKey("atlas"));
    for (int i=0; i<exsitArray->count(); i++) {
        int cardid= ((CCString* )exsitArray->objectAtIndex(i))->intValue();
        for (int j=0; j<m_vCardList.size(); j++) {
            if (cardid==m_vCardList[j]->m_pCard->m_icard_id) {
                m_vCardList[j]->isShiFouYongyouguo=true;
            }
        }
    }
    createTableView();
}
