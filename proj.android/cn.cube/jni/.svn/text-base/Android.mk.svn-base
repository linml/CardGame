LOCAL_PATH := $(call my-dir)

PATH_TO_COCO_CLASSES = ../../../Classes
PATH_TO_COCO_LIBS = ../../../libs

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   $(PATH_TO_COCO_CLASSES)/AppDelegate.cpp \
                   $(PATH_TO_COCO_CLASSES)/HelloWorldScene.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes                 

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../libs)\
$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
