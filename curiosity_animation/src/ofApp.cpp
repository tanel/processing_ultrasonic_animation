#include "ofApp.h"
#include "ofxXmlSettings.h"
#include "ofxJSONElement.h"

void ofApp::setup(){
    ofBackground(54, 54, 54, 255);
    
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
    
    ofSetWindowShape(1000, 200);
    ofSetWindowPosition(0, 0);
    
#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif
    
    // Distance reader
    serialReader.activeSerialPort = configuration.ActiveSerialPort;
    serialReader.startThread();
    
    // HUD
    if (!f.loadFont("verdana.ttf", 12, true, true)) {
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

void ofApp::exit() {
    serialReader.stopThread();
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
    totalNumOfFrames = videoPlayer.getTotalNumFrames();
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
        AutoSaveSeconds = xml.getValue("configuration:AutoSaveSeconds", AutoSaveSeconds);
        IntroFileName = xml.getValue("configuration:IntroFileName", IntroFileName);
        OutroFileName = xml.getValue("configuration:OutroFileName", OutroFileName);
    }
    
    return true;
}

void GameStats::AddKill() {
    totalKills++;
    updateDay();
    todayKills++;
    write();
}

void GameStats::AddSave() {
    totalSaves++;
    updateDay();
    todaySaves++;
    write();
}

void GameStats::updateDay() {
    std::string newDay = currentDate();
    if (today != newDay) {
        todayKills = 0;
        todaySaves = 0;
        today = newDay;
    }
}

bool GameStats::Read() {
    /* FIXME:
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
    */
    return true;
}

std::string GameStats::currentDate() {
    string timeFormat = "%Y-%m-%d";
    Poco::LocalDateTime now;
    return Poco::DateTimeFormatter::format(now, timeFormat);
}

void GameStats::write() const {
    ofxJSONElement data;
    data["totalSaves"] = totalSaves;
    data["totalKills"] = totalKills;
    data["todaySaves"] = todaySaves;
    data["todayKills"] = todayKills;
    data["today"] = today;
    ofFile f(ofToDataPath("gamestats.json"), ofFile::WriteOnly);
    f << data;
}

void ofApp::update(){
    long now = ofGetElapsedTimeMillis();
    
    const int distance = serialReader.reading();
    
    if (kStateWaiting == state.name) {
        if (distance) {
            startGame();
        }
    }
    
    // Determine if user is now in the death zone
    if (kStateStarted == state.name) {
        if (distance && distance < configuration.MinDistance + configuration.DeathZone) {
            killGame();
        }
    }
    
    // If save zone is active and user finds itself in it,
    // then declare the game saved and finish it.
    if (kStateStarted == state.name) {
        if (state.saveZoneActive && distance > saveZoneStartsAt()) {
            saveGame("user walked into save zone");
        }
    }
    
    // If user has moved out of save zone, and game is not finished
    // yet, activate save zone
    if (kStateStarted == state.name) {
        if (!state.saveZoneActive) {
            if (distanceMoved(distance) > configuration.DeathZone * 2) {
                state.saveZoneActive = true;
            }
        }
    }
    
    // Restart if needed
    if (kStateStats == state.name) {
        if (state.finishedAt < now - (configuration.RestartIntervalSeconds*1000)) {
            restartGame();
        }
    }
    
    // If nothing has happened for a while, save game automatically
    if (kStateStarted == state.name) {
        if (state.lastUserInputAt < now - (configuration.AutoSaveSeconds*1000)) {
            saveGame("automatically saved because of no user action");
        }
    }
    
    // if video has stopped playing and game is killed or saved, move into stats state
    if (kStateKilled == state.name || kStateSaved == state.name) {
        if (!isPlaying()) {
            state.name = kStateStats;
            state.finishedAt = now;
        }
    }
    
    calculateFPS(distance);
    
    updateVideo(distance);
    
    updateAudio(distance);
}

void ofApp::updateVideo(const int distance) {
    int currentFrame = videoPlayer.getCurrentFrame();
    int destinationFrame = frameForDistance(distance);
    if (isPlaying()) {
        if (videoPlayer.getSpeed() == kForward) {
            if (currentFrame >= destinationFrame) {
                videoPlayer.setPaused(true);
            }
        } else if (videoPlayer.getSpeed() == kBack) {
            if (currentFrame <= destinationFrame) {
                videoPlayer.setPaused(true);
            }
        }
    } else {
        if (currentFrame > destinationFrame) {
            videoPlayer.setSpeed(kBack);
            videoPlayer.setPaused(false);
        } else if (currentFrame < destinationFrame) {
            videoPlayer.setSpeed(kForward);
            videoPlayer.setPaused(false);
        }
    }
    videoPlayer.update();
}

void ofApp::calculateFPS(const int distance) {
    int millis = ofMap(distance,
                       configuration.MaxDistance,
                       configuration.MinDistance,
                       1000 / configuration.StartingFramesPerSecond,
                       1000 / configuration.FinishingFramesPerSecond);
    state.fps = 1000 / millis;
}

void ofApp::killGame() {
    long now = ofGetElapsedTimeMillis();
    
    std::cout << "Game finished with kill at " << now << std::endl;
    
    eventLog << "killed=" << now << std::endl;
    
    state.name = kStateKilled;
    state.gameWasSaved = false;
    
    gameStats.AddKill();
}

void ofApp::saveGame(const std::string reason) {
    long now = ofGetElapsedTimeMillis();
    
    std::cout
    << "Game finished with save at " << now
    << " because of " << reason << std::endl;
    
    eventLog << "saved=" << now << std::endl;
    
    state.name = kStateSaved;
    state.gameWasSaved = true;
    
    gameStats.AddSave();
}

void ofApp::updateAudio(const int distance) {
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
        heartbeatSound.setSpeed(ofMap(distance,
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
    
    if (!loadVideo()) {
        std::cerr << "Error loading video after kill" << std::endl;
    }
    std::cout << "frame after resettting video player: " << videoPlayer.getCurrentFrame() << std::endl;
    
    eventLog << "started=" << ofGetElapsedTimeMillis() << std::endl;
}

void ofApp::startGame() {
    long now = ofGetElapsedTimeMillis();
    
    std::cout << "Game started at " << now << std::endl;
    
    state.name = kStateStarted;
    
    eventLog << "started=" << ofGetElapsedTimeMillis() << std::endl;
}

bool ofApp::isAccepingInput() {
    if (kStateKilled == state.name || kStateSaved == state.name || kStateStats == state.name) {
        return false;
    }
    return isPlaying();
}

// Frame for current distance
// Note that this is not the actual frame that will be animated.
// Instead will start to animate towards this frame.
int ofApp::frameForDistance(const int distance) const {
    int d(0);
    // Override dest. frame on certain conditions, like kill, save, waiting etc
    if (kStateKilled == state.name) {
        d = configuration.MinDistance;
    } else if (kStateSaved == state.name) {
        d = configuration.MaxDistance;
    } else if (kStateWaiting == state.name) {
        d = configuration.MaxDistance
        ;
    } else {
        d = distance;
    }
    return ofMap(d,
                 configuration.MinDistance,
                 configuration.MaxDistance,
                 totalNumOfFrames,
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
    ofSetColor(255);
    
    const int distance = serialReader.reading();
    
    int y = 20;
    f.drawString("distance=" + ofToString(distance), 10, y);
    f.drawString("frame=" + ofToString(videoPlayer.getCurrentFrame()) + "/" + ofToString(totalNumOfFrames), 200, y);
    f.drawString("dest.f=" + ofToString(frameForDistance(distance)), 400, y);
    f.drawString("fps=" + ofToString(state.fps), 600, y);
    f.drawString("saves=" + ofToString(gameStats.TotalSaves()), 800, y);
    f.drawString("kills=" + ofToString(gameStats.TotalKills()), 900, y);
    
    y = 40;
    f.drawString("restart=" + ofToString(restartCountdownSeconds), 10, y);
    f.drawString("video=" + ofToString(isPlaying() ? "yes" : "no"), 200, y);
    f.drawString("deathzone=" + ofToString(configuration.DeathZone ? "yes" : "no"), 400, y);
    f.drawString("save active=" + ofToString(state.saveZoneActive ? "yes" : "no"), 600, y);
    f.drawString("state=" + state.name, 800, y);
    
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
            text = "LIFES SAVED: " + ofToString(gameStats.TotalSaves());
        } else {
            text = "TOTAL KILLS: " + ofToString(gameStats.TotalKills());
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

void SerialReader::threadedFunction() {
    serialPort.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serialPort.getDeviceList();
    for (int i = 0; i < deviceList.size(); i++) {
        std::cout << i << ". serial device: " << deviceList[i].getDeviceName() << std::endl;
    }
    if (activeSerialPort < deviceList.size()) {
        if (!serialPort.setup(activeSerialPort, 9600)) {
            std::cerr << "Failed to connect to serial device! "
            << deviceList[activeSerialPort].getDeviceName() << std::endl;
        }
    }
    
    while(isThreadRunning()) {
        if (!serialPort.isInitialized()) {
            continue;
        }
        
        if (!serialPort.available()) {
            continue;
        }
        char c = serialPort.readByte();
        if ('\n' != c) {
            serialbuf << c;
            continue;
        }
        
        std::string s = serialbuf.str();
        serialbuf.str("");
        
        if (!s.empty()) {
            int n = ofToInt(s);
            if (n) {
                std::cout << "serial thread input=" << n << std::endl;
                values.push_front(n);
                values.resize(std::min(kNumOfValues, int(values.size())));
            }
        }
    }
    
    // done
}