//
//  CPtEditBoxWidget.cpp
//  91.cube
//
//  Created by phileas on 13-7-4.
//
//

#include "CPtEditBoxWidget.h"

const static float DELTA = 20.0f;


CPtEditBoxHandler::CPtEditBoxHandler()
{
    m_pEditBoxes = new CCDictionary();
    m_pDataes = new  CCDictionary();
    
  
}
CPtEditBoxHandler::~CPtEditBoxHandler()
{
    CC_SAFE_RELEASE(m_pEditBoxes);
    CC_SAFE_RELEASE(m_pDataes);
}

bool CPtEditBoxHandler::registerEditBox(CCNode * node, const int pId)
{
    CCTextFieldTTF *tmp = dynamic_cast<CCTextFieldTTF*>(node);
    if (tmp)
    {
        m_pEditBoxes->setObject(tmp, pId);
        
        tmp->setDelegate(this);
        return  true;
    }
    return false;
}

void CPtEditBoxHandler::onEditBegin(CCTextFieldTTF * edit)
{
    CCLog("CPtEditBoxHandler::onEditBegin");
    
}
void CPtEditBoxHandler::onEditEnd(CCTextFieldTTF * edit)
{
    CCLog("CPtEditBoxHandler::onEditEnd ");
    CPtTextFieldTTF *tmp = dynamic_cast<CPtTextFieldTTF*>(edit);
    std::string * tmpStr = NULL;
    if (tmp)
    {
        tmpStr = tmp->getInputText();
        if (tmpStr)
        {
            CCLog("the context: %s", tmpStr->c_str());
        }
    }
    CCDictElement *element = NULL;
    CPtTextFieldTTF *editTmp = NULL;
    CCDICT_FOREACH(m_pEditBoxes, element)
    {
        CCObject * object = element->getObject();
        
        if (object && (editTmp = dynamic_cast<CPtTextFieldTTF*>(object)))
        {
            if (editTmp==edit)
            {
                if (tmp)
                {
                    tmpStr = tmp->getInputText();
                    if (tmpStr)
                    {
                        CCLog("the context: %s", tmpStr->c_str());
                    }
                }
                m_pDataes->setObject(CCString::create(*tmpStr), element->getIntKey());
            }
        }
        
    }
    
    
    
    
}

bool CPtEditBoxHandler::onTouchBegin(cocos2d::CCTouch *pTouch)
{
    m_beginPos = pTouch->getLocation();
    CCPoint tmpPoint;
    if (m_pEditBoxes)
    {
        CCDictElement *element = NULL;
        CPtTextFieldTTF * edit = NULL;
        CCDICT_FOREACH(m_pEditBoxes, element)
        {
            CCObject *tmp =  element->getObject();
            if (tmp && (edit = dynamic_cast<CPtTextFieldTTF*>(tmp)))
            {
                tmpPoint = edit->getParent()->convertTouchToNodeSpace(pTouch);
                if (edit->getRect().containsPoint(tmpPoint))
                {
                    return true;
                }else
                {
                    if (edit->getOpenFlag())
                    {
                        edit->closeIME();
                    }
                }
            }
            
        }
        
    }
    return false;
    
}

void CPtEditBoxHandler::onTouchEnd(cocos2d::CCTouch *pTouch)
{
    CCPoint endPos = pTouch->getLocation();
    
    // 判断是否为点击事件
    if (::abs(endPos.x - m_beginPos.x) > DELTA ||
        ::abs(endPos.y - m_beginPos.y) > DELTA)
    {
        // 不是点击事件
        m_beginPos.x = m_beginPos.y = -1;
        
        return;
    }
    
    if (m_pEditBoxes)
    {
        CCDictElement *element = NULL;
        CPtTextFieldTTF * edit = NULL;
        CCDICT_FOREACH(m_pEditBoxes, element)
        {
            CCObject *tmp =  element->getObject();
            if (tmp && (edit = dynamic_cast<CPtTextFieldTTF*>(tmp)))
            {
                endPos = edit->getParent()->convertTouchToNodeSpace(pTouch);
                if (edit->getRect().containsPoint(endPos))
                {
                    if (edit->getOpenFlag())
                    {
                        edit->closeIME();
                    }
                    else
                    {
                        edit->openIME();
                    }
                }else
                {
                    if (edit->getOpenFlag())
                    {
                        edit->closeIME();
                    }
                }
            }
            
        }
        
    }
    
}

// implement class of CPtEditBoxWidget:
static int _calcCharCount(const char * pszText)
{
    int n = 0;
    char ch = 0;
    while ((ch = *pszText))
    {
        CC_BREAK_IF(! ch);
        
        if (0x80 != (0xC0 & ch))
        {
            ++n;
        }
        ++pszText;
    }
    return n;
}


CPtTextFieldTTF::CPtTextFieldTTF()
{
    CCTextFieldTTF();
    
    m_pCursorSprite = NULL;
    m_pCursorAction = NULL;
    m_pHandler = NULL;
    m_pInputText = NULL;
    isPsw = false;
    m_bOpen = false;
    m_limitNum = 30;
}

CPtTextFieldTTF::~CPtTextFieldTTF()
{
    CC_SAFE_DELETE(m_pInputText);
}

void CPtTextFieldTTF::onEnter()
{
    
    CCTextFieldTTF::onEnter();
    this->setDelegate(this);
    
}
//静态生成函数
CPtTextFieldTTF * CPtTextFieldTTF::create(const char *placeholder, const char *fontName, float fontSize)
{
    CPtTextFieldTTF *pRet = new CPtTextFieldTTF();
    
    if(pRet && pRet->initWithString("", fontName, fontSize))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        pRet->initCursorSprite(fontSize);
        
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}

void CPtTextFieldTTF::initCursorSprite(int nHeight)
{
    // 初始化光标
    int column = 4;
    int pixels[nHeight][column];
    for (int i=0; i<nHeight; ++i) {
        for (int j=0; j<column; ++j) {
            pixels[i][j] = 0xffffffff;
        }
    }
    
    CCTexture2D *texture = new CCTexture2D();
    texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(column, nHeight));
    
    m_pCursorSprite =  CCSprite::createWithTexture (texture);
    CCSize winSize = getContentSize();
    m_cursorPos = ccp(0, winSize.height / 2);
    m_pCursorSprite->setPosition(m_cursorPos);
    this->addChild(m_pCursorSprite);
    
    m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), CCFadeIn::create(0.25f), NULL));
    
    m_pCursorSprite->runAction(m_pCursorAction);
    m_pCursorSprite->setVisible(false);
    m_pInputText = new std::string();
}



CCRect CPtTextFieldTTF::getRect()
{
    CCSize size;
    if (&m_designedSize != NULL) {
        size = m_designedSize;
    }else {
        size = getContentSize();
    }
    
    CCRect rect = CCRectMake(0 - size.width * getAnchorPoint().x, 0 - size.height * getAnchorPoint().y, size.width, size.height);
    return  rect;
}
//设置触摸弹出输入法的区域大小
void CPtTextFieldTTF::setDesignedSize(cocos2d::CCSize size)
{
    m_designedSize = size;
}

CCSize CPtTextFieldTTF::getDesignedSize()
{
    return m_designedSize;
}

bool CPtTextFieldTTF::isInTextField(cocos2d::CCTouch *pTouch)
{
    CCPoint pToushPos = convertTouchToNodeSpaceAR(pTouch);
    
    return getRect().containsPoint(pToushPos);
    
}


bool CPtTextFieldTTF::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    if (m_pHandler)
    {
        m_pHandler->onEditBegin(this);
    }
    
    if (m_pInputText->empty()) {
        return false;
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    return false;
}

bool CPtTextFieldTTF::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
{
    if (text[0] == 10)
    {
        closeIME();
    }
    if (isalnum((int)text[0]) == 0)
    {
        return true;
        
    }
    CCLog("Width: %f", pSender->getContentSize().width);
    CCLog("Text: %s", text);
    CCLog("Length: %d", nLen);
    
    std::string tempStr = m_pInputText->substr();
    tempStr.append(text);
    if (tempStr.length() > m_limitNum) {
        return true;
    }
    
    m_pInputText->append(text);
    
    
    if (isPsw) {
        std::string tempStr;
        for (int i = 0; i < m_pInputText->size(); i++) {
            tempStr.append("*");
        }
        setString(tempStr.c_str(), m_pInputText->c_str());
    }else {
        setString(m_pInputText->c_str(), m_pInputText->c_str());
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    return true;
}

bool CPtTextFieldTTF::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
{
    m_pInputText->resize(m_pInputText->size() - nLen);
    CCLog(m_pInputText->c_str());
    
    if (isPsw) {
        std::string tempStr;
        for (int i = 0; i < m_pInputText->size(); i++) {
            tempStr.append("*");
        }
        setString(tempStr.c_str(), m_pInputText->c_str());
    }else {
        setString(m_pInputText->c_str(), m_pInputText->c_str());
    }
    
    m_pCursorSprite->setPositionX(getContentSize().width);
    
    if (m_pInputText->empty()) {
        m_pCursorSprite->setPositionX(0);
    }
    
    return true;
}

bool CPtTextFieldTTF::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
{
    return false;
}

void CPtTextFieldTTF::openIME()
{
    m_bOpen = true;
    m_pCursorSprite->setVisible(true);
    this->attachWithIME();
}

void CPtTextFieldTTF::closeIME()
{
    m_bOpen = false;
    if (m_pHandler)
    {
        m_pHandler->onEditEnd(this);
    }
    m_pCursorSprite->setVisible(false);
    this->detachWithIME();
}

void CPtTextFieldTTF::onExit()
{
    
    CCTextFieldTTF::onExit();
    
}

bool CPtTextFieldTTF::getIsPsw()
{
    return isPsw;
}
//设置星号显示否
void CPtTextFieldTTF::setIsPsw( bool bFlag)
{
    isPsw = bFlag;
}

int CPtTextFieldTTF::getLimitNum()
{
    return m_limitNum;
}
//设置字符长度
void CPtTextFieldTTF::setLimitNum(int limitNum)
{
    m_limitNum = limitNum;
}

void CPtTextFieldTTF::setString(const char *displayTx, const char* inputTx)
{
    CC_SAFE_DELETE(m_pInputText);
    
    if (inputTx)
    {
        m_pInputText = new std::string(inputTx);
    }
    else
    {
        m_pInputText = new std::string;
    }
    
    // if there is no input text, display placeholder instead
    if (! m_pInputText->length())
    {
        CCLabelTTF::setString(m_pPlaceHolder->c_str());
    }
    else
    {
        CCLabelTTF::setString(displayTx);
    }
    m_nCharCount = _calcCharCount(m_pInputText->c_str());
}

void CPtTextFieldTTF::setCursorColor(const ccColor3B& color3)
{
    m_sColor = m_sColorUnmodified = color3;
    
    if (m_bOpacityModifyRGB)
    {
        m_sColor.r = color3.r * m_nOpacity/255.0f;
        m_sColor.g = color3.g * m_nOpacity/255.0f;
        m_sColor.b = color3.b * m_nOpacity/255.0f;
    }
    
    updateColor();
    m_pCursorSprite->setColor(color3);
}


void CPtTextFieldTTF::setHanlder(CPtEditBoxHandler *inHandler)
{
    if (inHandler && inHandler != m_pHandler)
    {
        m_pHandler = inHandler;
    }
    
}
// implement CPtEditBoxWidget


CPtEditBoxWidget* CPtEditBoxWidget::create(const char *pszFileName)
{
    CPtEditBoxWidget * editbox = new CPtEditBoxWidget();
    if (editbox && editbox->initWithFile(pszFileName))
    {
        editbox->autorelease();
        return editbox;
    }
    CC_SAFE_DELETE(editbox);
    return NULL;
    
}
CPtEditBoxWidget* CPtEditBoxWidget::create(const char *pszFileName, const CCRect& rect)
{
    CPtEditBoxWidget * editbox = new CPtEditBoxWidget();
    if (editbox && editbox->initWithFile(pszFileName, rect))
    {
        editbox->autorelease();
        return editbox;
    }
    CC_SAFE_DELETE(editbox);
    return NULL;
}

void CPtEditBoxWidget::initTextField(const char *placeholder, const char *fontName, float fontSize, int limit /*= 12*/, bool isPsw /*= false*/)
{
    if (m_pTextFieldTTF == NULL)
    {
        m_pTextFieldTTF = CPtTextFieldTTF::create(placeholder, fontName, fontSize);
        if (m_pTextFieldTTF)
        {
            m_pTextFieldTTF->setIsPsw(isPsw);
            m_pTextFieldTTF->setLimitNum(limit);
            m_pTextFieldTTF->setContentSize(this->getContentSize());
            m_pTextFieldTTF->setDesignedSize(this->getContentSize());
            m_pTextFieldTTF->setAnchorPoint(CCPointZero);
            this->addChild(m_pTextFieldTTF);
        }
    }
}

void CPtEditBoxWidget::setWordColor(const ccColor3B& color3)
{
    if (m_pTextFieldTTF)
    {
        m_pTextFieldTTF->setColor(color3);
    }
}
void CPtEditBoxWidget::setCursorColor(const ccColor3B& color3)
{
    if (m_pTextFieldTTF)
    {
        m_pTextFieldTTF->setCursorColor(color3);
    }
}

void CPtEditBoxWidget::setHanlder(CPtEditBoxHandler *inHandler, const int index)
{
    if (m_pTextFieldTTF && inHandler)
    {
        inHandler->registerEditBox(m_pTextFieldTTF, index);
        m_pTextFieldTTF->setHanlder(inHandler);
    }
}

bool CPtEditBoxWidget::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtEditBoxWidget::ccTouchBegan");
    if (m_pTextFieldTTF && m_pTextFieldTTF->getHanlder())
    {
        return m_pTextFieldTTF->getHanlder()->onTouchBegin(pTouch);
    }
    return false;
}
void CPtEditBoxWidget::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCLog("CPtEditBoxWidget::ccTouchEnd");
    
    if (m_pTextFieldTTF && m_pTextFieldTTF->getHanlder())
    {
        m_pTextFieldTTF->getHanlder()->onTouchEnd(pTouch);
    }
}

void CPtEditBoxWidget::onEnter()
{
    CCSprite::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -300, false);
    
}
void CPtEditBoxWidget::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

