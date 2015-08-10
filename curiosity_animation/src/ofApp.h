#pragma once

#include "ofMain.h"

class Configuration {
public:
    Configuration()
        : ImageCount(326)
        , Fullscreen(false)
        , MinDistance(0)
        , MaxDistance(1000)
        , DeathZone(50)
        , RestartIntervalSeconds(30)
        , ActiveSerialPort(0) {}
    
    int ImageCount;
    bool Fullscreen;
    int MinDistance;
    int MaxDistance;
    int DeathZone;
    int RestartIntervalSeconds;
    int ActiveSerialPort;
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

    long finishedAt = 0;
    
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
};
