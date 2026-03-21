#include "CommandParser.h"
#include <Arduino.h>

void CommandParser::parseCommand(char cmd) {
    switch (cmd) {
        // RGB LED Toggle
        case 'X':
            if (RGBLEDOn) {
                rgbLED.turnOff();
                Serial.println("[MANUAL] RGB LED turned OFF");
            } else {
                rgbLED.setGreen();
                Serial.println("[MANUAL] RGB LED set to GREEN");
            }
            RGBLEDOn = !RGBLEDOn;
            break;

        // Drive Forward
        case 'F':
            motors.driveForward(motors.BASE_SPEED);
            Serial.println("[MANUAL] Moving forward");
            break;

        // Drive Backwards
        case 'B':
            motors.driveBackward(motors.BASE_SPEED);
            Serial.println("[MANUAL] Moving backward");
            break;

        // Turn Left 90 Degrees
        case 'L':
            motors.pivotLeft90();
            Serial.println("[MANUAL] Pivot left 90°");
            break;

        // Turn Right 90 Degrees
        case 'R':
            motors.pivotRight90();
            Serial.println("[MANUAL] Pivot right 90°");
            break;

        // Pivot Left Gradually
        case '1':
            motors.pivotLeft(125);
            Serial.println("[MANUAL] Pivot left gradually");
            break;

        // Pivot Right Gradually
        case '2':
            motors.pivotRight(125);
            Serial.println("[MANUAL] Pivot right gradually");
            break;

        // Switch to [AUTONOMOUS] Mode
        case 'A':
            currentMode = RobotMode::AUTONOMOUS;
            Serial.println("[MANUAL] Switching to [AUTONOMOUS] mode...");
            break;

        default:
            Serial.println("[MANUAL] Unknown Command");
    }
}
