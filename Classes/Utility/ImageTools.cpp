#include "ImageTools.h"
#include "GameTools.h"

namespace ImageTools {
CCTexture2D* getTexture(const char* fileName) {
	int pos = -1;
	std::string newName = std::string(fileName);
	if ((pos = newName.find("_opposition")) != -1) {

	} else if ((pos = newName.find("_gray")) != -1) {

	} else if ((pos = newName.find("_color")) != -1) {

	}

	if (pos != -1) {
		newName = newName.substr(0, pos);
	}

	return CCTextureCache::sharedTextureCache()->textureForKey(newName.c_str());

}
    
void convertToGray(CCImage* img)
    {
        for (int i = 0; i < img->getDataLen(); i++) {
            if (img->hasAlpha()) {
                int graycolor = (int) (0.11 * img->getData()[0 + i * 4] + 0.59 * img->getData()[1 + i * 4] + 0.3 * img->getData()[2 + i * 4]);
                img->getData()[i * 4] = graycolor;
                img->getData()[1 + i * 4] = graycolor;
                img->getData()[2 + i * 4] = graycolor;
            } else {
                int graycolor = (int) (0.11 * img->getData()[0 + i * 3] + 0.59 * img->getData()[1 + i * 3] + 0.3 * img->getData()[2 + i * 3]);
                img->getData()[i * 3] = graycolor;
                img->getData()[1 + i * 3] = graycolor;
                img->getData()[2 + i * 3] = graycolor;
            }
        }
    }
    
    void convertToOpposition(CCImage* img)
    {
        for (int i = 0; i < img->getDataLen(); i++) {
			if (img->hasAlpha()) {
				int a = img->getData()[3 + i * 4];
				img->getData()[i * 4] = a - img->getData()[i * 4];
				img->getData()[1 + i * 4] = a - img->getData()[1 + i * 4];
				img->getData()[2 + i * 4] = a - img->getData()[2 + i * 4];
			} else {
				img->getData()[i * 3] = 255 - img->getData()[i * 3];
				img->getData()[1 + i * 3] = 255 - img->getData()[1 + i * 3];
				img->getData()[2 + i * 3] = 255 - img->getData()[2 + i * 3];
			}
            
		}
    }
    /*
    void convertToColor(CCImage* img)
    {
        for (int i = 0; i < img->getDataLen(); i++) {
			if (img->hasAlpha()) {
				int a = img->getData()[3 + i * 4];
				img->getData()[i * 4] = r * a / 255;
				img->getData()[1 + i * 4] = g * a / 255;
				img->getData()[2 + i * 4] = b * a / 255;
			} else {
				img->getData()[i * 3] = r;
				img->getData()[1 + i * 3] = g;
				img->getData()[2 + i * 3] = b;
			}
		}
    }*/

CCTexture2D* getOppositionTexture(const char* fileName) {
	int pos = -1;
	std::string newName = std::string(fileName);
		std::string baseName = newName;
	if ((pos = newName.find("_opposition")) != -1) {

	} else if ((pos = newName.find("_gray")) != -1) {

	} else if ((pos = newName.find("_color")) != -1) {

	}

	if (pos != -1) {
		newName = newName.substr(0, pos);
		baseName = newName;
	}

	if (newName.find("_opposition") == -1) {
		newName += "_opposition";
	}

	CCTexture2D* tex = CCTextureCache::sharedTextureCache()->textureForKey(newName.c_str());
	if (!tex) {
		CCImage* img = new CCImage();
		img->initWithImageFile(baseName.c_str());
		convertToOpposition(img);
		tex = CCTextureCache::sharedTextureCache()->addUIImage(img, newName.c_str());
        CCLog("Opposition:%s", baseName.c_str());
#if CC_ENABLE_CACHE_TEXTTURE_DATA
        VolatileTexture::addProcImageTexture(tex, baseName.c_str(), &ImageTools::convertToOpposition);
#endif
        delete img;
	}

	return tex;
}

CCTexture2D* getGrayTexture(const char* fileName, const char* sKey) {
	CCImage* img = new CCImage();
	img->initWithImageFile(fileName);
	convertToGray(img);
	CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addUIImage(img, sKey);
    CCLog("grayTexture:%s", fileName);
#if CC_ENABLE_CACHE_TEXTTURE_DATA
    VolatileTexture::addProcImageTexture(tex, fileName, &ImageTools::convertToGray);
#endif
    delete img;
	return tex;
}

CCTexture2D* getGrayTexture(const char* fileName) {
	int pos = -1;
	std::string newName = std::string(fileName);
    string filePath2 = CCFileUtils::sharedFileUtils()->getWriteablePath() + newName.substr(newName.find_last_of("/"));
	std::string baseName = newName;
	if ((pos = newName.find("_opposition")) != -1) {

	} else if ((pos = newName.find("_gray")) != -1) {

	} else if ((pos = newName.find("_color")) != -1) {

	}

	if (pos != -1) {
		newName = newName.substr(0, pos);
		baseName = newName;
	}

	if (newName.find("_gray") == -1) {
		newName += "_gray";
	}   
	CCTexture2D* tex = CCTextureCache::sharedTextureCache()->textureForKey(newName.c_str());
	if (!tex) {
		CCImage* img = new CCImage();
		img->initWithImageFile(baseName.c_str());
		convertToGray(img);
        tex = CCTextureCache::sharedTextureCache()->addUIImage(img, newName.c_str());
        delete img;
        CCLog("grayTexture:%s", baseName.c_str());
        
#if CC_ENABLE_CACHE_TEXTTURE_DATA
        VolatileTexture::addProcImageTexture(tex, baseName.c_str(), &ImageTools::convertToGray);
#endif
	}
	return tex;
}
    
CCTexture2D* getColorTexture(const char* fileName, int r, int g, int b) {
	int pos = -1;
	std::string newName = std::string(fileName);
	std::string baseName = newName;
	if ((pos = newName.find("_opposition")) != -1) {

	} else if ((pos = newName.find("_gray")) != -1) {

	} else if ((pos = newName.find("_color")) != -1) {

	}

	if (pos != -1) {
		newName = newName.substr(0, pos);
		baseName = newName;
	}

	if (newName.find("_color") == -1) {
		newName = std::string(fileName) + "_" + "color" + GameTools::ConvertToString(r) + GameTools::ConvertToString(g)
				+ GameTools::ConvertToString(b);
	}

	CCTexture2D* tex = CCTextureCache::sharedTextureCache()->textureForKey(newName.c_str());
	if (!tex) {
		CCImage* img = new CCImage();
		img->initWithImageFile(baseName.c_str());
		for (int i = 0; i < img->getDataLen(); i++) {
			if (img->hasAlpha()) {
				int a = img->getData()[3 + i * 4];
				img->getData()[i * 4] = a - img->getData()[i * 4];
				img->getData()[1 + i * 4] = a - img->getData()[1 + i * 4];
				img->getData()[2 + i * 4] = a - img->getData()[2 + i * 4];
			} else {
				img->getData()[i * 3] = 255 - img->getData()[i * 3];
				img->getData()[1 + i * 3] = 255 - img->getData()[1 + i * 3];
				img->getData()[2 + i * 3] = 255 - img->getData()[2 + i * 3];
			}
            
		}
		tex = CCTextureCache::sharedTextureCache()->addUIImage(img, newName.c_str());
        delete img;
        
        CCLog("grayTexture:%s", baseName.c_str());
        
#if CC_ENABLE_CACHE_TEXTTURE_DATA
        //VolatileTexture::addProcImageTexture(tex, baseName.c_str(), &ImageTools::convertToColor);
#endif
	}
	return tex;
}

}
