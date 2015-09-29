//
//  serial_reader.hpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#ifndef serial_reader_hpp
#define serial_reader_hpp

#include <stdio.h>

#include "ofMain.h"

class SerialReader : public ofThread {
public:
    SerialReader()
    : activeSerialPort(0) {}
    
    void threadedFunction();
    
    void AddReading(const int value);
    
    int reading() const;
    
    int activeSerialPort;
    
private:
    // Serial port, for reading distance from ultrasonic sensor.
    ofSerial serialPort;
    std::stringstream serialbuf;
    
    std::deque<int> values;
};

#endif /* serial_reader_hpp */
