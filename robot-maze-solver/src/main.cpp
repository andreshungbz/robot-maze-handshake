#include <Arduino.h>

#include "RGBLEDController.h"
#include "MotorController.h"
#include "UltrasonicSensor.h"
#include "LineSensor.h"
#include "MazeSolver.h"

RGBLEDController rgbLED{};
MotorController motors{};
UltrasonicSensor usSensor{};
LineSensor lfSensor{};

MazeSolver mazeSolver{motors, usSensor, lfSensor};

void setup() {
    // debugging from the MegaPi serial port
    Serial.begin(9600);
}

void loop() {
    mazeSolver.update();

    if (mazeSolver.isGoalReached()) {
        rgbLED.setGreen();
        motors.stop();
        while (true) {
            delay(1000); // keep robot stopped
        }
    }

    delay(100);
}
