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
    , DeathZone(40)
    , SaveZone(40)
    , RestartIntervalSeconds(30)
    , ActiveSerialPort(0)
    , StartingVolume(0.2)
    , FinishingVolume(1.0)
    , StartingHeartBeatSpeed(1.0)
    , FinishingHeartBeatSpeed(2.0)
    , FrameRate(60)
    , VideoFileName("shoot2_anim_edit.mov")
    , SetFrame(true)
    , IntroFileName("intro.jpg")
    , DebugOverlay(true)
    , AutoSaveSeconds(60) {}
    
    bool Read();
    
    bool Fullscreen;
    int MinDistance;
    int MaxDistance;
    int DeathZone;
    int SaveZone;
    int RestartIntervalSeconds;
    int ActiveSerialPort;
    float StartingVolume;
    float FinishingVolume;
    float StartingHeartBeatSpeed;
    float FinishingHeartBeatSpeed;
    int FrameRate;
    std::string VideoFileName;
    bool SetFrame;
    std::string IntroFileName;
    bool DebugOverlay;
    int AutoSaveSeconds;
};



#endif /* configuration_hpp */
