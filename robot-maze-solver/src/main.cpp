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
    Serial.begin(9600); // debugging

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
    Serial.print("Me Ultrasonic Sensor: ");
    Serial.print(usSensor.getDistanceCm());
    Serial.println(" cm");

    uint8_t sensorVal = lfSensor.getSensors();
    Serial.print("Me Line Follower Sensor: 0x");
    Serial.println(sensorVal, HEX);

    delay(200);
}
