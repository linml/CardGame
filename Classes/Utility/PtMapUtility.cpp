#include "PtMapUtility.h"
#include "gameTools.h"
//#include "CCFileUtils.h" 
#include "stdlib.h"
#include "HBMap.h"
#include "HBSprite.h"
#include "HBSpriteCache.h"
#include "MapLayerData.h"


namespace PtMapUtility {
    
    //生成编辑器Layer
    CCLayer* layerWithFile(const string& file, bool isBackGround) {
        CCLOG("file:%s",file.c_str());
        HBMap* hbMap = new HBMap(file.c_str());
        CCLayer* map = hbMap->createCCLayer();
        if (isBackGround) {
            
            //颜色底层
            ccColor4B mapBackGroundColor = hbMap->getMapBackGroundColor();
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCLayerColor *backgroundColor = CCLayerColor::create(mapBackGroundColor, size.width, size.height);
            map->addChild(backgroundColor);
        }
        
        //添加到缓存中
        addMapInfoToCache(hbMap, file, map);
        
        delete hbMap;
        return map;
        
    }
    
    //读取精灵相关信息（初始化属性，z，动作，runAction）
    void setLayerDatInfo(CCNode* layer, CCDictionary* data) {
        CCArray* vKey = data->allKeys();
        vector<ActionData*> vActions;
        
        //判断是否要初始化
        for (int i = 0; i < vKey->count(); i++) {
            CCString* key=(CCString*)vKey->objectAtIndex(i);
            if (key->m_sString == "initProperty") {
                //初始化数据
                CCDictionary* initData = (CCDictionary*) data->objectForKey("initProperty");
                initLayerProperty(layer, initData);
                break;
            }
        }
        
//        //判断是否要动作
//        for (int i = 0; i < vKey->count(); i++) {
//            CCString* key=(CCString*)vKey->objectAtIndex(i);
//            if (key->m_sString == "actions") {
//                //action数据
//                createActions(data, &vActions);
//                break;
//            }
//        }
//        
//        //判断是否runAction
//        string runActionName = string(valueForKey("runAction", data));
//        if (runActionName != "") {
//            setAction(layer, runActionName, &vActions);
//        }
//        
//        //删除vAction
//        for (int i = 0; i < vActions.size(); i++) {
//            delete vActions[i];
//        }
        
    }

    
    
    //脚本添加child;
    void addChildFromScript(CCNode* father, const string& file)
    {
        CCLOG("file:%s",file.c_str());
        std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(file.c_str());
#if CC_TARGET_PLATFORM==CC_PLATFORM_IOS
        if( (access(file.c_str(), 0 )) == -1 )
        {
            assert("file not exist");
        }
#elif CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind;
        
        printf ("Target file is %s. ",file.c_str());
        
        hFind = FindFirstFile(file.c_str(), &FindFileData);
        
        if (hFind == INVALID_HANDLE_VALUE)
        {
            printf ("Invalid File Handle. Get Last Error reports %d ", GetLastError ());
        }
#endif
       
        CCDictionary* data = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
        
        
        CCLOG("m_sPlistFile:%s",m_sPlistFile.c_str());
        CCDictElement *keyString = NULL;
         CCDICT_FOREACH(data, keyString)
         {
         const char *xps =  keyString->getStrKey();
         CCLog("dict:key %s", xps);
         }
        
        
        //遍历key
        CCArray* vNodeName = data->allKeys();
        assert(vNodeName!=NULL && "readfile error");
        //先初始化层加载数据，其他node需要相关数据
        for (int i = 0; i < vNodeName->count(); i++)
        {

            CCString* key=(CCString*)vNodeName->objectAtIndex(i);
            CCDictionary* nodeData = (CCDictionary*)data->objectForKey(key->m_sString.c_str());
            string nodeType = string(valueForKey("type", nodeData));
            //解析z
            int z = intForKey("z", nodeData);
             if (nodeType == type_layer) {
                CCLayer* layer = createLayerWithScript(nodeData);
                father->addChild(layer, z);
            }
            //优先载入的标记
            else if (key->m_sString.find("pre_") != -1)
            {
                createNodeWithScript(father, nodeType, nodeData);
            }
        }
         CCLOG("m_sPlistFileaaa");
        for (int i = 0; i < vNodeName->count(); i++) {

			CCString* key=(CCString*)vNodeName->objectAtIndex(i);
           //解析类型
            CCDictionary* nodeData = (CCDictionary*) data->objectForKey(key->m_sString.c_str());
            
             string nodeType = string(valueForKey("type", nodeData));
            
            //排除优先载入的（前面已经载过了）
            if (key->m_sString.find("pre_") == -1) {
                createNodeWithScript(father, nodeType, nodeData);
            }
        }
        CCLOG(".....");
    }
    
    //脚本生成Layer
    CCLayer* createLayerWithScript(CCDictionary* data)
    {
        
        CCLayer* layer;
        string layerFile = string(valueForKey("layerFile", data));
        if (layerFile == "") {
            layer = new CCLayer();
            layer->autorelease();
        } else {
            layerFile=mapPath+layerFile;
            layer = layerWithFile(layerFile);
        }
        
        //默认位置
        CCSize screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();        
        setLayerDatInfo(layer, data);
        
        return layer;
    }
    //脚本生成node
    CCNode* createNodeWithScript(CCNode* father, const string& nodeType, CCDictionary* nodeData) {
        //解析z
        int z = intForKey("z", nodeData);
        
        if (nodeType == type_sprite || nodeType == "")
        {
            CCSprite* sprite = createSpriteWithScript(nodeData);
            
            string sIsChild = string(valueForKey("isChild", nodeData));
            string sFatherNode = string(valueForKey("fatherTagArray", nodeData));
            if (sIsChild == "" && sFatherNode == "") {
                father->addChild(sprite, z);
            } else if (sFatherNode != "") {
                CCNode* fatherNode = Utility::getNodeByTag(father, sFatherNode, 0);
                fatherNode->addChild(sprite, z);
            }
            
            //特殊选项(目前只是取位置 取ui上某个sprite)
            CCDictionary* initData = (CCDictionary*)nodeData->objectForKey("speOption");
            if (initData != NULL) {
                initSpeOption(father, sprite, initData);
            }
        }
        else if (nodeType == type_init_existnode)
        {
            string sTagArray = string(valueForKey("tagArray", nodeData));
            vector<int> vTag = transToIntVector(splitString(sTagArray.c_str(), ","));
            
            CCNode* child = Utility::getNodeByTag(father, vTag, 0);
            //读取初始化和动作
            setLayerDatInfo(child, nodeData);
        }
        else if (nodeType == type_init_exitttf)
        {
            string sTagArray = string(valueForKey("tagArray", nodeData));
            vector<int> vTag = transToIntVector(splitString(sTagArray.c_str(), ","));
            
            CCLabelTTF* child = (CCLabelTTF*)Utility::getNodeByTag(father, vTag, 0);
            
            //修改文字
            string word = string(valueForKey("wordString", nodeData));
            CCLog("%s",word.c_str());
            if (word == "") {
                //第二种方式从某plist文件中的key获得文字
                string wordFile = string(valueForKey("wordFile", nodeData));
                if (wordFile == "") {
                    wordFile = wordFilePath;
                }
                string wordKey = string(valueForKey("key", nodeData));
                CCLOG("wordFile:%s",wordFile.c_str());
                word = Utility::getWordWithFile(wordFile, wordKey.c_str());
            }
            child->setString(word.c_str());
        }
        else if (nodeType == type_color_layer) {
            CCLayerColor* layer = createColorLayerWithScript(nodeData);
            string sIsChild = string(valueForKey("isChild", nodeData));
            string sFatherNode = string(valueForKey("fatherTagArray", nodeData));
            if (sIsChild == "" && sFatherNode == "") {
                father->addChild(layer, z);
            } else if (sFatherNode != "") {
                CCNode* fatherNode = Utility::getNodeByTag(father, sFatherNode, 0);
                fatherNode->addChild(layer, z);
            }
            
            //特殊选项(目前只是取位置 取ui上某个sprite)
            CCDictionary* initData = (CCDictionary*) nodeData->objectForKey("speOption");
            if (initData != NULL) {
                initSpeOption(father, layer, initData);
            }
        }
        else if (nodeType == type_labelttf) {
            CCLabelTTF* ttf = createLabelTTFWithScript(nodeData);
            string sIsChild = string(valueForKey("isChild", nodeData));
            string sFatherNode = string(valueForKey("fatherTagArray", nodeData));
            if (sIsChild == "" && sFatherNode == "") {
                father->addChild(ttf, z);
            } else if (sFatherNode != "") {
                CCNode* fatherNode = Utility::getNodeByTag(father, sFatherNode, 0);
                fatherNode->addChild(ttf, z);
            }
            
            //特殊选项(目前只是取位置 取ui上某个sprite)
            CCDictionary* initData = (CCDictionary*) nodeData->objectForKey("speOption");
            if (initData != NULL) {
                initSpeOption(father, ttf, initData);
            }
            
        }
        else if (nodeType == type_particle)
        {
            string fileName = string(valueForKey("fileName", nodeData));
            if (fileName != "")
            {
                CCParticleSystemQuad* particle = CCParticleSystemQuad::create(fileName.c_str());
//                #if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//                if(iPlatfrom == PLATFORM_IPHONE) {
//                    particle->setScale(0.5);
//                }            
//                #endif  // CC_PLATFORM_ANDROID
                string sFatherNode = string(valueForKey("fatherTagArray", nodeData));
                if (sFatherNode != "")
                {
                    CCNode* fatherNode = Utility::getNodeByTag(father, sFatherNode, 0);
                    fatherNode->addChild(particle, z);
                }
                else
                {
                    father->addChild(particle, z);
                }
                
                setLayerDatInfo(particle, nodeData);
            }
        }
        
        return NULL;
    }
    
    //生成脚本精灵（三种方式，1--用spriteImage+spriteRect(图片+区域)  2--用spriteName(帧缓存名字) 3--用spriteFile+spriteFile_spriteName(sprite文件+文件中的精灵名字)）
    CCSprite* createSpriteWithScript(CCDictionary* data) {
        
        string sType = string(valueForKey("spriteName", data));
        
        if (sType != "") {
            CCSprite* sprite = CCSprite::create(sType.c_str());
            //读取初始化和动作
            setSpriteDatInfo(sprite, data);
            return sprite;
        }
        sType = string(valueForKey("spriteImage", data));
        if (sType != "") {
            
            string sRect = string(valueForKey("spriteRect", data));
            if (sRect != "") {
                CCRect rect = ccRectForKey("spriteRect", data);
                //            sType = mapImagesPath + sType;
                CCSprite* sprite = CCSprite::create(sType.c_str(), rect);
                //读取初始化和动作
                setSpriteDatInfo(sprite, data);
                return sprite;
            } else {
                CCSprite* sprite = CCSprite::create(sType.c_str());
                //读取初始化和动作
                setSpriteDatInfo(sprite, data);
                return sprite;
            }
            
        }
        sType = string(valueForKey("spriteFile", data));
        if (sType != "spriteFile") {
            string spriteName = string(valueForKey("spriteFile_spriteName", data));
            CCSprite* sprite = spriteWithSpriteFile(sType, spriteName.c_str());
            //读取初始化和动作
            setSpriteDatInfo(sprite, data);
            return sprite;
        }
        
        return NULL;
    }
    
    //生成编辑器中精灵文件的sprite
    CCSprite* spriteWithSpriteFile(const string& file, const char* spriteName) {
        
        //先查找缓存中有没有该精灵文件
        HBSprite* hbSprite = HBSpriteCache::sharedHBSpriteCache()->HBSpriteByName(file.c_str());
        CCSprite* sprite = NULL;
        if (hbSprite == NULL) {
            //没找到 重新加载
            hbSprite = new HBSprite(file.c_str());
            //		hbSprite->autorelease();
            HBSpriteCache::sharedHBSpriteCache()->addHBSprite(hbSprite, file.c_str());
            sprite = hbSprite->createCCSpriteWithName(spriteName);
            hbSprite->release();
            //            delete hbSprite;
        } else {
            sprite = hbSprite->createCCSpriteWithName(spriteName);
        }
        
        return sprite;
        
    }
    
    //脚本生成ColorLayer
    CCLayerColor* createColorLayerWithScript(CCDictionary* data)
    {
        CCLayerColor* layer;
        ccColor4B color;
        //颜色
        string sColor = string(valueForKey("color", data));
        if (sColor == "") {
            color = ccc4(0, 0, 0, 50);
        } else {
            CCRect rect = ccRectForKey("color", data);
            color = ccc4(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        }
        //区域大小
        string sSize = string(valueForKey("size", data));
        if (sSize == "") {
            layer = CCLayerColor::create(color);
        } else {
            CCPoint size = pointFromString(sSize);
//            size = GameTools::HB_POINT_PIXELS_TO_POINTS(size);
            layer = CCLayerColor::create(color, size.x, size.y);
        }
        
        setLayerDatInfo(layer, data);
        
        return layer;
    }
    
    //脚本生成LabelTTF
    CCLabelTTF* createLabelTTFWithScript(CCDictionary* data)
    {
        CCLabelTTF* labelTTF;
        //字体
        string font = string(valueForKey("fontName", data));
        if (font == "") {
            font = sFont;
        }
       else if (font == "btn") {
            font = sFont_btn;
        }
        else if (font == "title") {
            font = sFont_title;
        }
        
        //字体大小
        int intFontSize = intForKey("fontSize", data);
        
        if(intFontSize <= 0)
        {
            //代表预定义字体大小
            int size = sizeof(custom_font_size)/sizeof(custom_font_size[0]);
            int index = -intFontSize;
            if(index < size)
            {
                intFontSize = custom_font_size[index];
            }
        }
//        else {
//            #if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
//            if(iPlatfrom == PLATFORM_IPHONE) {
//                intFontSize /= 2;
//            }
//            #endif
//        }
        
         //字体颜色
        ccColor3B color = color_white;
        //颜色
        string sColor = string(valueForKey("color", data));
        int iColor = atoi(sColor.c_str());
        int size = sizeof(custom_color)/sizeof(custom_color[0]);
        if (iColor > 0 && iColor <= size) {
            color = custom_color[iColor - 1];
        } else {
            if (sColor != "") {
                CCLOG("system color assign is not in the range use default");
                CCRect rect = ccRectForKey("color", data);
                color = ccc3(rect.origin.x, rect.origin.y, rect.size.width);
            }
            
        }
        
        //文字(两种方式1--直接输入文字 2---从某plist文件中的key获得文字)
        string word = string(valueForKey("wordString", data));
        ;
        if (word == "") {
            //第二种方式从某plist文件中的key获得文字
            string wordFile = string(valueForKey("wordFile", data));
            if (wordFile == "") {
                wordFile = wordFilePath;
            }
            string wordKey = string(valueForKey("key", data));
            word = Utility::getWordWithFile(wordFile, wordKey.c_str());
        }
        
        if(font == sFont_title) {
            word += " ";
        }
        
        //区域换行
        string dimensions = string(valueForKey("dimensions", data));
        if (dimensions != "") {
            //区域显示
            CCPoint point = pointFromString(dimensions);
            CCSize size = CCSizeMake(point.x, point.y);
            //对齐方式
            string align = string(valueForKey("align", data));
            CCTextAlignment text_align = kCCTextAlignmentCenter;
            if (align == "right") {
                text_align = kCCTextAlignmentRight;
            } else if (align == "left") {
                text_align = kCCTextAlignmentLeft;
            } else if (align == "center") {
                text_align = kCCTextAlignmentCenter;
            }
            size.height = 0;
            labelTTF = CCLabelTTF::labelWithString(word.c_str(), size, text_align, font.c_str(), intFontSize);
        } else {
            labelTTF = CCLabelTTF::labelWithString(word.c_str(), font.c_str(), intFontSize);
        }
        
        labelTTF->setColor(color);
        //读取初始化和动作
        setSpriteDatInfo(labelTTF, data);
        
        return labelTTF;
    }

    
    //初始化层信息
    void initLayerProperty(CCNode* layer, CCDictionary* data)
    {
        CCArray* vProperty = data->allKeys();
        for (int i = 0; i < vProperty->count(); i++)
        {
            CCString* value = (CCString*)vProperty->objectAtIndex(i);
            if (value->m_sString == "pos")
            {
                CCPoint pos = ccPointForKey("pos", data);
                layer->setPosition(pos);
            }
            else if (value->m_sString == "anchor")
            {
                CCPoint anchor = ccPointForKey("anchor", data);
                layer->setAnchorPoint(anchor);
            }
            else if (value->m_sString == "scale")
            {
                float scale = floatForKey("scale", data);
                layer->setScale(scale);
            }
            else if (value->m_sString == "scaleX")
            {
                float scaleX = floatForKey("scaleX", data);
                layer->setScaleX(scaleX);
            }
            else if (value->m_sString == "scaleY")
            {
                float scaleY = floatForKey("scaleY", data);
                layer->setScaleY(scaleY);
            }
            else if (value->m_sString == "rotate")
            {
                int rotate = intForKey("rotate", data);
                layer->setRotation(rotate);
            }
            else if (value->m_sString == "visible")
            {
                bool visible = boolForKey("visible", data);
                layer->setVisible(visible);
            }
            else if (value->m_sString == "tag")
            {
                int tag = intForKey("tag", data);
                layer->setTag(tag);
            }
        }
    }
    
    //初始化特殊选项
    void initSpeOption(CCNode* father, CCNode* node, CCDictionary* data)
    {
        CCNode* child;
        string sTagArray = string(valueForKey("tagArray", data));
        if (sTagArray != "") {
            vector<int> vTag = transToIntVector(splitString(sTagArray.c_str(), ","));
            
            child = Utility::getNodeByTag(father, vTag, 0);
            CCRect rect = Utility::getNodeRectInScreen(father, vTag, 0);
            CCPoint pos = ccp(0,0);
            string sZ = string(valueForKey("z", data));
            if (sZ != "") {
                int z = atoi(sZ.c_str());
                child->addChild(node, z);
            } else {
                rect.origin = Utility::getPositionByTag(father, father->getPosition(), vTag, 0);
                rect.origin.x -= child->getAnchorPoint().x * rect.size.width * child->getScale();
                rect.origin.y -= child->getAnchorPoint().y * rect.size.height * child->getScale();
                pos = rect.origin;
            }
            
            //            CCPoint pos = rect.origin;
            
            string offsetPoint = string(valueForKey("offsetPos", data));
            if (offsetPoint != "") {
                CCPoint offsetPos = pointFromString(offsetPoint);
                pos = ccp(pos.x + offsetPos.x,pos.y + offsetPos.y);
            }
            
            node->setPosition(pos);
            
        } else {
            
            int mapTag = intForKey("desMapTag", data);
            CCLayer* map = (CCLayer*) father->getChildByTag(mapTag);
            if (map != NULL) {
                int layerTag = intForKey("desLayerTag", data);
                CCLayer* layer = (CCLayer*) map->getChildByTag(layerTag);
                if (layer != NULL) {
                    int spriteTag = intForKey("desTag", data);
                    child = (CCNode*) layer->getChildByTag(spriteTag);
                    
                    string sZ = string(valueForKey("z", data));
                    if (sZ != "") {
                        int z = atoi(sZ.c_str());
                        map->addChild(node, z);
                        
                        CCPoint pos = ccp(layer->getPosition().x + child->getPosition().x,layer->getPosition().y + child->getPosition().y);
                        
                        string offsetPoint = string(valueForKey("offsetPos", data));
                        if (offsetPoint != "") {
                            CCPoint offsetPos = pointFromString(offsetPoint);
                            pos = ccp(pos.x + offsetPos.x,pos.y + offsetPos.y);
                        }
                        
                        node->setPosition(pos);
                        
                    } else {
                        CCPoint pos =
                        ccp(father->getPosition().x + map->getPosition().x + layer->getPosition().x + child->getPosition().x,father->getPosition().y + map->getPosition().y + layer->getPosition().y + child->getPosition().y);
                        
                        string offsetPoint = string(valueForKey("offsetPos", data));
                        if (offsetPoint != "") {
                            CCPoint offsetPos = pointFromString(offsetPoint);
                            pos = ccp(pos.x + offsetPos.x,pos.y + offsetPos.y);
                        }
                        
                        node->setPosition(pos);
                    }
                }
                
            }
            
        }
        
    }
    
    //读取精灵初始化属性(initProperty)
    void initSpriteProperty(CCSprite* sprite, CCDictionary* data) {
        CCArray* vProperty = data->allKeys();
        for (int i = 0; i < vProperty->count(); i++) {
            CCString* key = (CCString*)vProperty->objectAtIndex(i);
            if (key->m_sString == "pos") {
                CCPoint pos = ccPointForKey("pos", data);
                sprite->setPosition(pos);
            } else if (key->m_sString == "texturerect") {
                CCRect rect = ccRectForKey("texturerect", data);
                sprite->setTextureRect(rect);
            } else if (key->m_sString == "anchor") {
                CCPoint anchor = ccPointForKey("anchor", data);
                sprite->setAnchorPoint(anchor);
            } else if (key->m_sString == "scale") {
                float scale = floatForKey("scale", data);
                sprite->setScale(scale);
            } else if (key->m_sString == "scaleX") {
                float scaleX = floatForKey("scaleX", data);
                sprite->setScaleX(scaleX);
            } else if (key->m_sString == "scaleY") {
                float scaleY = floatForKey("scaleY", data);
                sprite->setScaleY(scaleY);
            } else if (key->m_sString == "rotate") {
                int rotate = intForKey("rotate", data);
                sprite->setRotation(rotate);
            } else if (key->m_sString == "visible") {
                bool visible = boolForKey("visible", data);
                sprite->setVisible(visible);
            } else if (key->m_sString == "flipX") {
                bool flipX = boolForKey("flipX", data);
                sprite->setFlipX(flipX);
            } else if (key->m_sString == "flipY") {
                bool flipY = boolForKey("flipY", data);
                sprite->setFlipY(flipY);
            } else if (key->m_sString == "tag") {
                int tag = intForKey("tag", data);
                sprite->setTag(tag);
            } else if (key->m_sString == "opacity") {
                float opacity = floatForKey("opacity", data);
                sprite->setOpacity(opacity);
            } else if (key->m_sString == "color") {
                CCRect rect = ccRectForKey("color", data);
                ccColor3B color = ccc3(rect.origin.x, rect.origin.y, rect.size.width);
                sprite->setColor(color);
            }
            //            else if(key->m_sString == "touchTag") {
            //                
            //                int touch_tag = intForKey("touchTag", data);
            //                CCRect rect = sprite->getTextureRect();
            //                rect.origin = ccp(sprite->getPositionInPixels().x - rect.size.width * sprite->getAnchorPoint().x,
            //                                  sprite->getPositionInPixels().y - rect.size.height * sprite->getAnchorPoint().y);
            //                
            //                addTouchSprite(rect, touch_tag);
            //            }
        }
    }
    
    //读取精灵相关信息（初始化属性，z，动作，runAction）
    void setSpriteDatInfo(CCSprite* sprite, CCDictionary* data) {
        CCArray* vKey = data->allKeys();
//        vector<ActionData*> vActions;
        
        //判断是否要初始化
        for (int i = 0; i < vKey->count(); i++) {
            CCString* key = (CCString*)vKey->objectAtIndex(i);
            if ( key->m_sString == "initProperty") {
                //初始化数据
                CCDictionary* initData = (CCDictionary*) data->objectForKey("initProperty");
                initSpriteProperty(sprite, initData);
                break;
            }
        }
        
//        //判断是否要动作
//        for (int i = 0; i < vKey.size(); i++) {
//            if (vKey[i] == "actions") {
//                //action数据
//                createActions(data, &vActions);
//                //            CCDictionary<std::string, CCObject*>* actionData = (CCDictionary<std::string, CCObject*>*)data->objectForKey("actions");
//                //            createActions(actionData,&vActions);
//                break;
//            }
//        }
//        
//
//        //判断是否runAction
//        for (int i = 0; i < vKey.size(); i++) {
//            if (vKey[i] == "runAction") {
//                string runActionName = string(valueForKey("runAction", data));
//                setAction(sprite, runActionName, &vActions);
//                
//                break;
//            }
//        }
//        
//        //删除vAction
//        for (int i = 0; i < vActions.size(); i++) {
//            delete vActions[i];
//        }
    }
    
    //添加相关地图信息到缓存中
    void addMapInfoToCache(HBMap* hbMap, const string& mapFileName, CCLayer* map) {
        //检查是否存在
        MapLayerData* mapLayerData = MapLayerDataCache::sharedMapLayerDataCache()->MapLayerDataByName(mapFileName.c_str());
        if (mapLayerData == NULL) {
            //生成相应的数据
            MapLayerData* mapLayerData = new MapLayerData(hbMap);
            mapLayerData->autorelease();
            mapLayerData->mapFileName = mapFileName;
            mapLayerData->map = map;
            //添加到缓存中
            MapLayerDataCache::sharedMapLayerDataCache()->addMapLayerData(mapLayerData, mapFileName.c_str());
        }
        
    }
    
//    //读取精灵动作
//    void createActions(CCDictionary* data, vector<ActionData*>* vActions)
//    {
//        CCArray*
//        CCMutableArray<CCString*>* vActionString = (CCMutableArray<CCString*>*) data->objectForKey(string("actions"));
//        CCMutableArray<CCString*>::CCMutableArrayIterator iter;
//        int i = 0;
//        char tmp[10];
//        for (iter = vActionString->begin(); iter != vActionString->end(); ++iter, i++)
//        {
//            //动作id
//            ActionData* actionData = new ActionData;
//            string str("Item ");
//            sprintf(tmp, "%d", i);
//            actionData->actionName = (str + tmp);
//            
//            string action = ((CCString*) (*iter))->m_sString;
//            actionData->action = action;
//            actionData->actionType = getSubStr_endStr(action, "(");
//            
//            vActions->push_back(actionData);
//        }
//    }
    
    //添加touchRect
    void addTouchRectFromScript(const string& fileName, CCNode* node, vector<TouchRect>* vTouchRect) {
        std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName.c_str());
        CCLOG("m_sPlistFile touch:%s",m_sPlistFile.c_str());
        CCDictionary* data = CCDictionary::createWithContentsOfFile(m_sPlistFile.c_str());
        
        //遍历key
        CCArray* vNodeName = data->allKeys();
        
        for (int i = 0; i < vNodeName->count(); i++) {
            CCString* nodeName = (CCString*)vNodeName->objectAtIndex(i);
            CCDictionary* nodeData = (CCDictionary*) data->objectForKey(nodeName->m_sString);
            string nodeType = string(valueForKey("type", nodeData));
            
            if (nodeType == type_touch) {
                
                string sTagArray = string(valueForKey("tagArray", nodeData));
                CCRect rect;
                int touch_tag = intForKey("touch_tag", nodeData);
                ;
                CCNode* child = NULL;
                if (sTagArray != "") {
                    rect = Utility::getNodeRectInScreen(node, node->getPosition(), sTagArray);
                    child = Utility::getNodeByTag(node, sTagArray, 0);
                    string sRect = string(valueForKey("touchRect", nodeData));
                    if (sRect != "") {
                        CCRect offsetRect = ccRectForKey("touchRect", nodeData);
                        rect.origin = ccpAdd(rect.origin, offsetRect.origin);
                        rect.size = offsetRect.size;
                    }
                } else if (string(valueForKey("rect", nodeData)) != "") {
                    rect = ccRectForKey("rect", nodeData);
                }
                
                //call_back
                string call_back = string(valueForKey("call_back", nodeData));
                
                TouchRect touchRect(rect, touch_tag, child,call_back);
                vTouchRect->push_back(touchRect);
                
            }
        }
    }

    
}



