#include <Arduino.h>

#include "RGBLEDController.h"
#include "MotorController.h"
#include "UltrasonicSensor.h"

RGBLEDController rgbLED;
MotorController motors;
UltrasonicSensor usSensor;

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

    delay(500);
}
