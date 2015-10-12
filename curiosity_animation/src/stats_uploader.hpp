//  Created by Tanel Lebedev on 29/09/15.

#ifndef stats_uploader_hpp
#define stats_uploader_hpp

#include <stdio.h>

#include "ofMain.h"

#include <Poco/Mutex.h>

class StatsUploader : public ofThread {
public:
    StatsUploader()
    : dirty(false)
    , uploadCommand("") {}

    void threadedFunction();
    
    void SetDirty() {
        Poco::FastMutex::ScopedLock lock(mutex);
        dirty = true;
    }

    void SetUploadCommand(const std::string value) {
        uploadCommand = value;
    }
    
private:
    bool dirty;
    std::string uploadCommand;
    Poco::FastMutex mutex;
};

#endif /* stats_uploader_hpp */
