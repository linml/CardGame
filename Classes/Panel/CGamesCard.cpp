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
#include "CPtTool.h"
#include "gamePlayer.h"
#define  TAG_GAMECARD_HERO 100
#define CARDHPTAG 10
#define CARDATKTAG 11
#define CARDRVCTAG 12
#define CARDDEFTAG 13

static string  g_strresource=g_mapImagesPath+"fighting/";
static string g_testtemp[5]={
    "001",
    "002",
    "003",
    "purple_princess",
    "red_wolf"
};




// static method of class CGamesCard
CGamesCard  *CGamesCard::Create(CFightCard *card)

{
    CGamesCard *cardSprite=new CGamesCard();
    if(cardSprite ==NULL|| !cardSprite->initCreate(card))
    {
        delete cardSprite;
        cardSprite=NULL;
    }else
    {
         cardSprite->autorelease();
    }
   
    return cardSprite;
}

CGamesCard *CGamesCard::create(CFightCard *inCard, CGameCardFactory *inFactory)
{
    if (inFactory == NULL)
    {
        inFactory = CGameCardFactory::getInstance();
    }
    CGamesCard *cardSprite = new CGamesCard();
    if (cardSprite)
    {
        if (cardSprite->initCGamesCard(inCard, inFactory))
        {
            cardSprite->autorelease();
        }
        else
        {
            delete cardSprite;
            cardSprite = NULL;
        }
    }
    return cardSprite;
}

// public method of class CGamesCard
CGamesCard::CGamesCard()
{
    isAddTexiao=false;
    m_pAtkLabel = NULL;
    m_pDefLabel = NULL;
    m_pHpLabel = NULL;
    m_pRvcLabel = NULL;
    m_pLogo = NULL;
}

CGamesCard::~CGamesCard()
{
    
    
}

bool CGamesCard::initCGamesCard(CFightCard *inCard, CGameCardFactory *inFactory)
{
    assert(inCard != NULL&&"card is null");
    m_sCardSize = CCSizeMake(115, 105);
    m_pCardData = inCard;
    if(!initWithTexture(NULL, CCRectZero))
    {
        return false;
    }
    int cardStar = inCard->m_pCard->m_nCard_star;
    int stirps = inCard->m_pCard->m_icard_stirps;
    
    createBgWithStar(cardStar, inFactory);
    createPersonWithName(inCard->m_pCard->m_scard_resources.c_str());
    createBorderWithStar(cardStar, inFactory);
    createStirp(stirps, inFactory);
    createStar(cardStar, inFactory);
    createLevel(inCard->m_iCurrLevel, inFactory);

    setContentSize(m_sCardSize);
    return true;
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
    CCNode *tmp = getChildByTag(TAG_GAMECARD_HERO);
    if (tmp)
    {
        setDead(tmp);
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
//    CCArray *children = getChildren();
//    CCNode *tmp = NULL;
//    for (int i = 0; i < children->count(); i++)
//    {
//        tmp = (CCNode *)(children->objectAtIndex(i));
//        if(tmp)
//        {
//            setLive(tmp);
//        }
//    }

    CCNode *tmp = getChildByTag(TAG_GAMECARD_HERO);
    if (tmp)
    {
        setLive(tmp);
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
    return  CPtTool::getSuit(m_pCardData->m_iSuit, outSuit, outSequence);
    
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
    label=CCLabelTTF::create(buff, "Arial", CARDATKTAG);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(27, 23));
    addChild(label,3,CARDATKTAG);
    m_pAtkLabel = label;
    // hp:
    sprintf(buff, "HP:%d", inHp);
    label=CCLabelTTF::create(buff, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(75, 23));
    addChild(label,3,CARDHPTAG);
    m_pHpLabel = label;
    
    
    // rcv:
    sprintf(buff, "RCV:%d", inRcv);
    label=CCLabelTTF::create(buff, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(27, 10));
    addChild(label,3,CARDRVCTAG);
    m_pRvcLabel = label;
    // def:
    sprintf(buff, "DEF:%d", inDef);
    label=CCLabelTTF::create(buff, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(75, 10));
     addChild(label,3,CARDDEFTAG);
    m_pDefLabel = label;
}


void CGamesCard::updateData(const int &inAtk, const int &inHp, const int &inRcv, const int &inDef)
{
    char buff[50]={0};
    
    // atk:
    sprintf(buff, "ATK:%d",inAtk);
    m_pAtkLabel->setString(buff);
    // hp:
    sprintf(buff, "HP:%d", inHp);
    m_pHpLabel->setString(buff);
    // rcv:
    sprintf(buff, "RCV:%d", inRcv);
    m_pRvcLabel->setString(buff);
    // def:
    sprintf(buff, "DEF:%d", inDef);
    m_pDefLabel->setString(buff);

}

void CGamesCard::createCardName(const char *str)
{
    CCLabelTTF *label=CCLabelTTF::create(str, "Arial", 10);
    label->setColor(ccc3(255, 255, 0));
    addChild(label,3, 8001);
    label->setPosition(ccp(55,180));
}
void CGamesCard::createHero(const char *str)
{

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

bool CGamesCard::initBg(CFightCard *card)
{
    return true;
}

bool CGamesCard::initCreate(CFightCard *card)
{
    assert(card != NULL&&"card is null");
    
    m_pCardData = card;
    if(!initWithTexture(NULL, CCRectZero))
    {
        return false;
    }
    initBg(card);
    createBackground(card->m_pCard->m_scard_ground.c_str());
    createHero(card->m_pCard->m_scard_resources.c_str());
    createStart(card->m_pCard->m_nCard_star);
    createSuit(card->m_iSuit);
    createStirps(card->m_pCard->m_icard_stirps);
    createCardName(card->m_pCard->m_scard_name.c_str());
    createData(card->m_attack, card->m_iHp, card->m_pCard->m_icard_leadership, card->m_defend);
    createLogo();
    return true;
}

void CGamesCard::updateToLevel(const int & inLevel)
{
    CFightCard *card = m_pCardData;
    m_pCardData->updateCard(inLevel);
    m_pCardData->m_iCurrLevel = inLevel; 
    updateData(card->m_attack, card->m_iHp, card->m_pCard->m_icard_leadership, card->m_defend);
}

void CGamesCard:: updateCard(CFightCard *card)
{
    removeAllChildrenWithCleanup(true);

    m_pCardData->updateFight(card->m_pCard, card->m_iCurrLevel);
    m_pCardData->m_iCurrExp = SingleLevleConfigData::instance()->getTopExpByStar(card->m_iCurrLevel, card->m_pCard->m_nCard_star);
    createBackground(card->m_pCard->m_scard_ground.c_str());
    createHero(card->m_pCard->m_scard_resources.c_str());
    createStart(card->m_pCard->m_nCard_star);
    createSuit(card->m_iSuit);
    createStirps(card->m_pCard->m_icard_stirps);
    createCardName(card->m_pCard->m_scard_name.c_str());
    createData(card->m_attack, card->m_iHp, card->m_pCard->m_icard_leadership, card->m_defend);

}
void CGamesCard::createLogo()
{
    m_pLogo = CCSprite::create();
    m_pLogo->setPosition(ccp(70, 100 ));
    addChild(m_pLogo, 20);
}

void  CGamesCard::hideNodeByTag(int tag)
{
    if (getChildByTag(tag)) {
        getChildByTag(tag)->setVisible(false);
    }
    
}

void CGamesCard::hideGameCardData()
{
        hideNodeByTag(CARDRVCTAG);
        hideNodeByTag(CARDATKTAG);
        hideNodeByTag(CARDDEFTAG);
        hideNodeByTag(CARDHPTAG);
    
}

void CGamesCard::setUpExpTo(int inExp)
{
    m_pCardData->m_iCurrExp = inExp;
}

/*
 * @param: inType: 0:普通卡牌 1:攻击阵容1 2: 攻击阵容2 3: 防御阵容
 */

void CGamesCard::setLogo(const int &inType)
{
    CCPoint point = m_pLogo->getPosition();
    switch (inType)
    {
        case 0:
            m_pLogo->init();
            break;
        case 1:
            m_pLogo->initWithFile(CSTR_FILEPTAH(g_mapImagesPath, "attack1_logo.png"));
            break;
        case 2:
            m_pLogo->initWithFile(CSTR_FILEPTAH(g_mapImagesPath, "attack2_logo.png"));
            break;
        case 3:
            m_pLogo->initWithFile(CSTR_FILEPTAH(g_mapImagesPath, "def_logo.png"));
            break;
        default:
            break;
    }
    m_pLogo->setPosition(point);
}
// protect methods of CGamesCard

/*
 * @brief 根据星级创建背景底纹（1->1-2; 2->3; 3->4; 4->5; 5->6; 6-7）
 * @param inStar(1-7)
 */
void CGamesCard::createBgWithStar(int inStar ,CGameCardFactory *inFactory)
{
    if (inStar <= 0 & inStar > 7)
    {
        return;
    }
    CCSprite * bgSprite = NULL;
    inStar = (inStar-1)==0? 1 : inStar-1;
    char diwenName[15]={0};
    sprintf(diwenName, "diwen_%d.png", inStar);
    bgSprite = CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName(diwenName));
    bgSprite->setAnchorPoint(CCPointZero);
    addChild(bgSprite, 0);
    
}
/*
 * @brief 创建小卡内的人物
 * @param
 */
void CGamesCard::createPersonWithName(const char *inName)
{
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_cardResourcePath, inName));
    CCSprite *roleSprite=CCSprite::createWithTexture(texture);
    roleSprite->setAnchorPoint(CCPointZero);
    addChild(roleSprite,1,TAG_GAMECARD_HERO);
}
/*
 * @brief 根据星级创建小卡的外框
 * @param
 */
void CGamesCard::createBorderWithStar(int inStar ,CGameCardFactory *inFactory)
{
    if (inStar <= 0 & inStar > 7)
    {
        return;
    }
    CCSprite * borderSprite = NULL;
    inStar = (inStar-1)==0? 1 : inStar-1;
    char kakuang[15]={0};
    sprintf(kakuang, "kakuang_%d.png", inStar);
    borderSprite = CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName(kakuang));
    borderSprite->setAnchorPoint(CCPointZero);
    addChild(borderSprite, 3);
}
/*
 * @brief 创建等级信息
 * @param
 */
void CGamesCard::createLevel(const int inLevel ,CGameCardFactory *inFactory)
{
    char buff[20]={0};
    sprintf(buff, "LV.%d", inLevel);

    CCLabelTTF *label = CCLabelTTF::create(buff, "ArialRoundedMTBold", 20);

    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(CCPointZero);
    label->setPosition(ccp(m_sCardSize.width*0.35, 3));
    addChild(label, 4);
}
/*
 * @brief 创建种族标示
 * @param
 */
void CGamesCard::createStirp(const int inStirp ,CGameCardFactory *inFactory)
{
 

    if (inStirp >=1 && inStirp <=4)
    {
        char zhongzhu[15]={0};
        sprintf(zhongzhu, "zhongzhu_%d.png", inStirp); 
        CCSprite * stripSprit =  CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName(zhongzhu));
        if (stripSprit)
        {
            stripSprit->setAnchorPoint(ccp(0, 1));
            stripSprit->setPosition(ccp(0-8, m_sCardSize.height+3));
            addChild(stripSprit,4);
        }
    }

}

/*
 * @brief 创建星级标示
 * @param
 */
void CGamesCard::createStar(int inStar ,CGameCardFactory *inFactory)
{
     CCSprite * starSprite =  CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName("xingji.png"));
    if (starSprite)
    {
        starSprite->setAnchorPoint(ccp(0, 0.5));
        starSprite->setPosition(ccp(m_sCardSize.width*0.7, m_sCardSize.height*0.33));
        addChild(starSprite, 4);
        CCSize size = starSprite->getContentSize();
        char buff[5]={0};
        sprintf(buff, "%d", inStar);
        CCLabelTTF *label = CCLabelTTF::create(buff, "ArialRoundedMTBold", 15);

        label->setPosition(ccp(size.width/2, size.height/2-2));
        starSprite->addChild(label);
    }
}

// implement class of CGameDaCard

CGameDaCard * CGameDaCard::create(CFightCard *inCard, CGameCardFactory *inFactory )
{
    if (inFactory == NULL)
    {
        inFactory = CGameCardFactory::getInstance();
    }
    CCAssert(inCard, "the fightcard is null");
    CGameDaCard *card = new CGameDaCard();
    if (card)
    {
        if(card->initWithCardData(inCard, inFactory))
        {
            card->autorelease();
        }else
        {
            delete card;
            card = NULL;
        }
    }
    return card;
}

CGameDaCard * CGameDaCard::createBasicCard(CCard *inCard, CGameCardFactory *inFactory)
{
    if (inFactory == NULL)
    {
        inFactory = CGameCardFactory::getInstance();
    }
    CCAssert(inCard, "the fightcard is null");
    CGameDaCard *card = new CGameDaCard();
    if (card)
    {
        if(card->initWithCardData(inCard, inFactory))
        {
            card->autorelease();
        }else
        {
            delete card;
            card = NULL;
        }
    }
    return card;
}
CGameDaCard::CGameDaCard()
{
    m_pBatchContainer = NULL;
    m_pCardData = NULL;
}
CGameDaCard::~CGameDaCard()
{
    
}
bool CGameDaCard::initWithCardData(CFightCard *inCard, CGameCardFactory *inFactory)
{
    bool bRet = false;
    if (!initWithTexture(NULL, CCRectZero))
    {
        return bRet;
    }
    m_pBasicCardNode = CCNode::create();
    m_pBasicCardNode->setAnchorPoint(CCPointZero);
    m_pBasicCardNode->setPosition(CCPointZero);
    addChild(m_pBasicCardNode, 0);
    m_pBatchContainer = CCSpriteBatchNode::createWithTexture(inFactory->getSpriteFrameWithName("daka_bg.png", CGameCardFactory::DAKA)->getTexture(), 15);
    m_pBasicCardNode->addChild(m_pBatchContainer, 0);
    createBg(inFactory);
    createPerson(inCard->m_pCard->m_scard_role.c_str());
    createStar(inCard->m_pCard->m_nCard_star, inFactory);
    createStirp(inCard->m_pCard->m_icard_stirps, inFactory);
    createLevel(inCard->m_iCurrLevel);
    createCardData(inCard->m_attack,inCard->m_defend, inCard->m_iHp,inFactory);
    createCardName(inCard->m_pCard->m_scard_name.c_str());
    setContentSize(m_sCardSize);
    bRet = true;
    return bRet;
}


bool CGameDaCard::initWithCardData(CCard *inCard, CGameCardFactory *inFactory)
{
    bool bRet = false;
    if (!initWithTexture(NULL, CCRectZero))
    {
        return bRet;
    }
    m_pBasicCardNode = CCNode::create();
    m_pBasicCardNode->setAnchorPoint(CCPointZero);
    m_pBasicCardNode->setPosition(CCPointZero);
    addChild(m_pBasicCardNode, 0);
    m_pBatchContainer = CCSpriteBatchNode::createWithTexture(inFactory->getSpriteFrameWithName("daka_bg.png", CGameCardFactory::DAKA)->getTexture(), 15);
    m_pBasicCardNode->addChild(m_pBatchContainer, 0);
    createBg(inFactory);
    createPerson(inCard->m_scard_role.c_str());
    createStar(inCard->m_nCard_star, inFactory);
    createStirp(inCard->m_icard_stirps, inFactory);
    createLevel(1);
    createCardData(inCard->m_icard_attack,inCard->m_icard_defend, inCard->m_icardhp,inFactory);
    createCardName(inCard->m_scard_name.c_str());
    setContentSize(m_sCardSize);
    bRet = true;
    return bRet;
}

void CGameDaCard::changePersionDirection()
{
    CCSprite * sprite = (CCSprite*) getChildByTag(TAG_GAMECARD_HERO);
    if (sprite)
    {
        sprite->setFlipX(true);
    }
}

void CGameDaCard::resetCard()
{
    m_pBasicCardNode->retain();
    removeAllChildrenWithCleanup(true);
    m_pBasicCardNode->setParent(NULL);
    addChild(m_pBasicCardNode, 0);
    CC_SAFE_RELEASE(m_pBasicCardNode);
}

void CGameDaCard::createBg(CGameCardFactory *inFactory)
{
    CCSprite *bg =CCSprite::createWithSpriteFrame(
                                                  inFactory->getSpriteFrameWithName("daka_bg.png", CGameCardFactory::DAKA));
    if (bg)
    {
        m_pBatchContainer->addChild(bg);
        bg->setAnchorPoint(CCPointZero);
        m_sCardSize = bg->getContentSize();
    }
}

void CGameDaCard::createPerson(const char *inName)
{
    CCLog("the name : %s", inName);
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_cardRolePath, inName));
    CCSprite *roleSprite=CCSprite::createWithTexture(texture);
    roleSprite->setPosition(ccp(m_sCardSize.width*0.45, m_sCardSize.height*0.6));
    m_pBasicCardNode->addChild(roleSprite,1,TAG_GAMECARD_HERO);

}
void CGameDaCard::createStar(int inStar, CGameCardFactory *inFactory)
{
    CCSpriteFrame *frame = inFactory->getSpriteFrameWithName("daka_xingji.png",CGameCardFactory::DAKA);
    if (frame)
    {
        for (int i = 0; i < inStar; i++)
        {
            CCSprite *starSprite = CCSprite::createWithSpriteFrame(frame);
            starSprite->setPosition(ccp(m_sCardSize.width*0.125 + i* 25, m_sCardSize.height*0.18));
            m_pBasicCardNode->addChild(starSprite,1, STARSPRITETAG+i);
        }
    }
}
void CGameDaCard::createCardData(int inAtk, int inDef, int inGP, CGameCardFactory *inFactory)
{
    CCSprite* atkSprite = CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName("daka_gong.png", CGameCardFactory::DAKA));
    CCSprite* defSprite = CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName("daka_fang.png", CGameCardFactory::DAKA));
    CCSprite* gpSprite = CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName("daka_xue.png", CGameCardFactory::DAKA));
    char buffer[30]={0};
    CCPoint point;
    CCLabelTTF *label = NULL;
    if (atkSprite)
    {
        point = ccp(m_sCardSize.width*0.08, m_sCardSize.height*0.07);
        atkSprite->setPosition(point);
        m_pBatchContainer->addChild(atkSprite);
        sprintf(buffer, "%d", inAtk);
        label = CCLabelTTF::create(buffer, "Arial", 16);
        label->setAnchorPoint(ccp(0,0.5));
        label->setPosition(ccp(point.x+10,point.y));
        m_pBasicCardNode->addChild(label);
    }
    
    if (defSprite)
    {
        point.x += 55;
        defSprite->setPosition(point);
        m_pBatchContainer->addChild(defSprite);
        sprintf(buffer, "%d", inDef);
        label = CCLabelTTF::create(buffer, "Arial", 16);
        label->setAnchorPoint(ccp(0,0.5));
        label->setPosition(ccp(point.x+10,point.y));
        m_pBasicCardNode->addChild(label);
    }
    
    if (gpSprite)
    {
        point.x += 55;
        gpSprite->setPosition(point);
        m_pBatchContainer->addChild(gpSprite);
        sprintf(buffer, "%d", inGP);
        label = CCLabelTTF::create(buffer, "Arial", 16);
        label->setAnchorPoint(ccp(0,0.5));
        label->setPosition(ccp(point.x+10,point.y));
        m_pBasicCardNode->addChild(label);

    }
}
void CGameDaCard::createStirp(int inStirp, CGameCardFactory *inFactory)
{
    if (inStirp >= 1 && inStirp <= 4)
    {
        char stirpName[20]= {0};
        sprintf(stirpName, "daka_zhongzhu_%d.png",inStirp);
        CCSprite *stirpSprite = CCSprite::createWithSpriteFrame(inFactory->getSpriteFrameWithName(stirpName, CGameCardFactory::DAKA));
        if (stirpSprite)
        {
            m_pBatchContainer->addChild(stirpSprite,1);
            stirpSprite->setPosition(ccp(m_sCardSize.width*0.125, m_sCardSize.height*0.925));
        }
    }
    
}
void CGameDaCard::createLevel(int inLevel)
{
    char buffer[10] = {0};
    sprintf(buffer, "LV.%d", inLevel);
    CCLabelTTF *label = CCLabelTTF::create(buffer, "Arial-Bold", 28);
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setAnchorPoint(ccp(0,0.5));
    label->setPosition(ccp(m_sCardSize.width*0.8, m_sCardSize.height*0.08));
    m_pBasicCardNode->addChild(label);
}
void CGameDaCard::createCardName(const char *inCardName)
{
    CCLabelTTF *label = CCLabelTTF::create(inCardName, "Arial-Bold", 32);
    label->setPosition(ccp(m_sCardSize.width*0.5, m_sCardSize.height*0.925));
    m_pBasicCardNode->addChild(label);
    
}

// implement class of CGameCardFactory

CGameCardFactory* CGameCardFactory::getInstance()
{
    if (s_pInstance == NULL)
    {
        s_pInstance = new CGameCardFactory();
    }
    return s_pInstance;
}
void CGameCardFactory::releaseCardFactory()
{
    CC_SAFE_DELETE(s_pInstance);
    s_pInstance = NULL;
}
CGameCardFactory* CGameCardFactory::s_pInstance = NULL;

CGamesCard * CGameCardFactory::createCGameCard(CFightCard *inCard)
{
    return CGamesCard::create(inCard, this);
}

CGameDaCard * CGameCardFactory::createCGameDaCard(CFightCard *inCard)
{
     return CGameDaCard::create(inCard, this);
}

CGameDaCard * CGameCardFactory::createBasicCGameDaCardByCardId(int inCardId)
{
    CCard *inCardData = SinglePlayer::instance()->getCardByCardId(inCardId);
    return createBasicCGameDaCardByCardData(inCardData);
}
CGameDaCard * CGameCardFactory::createBasicCGameDaCardByCardData(CCard *inCardData)
{
    CGameDaCard *card = NULL;
    if (inCardData)
    {
        card = CGameDaCard::createBasicCard(inCardData, this);
    }
    return card;
}
CCSprite *CGameCardFactory::createCardHead(CFightCard *inCard)
{
    CCAssert(inCard, "the card param is null");
    CCSprite *sprite = NULL;
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(CSTR_FILEPTAH(g_cardHeadPath, inCard->m_pCard->m_scard_head.c_str()));
    if (texture)
    {
        sprite = CCSprite::createWithTexture(texture);
    }
    return sprite;
}
CCSpriteFrame * CGameCardFactory::getSpriteFrameWithName(const char * spriteName ,CARDSTYLE inCardStyle)
{
    CCSpriteFrame *spriteFrame = (CCSpriteFrame* )m_pCardCache->objectForKey(spriteName);
    if (spriteFrame == NULL)
    {
        spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteName);
        if (spriteFrame == NULL)
        {
            if (inCardStyle == XIAOKA)
            {
                loadXiaoCardResource();
            }else
            {
                loadDaCardResource();
            }
           
            spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spriteName);
        }
        CCAssert(spriteFrame, "没有该card的纹理资源");
        m_pCardCache->setObject(spriteFrame, spriteName);
    }
    return spriteFrame;
}

void  CGameCardFactory::clearCache()
{
    if (m_pCardCache)
    {
        m_pCardCache->removeAllObjects();
    }
}

CGameCardFactory::CGameCardFactory()
{
    m_pCardCache = new CCDictionary();
}
CGameCardFactory::~CGameCardFactory()
{
    if (m_pCardCache)
    {
        m_pCardCache->removeAllObjects();
        delete m_pCardCache;
        m_pCardCache = NULL;
    }
}

void CGameCardFactory::loadXiaoCardResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_cardImagePath, "card_diwen.plist"), CSTR_FILEPTAH(g_cardImagePath, "card_diwen.png"));
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_cardImagePath, "card_waikuang.plist"), CSTR_FILEPTAH(g_cardImagePath, "card_waikuang.png"));
}
void CGameCardFactory::loadDaCardResource()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CSTR_FILEPTAH(g_cardImagePath, "card_daka.plist"), CSTR_FILEPTAH(g_cardImagePath, "card_daka.png"));
    
}

