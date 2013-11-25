//
//  CPtEditBoxWidget.h
//  91.cube
//
//  Created by phileas on 13-7-4.
//
//

#ifndef ___1_cube__CPtEditBoxWidget__
#define ___1_cube__CPtEditBoxWidget__


#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
USING_NS_CC;
USING_NS_CC_EXT;

class CPtEditBoxHandler : public CCTextFieldDelegate
{
public:
    CPtEditBoxHandler();
    virtual ~CPtEditBoxHandler();
    bool registerEditBox(CCNode * node ,const int  pId);
    CCDictionary *getData(){return m_pDataes;};
public:
    virtual void onEditBegin(CCTextFieldTTF * edit);
    virtual void onEditEnd(CCTextFieldTTF * edit);
    
    virtual bool onTouchBegin(CCTouch *pTouch);
    virtual void onTouchEnd(CCTouch *pTouch);
    
protected:
    CCDictionary * m_pEditBoxes;
    CCPoint m_beginPos;
    CCDictionary *m_pDataes;
};


class CPtTextFieldTTF: public CCTextFieldTTF, public CCTouchDelegate ,public CCTextFieldDelegate
{
    
    CC_SYNTHESIZE(bool, m_bOpen, OpenFlag);
protected:
    
    // 点击开始位置
    CCPoint m_beginPos;
    // 光标精灵
    CCSprite *m_pCursorSprite;
    // 光标动画
    CCAction *m_pCursorAction;
    // 光标坐标
    CCPoint m_cursorPos;
    // 是否加密显示
    bool isPsw;
    int m_limitNum;
    // 输入框内容
    CCSize m_designedSize;
    
    CPtEditBoxHandler * m_pHandler;
public:
    CPtTextFieldTTF();
    ~CPtTextFieldTTF();
    
    
    
    /** creates a CCLabelTTF from a fontname and font size */
    static CPtTextFieldTTF * create(const char *placeholder, const char *fontName, float fontSize);
    
    void onEnter();
    void onExit();
    
    // 初始化光标精灵
    void initCursorSprite(int nHeight);
    void setCursorColor(const ccColor3B& color3);
    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
    
    
    // 判断是否点击在TextField处
    bool isInTextField(CCTouch *pTouch);
    // 得到TextField矩形
    CCRect getRect();
    
    // 打开输入法
    void openIME();
    // 关闭输入法
    void closeIME();
    
    //设置是否星号显示
    bool getIsPsw();
    void setIsPsw(bool bFlag);
    //设置字符长度限制，一个汉字三个字符
    void setLimitNum(int limitNum);
    int getLimitNum();
    //重载原函数，用来显示星号
    void setString(const char* displayTx, const char* inputTx);
    //点击弹出输入法的尺寸范围
    void setDesignedSize(CCSize size);
    CCSize getDesignedSize();
    void setHanlder(CPtEditBoxHandler *inHandler);
    
    std::string *getInputText(){return  m_pInputText;};
    
    CPtEditBoxHandler *getHanlder(){ return m_pHandler;};
    
};

class CPtEditBoxWidget : public CCSprite , public CCTouchDelegate
{
    
public:
    CPtTextFieldTTF * m_pTextFieldTTF;
public:
    static CPtEditBoxWidget* create(const char *pszFileName);
    static CPtEditBoxWidget* create(const char *pszFileName, const CCRect& rect);
    
public:
    void initTextField(const char *placeholder, const char *fontName, float fontSize, int limit = 12, bool isPsw = false);
    
    void setWordColor(const ccColor3B& color3);
    void setCursorColor(const ccColor3B& color3);
    void setHanlder(CPtEditBoxHandler *inHandler, const int index);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    
    
};


#endif /* defined(___1_cube__CPtEditBoxWidget__) */
