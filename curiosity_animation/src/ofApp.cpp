#include "ofApp.h"
#include "ofxXmlSettings.h"

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
    
    setDistance("initialize", configuration.MaxDistance);
    
    // HUD
    if (!f.loadFont("verdana.ttf", 16, true, true)) {
        ofLogError() << "Error loading font";
    }
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);
    
    // Audio
    if (!heartbeatSound.loadSound("2.mp3")) {
        ofLogError() << "Error loading heartbeat sound";
    }
    heartbeatSound.setLoop(true);
    
    // Video
    if (!loadVideo()) {
        ofLogError() << "Error loading video";
    }
    
    // Intro and outro pics
    if (!intro.loadImage(ofToDataPath(configuration.IntroFileName))) {
        ofLogError() << "Error loading intro";
    }
    if (!outro.loadImage(ofToDataPath(configuration.OutroFileName))) {
        ofLogError() << "Error loading outro";
    }
}

const int kForward = 1;
const int kBack = -1;

void ofApp::animateVideo(const int direction) {
    if (direction != kForward && direction != kBack) {
        ofLogError() << "Invalid direction " << direction;
        return;
    }
    if (videoPlayer.getSpeed() != direction) {
        videoPlayer.setPaused(true);
        videoPlayer.setSpeed(direction);
    }
    
    if (!isPlaying()) {
        frameAtLastAnimationStart = videoPlayer.getCurrentFrame();
        videoPlayer.play();
    }

    videoPlayer.update();
}

bool ofApp::isPlaying() {
    return videoPlayer.isPlaying() && !videoPlayer.isPaused();
}

bool ofApp::loadVideo() {
    videoPlayer = ofVideoPlayer();
    if (!videoPlayer.loadMovie(ofToDataPath(configuration.VideoFileName))) {
        return false;
    }
    videoPlayer.setLoopState(OF_LOOP_NONE);
    return true;
}

bool Configuration::Read() {
    // Read configuration or create default
    ofxXmlSettings xml;
    if (!xml.loadFile("configuration.xml")) {
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
        xml.setValue("configuration:VideoFileName", VideoFileName);
        xml.setValue("configuration:CheckAfterNFrames", CheckAfterNFrames);
        xml.setValue("configuration:AutoSaveSeconds", AutoSaveSeconds);
        xml.setValue("configuration:IntroFileName", IntroFileName);
        xml.setValue("configuration:OutroFileName", OutroFileName);

        if (!xml.saveFile("configuration.xml")) {
            ofLogError() << "Error saving configuration file";
            return false;
        }
    } else {
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
        VideoFileName = xml.getValue("configuration:VideoFileName", VideoFileName);
        CheckAfterNFrames = xml.getValue("configuration:CheckAfterNFrames", CheckAfterNFrames);
        AutoSaveSeconds = xml.getValue("configuration:AutoSaveSeconds", AutoSaveSeconds);
        IntroFileName = xml.getValue("configuration:IntroFileName", IntroFileName);
        OutroFileName = xml.getValue("configuration:OutroFileName", OutroFileName);
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
    std::string path = ofToDataPath("gamestats.xml");
    if (ofFile::doesFileExist(path)) {
        ofFile::removeFile(path);
    }
    ofxXmlSettings xml;
    xml.setValue("gamestats:Saves", Saves);
    xml.setValue("gamestats:Kills", Kills);
    return xml.saveFile(path);
}

void ofApp::determineVideoState() {
    if (isPlaying()) {
        if (videoPlayer.getSpeed() == kForward) {
            if (videoPlayer.getCurrentFrame() >= frameForDistance()) {
                videoPlayer.setPaused(true);
            }
        } else if (videoPlayer.getSpeed() == kBack) {
            if (videoPlayer.getCurrentFrame() <= frameForDistance()) {
                videoPlayer.setPaused(true);
            }
        }
    }
}

void ofApp::update(){
    long now = ofGetElapsedTimeMillis();
    
    determineVideoState();

    // Determine if user is now in the death zone
    if (!state.finishedAt && (currentDistance < configuration.MinDistance + configuration.DeathZone)) {
        killGame();
    }

    // If save zone is active and user finds itself in it,
    // then declare the game saved and finish it.
    int saveZoneStartsAt = std::abs(configuration.MaxDistance - configuration.DeathZone);
    if (!state.finishedAt && state.saveZoneActive && currentDistance > saveZoneStartsAt) {
        saveGame("user walked into save zone");
    }
    
    // If user has moved out of save zone, and game is not finished
    // yet, activate save zone
    int moved = std::abs(configuration.MaxDistance - currentDistance);
    if (!state.finishedAt && moved > configuration.DeathZone * 2) {
        state.saveZoneActive = true;
    }

    // Restart if needed
    if (state.finishedAt && (state.finishedAt < now - (configuration.RestartIntervalSeconds*1000))) {
        restartGame();
    }
    
    // If nothing has happened for a while, save game automatically
    if (!state.finishedAt) {
        if (previousDistanceChangeAt < now - (configuration.AutoSaveSeconds*1000) && state.minDistance && state.minDistance < configuration.MaxDistance) {
            saveGame("automatically saved because of no user action");
        }
    }

    readSerial();

    calculateFPS();

    videoPlayer.update();

    updateAudio();
}

void ofApp::readSerial() {
    if (!isAccepingInput()) {
        return;
    }
    if (!serialPort.isInitialized()) {
        return;
    }
    if (!serialPort.available()) {
        return;
    }
    char c = serialPort.readByte();
    if ('\n' != c) {
        serialbuf << c;
        return;
    }
    std::string input = serialbuf.str();
    serialbuf.str("");
            
    float f = ofToFloat(input);
    
    setDistance("Serial input", f);

    if (previousDistance > currentDistance) {
        animateVideo(kForward);

        ofLogNotice() << "Serial input: " << input << " f: " << f << " moving: forward prev: " << previousDistance
                    << " current distance: " << currentDistance;
    }
    if (previousDistance < currentDistance) {
        animateVideo(kBack);

        ofLogNotice() << "Serial input: " << input << " f: " << f << " moving: back prev: " << previousDistance
                    << " current distance: " << currentDistance;

    }
}

void ofApp::calculateFPS() {
    int millis = ofMap(currentDistance,
                       configuration.MaxDistance,
                       configuration.MinDistance,
                       1000 / configuration.StartingFramesPerSecond,
                       1000 / configuration.FinishingFramesPerSecond);
    fps = 1000 / millis;
}

void ofApp::killGame() {
    long now = ofGetElapsedTimeMillis();

    ofLogNotice() << "Game finished with kill at " << now << " with current distance of " << currentDistance;
    
    eventLog << "killed=" << now << std::endl;
    
    state.finishedAt = now;
    state.saved = false;
    
    setDistance("killed", configuration.MinDistance);
    
    gameStats.Kills++;
    if (!gameStats.Write()) {
        ofLogError() << "Error writing game stats";
    }
}

void ofApp::saveGame(const std::string reason) {
    long now = ofGetElapsedTimeMillis();

    ofLogNotice() << "Game finished with save at " << now << " with current distance of " << currentDistance
        << " because of " << reason;
    
    eventLog << "saved=" << now << std::endl;
    
    state.finishedAt = now;
    state.saved = true;
    
    setDistance("saved", configuration.MaxDistance);
    
    gameStats.Saves++;
    if (!gameStats.Write()) {
        ofLogError() << "Error writing game stats";
    }
}

void ofApp::updateAudio() {
    if (!state.finishedAt) {
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

void ofApp::restartGame() {
    ofLogNotice() << "Game restarted";
    
    state = GameState();
    
    // Reset prev. distance
    previousDistance = configuration.MaxDistance;
    
    setDistance("restart", configuration.MaxDistance);
    
    if (!loadVideo()) {
        ofLogError() << "Error loading video after kill";
    }
    ofLogNotice() << "frame after resettting video player: " << videoPlayer.getCurrentFrame();
    
    eventLog << "started=" << ofGetElapsedTimeMillis() << std::endl;
}

bool ofApp::isAccepingInput() {
    if (state.finishedAt) {
        return false;
    }
    if (!isPlaying()) {
        return true;
    }
    int framesPlayed = std::abs(frameAtLastAnimationStart - videoPlayer.getCurrentFrame());
    return framesPlayed >= configuration.CheckAfterNFrames;
}

// Frame for current distance
// Note that this is not the actual frame that will be animated.
// Instead will start to animate towards this frame.
int ofApp::frameForDistance() {
    return ofMap(currentDistance,
                 configuration.MinDistance,
                 configuration.MaxDistance,
                 videoPlayer.getTotalNumFrames(),
                 0);
}

void ofApp::draw(){
    int restartCountdownSeconds = 0;
    if (state.finishedAt) {
        long now = ofGetElapsedTimeMillis();
        int beenDeadSeconds = (now - state.finishedAt) / 1000;
        restartCountdownSeconds = configuration.RestartIntervalSeconds - beenDeadSeconds;
    }

    // Update HUD
    if (true) {
        int y = 40;
        f.drawString("dist=" + ofToString(currentDistance), 10, y);
        f.drawString("f=" + ofToString(videoPlayer.getCurrentFrame()) + "/" + ofToString(videoPlayer.getTotalNumFrames()),
                     160, y);
        f.drawString("dest.f=" + ofToString(frameForDistance()), 300, y);
        f.drawString("fps=" + ofToString(fps), 460, y);
        f.drawString("sv=" + ofToString(gameStats.Saves), 560, y);
        f.drawString("kl=" + ofToString(gameStats.Kills), 660, y);
        f.drawString("rs=" + ofToString(restartCountdownSeconds), 760, y);
        f.drawString("vid=" + ofToString(isPlaying()), 860, y);
    }

    const int kMargin = 50;

    // Draw finished state stats if game is over
    if (state.finishedAt && !isPlaying()) {

        if (state.saved) {
            ofSetHexColor(0xFFFFFF);
        } else {
            ofSetHexColor(0x000000);
        }
        ofRect(0, kMargin, ofGetWindowWidth(), ofGetWindowHeight() - kMargin);
        ofFill();
        if (!state.saved) {
            ofSetHexColor(0xFFFFFF);
        } else {
            ofSetHexColor(0x000000);
        }
        std::string text("");
        if (state.saved) {
            text = "LIFES SAVED: " + ofToString(gameStats.Saves);
        } else {
            text = "TOTAL KILLS: " + ofToString(gameStats.Kills);
        }
        f.drawString(text,
                     ofGetWindowWidth() / 2 - 100,
                     ofGetWindowHeight() / 2);
    }
    
    // Draw intro image, if game has not started yet
    if (!state.finishedAt && !isPlaying() && configuration.MaxDistance == currentDistance && !previousDistance) {
        intro.draw(0, kMargin, ofGetWindowWidth(), ofGetWindowHeight() - kMargin);
    }

    // Draw outro image, if game is over
    if (state.finishedAt && !isPlaying() && configuration.MinDistance == currentDistance && previousDistance) {
        outro.draw(0, kMargin, ofGetWindowWidth(), ofGetWindowHeight() - kMargin);
    }

    // Draw video, if its running
    if (videoPlayer.isPlaying()) {
        ofSetHexColor(0xFFFFFF);
        ofFill();
        videoPlayer.draw(0, kMargin, ofGetWindowWidth(), ofGetWindowHeight() - kMargin);
    }
}

void ofApp::keyPressed(int key){
    ofLogNotice() << "keyPressed key=" << key;
    
    if (!isAccepingInput()) {
        return;
    }

    const int kMinStep = 100;

    if (OF_KEY_UP == key) {
        // distance decreases as viewer approaches
        int n = currentDistance - kMinStep - int(ofRandom(100));

        setDistance("keyboard up", n);

        animateVideo(kForward);
    } else if (OF_KEY_DOWN == key) {
        // distance incrases as viewer steps back
        int n = currentDistance + kMinStep + int(ofRandom(100));

        setDistance("keyboard down", n);

        animateVideo(kBack);
    }
}

void ofApp::setDistance(const std::string reason, const int value) {
    previousDistance = currentDistance;
    currentDistance = ofClamp(value, configuration.MinDistance, configuration.MaxDistance);
    if (previousDistance != currentDistance) {
        previousDistanceChangeAt = ofGetElapsedTimeMillis();
    }
    if (!state.minDistance) {
        state.minDistance = currentDistance;
    } else {
        state.minDistance = std::min(state.minDistance, currentDistance);
    }
}
