//
//  CCardPanel.cpp
//  91.cube
//
//  Created by linminglu on 13-6-14.
//
//

#include "CCardPanel.h"
CCardPanel::CCardPanel()
{
    
}

CCardPanel::~CCardPanel()
{
    
}

CCardPanel  *CCardPanel::Create(const char *pngFileName, const char *name)
{
    CCardPanel *cardSprite=new CCardPanel();
    if(cardSprite ==NULL|| !cardSprite->initSprite(pngFileName, name))
    {
        delete cardSprite;
        cardSprite=NULL;
    }
    cardSprite->autorelease();
    return cardSprite;
    
}

void CCardPanel::setDead()
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
    float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
    gl_FragColor = vec4(grey, grey, grey, 1.0); \n \
    }";
    
    CCGLProgram* pProgram = new CCGLProgram();
    pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
    this->setShaderProgram(pProgram);
    pProgram->release();
    CHECK_GL_ERROR_DEBUG();
    
    this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
    this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
    this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
    CHECK_GL_ERROR_DEBUG();
    
    this->getShaderProgram()->link();
    CHECK_GL_ERROR_DEBUG();
    
    this->getShaderProgram()->updateUniforms();
    CHECK_GL_ERROR_DEBUG();
    
}

bool CCardPanel::initSprite(const char *pngFileName,const char *name)
{
    this->initWithFile(pngFileName);
    CCLabelTTF *label=CCLabelTTF::create(name, "Arial", 15);
    addChild(label,1,11);
    label->setPosition(ccp(getContentSize().width/2, getContentSize().height+20));
    return true;
}

CCLabelTTF *CCardPanel::getLabel()
{
    return (CCLabelTTF *)getChildByTag(11);
}
