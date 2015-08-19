#include "ofApp.h"
#include "ofxXmlSettings.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // start logging
    ofLogToFile("app.log");
    
    // start recoding events
    if (!eventLog.open(ofToDataPath("events.txt"), ofFile::WriteOnly)) {
        ofLogError() << "Error opening events.txt - !!!DATA WILL BE LOST!!!";
    }

    if (!configuration.Read()) {
        ofLogError() << "Error reading configuration";
    }
    
    if (!gameStats.Read()) {
        ofLogError() << "Error reading game stats.";
    }
    
    ofSetFrameRate(configuration.FrameRate);

    ofSetFullscreen(configuration.Fullscreen);

#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif
    
    // Distance reading
    serialPort.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serialPort.getDeviceList();
    if (configuration.ActiveSerialPort < deviceList.size()) {
        if (!serialPort.setup(configuration.ActiveSerialPort, 9600)) {
            ofLogError() << "Failed to connect to serial device! "
                << deviceList[configuration.ActiveSerialPort].getDeviceName();
        }
    }

    currentDistance = configuration.MaxDistance;

    previousDistanceChangeAt = ofGetElapsedTimeMillis();

    // HUD
    if (!f.loadFont("verdana.ttf", 16, true, true)) {
        ofLogError() << "Error loading font";
    }
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);

    // Audio
    if (!backgroundSound.loadSound("1.mp3")) {
        ofLogError() << "Error loading background sound";
    }
    backgroundSound.setLoop(true);

    if (!heartbeatSound.loadSound("2.mp3")) {
        ofLogError() << "Error loading heartbeat sound";
    }
    heartbeatSound.setLoop(true);
}

bool Configuration::Read() {
    // Read configuration or create default
    ofxXmlSettings xml;
    if (!xml.loadFile("configuration.xml")) {
        xml.setValue("configuration:ImageCount", ImageCount);
        xml.setValue("configuration:Fullscreen", Fullscreen);
        xml.setValue("configuration:MinDistance", MinDistance);
        xml.setValue("configuration:MaxDistance", MaxDistance);
        xml.setValue("configuration:DeathZone", DeathZone);
        xml.setValue("configuration:RestartIntervalSeconds", RestartIntervalSeconds);
        xml.setValue("configuration:ActiveSerialPort", ActiveSerialPort);
        xml.setValue("configuration:StartingFramesPerSecond", StartingFramesPerSecond);
        xml.setValue("configuration:FinishingFramesPerSecond", FinishingFramesPerSecond);
        xml.setValue("configuration:StartingVolume", StartingVolume);
        xml.setValue("configuration:FinishingVolume", FinishingVolume);
        xml.setValue("configuration:StartingHeartBeatSpeed", StartingHeartBeatSpeed);
        xml.setValue("configuration:FinishingHeartBeatSpeed", FinishingHeartBeatSpeed);
        xml.setValue("configuration:FrameRate", FrameRate);
        
        if (!xml.saveFile("configuration.xml")) {
            ofLogError() << "Error saving configuration file";
            return false;
        }
    } else {
        ImageCount = xml.getValue("configuration:ImageCount", ImageCount);
        Fullscreen = xml.getValue("configuration:Fullscreen", Fullscreen);
        MinDistance = xml.getValue("configuration:MinDistance", MinDistance);
        MaxDistance = xml.getValue("configuration:MaxDistance", MaxDistance);
        DeathZone = xml.getValue("configuration:DeathZone", DeathZone);
        RestartIntervalSeconds = xml.getValue("configuration:RestartIntervalSeconds", RestartIntervalSeconds);
        ActiveSerialPort = xml.getValue("configuration:ActiveSerialPort", ActiveSerialPort);
        StartingFramesPerSecond = xml.getValue("configuration:StartingFramesPerSecond", StartingFramesPerSecond);
        FinishingFramesPerSecond = xml.getValue("configuration:FinishingFramesPerSecond", FinishingFramesPerSecond);
        StartingVolume = xml.getValue("configuration:StartingVolume", StartingVolume);
        FinishingVolume = xml.getValue("configuration:FinishingVolume", FinishingVolume);
        StartingHeartBeatSpeed = xml.getValue("configuration:StartingHeartBeatSpeed", StartingHeartBeatSpeed);
        FinishingHeartBeatSpeed = xml.getValue("configuration:FinishingHeartBeatSpeed", FinishingHeartBeatSpeed);
        FrameRate = xml.getValue("configuration:FrameRate", FrameRate);
    }
    
    return true;
}

bool GameStats::Read() {
    // Read game stats or create default
    ofxXmlSettings xml;
    if (!xml.loadFile("gamestats.xml")) {
        if (!Write()) {
            return false;
        }
    } else {
        Saves = xml.getValue("gamestats:Saves", Saves);
        Kills = xml.getValue("gamestats:Kills", Kills);
    }
    
    return true;
}

bool GameStats::Write() const {
    ofFile f(ofToDataPath("gamestats.xml"));
    f.moveTo("gamestats_backup.xml");

    ofxXmlSettings xml;
    xml.setValue("gamestats:Saves", Saves);
    xml.setValue("gamestats:Kills", Kills);
    return xml.saveFile("gamestats.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    long now = ofGetElapsedTimeMillis();

    // Determine if user is now in the death zone
    if (!finishedAt && (currentDistance < configuration.MinDistance + configuration.DeathZone)) {
        finishedAt = now;
        ofLogNotice() << "Game finished with kill at " << now;
        eventLog << "killed=" << now << std::endl;
        gameStats.Kills++;
        if (!gameStats.Write()) {
            ofLogError() << "Error writing game stats";
        }
    }
    
    // If save zone is active and user finds itself in it,
    // then declare the game saved and finish it.
    int saveZoneStartsAt = std::abs(configuration.MaxDistance - configuration.DeathZone);
    if (!finishedAt && saveZoneActive && currentDistance > saveZoneStartsAt) {
        finishedAt = now;
        ofLogNotice() << "Game finished with save at " << now;
        eventLog << "saved=" << now << std::endl;
        gameStats.Saves++;
        if (!gameStats.Write()) {
            ofLogError() << "Error writing game stats";
        }
    }

    // If user has moved out of save zone, and game is not finished
    // yet, activate save zone
    int moved = std::abs(configuration.MaxDistance - currentDistance);
    if (!finishedAt && moved > configuration.DeathZone) {
        saveZoneActive = true;
    }

    // Restart if needed
    if (finishedAt && (finishedAt < now - (configuration.RestartIntervalSeconds*1000))) {
        finishedAt = 0;
        saveZoneActive = false;
        setFrame(0);
        setDistance("restart", configuration.MaxDistance);
        ofLogNotice() << "Game restarted";
        eventLog << "started=" << now << std::endl;
    }

    // Read serial
    if (serialPort.isInitialized()) {
        if (serialPort.available()) {
            char c = serialPort.readByte();
            if ('\n' == c) {
                std::string input = serialbuf.str();
                serialbuf.str("");
                ofLogNotice() << "Serial input: " << input << std::endl;
                float f = ofToFloat(input);
                setDistance("Serial input", f);
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
        // Calculate step. When the distance is decreasing
        // fast, we want the animation to be more responsive.
        // So we step more frames if the
        int step = std::abs(frame - destinationFrame);
        if (step) {
            if (step > 1) {
                step = std::sqrt(step);
            }
            if (step < 1) {
                step = 1;
            }
        }

        // move towards destination
        if (destinationFrame > frame) {
            setFrame(frame + step);
        } else if (destinationFrame < frame) {
            setFrame(frame - step);
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
            ofLogError() << "Error loading image";
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
    int y = 40;
    f.drawString("dist=" + ofToString(currentDistance), 10, y);
    f.drawString("f=" + ofToString(frame) + "/" + ofToString(configuration.ImageCount), 160, y);
    f.drawString("dest.f=" + ofToString(destinationFrame), 310, y);
    f.drawString("fps=" + ofToString(fps), 460, y);
    f.drawString("saves=" + ofToString(gameStats.Saves), 610, y);
    f.drawString("kills=" + ofToString(gameStats.Kills), 760, y);

    // Draw the current animation frame
    ofTexture *img = getImage(frame);
    img->draw( 0, 60, ofGetWidth(), ofGetHeight() - 60 );
    clearImage(frame);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofLogNotice() << "keyPressed key=" << key;

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
