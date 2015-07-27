#include "ofApp.h"

// Configuration
int imgcount = 326;
bool usePort = false;
bool fullscreen = false;
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

//--------------------------------------------------------------
void ofApp::setup(){
    //Set screen frame rate
    ofSetFrameRate( 60 );
    
    ofSetFullscreen(fullscreen);
    
    ofSetDataPathRoot("../Resources/data/");
    
    // FIXME: draws all shapes with smooth edges.
    
    if (usePort) {
        // FIXME: serialPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
        // FIXME: serialPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
    }
    
    if (!usePort) {
        currentDistance = maxDistance; // fake initial distance for simulation
    }
    
    // FIXME: f = createFont("Arial",16,true); // Arial, 16 point, anti-aliasing on
}

//--------------------------------------------------------------
void ofApp::update(){
    currentDistance = ofClamp(currentDistance, 0, maxDistance);
}

ofImage *ofApp::getImage(const int i) {
    if (images.end() == images.find(i)) {
        ofImage img;
        assert(img.loadImage(ofToString(i) + ".jpg"));
        images[i] = img;
    }
    return &images[i];
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw the current animation frame
    ofImage *img = getImage(frame);
    assert(img);
    img->draw( 100, 50 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
