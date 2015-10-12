#include "stats_uploader.hpp"

void StatsUploader::threadedFunction() {

    while(isThreadRunning()) {
        bool wasDirty(false);
        {
            Poco::FastMutex::ScopedLock lock(mutex);
            wasDirty = dirty;
            dirty = false;
        }
        if (wasDirty && !uploadCommand.empty()) {
            long start = ofGetElapsedTimeMillis();
            std::cout << "Uploading stats" << std::endl;
            std::cout << ofSystem(uploadCommand) << std::endl;
            int millis = ofGetElapsedTimeMillis() - start;
            std::cout << "Upload of stats done in "
            << millis << " ms" << std::endl;
        }
        ofSleepMillis(1000);
    }

    // done
}