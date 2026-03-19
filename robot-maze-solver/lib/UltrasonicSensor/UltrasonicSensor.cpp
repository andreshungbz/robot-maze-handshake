#include "UltrasonicSensor.h"

// forward declaration
int hw_get_ultrasonic_distance();

uint16_t UltrasonicSensor::getDistanceCm() {
    return hw_get_ultrasonic_distance();
}
