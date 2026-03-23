#include "UltrasonicSensor.h"

// forward declaration for function in hardware.cpp
int hw_get_ultrasonic_distance();

// Public Methods

uint16_t UltrasonicSensor::getDistanceCm() {
    // take 3 readings
    uint16_t a{ hw_get_ultrasonic_distance() };
    uint16_t b{ hw_get_ultrasonic_distance() };
    uint16_t c{ hw_get_ultrasonic_distance() };

    // return the average of the readings
    return (a + b + c) / 3;
}
