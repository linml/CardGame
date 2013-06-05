#include "PtSoundTool.h"
#include "cocos2d.h"
#include "gameConfig.h"
#include "SimpleAudioEngine.h" 

USING_NS_CC;

namespace PtSoundTool {
        
    //播放系统音效
    void playSysSoundEffect(const string& file, bool bLoop)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect((sysSoundPath+file).c_str(),bLoop);
    }

    //播放背景音乐
    void playBgSound(const string& file, bool bReleaseData)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic((bgSoundPath+file).c_str(),bReleaseData);
    }
    
    //停止音乐
    void stopBgSound()
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    }
    
    void setBackgroundVolume(float volume)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
    }
    
    void setEffectsVolume(float volume)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
    }
        
}
