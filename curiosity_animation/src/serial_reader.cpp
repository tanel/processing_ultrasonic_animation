//
//  serial_reader.cpp
//  curiosity_animation
//
//  Created by Tanel Lebedev on 29/09/15.
//
//

#include "serial_reader.hpp"

int SerialReader::reading() const {
    if (values.empty()) {
        return 0;
    }
    return std::accumulate(values.begin(), values.end(), 0) / values.size();
}

const int kNumOfValues = 5;

void SerialReader::AddReading(const int value) {
    if (!value) {
        return;
    }
    std::cout << "serial thread input=" << value << std::endl;
    values.push_front(value);
    values.resize(std::min(kNumOfValues, int(values.size())));
}

void SerialReader::threadedFunction() {
    serialPort.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serialPort.getDeviceList();
    for (int i = 0; i < deviceList.size(); i++) {
        std::cout << i << ". serial device: " << deviceList[i].getDeviceName() << std::endl;
    }
    if (activeSerialPort < deviceList.size()) {
        if (!serialPort.setup(activeSerialPort, 9600)) {
            std::cerr << "Failed to connect to serial device! "
            << deviceList[activeSerialPort].getDeviceName() << std::endl;
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
            AddReading(ofToInt(s));
        }
    }
    
    // done
}