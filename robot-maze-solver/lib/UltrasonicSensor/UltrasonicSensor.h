#pragma once
#include <Arduino.h>

// UltrasonicSensor senses from the ultrasonic module.
class UltrasonicSensor {
public:
    // getDistanceCm returns the distance from a bounced wave in centimeters.
    uint16_t getDistanceCm();
};
