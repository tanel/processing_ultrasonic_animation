#pragma once

#include "ofMain.h"

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
    ofImage *getImage(const int i);
    void clearImage(const int i);
    int frameForDistance() const;
    void calculateFrame();
    void setDistance(const std::string readson, const int value);
	void setFrame(const int i);
    void setDestinationFrame(const int i);
    void randomMovement();

};
