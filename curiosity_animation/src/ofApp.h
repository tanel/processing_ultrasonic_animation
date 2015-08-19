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
        : ImageCount(326)
        , Fullscreen(false)
        , MinDistance(0)
        , MaxDistance(1000)
        , DeathZone(50)
        , RestartIntervalSeconds(30)
        , ActiveSerialPort(0)
        , StartingFramesPerSecond(6)
        , FinishingFramesPerSecond(100)
        , StartingVolume(0.2)
        , FinishingVolume(1.0)
        , StartingHeartBeatSpeed(1.0)
        , FinishingHeartBeatSpeed(2.0)
        , FrameRate(60) {}
    
    bool Read();
    
    int ImageCount;
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
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
    ofTexture *getImage(const int i);
    int frameForDistance() const;
    void calculateFrame();
    void setDistance(const std::string readson, const int value);
	void setFrame(const int i);
    void setDestinationFrame(const int i);
    void clearImage(const int i);

    // Current game state
    long finishedAt = 0;
    bool saveZoneActive = false;
    
    Configuration configuration;

    // Serial port, for reading distance from ultrasonic sensor.
    // Optional.
    ofSerial serialPort;
    std::stringstream serialbuf;

    // Images that make up the animation sequence
    std::map<int, ofTexture> images;

    // App state, you should not touch these;
    int currentDistance = 0;
    int previousDistance = currentDistance;
    long previousFrameDrawnAt = 0;
    long previousDistanceChangeAt = 0;
    int frame = 0;
    int destinationFrame = frame;

    // HUD
    ofTrueTypeFont f;
    int fps = 0;

    ofSoundPlayer backgroundSound;
    ofSoundPlayer heartbeatSound;
    
    ofFile eventLog;
    GameStats gameStats;
};
