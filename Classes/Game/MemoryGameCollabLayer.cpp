//
//  MemoryGameCollabLayer.cpp
//  91.cube
//
//  Created by linminglu on 13-5-28.
//
//

#include "MemoryGameCollabLayer.h"
#include "gameConfig.h"
#include <vector>
#include <algorithm>
MemoryGameCollabLayer::MemoryGameCollabLayer()
{
    
}

MemoryGameCollabLayer::~MemoryGameCollabLayer()
{
    
}
bool  MemoryGameCollabLayer::isEqualt(int indexBefore,int indexAfter)
{
    if(indexAfter==indexBefore)
    {
        return false;
    }
    if(m_vigameSpriteValue[indexAfter-1000]==m_vigameSpriteValue[indexBefore-1000])
    {
        return true;
    }
    return false;
}

bool MemoryGameCollabLayer::initWithMapFile(const char *fileName)
{
    CCSprite *sprite=CCSprite::create((mapImagesPath+"memorybackgroud.png").c_str());
    addChild(sprite,0);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile((mapImagesPath+"memorygame.plist").c_str());
    srand(time(0));
    addChildSprite();
    setTouchEnabled(true);
    return true;
}
bool MemoryGameCollabLayer::addChildSprite()
{
    m_iFanPanCount=0;
    countSize=0;
    for (int i=0;i<12;)
    {
        int value=rand()%35+11;
        bool conituflag=false;
        for (int j=0; j<m_vigameSpriteValue.size(); j++)
        {
            if(m_vigameSpriteValue[j]==value)
            {
                conituflag=true;
                break;
            }
        }
        if(conituflag)
        {
            continue;
        }
        m_vigameSpriteValue.push_back(value);
        i++;
    }
    for (int i=0;i<12;i++)
    {
        m_vigameSpriteValue.push_back(m_vigameSpriteValue[i]);
    }
    random_shuffle(m_vigameSpriteValue.begin(), m_vigameSpriteValue.end());
    //24个
    char data[20];
    int row=0;
    int column=0;
    int bgrow=0;
    int bgcolumn=0;
    countSize=m_vigameSpriteValue.size();
    for (int i=0; i<m_vigameSpriteValue.size(); i++)
    {
        sprintf(data, "%d.png",m_vigameSpriteValue[i]);
        CCSprite *sprite=CCSprite::createWithSpriteFrameName(data);
        addChild(sprite,1,i+1000);
        sprite->setPosition(getSpritePosition(row, column));
        CCSprite *sprite2=CCSprite::createWithSpriteFrameName("bg.png");
        addChild(sprite2,2,i+countSize+1000);
        sprite2->setPosition(getSpritePosition(bgrow, bgcolumn));
        TouchRect recttouct;
        recttouct.node=sprite;
        recttouct.rect=Utility::getNodeTouchRect(sprite);//->boundingBox();
        //CCPoint point=sprite->convertToWorldSpace(ccp(0,0));
        recttouct.tag=i+1000;
        this->vTouchMapRect.push_back(recttouct);
    }
    
    return true;
}
CCPoint MemoryGameCollabLayer::getSpritePosition(int &row, int &colum)
{
    CCPoint point=ccp(-210,-120);
    point.x += colum*60 ;
    point.y += row*60;
    colum++;
    if(colum>=6)
    {
        colum=0;
        row++;
    }
    return point;
}
void MemoryGameCollabLayer::needToScenePoint(CCPoint &point,CCTouch *pTouch)
{
    point=convertTouchToNodeSpace(pTouch);
}


void MemoryGameCollabLayer::dealWhithTouchEndSprite(CCSprite * touchSprite,int touch_tag)
{
    if(m_iFanPanCount==2)
    {
        return ;
    }
    m_iFanPanCount++;
    m_iCurrTouch_tag=touch_tag;
    if(m_iFanPanCount==1)
    {
        valueBefore=m_iCurrTouch_tag;
    }
    else
    {
        valueAfter=m_iCurrTouch_tag;
    }
    CCLog("%d,%d",valueBefore,valueAfter);
    
    CCSprite *sprite=(CCSprite *)(getChildByTag(touch_tag+countSize));
    if(sprite)
    {
        sprite->setVisible(false);
    }
    if(m_iFanPanCount==2)
    {
        dealWith(touchSprite);
    }
    
    
}

void MemoryGameCollabLayer::actionMoveCoverDone(CCNode* node)
{
    node->setVisible(false);
}

void MemoryGameCollabLayer::actionMoveRemoveDone(CCNode* node)
{
    this->removeChild(node, true);
}

void MemoryGameCollabLayer::removeTouchRectBytag(int tag)
{
    vector<TouchRect>::iterator itr = vTouchMapRect.begin();
    while (itr != vTouchMapRect.end())
    {
        if ((*itr).tag == tag)
        {
            vTouchMapRect.erase(itr);//删除值为3的元素
            break;
        }
        ++itr;
    }
}

void MemoryGameCollabLayer::dealWith(CCNode* node)
{
    
    if(m_iFanPanCount!=2)
    {
        return;
    }
    if(isEqualt(valueBefore, valueAfter))
    {
        //消失两张牌 同时处理别的事件
        CCSprite *sprite1=(CCSprite *)getChildByTag(valueAfter);
        sprite1->runAction(CCSequence::create(CCFadeOut::create(0.5),CCCallFuncN::create(this, callfuncN_selector(MemoryGameCollabLayer::actionMoveRemoveDone)),NULL));
        CCSprite *sprite2=(CCSprite *)getChildByTag(valueBefore);
        sprite2->runAction(CCSequence::create(CCFadeOut::create(0.5),CCCallFuncN::create(this, callfuncN_selector(MemoryGameCollabLayer::actionMoveRemoveDone)),CCCallFuncN::create(this, callfuncN_selector(MemoryGameCollabLayer::onAnimationEnd)),NULL));
        removeChildByTag(valueAfter+countSize,true);
        removeChildByTag(valueBefore+countSize,true);
        removeTouchRectBytag(valueAfter);
        removeTouchRectBytag(valueBefore);
    }
    else
    {
        CCSprite *sprite2=(CCSprite *)getChildByTag(valueBefore);

        sprite2->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCCallFuncN::create(this, callfuncN_selector(MemoryGameCollabLayer::reSetCanTouch)),NULL));
    }
}
void MemoryGameCollabLayer::reSetCanTouch(CCNode* node)
{
    getChildByTag(valueAfter+countSize)->setVisible(true);
    if(valueBefore!=valueAfter)
    {
        getChildByTag(valueBefore+countSize)->setVisible(true);
    }
    onAnimationEnd(node);
}

void MemoryGameCollabLayer::onAnimationEnd(cocos2d::CCNode *node)
{
    m_iFanPanCount=0;
}


void MemoryGameCollabLayer::actionMoveCardDone(CCNode* node)
{
	node->setVisible(true);
	node->runAction(CCSpawn::create(CCSequence::create(CCOrbitCamera::create(1.0f,1, 0, -90, 90, 0, 0),CCCallFuncN::create(this, callfuncN_selector(MemoryGameCollabLayer::dealWith)),NULL),NULL));
}