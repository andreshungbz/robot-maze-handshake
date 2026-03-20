#include <Arduino.h>

#include "RGBLEDController.h"
#include "MotorController.h"
#include "UltrasonicSensor.h"
#include "LineSensor.h"

RGBLEDController rgbLED;
MotorController motors;
UltrasonicSensor usSensor;
LineSensor lfSensor;

void setup() {
    // debugging from the MegaPi serial port
    Serial.begin(9600);

    rgbLED.setGreen();

    motors.pivotLeft90();
    motors.pivotLeft90();
    motors.pivotLeft90();
    motors.pivotLeft90();

    motors.pivotRight90();
    motors.pivotRight90();
    motors.pivotRight90();
    motors.pivotRight90();
}

void loop() {
    // testing ultrasonic sensor
    Serial.print("Me Ultrasonic Sensor: ");
    Serial.print(usSensor.getDistanceCm());
    Serial.println(" cm");

    // testing line follower sensor
    uint8_t sensorVal = lfSensor.getSensors();
    Serial.print("Me Line Follower Sensor: 0x");
    Serial.println(sensorVal, HEX);

    delay(200);
}
