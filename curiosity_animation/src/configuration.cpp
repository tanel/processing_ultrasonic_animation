//
//  configuration.cpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#include "configuration.hpp"

#include "ofxXmlSettings.h"

bool Configuration::Read() {
    // Read configuration or create default
    ofxXmlSettings xml;
    if (!xml.loadFile("configuration.xml")) {
        xml.setValue("configuration:Fullscreen", Fullscreen);
        xml.setValue("configuration:MinDistance", MinDistance);
        xml.setValue("configuration:MaxDistance", MaxDistance);
        xml.setValue("configuration:DeathZone", DeathZone);
        xml.setValue("configuration:RestartIntervalSeconds", RestartIntervalSeconds);
        xml.setValue("configuration:ActiveSerialPort", ActiveSerialPort);
        xml.setValue("configuration:StartingFramesPerSecond", StartingFramesPerSecond);
        xml.setValue("configuration:FinishingFramesPerSecond", FinishingFramesPerSecond);
        xml.setValue("configuration:StartingVolume", StartingVolume);
        xml.setValue("configuration:FinishingVolume", FinishingVolume);
        xml.setValue("configuration:StartingHeartBeatSpeed", StartingHeartBeatSpeed);
        xml.setValue("configuration:FinishingHeartBeatSpeed", FinishingHeartBeatSpeed);
        xml.setValue("configuration:FrameRate", FrameRate);
        xml.setValue("configuration:VideoFileName", VideoFileName);
        xml.setValue("configuration:AutoSaveSeconds", AutoSaveSeconds);
        xml.setValue("configuration:IntroFileName", IntroFileName);
        xml.setValue("configuration:OutroFileName", OutroFileName);
        
        if (!xml.saveFile("configuration.xml")) {
            std::cerr << "Error saving configuration file" << std::endl;
            return false;
        }
    } else {
        Fullscreen = xml.getValue("configuration:Fullscreen", Fullscreen);
        MinDistance = xml.getValue("configuration:MinDistance", MinDistance);
        MaxDistance = xml.getValue("configuration:MaxDistance", MaxDistance);
        DeathZone = xml.getValue("configuration:DeathZone", DeathZone);
        RestartIntervalSeconds = xml.getValue("configuration:RestartIntervalSeconds", RestartIntervalSeconds);
        ActiveSerialPort = xml.getValue("configuration:ActiveSerialPort", ActiveSerialPort);
        StartingFramesPerSecond = xml.getValue("configuration:StartingFramesPerSecond", StartingFramesPerSecond);
        FinishingFramesPerSecond = xml.getValue("configuration:FinishingFramesPerSecond", FinishingFramesPerSecond);
        StartingVolume = xml.getValue("configuration:StartingVolume", StartingVolume);
        FinishingVolume = xml.getValue("configuration:FinishingVolume", FinishingVolume);
        StartingHeartBeatSpeed = xml.getValue("configuration:StartingHeartBeatSpeed", StartingHeartBeatSpeed);
        FinishingHeartBeatSpeed = xml.getValue("configuration:FinishingHeartBeatSpeed", FinishingHeartBeatSpeed);
        FrameRate = xml.getValue("configuration:FrameRate", FrameRate);
        VideoFileName = xml.getValue("configuration:VideoFileName", VideoFileName);
        AutoSaveSeconds = xml.getValue("configuration:AutoSaveSeconds", AutoSaveSeconds);
        IntroFileName = xml.getValue("configuration:IntroFileName", IntroFileName);
        OutroFileName = xml.getValue("configuration:OutroFileName", OutroFileName);
    }
    
    return true;
}