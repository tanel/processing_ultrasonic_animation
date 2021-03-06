//
//  serial_reader.cpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#include "serial_reader.hpp"

int SerialReader::Reading() const {
    return reading;
}

void SerialReader::AddReading(const int value) {
    if (reading != value) {
        lastUserInputAt = ofGetElapsedTimeMillis();
    }
    reading = value;
}

void SerialReader::threadedFunction() {
    serialPort.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serialPort.getDeviceList();
    for (int i = 0; i < deviceList.size(); i++) {
        std::cout << i << ". serial device: " << deviceList[i].getDeviceName() << std::endl;
    }
    if (ActiveSerialPort < deviceList.size()) {
        if (!serialPort.setup(ActiveSerialPort, 9600)) {
            std::cerr << "Failed to connect to serial device! "
            << deviceList[ActiveSerialPort].getDeviceName() << std::endl;
        }
    }
    
    while(isThreadRunning()) {
        if (!serialPort.isInitialized()) {
            continue;
        }
        
        if (!serialPort.available()) {
            continue;
        }
        char c = serialPort.readByte();
        if ('\n' != c) {
            serialbuf << c;
            continue;
        }
        
        std::string s = serialbuf.str();
        serialbuf.str("");
        
        if (!s.empty()) {
            int n = ofToInt(s);
            if (!enabled) {
                std::cout << "serial reader is disabled, ignoring value " << s << std::endl;
            } else if (n > maxReading) {
                std::cout << "serial reader value is too big, ignoring value " << s << std::endl;
            } else {
                AddReading(n);
            }
        }
    }
    
    // done
}