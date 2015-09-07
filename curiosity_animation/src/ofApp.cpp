#include "ofApp.h"
#include "ofxXmlSettings.h"

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
    if (!heartbeatSound.loadSound("2.mp3")) {
        ofLogError() << "Error loading heartbeat sound";
    }
    heartbeatSound.setLoop(true);

    // Video
    if (!loadVideo()) {
        ofLogError() << "Error loading video";
    }
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
    f.moveTo("gamestats_backup.xml", false, true);

    ofxXmlSettings xml;
    xml.setValue("gamestats:Saves", Saves);
    xml.setValue("gamestats:Kills", Kills);
    return xml.saveFile("gamestats.xml");
}

void ofApp::update(){
    long now = ofGetElapsedTimeMillis();

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

    // Determine if user is now in the death zone
    if (!state.finishedAt && (currentDistance < configuration.MinDistance + configuration.DeathZone)) {
        ofLogNotice() << "Game finished with kill at " << now;

        eventLog << "killed=" << now << std::endl;

        state.finishedAt = now;
        state.saved = false;

        setDistance("killed", configuration.MinDistance);

        gameStats.Kills++;
        if (!gameStats.Write()) {
            ofLogError() << "Error writing game stats";
        }
    }

    // If save zone is active and user finds itself in it,
    // then declare the game saved and finish it.
    int saveZoneStartsAt = std::abs(configuration.MaxDistance - configuration.DeathZone);
    if (!state.finishedAt && state.saveZoneActive && currentDistance > saveZoneStartsAt) {
        ofLogNotice() << "Game finished with save at " << now;

        eventLog << "saved=" << now << std::endl;

        state.finishedAt = now;
        state.saved = true;

        setDistance("saved", configuration.MaxDistance);

        gameStats.Saves++;
        if (!gameStats.Write()) {
            ofLogError() << "Error writing game stats";
        }
    }
    
    // If user has moved out of save zone, and game is not finished
    // yet, activate save zone
    int moved = std::abs(configuration.MaxDistance - currentDistance);
    if (!state.finishedAt && moved > configuration.DeathZone) {
        state.saveZoneActive = true;
    }

    // Restart if needed
    if (state.finishedAt && (state.finishedAt < now - (configuration.RestartIntervalSeconds*1000))) {
        ofLogNotice() << "Game restarted";

        state = GameState();

        setDistance("restart", configuration.MaxDistance);
        
        if (!loadVideo()) {
            ofLogError() << "Error loading video after kill";
        }
        ofLogNotice() << "frame after resettting video player: " << videoPlayer.getCurrentFrame();

        eventLog << "started=" << now << std::endl;
    }

    // Read serial
    if (serialPort.isInitialized() && serialPort.available()) {
        char c = serialPort.readByte();
        if ('\n' == c) {
            std::string input = serialbuf.str();
            serialbuf.str("");
            ofLogNotice() << "Serial input: " << input << std::endl;

            if (state.finishedAt) {
                return;
            }

            if (isPlaying()) {
                return;
            }

            float f = ofToFloat(input);

            int prev = currentDistance;

            setDistance("Serial input", f);

            if (prev > currentDistance) {
                animateVideo(kForward);
            } else if (prev < currentDistance) {
                animateVideo(kBack);
            }
        } else {
            serialbuf << c;
        }
    }

    // Update visual
    int millis = ofMap(currentDistance,
                       configuration.MaxDistance,
                       configuration.MinDistance,
                       1000 / configuration.StartingFramesPerSecond,
                       1000 / configuration.FinishingFramesPerSecond);
    fps = 1000 / millis;

    // Update video
    videoPlayer.update();

    // Update audio
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

    const int margin = 50;

    // Draw video
    if (state.finishedAt && !isPlaying()) {
        if (state.saved) {
            ofSetHexColor(0xFFFFFF);
        } else {
            ofSetHexColor(0x000000);
        }
        ofRect(0, margin, ofGetWindowWidth(), ofGetWindowHeight() - margin);
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

        return;
    }
    
    ofSetHexColor(0xFFFFFF);
    ofFill();
    videoPlayer.draw(0, margin, ofGetWindowWidth(), ofGetWindowHeight() - margin);
}

void ofApp::keyPressed(int key){
    ofLogNotice() << "keyPressed key=" << key;

    if (state.finishedAt) {
        return;
    }

    if (isPlaying()) {
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
    currentDistance = ofClamp(value, configuration.MinDistance, configuration.MaxDistance);
}
