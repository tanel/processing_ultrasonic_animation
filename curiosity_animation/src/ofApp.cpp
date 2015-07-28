#include "ofApp.h"

// Configuration
int imgcount = 326;
bool usePort = false;
bool fullscreen = false;
int minDistance = 0;
int maxDistance = 1000;
int sleepMillis = 100;

// Serial port, for reading distance from ultrasonic sensor.
// Optional.
ofSerial serialPort;

// Images that make up the animation sequence
std::map<int, ofImage> images;

// App state, you should not touch these;
int currentDistance = 0;
long previousMillis = 0;
int frame = 0;
float period = 1;
float distanceOfFrame = maxDistance / imgcount;
int loadTime = -1;
int destinationFrame = frame;

// HUD
ofTrueTypeFont f;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate( 60 );
    
    ofSetFullscreen(fullscreen);
    
    ofSetDataPathRoot("../Resources/data/");
    
    // FIXME: draws all shapes with smooth edges.
    
    if (usePort) {
        // FIXME: serialPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
        // FIXME: serialPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
    }
    
    if (!usePort) {
         // fake initial distance for simulation
        currentDistance = maxDistance;
    }
    
    assert(f.loadFont("verdana.ttf", 16, true, true));
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);
}

//--------------------------------------------------------------
void ofApp::update(){
    calculateFrame();
}

void ofApp::clearImage(const int i) {
    images.erase(images.find(i));
}

ofImage *ofApp::getImage(const int i) {
    if (images.end() == images.find(i)) {
        ofImage img;
        assert(img.loadImage(ofToString(i) + ".jpg"));
        images[i] = img;
    }
    return &images[i];
}

// Frame for current distance
// Note that this is not the actual frame that will be animated.
// Instead will start to animate towards this frame.
int ofApp::frameForDistance() const {
    int i = int(currentDistance / distanceOfFrame);
    int result = imgcount - i;
    return ofClamp(result, 0, imgcount - 1);
}

void ofApp::setFrame(const int i) {
    frame = ofClamp(i, 0, imgcount - 1);
}

void ofApp::setDestinationFrame(const int i) {
    destinationFrame = ofClamp(i, 0, imgcount - 1);
}

void ofApp::calculateFrame() {
    long now = ofGetElapsedTimeMillis();

    long timePassed = now - previousMillis;

    if (timePassed < sleepMillis) {
        return;
    }

    if (destinationFrame == frame) {
        // User is not moving, attempt some random stuff
        switch (int(ofRandom(10))) {
            case 0:
                setDestinationFrame(frameForDistance() + int(ofRandom(10)));
                break;
            case 1:
                setDestinationFrame(frameForDistance() - int(ofRandom(10)));
                break;
            }
    }

    // move towards destination
    if (destinationFrame > frame) {
        setFrame(frame + 1);
    } else if (destinationFrame < frame) {
        setFrame(frame -  1);
    }

    previousMillis = now;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Update HUD
    f.drawString("distance=" + ofToString(currentDistance), 10, 40);
    f.drawString("max=" + ofToString(maxDistance), 210, 40);
    f.drawString("framelen=" + ofToString(distanceOfFrame), 410, 40);
    f.drawString("frame=" + ofToString(frame) + "/" + ofToString(imgcount), 610, 40);
    f.drawString("destination=" + ofToString(destinationFrame), 810, 40);

    // Draw the current animation frame
    ofImage *img = getImage(frame);
    assert(img);
    img->draw( 0, 60, ofGetWidth(), ofGetHeight() - 60 );
    clearImage(frame);
}

#define UP 357
#define DOWN 359

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    cout << "keyPressed key=" << key << std::endl;
    
    if (UP == key) {
        // distance decreases as viewer approaches
        setDistance(currentDistance - 10 - int(ofRandom(50)));
    } else if (DOWN == key) {
        // distance incrases as viewer steps back
        setDistance(currentDistance + 10 + int(ofRandom(50)));
    }
}

void ofApp::setDistance(const int value) {
    currentDistance = ofClamp(value, minDistance, maxDistance);
    setDestinationFrame(frameForDistance());
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
