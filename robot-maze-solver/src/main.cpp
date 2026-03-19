#include <Arduino.h>

#include "RGBLEDController.h"
#include "MotorController.h"

RGBLEDController rgbLED;
MotorController motors;

void setup() {
    // rgbLED.setGreen();

    // forward
    motors.driveForward(125);
    delay(3500);
    motors.stop();
    delay(1000);

    // pivot right
    motors.pivotRight(125);
    delay(500);
    motors.stop();
    delay(1000);
    
    // pivot right
    motors.pivotRight(125);
    delay(490);
    motors.stop();
    delay(1000);

    // pivot right
    motors.pivotRight(125);
    delay(490);
    motors.stop();
    delay(1000);

    // pivot right
    motors.pivotRight(125);
    delay(490);
    motors.stop();
    delay(1000);

    // pivot left
    motors.pivotLeft(120);
    delay(490);
    motors.stop();
    delay(1000);

    // pivot left
    motors.pivotLeft(120);
    delay(500);
    motors.stop();
    delay(1000);

    // pivot left
    motors.pivotLeft(120);
    delay(500);
    motors.stop();
    delay(1000);

    // pivot left
    motors.pivotLeft(120);
    delay(500);
    motors.stop();
    delay(1000);

    // reverse
    motors.driveBackward(175);
    delay(250);
    motors.stop();
}

void loop() {

}
