#pragma once
#include <Arduino.h>

// LineSensor senses using the line follower module.
class LineSensor {
public:
    // isWallAhead returns true if a wall is detected in front.
    bool isWallAhead();
    // getSensors returns the values read of the line follower sensor.
    uint8_t getSensors();

private:
    // consecutiveDetections is a stability counter to filter noisy readings.
    int consecutiveDetections{ 0 };

    // LineSensor Configuration

    // DETECTION_THRESHOLD represents the number of consecutive valid readings
    // are needed before confirming isWallAhead().
    static constexpr int DETECTION_THRESHOLD{ 3 };
};
