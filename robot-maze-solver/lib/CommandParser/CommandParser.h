#pragma once

#include "BLEController.h"
#include "MotorController.h"
#include "RGBLEDController.h"
#include "RobotMode.h"

// CommandParser parses and activates commands sent from the robot-bt-controller
// program while the robot is in [MANUAL] mode.
class CommandParser {
public:
    // constructor
    CommandParser(BLEController& ble, RGBLEDController& led, MotorController& motors, RobotMode& mode)
        : ble(ble), rgbLED(led), motors(motors), currentMode(mode), RGBLEDOn(false) {
    }

    // parseCommand examines commands and activates the appropriate module or motors.
    void parseCommand(char cmd);

private:
    // Data Members

    BLEController& ble;
    RGBLEDController& rgbLED;
    MotorController& motors;
    RobotMode& currentMode;
    bool RGBLEDOn;
};
