//
//  CGamesCard.h
//  91.cube
//
//  Created by linminglu on 13-6-28.
//
//

#ifndef ___1_cube__CGamesCard__
#define ___1_cube__CGamesCard__

#include <iostream>
using namespace  std;
#include "CCard.h"
#include "cocos2d.h"
using namespace cocos2d;
class CFightCard;
class CCard;
class CGameCardFactory;

/*
 * @class CGamesCard: 创建小卡牌
 * @brief 小卡牌的结构:｛1.底纹－2.人物－3.(外框－种族－星级－等级)-4.发光(战斗时的选中标示)｝
 */
class CGamesCard :public  CCSprite
{
public:
    static  CGamesCard  *Create(CFightCard *card);
    static CGamesCard *create(CFightCard *inCard, CGameCardFactory *inFactory = NULL);
public:
    CGamesCard();
    ~CGamesCard();
    bool initCreate(CFightCard *card);
    virtual  void setDead();
    virtual  void setLive();
    virtual CGamesCard *getCopy();
    bool getSuit(int &outSuit, int &outSequence);
    void updateToLevel(const int & inLevel);
    CFightCard *getCardData(){ return m_pCardData;};
    void updateCard(CFightCard *card);
    void setLogo(const int &inType);
    void hideGameCardData();
    void setUpExpTo(int inExp);
    
public:
    bool initCGamesCard(CFightCard *inCard, CGameCardFactory *inFactory);
protected:
   
    /*
     * @brief 根据星级创建背景底纹（1->1-2; 2->3; 3->4; 4->5; 5->6; 6-7）
     * @param inStar(1-7)
     */
    void createBgWithStar(int inStar ,CGameCardFactory *inFactory);
    /*
     * @brief 创建小卡内的人物
     * @param
     */
    void createPersonWithName(const char *inName);
    /*
     * @brief 根据星级创建小卡的外框
     * @param
     */
    void createBorderWithStar(int inStar ,CGameCardFactory *inFactory);
    /*
     * @brief 创建等级信息
     * @param
     */
    void createLevel(const int inLevel ,CGameCardFactory *inFactory);
    /*
     * @brief 创建种族标示
     * @param
     */
    void createStirp(const int inStirp ,CGameCardFactory *inFactory);
    
    /*
     * @brief 创建星级标示
     * @param
     */
    void createStar(int inStar ,CGameCardFactory *inFactory);
    
    
    
    void hideNodeByTag(int tag);
    void setLive(CCNode *node);
    void setDead(CCNode *node);
    bool initBg(CFightCard *card);
    void createCardName(const char *str);
    void createStart(int start);
    void createHero(const char *str);
    void createBackground(const char *str);
    void createSuit(int inSuit);
    void createStirps(int inStirps);
    void createData(const int &inAtk, const int &inHp, const int &inRcv, const int &inDef);
    
    void updateData(const int &inAtk, const int &inHp, const int &inRcv, const int &inDef);
    
    void createLogo(); //!<  在卡包里面的 在哪个阵容的 后面有初始化该图案

protected:
    CCLabelTTF *label;
    
    CCLabelTTF* m_pAtkLabel;
    CCLabelTTF* m_pDefLabel;
    CCLabelTTF* m_pHpLabel;
    CCLabelTTF* m_pRvcLabel;

    CCSprite * m_pLogo;
    CFightCard *m_pCardData;

    // new ui:
    CCSize m_sCardSize; // 小卡的整体外观大小
    
public:
    bool isAddTexiao;   
};

/*
 * @class: 大卡
 * @breif: 用于出现场景为：1.战斗场景
 */
#define STARSPRITETAG 80
class CGameDaCard : public CCSprite
{
    
public:
    static CGameDaCard *create(CFightCard *inCard, CGameCardFactory *inFactory = NULL);
    static CGameDaCard *createBasicCard(CCard *inCard, CGameCardFactory *inFactory = NULL);
public:
    CGameDaCard();
    virtual ~CGameDaCard();
    bool initWithCardData(CFightCard *inCard, CGameCardFactory *inFactory);
    bool initWithCardData(CCard *inCard, CGameCardFactory *inFactory);
    void changePersionDirection();
    void resetCard();
protected:
    void createBg(CGameCardFactory *inFactory);
    void createPerson(const char *inName);
    void createStar(int inStar, CGameCardFactory *inFactory);
    void createCardData(int inAtk, int Def, int inGP, CGameCardFactory *inFactory);
    void createStirp(int inStirp, CGameCardFactory *inFactory);
    void createLevel(int inLevel);
    void createCardName(const char *inCardName);
    
    
protected:
    CCSpriteBatchNode *m_pBatchContainer;
    CCSize m_sCardSize;
    CFightCard *m_pCardData;
    CCNode *m_pBasicCardNode;

};
/*
 * @class: 卡牌创建的工厂类
 * @breif: 1.用于统一管理卡牌图片资源的加载和卸载; 2.创建相关卡牌类（小卡，大卡，卡牌头像）
 *
 */
class CGameCardFactory
{
public:
    enum CARDSTYLE
    {
        DAKA = 1,
        XIAOKA
    };
public:
    static CGameCardFactory* getInstance();
    static void releaseCardFactory();
protected:
    static CGameCardFactory* s_pInstance;
public:
    CGamesCard *createCGameCard(CFightCard *inCard);
    CGameDaCard *createCGameDaCard(CFightCard *inCard);
    CGameDaCard *createBasicCGameDaCardByCardId(int inCardId);
    CGameDaCard *createBasicCGameDaCardByCardData(CCard *inCardData);
    CCSprite *createCardHead(CFightCard *inCard);
    CCSpriteFrame * getSpriteFrameWithName(const char * spriteName, CARDSTYLE inCardStyle = XIAOKA);
    void clearCache();
protected:
    CGameCardFactory();
    ~CGameCardFactory();
    
    void loadXiaoCardResource();
    void loadDaCardResource();

   
protected:
    CCDictionary *m_pCardCache;
};
#endif /* defined(___1_cube__CGamesCard__) */
