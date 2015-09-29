//
//  game_stats.hpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#ifndef game_stats_hpp
#define game_stats_hpp

#include <stdio.h>
#include "ofMain.h"
#include "game_result.hpp"

class GameStats {
public:
    GameStats() {}
    
    void Read();
    
    void AddKill();
    void AddSave();
    
    int TotalSaves() const {
        return total.Saves;
    }
    
    int TotalKills() const {
        return total.Kills;
    }
    
private:
    GameResult total;
    std::map<std::string, GameResult> history;
    
    void write() const;
    
    static std::string currentDate();
};

#endif /* game_stats_hpp */
