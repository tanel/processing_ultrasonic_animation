//
//  game_stats.cpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#include "game_stats.hpp"

#include "ofxJSONElement.h"

#include "Poco/String.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/URI.h"

void GameStats::AddKill() {
    total.Kills++;
    
    std::string date = currentDate();
    GameResult today = history[date];
    today.Kills++;
    history[date] = today;
    
    write();
}

void GameStats::AddSave() {
    total.Saves++;
    
    std::string date = currentDate();
    GameResult today = history[date];
    today.Saves++;
    history[date] = today;
    
    write();
}

void GameStats::Read() {
    // Load in file contents, if file exists
    std::string path = ofToDataPath("gamestats.json");
    ofFile f(path, ofFile::ReadOnly);
    if (!f.exists()) {
        return;
    }
    
    ofxJSONElement data;
    f >> data;
    
    ofxJSONElement totalData = data["total"];
    total.Saves = totalData["saves"].asInt();
    total.Kills = totalData["kills"].asInt();
    
    ofxJSONElement historyData = data["history"];
    Json::Value::Members members = historyData.getMemberNames();
    for (int i = 0; i < members.size(); i++) {
        std::string date = members[i];
        Json::Value valueData = historyData.get(date, Json::Value());
        ofxJSONElement resultData(valueData);
        GameResult dateResult;
        dateResult.Saves = resultData["saves"].asInt();
        dateResult.Kills = resultData["kills"].asInt();
        history[date] = dateResult;
    }
    
    f.close();
}

std::string GameStats::currentDate() {
    string timeFormat = "%Y-%m-%d";
    Poco::LocalDateTime now;
    return Poco::DateTimeFormatter::format(now, timeFormat);
}

void GameStats::write() const {
    // Totals
    ofxJSONElement totalData;
    totalData["saves"] = total.Saves;
    totalData["kills"] = total.Kills;
    
    // History
    ofxJSONElement historyData;
    for(std::map<std::string, GameResult>::const_iterator it = history.begin();
        it != history.end();
        ++it)
    {
        std::string date = it->first;
        GameResult res = it->second;
        ofxJSONElement dateData;
        dateData["saves"] = res.Saves;
        dateData["kills"] = res.Kills;
        historyData[date] = dateData;
    }
    
    // Put it all together
    ofxJSONElement data;
    data["total"] = totalData;
    data["history"] = historyData;
    
    // And write it to a file
    std::string path = ofToDataPath("gamestats.json");
    ofFile f(path, ofFile::WriteOnly);
    f << data;
    f.flush();
    f.close();
}

