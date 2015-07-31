#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate( 60 );

    ofSetFullscreen(kFullscreen);

#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif

    // FIXME: draws all shapes with smooth edges.

    // Distance reading
    if (kUsePort) {
        // FIXME: serialPort = new Serial(this, "/dev/tty.usbmodem1411", 9600);
        // FIXME: serialPort.bufferUntil('\n'); // Trigger a SerialEvent on new line
    } else {
         // fake initial distance for simulation
        currentDistance = kMaxDistance;
    }
    previousDistanceChangeAt = ofGetElapsedTimeMillis();

    // HUD
    if (!f.loadFont("verdana.ttf", 16, true, true)) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);

    // Audio
    if (!backgroundSound.loadSound("1.mp3")) {
        std::cerr << "Error loading background sound" << std::endl;
        return;
    }
    backgroundSound.setLoop(true);

    if (!heartbeatSound.loadSound("2.mp3")) {
        std::cerr << "Error loading heartbeat sound" << std::endl;
        return;
    }
    heartbeatSound.setLoop(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    long now = ofGetElapsedTimeMillis();

    // Update visual
    long timePassed = now - previousFrameDrawnAt;
    int millis = ofMap(currentDistance, kMaxDistance, kMinDistance, 1000 / 6, 1000 / 30);
    fps = 1000 / millis;
    if (timePassed >= millis) {
        if (isAlive()) {
            if (destinationFrame == frame) {
                // User is not moving, attempt some random stuff
                randomMovement();
            }
        }

        // move towards destination
        if (destinationFrame > frame) {
            setFrame(frame + 1);
        } else if (destinationFrame < frame) {
            setFrame(frame -  1);
        }

        previousFrameDrawnAt = now;
    }
    
    ofSoundUpdate();
}

bool ofApp::isAlive() const {
    return currentDistance > kMinDistance;
}

ofTexture *ofApp::getImage(const int i) {
    if (images.end() == images.find(i)) {
        ofTexture img;
        if (!ofLoadImage(img, ofToString(i) + ".jpg")) {
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
    return ofMap(currentDistance, kMinDistance, kMaxDistance, kImageCount, 0);
}

void ofApp::setFrame(const int i) {
    frame = ofClamp(i, 0, kImageCount - 1);
}

void ofApp::setDestinationFrame(const int i) {
    destinationFrame = ofClamp(i, 0, kImageCount - 1);
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
    f.drawString("frame=" + ofToString(frame) + "/" + ofToString(kImageCount), 210, 40);
    f.drawString("destination=" + ofToString(destinationFrame), 410, 40);
    f.drawString("fps=" + ofToString(fps), 610, 40);

    // Draw the current animation frame
    ofTexture *img = getImage(frame);
    img->draw( 0, 60, ofGetWidth(), ofGetHeight() - 60 );
    
    // Update audio
    if (!backgroundSound.getIsPlaying()) {
        backgroundSound.play();
    }
    backgroundSound.setVolume(ofMap(currentDistance, kMaxDistance, kMinDistance, 0.2, 1.0));
    
    if (isAlive()) {
        if (!heartbeatSound.getIsPlaying()) {
            heartbeatSound.play();
        }
        heartbeatSound.setSpeed(ofMap(currentDistance, kMaxDistance, kMinDistance, 1.0, 2.0));
    } else {
        if (heartbeatSound.getIsPlaying()) {
            heartbeatSound.stop();
        }
    }
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
    currentDistance = ofClamp(value, kMinDistance, kMaxDistance);

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
