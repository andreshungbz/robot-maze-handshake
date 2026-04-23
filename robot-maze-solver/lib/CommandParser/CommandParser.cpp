#include "CommandParser.h"
#include <Arduino.h>

void CommandParser::parseCommand(char cmd) {
    switch (cmd) {
        // RGB LED Toggle
    case 'X':
        if (RGBLEDOn) {
            // ble.write("[MANUAL] RGB LED turned OFF");
            rgbLED.turnOff();
        }
        else {
            // ble.write("[MANUAL] RGB LED set to GREEN");
            rgbLED.setGreen();
        }
        RGBLEDOn = !RGBLEDOn;
        break;

        // Drive Forward
    case 'F':
        // ble.write("[MANUAL] Moving forward");
        motors.driveForward(150);
        break;

        // Drive Backwards
    case 'B':
        // ble.write("[MANUAL] Moving backward");
        motors.driveBackward(150);
        break;

        // Turn Left 90 Degrees
    case 'L':
        // ble.write("[MANUAL] Pivot left 90°");
        motors.pivotLeft90();
        break;

        // Turn Right 90 Degrees
    case 'R':
        // ble.write("[MANUAL] Pivot right 90°");
        motors.pivotRight90();
        break;

        // Pivot Left Gradually
    case '1':
        // ble.write("[MANUAL] Pivot left gradually");
        motors.pivotLeft(55);
        break;

        // Pivot Right Gradually
    case '2':
        // ble.write("[MANUAL] Pivot right gradually");
        motors.pivotRight(55);
        break;

        // Stop
    case 'S':
        // ble.write("[MANUAL] Stop motors");
        motors.stop();
        break;

        // Switch to [AUTONOMOUS] Mode
    case 'A':
        ble.write("[MANUAL] Switching to [AUTONOMOUS] mode...");
        currentMode = RobotMode::AUTONOMOUS;
        break;

        // Switch to [AUTONOMOUS] Mode with Maze Solver set to IN_ISLAND Mode
    case 'I':
        ble.write("[MANUAL] Switching to [AUTONOMOUS] mode with [IN_ISLAND] set...");
        currentMode = RobotMode::AUTONOMOUS;
        mazeSolver.setInIsland();
        break;

        // Redundancy message for [MANUAL] Mode
    case 'M':
        ble.write("[MANUAL] Already in [MANUAL] mode");
        break;

    default:
        ble.write("[MANUAL] Unknown Command");
    }
}
