//
//  CGamesCard.cpp
//  91.cube
//
//  Created by linminglu on 13-6-28.
//
//

#include "CGamesCard.h"
#include "CCard.h"
#include "gameConfig.h"
#define  TAG_GAMECARD_HERO 100


static string  g_strresource=g_mapImagesPath+"fighting/";
static string g_testtemp[5]={
    "001",
    "002",
    "003",
    "purple_princess",
    "red_wolf"
};


CGamesCard::CGamesCard()
{
    isAddTexiao=false;
}

CGamesCard::~CGamesCard()
{


}

CGamesCard  *CGamesCard::Create(CCard *card,bool isYongHu)
{
    CGamesCard *cardSprite=new CGamesCard();
    if(cardSprite ==NULL|| !cardSprite->initCreate(card,isYongHu))
    {
        delete cardSprite;
        cardSprite=NULL;
    }
    cardSprite->autorelease();
    return cardSprite;
}

void CGamesCard::setDead()
{
        CCNode *node=getChildByTag(TAG_GAMECARD_HERO);
       // CCNode *node=(CCNode *)nodeArray->objectAtIndex(i);
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
        if(node)
        {
        CCGLProgram* pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
        node->setShaderProgram(pProgram);
        pProgram->release();
        CHECK_GL_ERROR_DEBUG();
        
        node->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        node->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        node->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        
        node->getShaderProgram()->link();
        CHECK_GL_ERROR_DEBUG();
        
        node->getShaderProgram()->updateUniforms();
        CHECK_GL_ERROR_DEBUG();
        }

}

void CGamesCard::createStart(int start)
{
    
}
void CGamesCard::createCardName(const char *str)
{
    CCLabelTTF *label=CCLabelTTF::create(str, "Arial", 15);
    label->setColor(ccc3(255, 255, 0));
    addChild(label,2);
    label->setPosition(ccp(0,-80));
}
void CGamesCard::createHero(const char *str)
{
    FILE *fp = fopen((g_strresource+str).c_str(), "r");
    if(fp==NULL)
    {
        string cardfile=g_strresource+"card_res_"+g_testtemp[rand()%3]+".png";
        CCSprite *sprite=CCSprite::create(cardfile.c_str());
        addChild(sprite,1,TAG_GAMECARD_HERO);
        //sprite->setAnchorPoint(CCPointZero);
    }
    else
    {
        fclose(fp);
        CCSprite *sprite=CCSprite::create((g_strresource+str).c_str());
        addChild(sprite,1,TAG_GAMECARD_HERO);
        //sprite->setAnchorPoint(CCPointZero);
    }
}

bool CGamesCard::initBg(CCard *card)
{
//    char data[20];
//    sprintf(data, "%s_%0.2d.png",card->m_scard_groud.c_str(),(int)card->m_ccard_color);
//    if(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(card->m_scard_groud.c_str())==NULL)
//    {
////
//        return false;
//    }
//    initWithFile(data);
    return true;
}

bool CGamesCard::initCreate(CCard *card,bool isYongHu)
{
    assert(card != NULL&&"card is null");
    
    if(!initWithTexture(NULL, CCRectZero))
    {
        return false;
    }
    if(!isYongHu)
    {
        initBg(card);
        createHero(card->m_scard_resources.c_str());
        createStart(card->m_sicard_star);
        createCardName(card->m_scard_name.c_str());
    }
    else
    {
        FILE *pFile=fopen((g_strresource +card->m_scard_resources).c_str(), "r");
        if(!pFile)
        {
            string cardfiletemp=g_strresource+"card_res_"+g_testtemp[rand()%3]+".png";
            cout<<cardfiletemp<<endl;
            CCSprite *sprite=CCSprite::create(cardfiletemp.c_str());
            addChild(sprite,1);
            sprite->setAnchorPoint(CCPointZero);
        }
        else
        {
            fclose(pFile);
            CCSprite *sprite=CCSprite::create((g_strresource +card->m_scard_resources).c_str());
            addChild(sprite,1);
            sprite->setAnchorPoint(CCPointZero);
        }
    }
    return true;
}
