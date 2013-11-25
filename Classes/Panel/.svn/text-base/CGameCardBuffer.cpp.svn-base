//
//  CGameCardBuffer.cpp
//  91.cube
//
//  Created by linminglu on 13-8-5.
//
//

#include "CGameCardBuffer.h"
CGameCardBuffer *CGameCardBuffer::CreateBuffer(const char *pngFileName,int bufferCount)
{
    CGameCardBuffer *gameCardBuffer=new CGameCardBuffer;
    if( !gameCardBuffer || !gameCardBuffer->init(pngFileName,bufferCount))
    {
        delete gameCardBuffer;
        gameCardBuffer=NULL;
        return false;
    }
    gameCardBuffer->autorelease();
    return gameCardBuffer;
    
}
void CGameCardBuffer::setBuffer(int count)
{
    if(getChildByTag(100))
    {
        CCLabelTTF *label=(CCLabelTTF *)getChildByTag(100);
        char data[10];
        sprintf(data, "%d",count);
        label->setString(data);
    }
    
}
bool CGameCardBuffer::init(const char *pngFileName,int bufferNum)
{
    bool result=false;
    result=this->initWithFile(pngFileName);
    char data[10];
    sprintf(data, "%d",bufferNum);
    CCLabelTTF *labelttf=CCLabelTTF::create(data, "Arial", 20);
    addChild(labelttf,1,100);
    labelttf->setPosition(ccp(getContentSize().width*0.5, getContentSize().height*0.5));
    labelttf->setColor(ccc3(255, 0, 0));
    return result;
}