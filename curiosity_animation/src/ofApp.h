#pragma once

#include "ofMain.h"

#include "Poco/String.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/URI.h"

#include "configuration.hpp"
#include "game_result.hpp"
#include "game_state.hpp"
#include "game_stats.hpp"
#include "serial_reader.hpp"

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
