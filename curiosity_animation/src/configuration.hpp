//
//  configuration.hpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#ifndef configuration_hpp
#define configuration_hpp

#include <stdio.h>
#include "ofMain.h"

class Configuration {
public:
    Configuration()
    : Fullscreen(false)
    , MinDistance(0)
    , MaxDistance(1000)
    , DeathZone(100)
    , RestartIntervalSeconds(30)
    , ActiveSerialPort(0)
    , StartingFramesPerSecond(6)
    , FinishingFramesPerSecond(100)
    , StartingVolume(0.2)
    , FinishingVolume(1.0)
    , StartingHeartBeatSpeed(1.0)
    , FinishingHeartBeatSpeed(2.0)
    , FrameRate(60)
    , VideoFileName("shoot2_anim_edit.mov")
    , SetFrame(true)
    , CheckAfterNFrames(20)
    , AutoSaveSeconds(10)
    , IntroFileName("intro.jpg"){}
    
    bool Read();
    
    bool Fullscreen;
    int MinDistance;
    int MaxDistance;
    int DeathZone;
    int RestartIntervalSeconds;
    int ActiveSerialPort;
    int StartingFramesPerSecond;
    int FinishingFramesPerSecond;
    float StartingVolume;
    float FinishingVolume;
    float StartingHeartBeatSpeed;
    float FinishingHeartBeatSpeed;
    int FrameRate;
    std::string VideoFileName;
    bool SetFrame;
    int CheckAfterNFrames;
    int AutoSaveSeconds;
    std::string IntroFileName;
    std::string OutroFileName;
};



#endif /* configuration_hpp */
