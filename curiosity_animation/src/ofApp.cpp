#include "ofApp.h"
#include "ofxXmlSettings.h"
#include "ofxJSONElement.h"

void ofApp::setup(){
    ofBackground(54, 54, 54, 255);
    
#ifdef TARGET_OSX
    ofSetDataPathRoot("~/curiosity_data/");
#endif
    
    // start logging
    ofLogToFile("app.log");
    
    // start recoding events
    if (!eventLog.open(ofToDataPath("events.txt"), ofFile::WriteOnly)) {
        std::cerr << "Error opening events.txt - !!!DATA WILL BE LOST!!!" << std::endl;
    }
    
    if (!configuration.Read()) {
        std::cerr << "Error reading configuration" << std::endl;
    }
    
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
    if (!stateFont.loadFont("verdana.ttf", 100, true, true)) {
        std::cerr << "Error loading status font" << std::endl;
    }
    
    // Audio
    if (!heartbeatSound.loadSound("2.mp3")) {
        std::cerr << "Error loading heartbeat sound" << std::endl;
    }
    heartbeatSound.setLoop(true);
    
    // Video
    if (!loadVideo()) {
        std::cerr << "Error loading video(s)" << std::endl;
    }
    
    // Intro and outro pics
    if (!intro.loadImage(ofToDataPath(configuration.IntroFileName))) {
        std::cerr << "Error loading intro" << std::endl;
    }
    
    serialReader.SetMaxReading(configuration.MaxDistance);
    serialReader.Enable();
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
        std::cerr << "error loading movie" << std::endl;
        return false;
    }
    videoPlayer.setLoopState(OF_LOOP_NONE);
    totalNumOfFrames = videoPlayer.getTotalNumFrames();
    
    killVideoPlayer = ofVideoPlayer();
    if (!killVideoPlayer.loadMovie(ofToDataPath(configuration.KillVideoFileName))) {
        std::cerr << "error loading kill movie" << std::endl;
        return false;
    }
    
    return true;
}

void ofApp::update(){
    long now = ofGetElapsedTimeMillis();
    
    const int distance = serialReader.Reading();
    
    if (kStateWaiting == state.name) {
        // If user finds itself *in* the save zone, we start the game.
        if (distance < configuration.MaxDistance
            && distance >= configuration.MaxDistance - configuration.SaveZone) {
            startGame();
        }
        
    } else if (kStateStarted == state.name) {
        
        if (distance < configuration.MinDistance + configuration.DeathZone) {
            // Determine if user is now in the death zone
            killGame();
            
        } else if (state.saveZoneActive
                   && distance > configuration.MaxDistance - configuration.SaveZone) {
            // If save zone is active and user finds itself in it,
            // then declare the game saved and finish it.
            saveGame("user walked into save zone");
            
        } else if (!state.saveZoneActive
                   && distance < configuration.MaxDistance - configuration.SaveZone) {
            // If user has moved out of save zone, and game is not finished
            // yet, activate save zone
            state.saveZoneActive = true;
            
        } else if (serialReader.LastUserInputAt() < now - (configuration.AutoSaveSeconds*1000)) {
            
            // If we have no new input for N seconds, consider the game
            // as saved, as it seems that the user has left the building
            saveGame("user has left the game");
        }
        
    } else if (kStateStatsKilled == state.name || kStateStatsSaved == state.name) {
        if (state.finishedAt < now - (configuration.RestartIntervalSeconds*1000)) {
            restartGame();
        }
        
    } else if (kStateKilled == state.name || kStateSaved == state.name) {
        int destinationFrame = frameForDistance(distance);
        if (destinationFrame == videoPlayer.getCurrentFrame()) {
            showStats();
        }
        
    } else {
        throw("invalid state");
    }
    
    updateVideo(distance);
    updateAudio(distance);
}

void ofApp::showStats() {
    long now = ofGetElapsedTimeMillis();
    
    serialReader.Disable();
    
    std::cout << "Showing stats " << now << std::endl;
    
    if (kStateKilled == state.name) {
        state.name = kStateStatsKilled;
    } else if (kStateSaved == state.name) {
        state.name = kStateStatsSaved;
    } else {
        throw("invalid state");
    }
    state.finishedAt = now;
}

void ofApp::updateVideo(const int distance) {
    if (kStateWaiting == state.name
        || kStateStatsKilled == state.name
        || kStateStatsSaved == state.name) {
        if (videoPlayer.isPlaying()) {
            videoPlayer.stop();
        }
        return;
    }
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
    
    serialReader.Disable();
    
    std::cout << "Game finished with kill at " << now << std::endl;
    
    eventLog << "killed=" << now << std::endl;
    
    state.name = kStateKilled;
    
    gameStats.AddKill();
}

void ofApp::saveGame(const std::string reason) {
    long now = ofGetElapsedTimeMillis();
    
    std::cout
    << "Game finished with save at " << now
    << " because of " << reason << std::endl;
    
    eventLog << "saved=" << now << std::endl;
    
    state.name = kStateSaved;
    
    gameStats.AddSave();
}

void ofApp::keyPressed(int key) {
    ofLogNotice() << "keyPressed key=" << key;
    
    const int kMinStep = 50;
    
    if (OF_KEY_UP == key) {
        serialReader.AddReading(ofClamp(serialReader.Reading() - kMinStep,
                                        configuration.MinDistance,
                                        configuration.MaxDistance));
    } else if (OF_KEY_DOWN == key) {
        serialReader.AddReading(ofClamp(serialReader.Reading() + kMinStep,
                                        configuration.MinDistance,
                                        configuration.MaxDistance));
    }
}

void ofApp::updateAudio(const int distance) {
    // Game over, dudes
    if (kStateStatsSaved == state.name || kStateStatsKilled == state.name) {
        if (heartbeatSound.getIsPlaying()) {
            heartbeatSound.stop();
        }
    } else {
        if (!heartbeatSound.getIsPlaying()) {
            heartbeatSound.play();
        }
        heartbeatSound.setSpeed(ofMap(videoPlayer.getCurrentFrame(),
                                      0,
                                      totalNumOfFrames,
                                      configuration.StartingHeartBeatSpeed,
                                      configuration.FinishingHeartBeatSpeed));
    }
    ofSoundUpdate();
}

void ofApp::restartGame() {
    std::cout << "Game restarted" << std::endl;
    
    state = GameState();
    
    serialReader.Disable();
    serialReader.Clear();
    serialReader.Enable();
    
    if (!loadVideo()) {
        std::cerr << "Error loading video after kill" << std::endl;
    }
    std::cout << "frame after resettting video player: " << videoPlayer.getCurrentFrame() << std::endl;
    
    eventLog << "started=" << ofGetElapsedTimeMillis() << std::endl;
}

void ofApp::startGame() {
    long now = ofGetElapsedTimeMillis();
    
    serialReader.Enable();
    
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
    if (kStateKilled == state.name || kStateStatsKilled == state.name) {
        d = configuration.MinDistance;
    } else if (kStateSaved == state.name || kStateStatsSaved == state.name) {
        d = configuration.MaxDistance;
    } else if (kStateWaiting == state.name || kStateStarted == state.name) {
        d = distance;
    } else {
        throw("invalid state!");
    }
    return ofMap(d,
                 configuration.MaxDistance,
                 configuration.MinDistance,
                 0,
                 totalNumOfFrames);
}

const int kColorWhite = 0xFFFFFF;
const int kColorBlack = 0x000000;

void ofApp::draw(){
    long now = ofGetElapsedTimeMillis();
    
    int restartCountdownSeconds = 0;
    if (kStateStatsSaved == state.name || kStateStatsKilled == state.name) {
        int beenDeadSeconds = (now - state.finishedAt) / 1000;
        restartCountdownSeconds = configuration.RestartIntervalSeconds - beenDeadSeconds;
    }
    
    int autosaveCountdownSeconds = 0;
    if (kStateStarted == state.name) {
        int inactiveSeconds = (now - serialReader.LastUserInputAt()) / 1000;
        autosaveCountdownSeconds = configuration.AutoSaveSeconds - inactiveSeconds;
    }
    
    // Update HUD
    
    
    const int distance = serialReader.Reading();
    
    ofSetColor(255);
    
    if (configuration.DebugOverlay) {
        int y = 20;
        hudFont.drawString("distance=" + ofToString(distance), 10, y);
        hudFont.drawString("frame=" + ofToString(videoPlayer.getCurrentFrame()) + "/" + ofToString(totalNumOfFrames), 200, y);
        hudFont.drawString("dest.f=" + ofToString(frameForDistance(distance)), 400, y);
        hudFont.drawString("max distance=" + ofToString(configuration.MaxDistance), 600, y);
        hudFont.drawString("video=" + ofToString(isPlaying() ? "yes" : "no"), 800, y);
        
        y = 40;
        hudFont.drawString("restart=" + ofToString(restartCountdownSeconds), 10, y);
        hudFont.drawString("save zone=" + ofToString(configuration.SaveZone), 200, y);
        hudFont.drawString("death zone=" + ofToString(configuration.DeathZone), 400, y);
        hudFont.drawString("save active=" + ofToString(state.saveZoneActive ? "yes" : "no"), 600, y);
        hudFont.drawString("autosave=" + ofToString(autosaveCountdownSeconds), 800, y);
    }
    
    int margin(0);
    if (configuration.DebugOverlay) {
        margin = 50;
    }
    
    if (kStateStatsSaved == state.name) {
        ofSetHexColor(kColorWhite);
        ofRect(0, margin, ofGetWindowWidth(), ofGetWindowHeight() - margin);
        ofFill();
        ofSetHexColor(kColorBlack);
        hudFont.drawString("LIFES SAVED: " + ofToString(gameStats.TotalSaves()),
                           ofGetWindowWidth() / 2 - 100,
                           ofGetWindowHeight() / 2);
        
    } else if (kStateStatsKilled == state.name) {
        ofSetHexColor(kColorBlack);
        ofRect(0, margin, ofGetWindowWidth(), ofGetWindowHeight() - margin);
        ofFill();
        ofSetHexColor(kColorWhite);
        hudFont.drawString("TOTAL KILLS: " + ofToString(gameStats.TotalKills()),
                           ofGetWindowWidth() / 2 - 100,
                           ofGetWindowHeight() / 2);
        
    } else if (kStateWaiting == state.name) {
        ofSetHexColor(kColorWhite);
        ofFill();
        intro.draw(0, margin, ofGetWindowWidth(), ofGetWindowHeight() - margin);
        
    } else if (kStateStarted == state.name || kStateKilled == state.name || kStateSaved == state.name) {
        ofSetHexColor(kColorWhite);
        ofFill();
        videoPlayer.draw(0, margin, ofGetWindowWidth(), ofGetWindowHeight() - margin);
        
        // Draw overlay, for debugging
        if (configuration.DebugOverlay) {
            ofSetHexColor(kColorBlack);
            overlayFont.drawString(ofToString(distance),
                                   100,
                                   ofGetWindowHeight() / 2);
        }
        
    } else {
        throw("invalid state");
    }
    
    if (configuration.DebugOverlay) {
        ofSetHexColor(kColorBlack);
        stateFont.drawString(state.name,
                             100,
                             ofGetWindowHeight() / 2 + 200);
    }
}