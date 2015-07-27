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
int destinationFrame = frame;

// HUD
ofTrueTypeFont f;

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
    
    assert(f.loadFont("verdana.ttf", 16, true, true));
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);
}

//--------------------------------------------------------------
void ofApp::update(){
    currentDistance = ofClamp(currentDistance, 0, maxDistance);
    
    calculateFrame();
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
    int result = imgcount - int(currentDistance / distanceOfFrame);
    if (result < 0) {
        result = 0;
    }
    if (result >= imgcount - 1) {
        result = imgcount - 1;
    }
    return result;
}

void ofApp::calculateFrame() {
        long now = ofGetElapsedTimef();
        
        long timePassed = now - previousMillis;
        
        destinationFrame = frameForDistance();
        
        if (timePassed > sleepMillis) {
            // move towards destination
            if (destinationFrame > frame) {
                frame = frame + 1;
            } else if (destinationFrame > frame) {
                frame = frame + 1;
            } else {
                // User is not moving, attempt some random stuff
                int r = int(ofRandom(4));
                if (r == 0) {
                    r = int(ofRandom(3));
                    if (0 == r) {
                        frame = frame + 1;
                    } else {
                        frame = frame - 1;
                    }
                }
            }
            
            previousMillis = now;
        }
        
        if (frame < 0) {
            frame = 0;
        } else if (frame >= imgcount) {
            frame = imgcount - 1;
        }
        
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
