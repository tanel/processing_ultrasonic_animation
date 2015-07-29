#include "ofApp.h"

// Configuration
const int imgcount = 326;
const bool usePort = false;
const bool fullscreen = false;
const int minDistance = 0;
const int maxDistance = 1000;

// Serial port, for reading distance from ultrasonic sensor.
// Optional.
ofSerial serialPort;

// Images that make up the animation sequence
std::map<int, ofImage> images;

// App state, you should not touch these;
int currentDistance = 0;
int previousDistance = currentDistance;
long previousFrameDrawnAt = 0;
long previousDistanceChangeAt = 0;
int frame = 0;
int destinationFrame = frame;
int animationDurationMillis = 100;

// HUD
ofTrueTypeFont f;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate( 60 );

    ofSetFullscreen(fullscreen);

#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif

    // FIXME: draws all shapes with smooth edges.

    if (usePort) {
        // FIXME: serialPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
        // FIXME: serialPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
    }

    if (!usePort) {
         // fake initial distance for simulation
        currentDistance = maxDistance;
    }

    if (!f.loadFont("verdana.ttf", 16, true, true)) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);

    previousDistanceChangeAt = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){
    long now = ofGetElapsedTimeMillis();

    long timePassed = now - previousFrameDrawnAt;

    if (timePassed < animationDurationMillis) {
        return;
    }

    if (destinationFrame == frame) {
        // User is not moving, attempt some random stuff
        randomMovement();
    }

    // move towards destination
    if (destinationFrame > frame) {
        setFrame(frame + 1);
    } else if (destinationFrame < frame) {
        setFrame(frame -  1);
    }

    previousFrameDrawnAt = now;
}

void ofApp::clearImage(const int i) {
    images.erase(images.find(i));
}

ofImage *ofApp::getImage(const int i) {
    if (images.end() == images.find(i)) {
        ofImage img;
        if (!img.loadImage(ofToString(i) + ".jpg")) {
            std::cerr << "Error loading image" << std::endl;
            return 0;
        }
        images[i] = img;
    }
    return &images[i];
}

// Frame for current distance
// Note that this is not the actual frame that will be animated.
// Instead will start to animate towards this frame.
int ofApp::frameForDistance() const {
    return ofMap(currentDistance, 0, maxDistance, imgcount, 0);
}

void ofApp::setFrame(const int i) {
    frame = ofClamp(i, 0, imgcount - 1);
}

void ofApp::setDestinationFrame(const int i) {
    destinationFrame = ofClamp(i, 0, imgcount - 1);
}

void ofApp::randomMovement() {
    switch (int(ofRandom(10))) {
        case 0:
            setDestinationFrame(frameForDistance() + int(ofRandom(10)));
            break;
        case 1:
            setDestinationFrame(frameForDistance() - int(ofRandom(10)));
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Update HUD
    f.drawString("distance=" + ofToString(currentDistance), 10, 40);
    f.drawString("frame=" + ofToString(frame) + "/" + ofToString(imgcount), 210, 40);
    f.drawString("destination=" + ofToString(destinationFrame), 410, 40);

    // Draw the current animation frame
    ofImage *img = getImage(frame);
    img->draw( 0, 60, ofGetWidth(), ofGetHeight() - 60 );
    clearImage(frame);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    cout << "keyPressed key=" << key << std::endl;

    if (OF_KEY_UP == key) {
        // distance decreases as viewer approaches
        setDistance("keyboard up", currentDistance - 10 - int(ofRandom(50)));
    } else if (OF_KEY_DOWN == key) {
        // distance incrases as viewer steps back
        setDistance("keyboar down", currentDistance + 10 + int(ofRandom(50)));
    }
}

void ofApp::setDistance(const std::string reason, const int value) {
    currentDistance = ofClamp(value, minDistance, maxDistance);

    // Start animating towards this new distance
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
