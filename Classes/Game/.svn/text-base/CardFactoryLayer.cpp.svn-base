//
//  CardFactoryLayer.cpp
//  kapeipuzzle
//
//  Created by linminglu on 13-5-22.
//
//

#include "CardFactoryLayer.h"
#include "PtMapUtility.h"
#include "PuzzleStruct.h"
#include "SceneManager.h"


void CreateGray(CCSprite *sprite)
{
    // CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));
    
    char pszFragSource[] ="#ifdef GL_ES \n \
    precision mediump float; \n \
    #endif \n \
    uniform sampler2D u_texture; \n \
    varying vec2 v_texCoord; \n \
    varying vec4 v_fragmentColor; \n \
    void main(void) \n \
    { \n \
    // Convert to greyscale using NTSC weightings \n \
    float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
    gl_FragColor = vec4(grey, grey, grey, 1.0); \n \
    }";
    
    CCGLProgram* pProgram = new CCGLProgram();
    pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
    sprite->setShaderProgram(pProgram);
    pProgram->release();
    CHECK_GL_ERROR_DEBUG();
    
    sprite->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    sprite->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    sprite->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    CHECK_GL_ERROR_DEBUG();
    sprite->getShaderProgram()->link();
    CHECK_GL_ERROR_DEBUG();
    sprite->getShaderProgram()->updateUniforms();
    CHECK_GL_ERROR_DEBUG();
}


CardFactoryLayer::CardFactoryLayer()
{
    
}

CardFactoryLayer::~CardFactoryLayer()
{
    
}

void CardFactoryLayer::setLeftPingHeButton(bool flag)
{
    if(!flag)
    {
        if(getChildByTag(1)&&getChildByTag(1)->getChildByTag(0))
        {
            CCSprite *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(2));
            CreateGray(sprite);
        }
    }
    else
    {
//        if(getChildByTag(1)&&getChildByTag(1)->getChildByTag(0))
//        {
//            CCSprite *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(2));
//            sprite->
//             
//        }
    }
    
}

void CardFactoryLayer::setRightPingHeButton(bool flag)
{
    if(!flag)
    {
            if(getChildByTag(1)&&getChildByTag(1)->getChildByTag(0))
            {
                CCSprite *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(3));
                CreateGray(sprite);
            }
    }

    else{
        if(getChildByTag(1)&&getChildByTag(1)->getChildByTag(0))
        {
            CCSprite *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(3));
            sprite->setTexture(rightButton);
        }
    }
}
void CardFactoryLayer::reInitButton()
{
    for ( int i=0; i<12; i++)
    {
        if(vCardSpritePuzzle[i].icount==0)
        {
            if(leftButtonIsFull)
            {
                leftButtonIsFull=false;
                setLeftPingHeButton(false);
            }
            return;
        }
    }
    if(!leftButtonIsFull)
    {
        leftButtonIsFull=true;
        setLeftPingHeButton(true);
    }
}

void CardFactoryLayer::hideCardPuzzle(int index,int tag)
{
    if(getChildByTag(1) && getChildByTag(1)->getChildByTag(0))
    {
        CCSprite  *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(tag));
        vCardSpritePuzzle.push_back(pagSprite(index,1,0,tag));
        sprite->setVisible(false);
    }
}

void CardFactoryLayer::leftButtonClickOn()
{
    if(leftButtonIsFull)
    {
        unschedule(schedule_selector(CardFactoryLayer::updateAddCount));
        for (int i=0; i < vCardSpritePuzzle.size();i++) {
            vCardSpritePuzzle[i].icount--;
            printf("%d ", vCardSpritePuzzle[i].icount);
            if(vCardSpritePuzzle[i].icount==0)
            {
                reInit(i,vCardSpritePuzzle[i].SpriteGGtag);
                leftButtonIsFull=false;
                setLeftPingHeButton(false);
            }
            
        }
        printf("\n");
        
        schedule(schedule_selector(CardFactoryLayer::updateAddCount), 1.0f);
    }
    
}

void CardFactoryLayer::pinheCurrCard(int SpriteGGtag)
{

}


void CardFactoryLayer::dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag)
{
    CCLog("touch_tag%d",touch_tag);
    if(touch_tag!=-1 &&touchSprite)
    {
        if(touch_tag==1010)
        {
            leftButtonClickOn();
        }
        if(!SingleSceneManager::instance()->runSceneSelect(touch_tag))
        {
            
        }
    }
}

bool CardFactoryLayer::initWithMapFile(const char *fileName)
{
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	CCLog("CardFactoryLayer::initWithMapFile");
	bool bRet = false;
	do
	{
        setUserData((void *)fileName);
		char *tmp = NULL;
		CC_BREAK_IF(!CCLayer::init());
		tmp = (char *)this->getUserData();
        mapFileName=tmp;
		PtMapUtility::addChildFromScript(this,mapFileName.c_str());
		PtMapUtility::addTouchRectFromScript(mapFileName.c_str(), this, &vTouchMapRect);
		setTouchEnabled(true);
        setCardBgGray(0);
        setCardBgGray(1);
        for ( int i=0; i<12; i++)
        {
            hideCardPuzzle(i,6+i);
        }
        leftButtonIsFull=false;
		bRet = true;
	} while (0);
    srand(time(0));
    backButtonTextture();
    leftButtonIsFull=false;
    setLeftPingHeButton(false);
    
//    CCSprite *sprite2=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(2));
//    CCSprite * sprite=CCSprite::createWithTexture(leftButton, sprite2->getTextureRect());
//    addChild(sprite,1000,2000);
//    sprite->setPosition(ccp(200,100));
    schedule(schedule_selector(CardFactoryLayer::updateAddCount), 1.0f);
	return bRet;
}

void CardFactoryLayer::backButtonTextture()
{
    if(getChildByTag(1)&&getChildByTag(1)->getChildByTag(0))
    {
        CCSprite *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(2));
        CCSpriteFrame *frame=CCSpriteFrame::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
        CCLog("%f,%f,%f,%f",sprite->getTextureRect().origin.x,sprite->getTextureRect().origin.y,sprite->getTextureRect().size.width,sprite->getTextureRect().size.height);
        if(frame)
        {
            leftButton=frame->getTexture();
        }
        else{
            CCLog("dddddd");
        }
        //leftButton->sett sprite->getTexture();
    }
    if(getChildByTag(1)&&getChildByTag(1)->getChildByTag(0))
    {
        CCSprite *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(3));
        rightButton=sprite->getTexture();
    }
}


void CardFactoryLayer::updateAddCount(float ctime)
{
    int index=rand()%12;
    bool isNeedReinit=false;
    for ( int i=0 ; i < vCardSpritePuzzle.size(); i++) {
        if (vCardSpritePuzzle[i].index==index)
        {
            vCardSpritePuzzle[i].icount++;
            if(vCardSpritePuzzle[i].icount==1)
            {
                isNeedReinit=true;
                reInit(index, vCardSpritePuzzle[i].SpriteGGtag);
            }
                break;
        }
    }
    reInitButton();
    
}
void CardFactoryLayer::reInit(int index, int cspriteCard)
{
    if(getChildByTag(1) && getChildByTag(1)->getChildByTag(0))
    {
        CCSprite  *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->
                                       getChildByTag(vCardSpritePuzzle[index].spriteTag));
        sprite->setVisible(!sprite->isVisible());
    }                                                                               
}


void CardFactoryLayer::setCardBgGray(int tag)
{
    if(getChildByTag(1) && getChildByTag(1)->getChildByTag(0))
    {
        CCSprite  *sprite=(CCSprite *)(getChildByTag(1)->getChildByTag(0)->getChildByTag(tag));
        CreateGray(sprite);        
    }
}
