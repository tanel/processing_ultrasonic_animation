#include "ofApp.h"
#include "ofxXmlSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Read configuration or create default
    ofxXmlSettings xml;
    if (!xml.loadFile("configuration.xml")) {
        xml.setValue("configuration:ImageCount", configuration.ImageCount);
        xml.setValue("configuration:Fullscreen", configuration.Fullscreen);
        xml.setValue("configuration:MinDistance", configuration.MinDistance);
        xml.setValue("configuration:MaxDistance", configuration.MaxDistance);
        xml.setValue("configuration:DeathZone", configuration.DeathZone);
        xml.setValue("configuration:RestartIntervalSeconds", configuration.RestartIntervalSeconds);
        xml.setValue("configuration:ActiveSerialPort", configuration.ActiveSerialPort);
        xml.setValue("configuration:StartingFramesPerSecond", configuration.StartingFramesPerSecond);
        xml.setValue("configuration:FinishingFramesPerSecond", configuration.FinishingFramesPerSecond);
        xml.setValue("configuration:StartingVolume", configuration.StartingVolume);
        xml.setValue("configuration:FinishingVolume", configuration.FinishingVolume);
        xml.setValue("configuration:StartingHeartBeatSpeed", configuration.StartingHeartBeatSpeed);
        xml.setValue("configuration:FinishingHeartBeatSpeed", configuration.FinishingHeartBeatSpeed);

        if (!xml.saveFile("configuration.xml")) {
            std::cerr << "Error saving configuration file" << std::endl;
        }
    } else {
        configuration.ImageCount = xml.getValue("configuration:ImageCount", configuration.ImageCount);
        configuration.Fullscreen = xml.getValue("configuration:Fullscreen", configuration.Fullscreen);
        configuration.MinDistance = xml.getValue("configuration:MinDistance", configuration.MinDistance);
        configuration.MaxDistance = xml.getValue("configuration:MaxDistance", configuration.MaxDistance);
        configuration.DeathZone = xml.getValue("configuration:DeathZone", configuration.DeathZone);
        configuration.RestartIntervalSeconds = xml.getValue("configuration:RestartIntervalSeconds", configuration.RestartIntervalSeconds);
        configuration.ActiveSerialPort = xml.getValue("configuration:ActiveSerialPort", configuration.ActiveSerialPort);
        configuration.StartingFramesPerSecond = xml.getValue("configuration:StartingFramesPerSecond", configuration.StartingFramesPerSecond);
        configuration.FinishingFramesPerSecond = xml.getValue("configuration:FinishingFramesPerSecond", configuration.FinishingFramesPerSecond);
        configuration.StartingVolume = xml.getValue("configuration:StartingVolume", configuration.StartingVolume);
        configuration.FinishingVolume = xml.getValue("configuration:FinishingVolume", configuration.FinishingVolume);
        configuration.StartingHeartBeatSpeed = xml.getValue("configuration:StartingHeartBeatSpeed", configuration.StartingHeartBeatSpeed);
        configuration.FinishingHeartBeatSpeed = xml.getValue("configuration:FinishingHeartBeatSpeed", configuration.FinishingHeartBeatSpeed);
    }
    
    ofSetFrameRate( 60 );

    ofSetFullscreen(configuration.Fullscreen);

#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif

    // Distance reading
    serialPort.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serialPort.getDeviceList();
    std::cout << "Available serial devices:" << std::endl;
    for (int i = 0; i < deviceList.size(); i++) {
        std::cout << i << ") " << deviceList[i].getDeviceName() << std::endl;
    }
    if (configuration.ActiveSerialPort < deviceList.size()) {
        if (!serialPort.setup(configuration.ActiveSerialPort, 9600)) {
            std::cerr << "Failed to connect to serial device! "
                << deviceList[configuration.ActiveSerialPort].getDeviceName() << std::endl;
        }
    }

    currentDistance = configuration.MaxDistance;

    previousDistanceChangeAt = ofGetElapsedTimeMillis();

    // HUD
    if (!f.loadFont("verdana.ttf", 16, true, true)) {
        std::cerr << "Error loading font" << std::endl;
    }
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);

    // Audio
    if (!backgroundSound.loadSound("1.mp3")) {
        std::cerr << "Error loading background sound" << std::endl;
    }
    backgroundSound.setLoop(true);

    if (!heartbeatSound.loadSound("2.mp3")) {
        std::cerr << "Error loading heartbeat sound" << std::endl;
    }
    heartbeatSound.setLoop(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    long now = ofGetElapsedTimeMillis();

    // Determine if user is now in the death zone
    if (!finishedAt && (currentDistance < configuration.MinDistance + configuration.DeathZone)) {
        finishedAt = now;
    }

    // Restart if needed
    if (finishedAt && (finishedAt < now - (configuration.RestartIntervalSeconds*1000))) {
        finishedAt = 0;
        setFrame(0);
        setDistance("restart", configuration.MaxDistance);
    }

    // Read serial
    if (serialPort.isInitialized()) {
        if (serialPort.available()) {
            char c = serialPort.readByte();
            if ('\n' == c) {
                std::string input = serialbuf.str();
                serialbuf.str("");
                std::cout << "Serial input: " << input << std::endl;
            } else {
                serialbuf << c;
            }
        }
    }

    // Update visual
    long timePassed = now - previousFrameDrawnAt;
    int millis = ofMap(currentDistance,
                       configuration.MaxDistance, configuration.MinDistance,
                       1000 / configuration.StartingFramesPerSecond, 1000 / configuration.FinishingFramesPerSecond);
    fps = 1000 / millis;
    if (timePassed >= millis) {
        // move towards destination
        if (destinationFrame > frame) {
            setFrame(frame + 1);
        } else if (destinationFrame < frame) {
            setFrame(frame -  1);
        }

        previousFrameDrawnAt = now;
    }

    // Update audio
    if (!backgroundSound.getIsPlaying()) {
        backgroundSound.play();
    }
    
    backgroundSound.setVolume(ofMap(currentDistance,
                                    configuration.MaxDistance, configuration.MinDistance,
                                    configuration.StartingVolume, configuration.FinishingVolume));

    if (!finishedAt) {
        if (!heartbeatSound.getIsPlaying()) {
            heartbeatSound.play();
        }
        heartbeatSound.setSpeed(ofMap(currentDistance,
                                      configuration.MaxDistance, configuration.MinDistance,
                                      configuration.StartingHeartBeatSpeed, configuration.FinishingHeartBeatSpeed));
    } else {
        if (heartbeatSound.getIsPlaying()) {
            heartbeatSound.stop();
        }
    }

    ofSoundUpdate();
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
    return ofMap(currentDistance, configuration.MinDistance, configuration.MaxDistance, configuration.ImageCount, 0);
}

void ofApp::setFrame(const int i) {
    frame = ofClamp(i, 0, configuration.ImageCount - 1);
}

void ofApp::setDestinationFrame(const int i) {
    destinationFrame = ofClamp(i, 0, configuration.ImageCount - 1);
}

void ofApp::clearImage(const int i) {
    images.erase(images.find(i));
}


//--------------------------------------------------------------
void ofApp::draw(){
    // Update HUD
    f.drawString("distance=" + ofToString(currentDistance), 10, 40);
    f.drawString("frame=" + ofToString(frame) + "/" + ofToString(configuration.ImageCount), 210, 40);
    f.drawString("destination=" + ofToString(destinationFrame), 410, 40);
    f.drawString("fps=" + ofToString(fps), 610, 40);
    f.drawString("finished=" + ofToString(finishedAt), 810, 40);

    // Draw the current animation frame
    ofTexture *img = getImage(frame);
    img->draw( 0, 60, ofGetWidth(), ofGetHeight() - 60 );
    clearImage(frame);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    cout << "keyPressed key=" << key << std::endl;

    if (finishedAt) {
        return;
    }

    if (OF_KEY_UP == key) {
        // distance decreases as viewer approaches
        setDistance("keyboard up", currentDistance - 50 - int(ofRandom(100)));
    } else if (OF_KEY_DOWN == key) {
        // distance incrases as viewer steps back
        setDistance("keyboar down", currentDistance + 50 + int(ofRandom(100)));
    }
}

void ofApp::setDistance(const std::string reason, const int value) {
    currentDistance = ofClamp(value, configuration.MinDistance, configuration.MaxDistance);

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
