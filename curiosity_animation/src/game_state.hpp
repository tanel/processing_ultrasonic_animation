//
//  game_state.hpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#ifndef game_state_hpp
#define game_state_hpp

#include <stdio.h>

#include "ofMain.h"

const std::string kStateWaiting = "waiting";
const std::string kStateStarted = "started";
const std::string kStateSaved = "saved";
const std::string kStateKilled = "killed";
const std::string kStateStats = "stats";

class GameState {
public:
    GameState()
    : name(kStateWaiting)
    , saveZoneActive(false)
    , finishedAt(0)
    , gameWasSaved(false)
    , lastUserInputAt(0)
    , fps(0) {}
    
    // Current game state
    std::string name;
    bool saveZoneActive;
    long finishedAt;
    bool gameWasSaved;
    long lastUserInputAt;
    
    // App state, you should not touch these;
    int fps;
};


#endif /* game_state_hpp */
