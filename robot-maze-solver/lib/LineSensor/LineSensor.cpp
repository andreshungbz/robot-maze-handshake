#include "LineSensor.h"

// forward declaration for function in hardware.cpp
uint8_t hw_read_line_sensor();

bool LineSensor::isWallAhead() {
    uint8_t value = getSensors();

    // anything not 0x00 (both sensors detecting black) is considered "white" and a valid value
    if (value != 0x00) {
        ++consecutiveDetections;
    } else {
        consecutiveDetections = 0;
    }

    return consecutiveDetections >= DETECTION_THRESHOLD;
}

uint8_t LineSensor::getSensors() {
    return hw_read_line_sensor();
}
