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
    std::string path = ofToDataPath("configuration.xml");
    // Read configuration or create default
    ofxXmlSettings xml;
    if (!xml.loadFile(path)) {
        xml.setValue("configuration:Fullscreen", Fullscreen);
        xml.setValue("configuration:MinDistance", MinDistance);
        xml.setValue("configuration:MaxDistance", MaxDistance);
        xml.setValue("configuration:DeathZone", DeathZone);
        xml.setValue("configuration:RestartIntervalSeconds", RestartIntervalSeconds);
        xml.setValue("configuration:ActiveSerialPort", ActiveSerialPort);
        xml.setValue("configuration:StartingVolume", StartingVolume);
        xml.setValue("configuration:FinishingVolume", FinishingVolume);
        xml.setValue("configuration:StartingHeartBeatSpeed", StartingHeartBeatSpeed);
        xml.setValue("configuration:FinishingHeartBeatSpeed", FinishingHeartBeatSpeed);
        xml.setValue("configuration:FrameRate", FrameRate);
        xml.setValue("configuration:VideoFileName", VideoFileName);
        xml.setValue("configuration:IntroFileName", IntroFileName);
        xml.setValue("confoguration:DebugOverlay", DebugOverlay);
        
        if (!xml.saveFile(path)) {
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
        StartingVolume = xml.getValue("configuration:StartingVolume", StartingVolume);
        FinishingVolume = xml.getValue("configuration:FinishingVolume", FinishingVolume);
        StartingHeartBeatSpeed = xml.getValue("configuration:StartingHeartBeatSpeed", StartingHeartBeatSpeed);
        FinishingHeartBeatSpeed = xml.getValue("configuration:FinishingHeartBeatSpeed", FinishingHeartBeatSpeed);
        FrameRate = xml.getValue("configuration:FrameRate", FrameRate);
        VideoFileName = xml.getValue("configuration:VideoFileName", VideoFileName);
        IntroFileName = xml.getValue("configuration:IntroFileName", IntroFileName);
        DebugOverlay = xml.getValue("configuration:DebugOverlay", DebugOverlay);
    }
    
    return true;
}
