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

class GameState {
    
public:
    GameState()
    : finishedAt(0)
    , savedAt(0)
    , killedAt(0)
    , saveZoneActive(false)
    , saved(false)
    , minDistance(0)
    , startedAt(0)
    , currentDistance(0)
    , previousDistance(currentDistance)
    , previousFrameDrawnAt(0)
    , previousDistanceChangeAt(0)
    , destinationFrame(0)
    , frameAtLastAnimationStart(0)
    , fps(0) {}
    
    // Current game state
    long finishedAt;
    long savedAt;
    long killedAt;
    bool saveZoneActive;
    bool saved;
    int minDistance;
    long startedAt;
    
    std::string serialInput;
    
    // App state, you should not touch these;
    int currentDistance;
    int previousDistance;
    long previousFrameDrawnAt;
    long previousDistanceChangeAt;
    int destinationFrame;
    int frameAtLastAnimationStart;
    int fps;
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
    void startGame();
    void updateAudio();
    void saveGame(const std::string reason);
    void calculateFPS();
    void killGame();
    void readSerial();
    void determineVideoState();

    Configuration configuration;
    
    GameState state;
    
    std::stringstream serialbuf;
    
    // Serial port, for reading distance from ultrasonic sensor.
    // Optional.
    ofSerial serialPort;
    
    // HUD
    ofTrueTypeFont f;
    
    ofSoundPlayer heartbeatSound;
    
    ofVideoPlayer videoPlayer;
    
    ofFile eventLog;
    GameStats gameStats;
    
    // Intro and outro pics
    ofImage intro;
    ofImage outro;
};
