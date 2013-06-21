#ifndef __IMAGETOOLS_H_
#define __IMAGETOOLS_H_
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

namespace ImageTools {

	CCTexture2D* getTexture(const char* fileName);

	CCTexture2D* getOppositionTexture(const char* fileName);

	CCTexture2D* getGrayTexture(const char* fileName);
    
	CCTexture2D* getGrayTexture(const char* fileName, const char* sKey);

	CCTexture2D* getColorTexture(const char* fileName,int r,int g,int b);
    
    void convertToGray(CCImage* img);
    
    void convertToOpposition(CCImage* img);
    
    //void convertToColor(CCImage* img);

}

#endif
