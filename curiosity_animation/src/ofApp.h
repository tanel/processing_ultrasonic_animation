#pragma once

#include "ofMain.h"

#include "Poco/String.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/URI.h"

const int kNumOfValues = 5;

class SerialReader : public ofThread {
public:
    SerialReader()
    : activeSerialPort(0) {}
    
    void threadedFunction();
    
    void AddReading(const int value);
    
    int reading() const;
    
    int activeSerialPort;
    
private:
    // Serial port, for reading distance from ultrasonic sensor.
    ofSerial serialPort;
    std::stringstream serialbuf;
    
    std::deque<int> values;
};

class GameResult {
public:
    GameResult()
    : Saves(0)
    , Kills(0) {}
    
    int Saves;
    int Kills;
};

class GameStats {
public:
    GameStats() {}
    
    void Read();
    
    void AddKill();
    void AddSave();
    
    int TotalSaves() const {
        return total.Saves;
    }
    
    int TotalKills() const {
        return total.Kills;
    }
    
private:
    GameResult total;
    std::map<std::string, GameResult> history;
    
    void write() const;
    
    static std::string currentDate();
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

const std::string kStateWaiting = "waiting";
const std::string kStateStarted = "started";
const std::string kStateSaved = "saved";
const std::string kStateKilled = "killed";
const std::string kStateStats = "stats";

class GameState {
public:
    GameState()
    : name(kStateWaiting)
    , saveZoneActive(false)
    , finishedAt(0)
    , gameWasSaved(false)
    , lastUserInputAt(0)
    , fps(0) {}
    
    // Current game state
    std::string name;
    bool saveZoneActive;
    long finishedAt;
    bool gameWasSaved;
    long lastUserInputAt;
    
    // App state, you should not touch these;
    int fps;
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void exit();
    void update();
    void draw();
    void keyPressed(int key);
    
private:
    int frameForDistance(const int distance) const;
    bool loadVideo();
    void animateVideo(const int direction);
    bool isPlaying();
    void restartGame();
    void startGame();
    void updateAudio(const int distance);
    void updateVideo(const int distance);
    void saveGame(const std::string reason);
    void calculateFPS(const int distance);
    void killGame();
    
    int saveZoneStartsAt() const {
        return std::abs(configuration.MaxDistance - configuration.DeathZone);
    }
    
    int distanceMoved(const int distance) const {
        return std::abs(configuration.MaxDistance - distance);
    }
    
    Configuration configuration;
    
    GameState state;
    
    SerialReader serialReader;
    
    // HUD
    ofTrueTypeFont f;
    
    ofSoundPlayer heartbeatSound;
    
    ofVideoPlayer videoPlayer;
    
    // cached from videoplayer
    int totalNumOfFrames;
    
    ofFile eventLog;
    GameStats gameStats;
    
    // Intro and outro pics
    ofImage intro;
    ofImage outro;
};
