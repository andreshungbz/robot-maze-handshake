#include "CommandParser.h"
#include <Arduino.h>

void CommandParser::parseCommand(char cmd) {
    switch (cmd) {
        // RGB LED Toggle
    case 'X':
        if (RGBLEDOn) {
            rgbLED.turnOff();
            // ble.write("[MANUAL] RGB LED turned OFF");
        }
        else {
            rgbLED.setGreen();
            // ble.write("[MANUAL] RGB LED set to GREEN");
        }
        RGBLEDOn = !RGBLEDOn;
        break;

        // Drive Forward
    case 'F':
        motors.driveForward(175);
        // ble.write("[MANUAL] Moving forward");
        break;

        // Drive Backwards
    case 'B':
        motors.driveBackward(175);
        // ble.write("[MANUAL] Moving backward");
        break;

        // Turn Left 90 Degrees
    case 'L':
        motors.pivotLeft90();
        // ble.write("[MANUAL] Pivot left 90°");
        break;

        // Turn Right 90 Degrees
    case 'R':
        motors.pivotRight90();
        // ble.write("[MANUAL] Pivot right 90°");
        break;

        // Pivot Left Gradually
    case '1':
        motors.pivotLeft(75);
        // ble.write("[MANUAL] Pivot left gradually");
        break;

        // Pivot Right Gradually
    case '2':
        motors.pivotRight(75);
        // ble.write("[MANUAL] Pivot right gradually");
        break;

        // Stop
    case 'S':
        motors.stop();
        // ble.write("[MANUAL] Stop motors");
        break;

        // Switch to [AUTONOMOUS] Mode
    case 'A':
        currentMode = RobotMode::AUTONOMOUS;
        ble.write("[MANUAL] Switching to [AUTONOMOUS] mode...");
        break;

        // Redundancy message for [MANUAL] Mode
    case 'M':
        ble.write("[MANUAL] Already in [MANUAL] mode");
        break;

    default:
        ble.write("[MANUAL] Unknown Command");
    }
}
