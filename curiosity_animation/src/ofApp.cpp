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
    
#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif
    
    gameStats.Read();
    
    ofSetFrameRate(configuration.FrameRate);
    
    ofSetFullscreen(configuration.Fullscreen);
    
    //ofSetWindowShape(1000, 200);
    ofSetWindowPosition(0, 0);
    
    // Distance reader
    serialReader.ActiveSerialPort = configuration.ActiveSerialPort;
    serialReader.startThread();
    
    // HUD
    if (!hudFont.loadFont("verdana.ttf", 12, true, true)) {
        std::cerr << "Error loading HUD font" << std::endl;
    }
    hudFont.setLineHeight(18.0f);
    hudFont.setLetterSpacing(1.037);
    
    // Overlay
    if (!overlayFont.loadFont("verdana.ttf", 300, true, true)) {
        std::cerr << "Error loading overlay font" << std::endl;
    }
    
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

void ofApp::update(){
    long now = ofGetElapsedTimeMillis();
    
    const int distance = serialReader.Reading();
    
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

void ofApp::keyPressed(int key) {
    ofLogNotice() << "keyPressed key=" << key;
    
    const int kMinStep = 50;
    
    if (OF_KEY_UP == key) {
        // distance decreases as viewer approaches
        serialReader.AddReading(serialReader.Reading() - kMinStep);
    } else if (OF_KEY_DOWN == key) {
        // distance incrases as viewer steps back
        serialReader.AddReading(serialReader.Reading() + kMinStep);
    }
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
    
    const int distance = serialReader.Reading();
    
    int y = 20;
    hudFont.drawString("distance=" + ofToString(distance), 10, y);
    hudFont.drawString("frame=" + ofToString(videoPlayer.getCurrentFrame()) + "/" + ofToString(totalNumOfFrames), 200, y);
    hudFont.drawString("dest.f=" + ofToString(frameForDistance(distance)), 400, y);
    hudFont.drawString("saves=" + ofToString(gameStats.TotalSaves()), 600, y);
    hudFont.drawString("kills=" + ofToString(gameStats.TotalKills()), 800, y);
    
    y = 40;
    hudFont.drawString("restart=" + ofToString(restartCountdownSeconds), 10, y);
    hudFont.drawString("video=" + ofToString(isPlaying() ? "yes" : "no"), 200, y);
    hudFont.drawString("kill active=" + ofToString(configuration.DeathZone ? "yes" : "no"), 400, y);
    hudFont.drawString("save active=" + ofToString(state.saveZoneActive ? "yes" : "no"), 600, y);
    hudFont.drawString("state=" + state.name, 800, y);
    
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
        hudFont.drawString(text,
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
        
        // Draw overlay, for debugging
        if (configuration.DebugOverlay) {
            ofSetHexColor(kColorBlack);
            overlayFont.drawString(ofToString(distance),
                                   100,
                                   ofGetWindowHeight() / 2);
        }
    }
    
}
