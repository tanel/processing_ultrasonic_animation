//
//  game_result.hpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#ifndef game_result_hpp
#define game_result_hpp

#include <stdio.h>

class GameResult {
public:
    GameResult()
    : Saves(0)
    , Kills(0) {}
    
    int Saves;
    int Kills;
};

#endif /* game_result_hpp */

