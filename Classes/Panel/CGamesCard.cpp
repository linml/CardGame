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

CGamesCard  *CGamesCard::Create(CCard *card)
{
    CGamesCard *cardSprite=new CGamesCard();
    if(cardSprite ==NULL|| !cardSprite->initCreate(card))
    {
        delete cardSprite;
        cardSprite=NULL;
    }
    cardSprite->autorelease();
    return cardSprite;
}

CGamesCard * CGamesCard::getCopy()
{
    return Create(m_pCardData);
}

/*
 * @breif: set all the sprite in CGamesCard gray
 */
void CGamesCard::setDead()
{
    CCArray *children = getChildren();
    CCNode *tmp = NULL;
    for (int i = 0; i < children->count(); i++)
    {
        tmp = dynamic_cast<CCNode*>(children->objectAtIndex(i));
        if(tmp)
        {
            setDead(tmp);
        }
    }
}

void CGamesCard::setDead(CCNode *node)
{
    
        char pszFragSource[] ="#ifdef GL_ES \n \
        precision mediump float; \n \
        #endif \n \
        uniform sampler2D u_texture; \n \
        varying vec2 v_texCoord; \n \
        varying vec4 v_fragmentColor; \n \
        void main(void) \n \
        { \n \
        // Convert to greyscale using NTSC weightings \n \
        vec4 col = texture2D(u_texture, v_texCoord); \n \
        float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
        gl_FragColor = vec4(grey, grey, grey, col.a); \n \
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

/*
 * @brief : set all the sprite in CGamesCard light.
 */

void CGamesCard::setLive()
{
    CCArray *children = getChildren();
    CCNode *tmp = NULL;
    for (int i = 0; i < children->count(); i++)
    {
        tmp = dynamic_cast<CCNode*>(children->objectAtIndex(i));
        if(tmp)
        {
            setLive(tmp);
        }
    }

}

void CGamesCard::setLive(CCNode *node)
{
    if (node == NULL)
    {
        return;
    }
    char pszFragSource[] ="#ifdef GL_ES \n \
    precision mediump float; \n \
    #endif \n \
    uniform sampler2D u_texture; \n \
    varying vec2 v_texCoord; \n \
    varying vec4 v_fragmentColor; \n \
    void main(void) \n \
    { \n \
    // Convert to greyscale using NTSC weightings \n \
    vec4 col = texture2D(u_texture, v_texCoord); \n \
    gl_FragColor = col; \n \
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
/*
 * @param inSuit : 1-13(红), 14-26(黑), 27-39(方), 40-52(梅)
 */

void CGamesCard::createSuit(int inSuit)
{
    if (inSuit <= 0 || inSuit > 52)
    {
        CCLog("suit: error");
        return;
    }
    char buff[10]={0};
    
    int suit = inSuit/13 + (inSuit%13 == 0 ? 0 : 1);
    inSuit = inSuit- (13* (suit-1));
    sprintf(buff, "suit_%d.png", suit);
    
    CCSprite * suitSprite = CCSprite::create(CSTR_FILEPTAH(g_strresource, buff));
    label = NULL;
    if (inSuit >1 && inSuit < 11)
    {
        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%2d", inSuit);
       label=CCLabelTTF::create(buff, "Arial", 6 );
    }else
    {
        char c[2]={'\n'};
        switch (inSuit)
        {
            case 1:
                c[0]='A';
                break;
            case 11:
                c[0]='J';
                break;
            case 12:
                c[0]='Q';
                break;
            case 13:
                c[0]='K';
                break;
            default:
                break;
        }
        label=CCLabelTTF::create(c, "Arial", 6);
    }
    //label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(8,8));
    suitSprite->addChild(label);
    suitSprite->setAnchorPoint(CCPointZero);
    suitSprite->setPosition(ccp(13, 148));
    addChild(suitSprite,3);
    
}

bool CGamesCard::getSuit(int &outSuit, int &outSequence)
{
    int inSuit = m_pCardData->m_icard_suit;
    if (inSuit <= 0 || inSuit > 52)
    {
        CCLog("suit: error");
        return false;
    }

    
    outSuit = inSuit/13 + (inSuit%13 == 0 ? 0 : 1);
    outSequence = inSuit- (13* (outSuit-1));
    return  true;

}

/*
 * @param start<-> border (1-> 1, 2-> 1, 3->2, 4->3, 5->4, 6->5, 7->6)
 */

void CGamesCard::createStart(int start)
{

    if (start <= 0 & start > 7)
    {
        return;
    }
   
    if (start== 1 || start == 2)
    {
        
        CCSprite * borderSprite =CCSprite::create(CSTR_FILEPTAH(g_strresource, "kuang1.png"));
        borderSprite->setAnchorPoint(CCPointZero);
        addChild(borderSprite, 2);
    }else
    {
        char buff[10]={0};
        sprintf(buff, "kuang%d.png", start-1);
        CCSprite * borderSprite =CCSprite::create(CSTR_FILEPTAH(g_strresource, buff));
        borderSprite->setAnchorPoint(CCPointZero);
        addChild(borderSprite, 2);
    }
    
    CCSprite * startSprite = NULL;
    for (int i = 0; i < start; i++)
    {
        startSprite = CCSprite::create(CSTR_FILEPTAH(g_strresource,"star.png"));
        startSprite->setAnchorPoint(CCPointZero);
        startSprite->setPosition(ccp(90+i*6, 175));
        addChild(startSprite, 3);
    }
}

/*
 * @brief : create strips sprint:
 * @param inStrips: [1-5], 1:orca， 2:giants，3: human, 4: god, 5. half－orca
 */

void CGamesCard::createStirps(int inStirps)
{
    if (inStirps >=1 && inStirps <=5)
    {
        char buff[10]={0};
        sprintf(buff, "zz%d.png", inStirps);
        CCSprite * stripSprit =  CCSprite::create(CSTR_FILEPTAH(g_strresource, buff));
        if (stripSprit)
        {
            stripSprit->setAnchorPoint(CCPointZero);
            stripSprit->setPosition(ccp(12, 166));
            addChild(stripSprit,3);
        }
    }
}

/*
 * @brief : create the background sprite:
 */
void CGamesCard::createBackground(const char *str)
{
    if (str == NULL)
    {
        return;
    }
    CCSprite *bg = CCSprite::create(CSTR_FILEPTAH(g_strresource,str));
    CCSprite *border = CCSprite::create(CSTR_FILEPTAH(g_strresource, "kuang0.png"));
    border->setAnchorPoint(CCPointZero);
    bg->setAnchorPoint(CCPointZero);
    addChild(bg, 0);
    addChild(border, 2);
    this->setContentSize(bg->getContentSize());
}

/*
 * @breif : create labels of atk, hp, rec, def
 */

void CGamesCard::createData(const int &inAtk, const int &inHp, const int &inRcv, const int &inDef)
{
    CCLabelTTF *label= NULL;
    char buff[50]={0};
    
    // atk:
    sprintf(buff, "ATK:%d",inAtk);
    label=CCLabelTTF::create(buff, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(27, 23));
    addChild(label,3);
    // hp:
    sprintf(buff, "HP:%d", inHp);
    label=CCLabelTTF::create(buff, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(75, 23));
    addChild(label,3);
    
    
    // rcv:
    sprintf(buff, "RCV:%d", inRcv);
    label=CCLabelTTF::create(buff, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(27, 10));
     addChild(label,3);

    // def:
    sprintf(buff, "DEF:%d", inDef);
    label=CCLabelTTF::create(buff, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(75, 10));
     addChild(label,3);
}


void CGamesCard::createCardName(const char *str)
{
    CCLabelTTF *label=CCLabelTTF::create(str, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    addChild(label,3, 20);

    label->setPosition(ccp(55,180));
}
void CGamesCard::createHero(const char *str)
{
 //   CCSprite *sprite=CCSprite::create(CSTR_FILEPTAH(g_strresource, str));
//    addChild(sprite,1,TAG_GAMECARD_HERO);
//    sprite->setAnchorPoint(CCPointZero);
    CCLog("the %s", (g_strresource+str).c_str());
       CCLog("1the %s", CSTR_FILEPTAH(g_strresource, str));

    FILE *fp = fopen(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath((g_strresource+str).c_str()), "r");
    if(fp==NULL)
    {
        string cardfile=g_strresource+"card_res_"+g_testtemp[rand()%3]+".png";
        CCSprite *sprite=CCSprite::create(cardfile.c_str());
        addChild(sprite,1,TAG_GAMECARD_HERO);       
        sprite->setAnchorPoint(CCPointZero);
    }
    else
    {
        fclose(fp);
        CCSprite *sprite=CCSprite::create((g_strresource+str).c_str());
        addChild(sprite,1,TAG_GAMECARD_HERO);
        sprite->setAnchorPoint(CCPointZero);
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

bool CGamesCard::initCreate(CCard *card)
{
    assert(card != NULL&&"card is null");
    
    m_pCardData = card;
    if(!initWithTexture(NULL, CCRectZero))
    {
        return false;
    }
    initBg(card);
    createBackground(card->m_scard_groud.c_str());
    createHero(card->m_scard_resources.c_str());
    createStart(card->m_sicard_star);
    createSuit(card->m_icard_suit);
    createStirps(card->m_icard_stirps);
    createCardName(card->m_scard_name.c_str());
    createData(card->m_icard_attack, card->m_icardhp, card->m_icard_leadership, card->m_icard_defend);
    return true;
}
