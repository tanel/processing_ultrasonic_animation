#pragma once

#include "ofMain.h"

class GameStats {
public:
    GameStats()
    : Saves(0)
    , Kills(0) {}
    
    bool Read();
    bool Write() const;
    
    int Saves;
    int Kills;
};

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
    , AutoSaveSeconds(60) {}
    
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
};

class GameState {
    
public:
    GameState() {}
    
    // Current game state
    long finishedAt = 0;
    bool saveZoneActive = false;
    bool saved = false;
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
private:
    int frameForDistance();
    void setDistance(const std::string reason, const int value);
    bool loadVideo();
    void animateVideo(const int direction);
    bool isPlaying();
    bool isAccepingInput();
    void restartGame();
    void updateAudio();
    void saveGame();
    void calculateFPS();
    void killGame();
    void readSerial();
    void determineVideoState();

    Configuration configuration;
    
    GameState state;
    
    // Serial port, for reading distance from ultrasonic sensor.
    // Optional.
    ofSerial serialPort;
    std::stringstream serialbuf;
    
    // App state, you should not touch these;
    int currentDistance = 0;
    int previousDistance = currentDistance;
    long previousFrameDrawnAt = 0;
    long previousDistanceChangeAt = 0;
    int destinationFrame = 0;
    int frameAtLastAnimationStart = 0;
    
    // HUD
    ofTrueTypeFont f;
    int fps = 0;
    
    ofSoundPlayer heartbeatSound;
    
    ofVideoPlayer videoPlayer;
    
    ofFile eventLog;
    GameStats gameStats;
};
