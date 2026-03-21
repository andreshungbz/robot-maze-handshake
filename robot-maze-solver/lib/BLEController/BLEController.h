#pragma once
#include <Arduino.h>

// BLEController provides a simple interface for reading/writing
// over the Makeblock BLEV1.0_Z module.
class BLEController {
public:
    // begin initializes the BLE module at a specific baud rate.
    void begin(long baudRate = 115200);
    // available returns true if data is available to read.
    bool available() const;
    // read reads a single byte from the BLE module.
    char read();
    // write writes a single byte over the BLE module.
    void write(char c);
    // write writes a string over the BLE module.
    void write(const char* str);
};
