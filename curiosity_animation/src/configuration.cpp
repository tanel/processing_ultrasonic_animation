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
        return false;
    }
    Fullscreen = xml.getValue("configuration:Fullscreen", Fullscreen);
    MinDistance = xml.getValue("configuration:MinDistance", MinDistance);
    MaxDistance = xml.getValue("configuration:MaxDistance", MaxDistance);
    DeathZone = xml.getValue("configuration:DeathZone", DeathZone);
    SaveZone = xml.getValue("configuration:SaveZone", SaveZone);
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
    AutoSaveSeconds = xml.getValue("configuration:AutoSaveSeconds", AutoSaveSeconds);
    KillVideoFileName = xml.getValue("configuration:KillVideoFileName", KillVideoFileName);
    WaitingVolume = xml.getValue("configuration:WaitingVolume", WaitingVolume);
    SaveActivateSeconds = xml.getValue("configuration:SaveActivateSeconds", SaveActivateSeconds);
    UploadCommand = xml.getValue("configuration:UploadCommand", UploadCommand);
    return true;
}
