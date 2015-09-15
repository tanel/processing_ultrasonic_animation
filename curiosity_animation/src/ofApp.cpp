#include "ofApp.h"
#include "ofxXmlSettings.h"

void ofApp::setup(){
    // start logging
    ofLogToFile("app.log");
    
    // start recoding events
    if (!eventLog.open(ofToDataPath("events.txt"), ofFile::WriteOnly)) {
        std::cerr << "Error opening events.txt - !!!DATA WILL BE LOST!!!" << std::endl;
    }
    
    if (!configuration.Read()) {
        std::cerr << "Error reading configuration" << std::endl;
    }
    
    if (!gameStats.Read()) {
        std::cerr << "Error reading game stats." << std::endl;
    }
    
    ofSetFrameRate(configuration.FrameRate);
    
    ofSetFullscreen(configuration.Fullscreen);
    
#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif
    
    // Distance reading
    serialPort.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serialPort.getDeviceList();
    for (int i = 0; i < deviceList.size(); i++) {
        std::cout << i << ". serial device: " << deviceList[i].getDeviceName() << std::endl;
    }
    if (configuration.ActiveSerialPort < deviceList.size()) {
        if (!serialPort.setup(configuration.ActiveSerialPort, 9600)) {
            std::cerr << "Failed to connect to serial device! "
            << deviceList[configuration.ActiveSerialPort].getDeviceName() << std::endl;
        }
    }
    
    setDistance("initialize", configuration.MaxDistance);
    
    // HUD
    if (!f.loadFont("verdana.ttf", 16, true, true)) {
        std::cerr << "Error loading font" << std::endl;
    }
    f.setLineHeight(18.0f);
    f.setLetterSpacing(1.037);
    
    // Audio
    if (!heartbeatSound.loadSound("2.mp3")) {
        std::cerr << "Error loading heartbeat sound" << std::endl;
    }
    heartbeatSound.setLoop(true);
    
    // Video
    if (!loadVideo()) {
        std::cerr << "Error loading video" << std::endl;
    }
    
    // Intro and outro pics
    if (!intro.loadImage(ofToDataPath(configuration.IntroFileName))) {
        std::cerr << "Error loading intro" << std::endl;
    }
    if (!outro.loadImage(ofToDataPath(configuration.OutroFileName))) {
        std::cerr << "Error loading outro" << std::endl;
    }
}

const int kForward = 1;
const int kBack = -1;

void ofApp::animateVideo(const int direction) {
    if (direction != kForward && direction != kBack) {
        std::cerr << "Invalid direction " << direction << std::endl;
        return;
    }
    if (videoPlayer.getSpeed() != direction) {
        videoPlayer.setPaused(true);
        videoPlayer.setSpeed(direction);
    }
    if (kStateWaiting == state.name) {
        startGame();
    }
    if (!isPlaying()) {
        state.frameAtLastAnimationStart = videoPlayer.getCurrentFrame();
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
            std::cerr << "Error saving configuration file" << std::endl;
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
    if (kStateStarted == state.name && (state.currentDistance < configuration.MinDistance + configuration.DeathZone)) {
        killGame();
    }

    // If save zone is active and user finds itself in it,
    // then declare the game saved and finish it.
    int saveZoneStartsAt = std::abs(configuration.MaxDistance - configuration.DeathZone);
    if (kStateStarted == state.name && state.saveZoneActive && state.currentDistance > saveZoneStartsAt) {
        saveGame("user walked into save zone");
    }
    
    // If user has moved out of save zone, and game is not finished
    // yet, activate save zone
    int moved = std::abs(configuration.MaxDistance - state.currentDistance);
    if (kStateStarted == state.name && moved > configuration.DeathZone * 2) {
        state.saveZoneActive = true;
    }

    // Restart if needed
    if (kStateStats == state.name && (state.finishedAt < now - (configuration.RestartIntervalSeconds*1000))) {
        restartGame();
    }
    
    // If nothing has happened for a while, save game automatically
    if (kStateStarted == state.name) {
        if (state.previousDistanceChangeAt < now - (configuration.AutoSaveSeconds*1000) && state.minDistance && state.minDistance < configuration.MaxDistance) {
            saveGame("automatically saved because of no user action");
        }
    }
    
    // if video has stopped playing and game is killed or saved, move into stats state
    if (!isPlaying() && (kStateKilled == state.name || kStateSaved == state.name)) {
        state.name = kStateStats;
        state.finishedAt = now;
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
    
    state.serialInput = serialbuf.str();
    serialbuf.str("");
    
    if (state.serialInput.empty()) {
        return;
    }
    
    float f = ofToFloat(state.serialInput);
    
    setDistance("Serial input", f);

    if (state.previousDistance > state.currentDistance) {
        animateVideo(kForward);

        std::cout
            << "Serial input: " << state.serialInput
            << " f: " << f
            << " moving: forward prev: " << state.previousDistance
        << " current distance: " << state.currentDistance << std::endl;
    }
    if (state.previousDistance < state.currentDistance) {
        animateVideo(kBack);

        std::cout
            << "Serial input: " << state.serialInput
            << " f: " << f
            << " moving: back prev: " << state.previousDistance
        << " current distance: " << state.currentDistance << std::endl;
    }
}

void ofApp::calculateFPS() {
    int millis = ofMap(state.currentDistance,
                       configuration.MaxDistance,
                       configuration.MinDistance,
                       1000 / configuration.StartingFramesPerSecond,
                       1000 / configuration.FinishingFramesPerSecond);
    state.fps = 1000 / millis;
}

void ofApp::killGame() {
    long now = ofGetElapsedTimeMillis();

    std::cout << "Game finished with kill at " << now
    << " with current distance of " << state.currentDistance << std::endl;
    
    eventLog << "killed=" << now << std::endl;
    
    state.name = kStateKilled;
    state.gameWasSaved = false;
    
    setDistance("killed", configuration.MinDistance);
    
    gameStats.Kills++;
    if (!gameStats.Write()) {
        std::cerr << "Error writing game stats" << std::endl;
    }
}

void ofApp::saveGame(const std::string reason) {
    long now = ofGetElapsedTimeMillis();

    std::cout
        << "Game finished with save at " << now
        << " with current distance of " << state.currentDistance
        << " because of " << reason << std::endl;
    
    eventLog << "saved=" << now << std::endl;
    
    state.name = kStateSaved;
    state.gameWasSaved = true;
    
    setDistance("saved", configuration.MaxDistance);
    
    gameStats.Saves++;
    if (!gameStats.Write()) {
        std::cerr << "Error writing game stats" << std::endl;
    }
}

void ofApp::updateAudio() {
    // Game over, dudes
    if (kStateStats == state.name) {
        if (heartbeatSound.getIsPlaying()) {
            heartbeatSound.stop();
        }
        return;
    } else {
        if (!heartbeatSound.getIsPlaying()) {
            heartbeatSound.play();
        }
        heartbeatSound.setSpeed(ofMap(
                                      state.currentDistance,
                                      configuration.MaxDistance,
                                      configuration.MinDistance,
                                      configuration.StartingHeartBeatSpeed,
                                      configuration.FinishingHeartBeatSpeed));
    }
    ofSoundUpdate();
}

void ofApp::restartGame() {
    std::cout << "Game restarted" << std::endl;
    
    state = GameState();
    
    // Reset prev. distance
    state.previousDistance = configuration.MaxDistance;
    
    setDistance("restart", configuration.MaxDistance);
    
    if (!loadVideo()) {
        std::cerr << "Error loading video after kill" << std::endl;
    }
    std::cout << "frame after resettting video player: " << videoPlayer.getCurrentFrame() << std::endl;
    
    eventLog << "started=" << ofGetElapsedTimeMillis() << std::endl;
}

void ofApp::startGame() {
    long now = ofGetElapsedTimeMillis();
    
    std::cout << "Game started at " << now
    << " with current distance of " << state.currentDistance << std::endl;

    state.name = kStateStarted;
    
    eventLog << "started=" << ofGetElapsedTimeMillis() << std::endl;
}

bool ofApp::isAccepingInput() {
    if (kStateKilled == state.name || kStateSaved == state.name || kStateStats == state.name) {
        return false;
    }
    if (!isPlaying()) {
        return true;
    }
    int framesPlayed = std::abs(state.frameAtLastAnimationStart - videoPlayer.getCurrentFrame());
    return framesPlayed >= configuration.CheckAfterNFrames;
}

// Frame for current distance
// Note that this is not the actual frame that will be animated.
// Instead will start to animate towards this frame.
int ofApp::frameForDistance() {
    return ofMap(state.currentDistance,
                 configuration.MinDistance,
                 configuration.MaxDistance,
                 videoPlayer.getTotalNumFrames(),
                 0);
}

const int kColorWhite = 0xFFFFFF;
const int kColorBlack = 0x000000;

void ofApp::draw(){
    int restartCountdownSeconds = 0;
    if (kStateStats == state.name) {
        long now = ofGetElapsedTimeMillis();
        int beenDeadSeconds = (now - state.finishedAt) / 1000;
        restartCountdownSeconds = configuration.RestartIntervalSeconds - beenDeadSeconds;
    }

    // Update HUD
    int y = 40;
    f.drawString("dist=" + ofToString(state.currentDistance), 10, y);
    f.drawString("f=" + ofToString(videoPlayer.getCurrentFrame()) + "/" + ofToString(videoPlayer.getTotalNumFrames()),
                     160, y);
    f.drawString("dest.f=" + ofToString(frameForDistance()), 300, y);
    f.drawString("fps=" + ofToString(state.fps), 460, y);
    f.drawString("sv=" + ofToString(gameStats.Saves), 560, y);
    f.drawString("kl=" + ofToString(gameStats.Kills), 660, y);
    f.drawString("rs=" + ofToString(restartCountdownSeconds), 760, y);
    f.drawString("vid=" + ofToString(isPlaying()), 860, y);
    f.drawString("ser=" + ofToString(state.serialInput), 960, y);
    f.drawString("state=" + state.name, 1060, y);

    const int kMargin = 50;

    // Draw finished state stats if game is over
    if (kStateStats == state.name) {
        if (state.gameWasSaved) {
            ofSetHexColor(kColorWhite);
        } else {
            ofSetHexColor(kColorBlack);
        }
        ofRect(0, kMargin, ofGetWindowWidth(), ofGetWindowHeight() - kMargin);
        ofFill();
        if (state.gameWasSaved) {
            ofSetHexColor(kColorBlack);
        } else {
            ofSetHexColor(kColorWhite);
        }
        std::string text("");
        if (state.gameWasSaved) {
            text = "LIFES SAVED: " + ofToString(gameStats.Saves);
        } else {
            text = "TOTAL KILLS: " + ofToString(gameStats.Kills);
        }
        f.drawString(text,
                     ofGetWindowWidth() / 2 - 100,
                     ofGetWindowHeight() / 2);
    }
    
    // Draw intro image, if game has not started yet
    if (kStateWaiting == state.name) {
        ofSetHexColor(kColorWhite);
        ofFill();
        intro.draw(0, kMargin, ofGetWindowWidth(), ofGetWindowHeight() - kMargin);
    }

    // Draw video, if its running
    if (kStateStarted == state.name || kStateKilled == state.name || kStateSaved == state.name) {
        ofSetHexColor(kColorWhite);
        ofFill();
        videoPlayer.draw(0, kMargin, ofGetWindowWidth(), ofGetWindowHeight() - kMargin);
    }
}

void ofApp::keyPressed(int key){
    std::cout << "keyPressed key=" << key << std::endl;
    
    if (!isAccepingInput()) {
        return;
    }

    const int kMinStep = 100;

    if (OF_KEY_UP == key) {
        // distance decreases as viewer approaches
        int n = state.currentDistance - kMinStep - int(ofRandom(100));
        if (n >= configuration.MinDistance) {
            setDistance("keyboard up", n);
            animateVideo(kForward);
        }
    } else if (OF_KEY_DOWN == key) {
        // distance incrases as viewer steps back
        int n = state.currentDistance + kMinStep + int(ofRandom(100));
        if (n <= configuration.MaxDistance) {
            setDistance("keyboard down", n);
            animateVideo(kBack);
        }
    }
}

void ofApp::setDistance(const std::string reason, const int value) {
    std::cout << "setDistance reason=" << reason << " value=" << value << std::endl;
    state.previousDistance = state.currentDistance;
    state.currentDistance = ofClamp(value, configuration.MinDistance, configuration.MaxDistance);
    if (state.previousDistance != state.currentDistance) {
        state.previousDistanceChangeAt = ofGetElapsedTimeMillis();
    }
    if (!state.minDistance) {
        state.minDistance = state.currentDistance;
    } else {
        state.minDistance = std::min(state.minDistance, state.currentDistance);
    }
}
