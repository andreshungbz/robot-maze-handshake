#pragma once

#include "RGBLEDController.h"
#include "RobotMode.h"
#include "MotorController.h"

// CommandParser parses and activates commands sent from the robot-bt-controller
// program while the robot is in [MANUAL] mode.
class CommandParser {
public:
    // constructor
    CommandParser(RGBLEDController& led, MotorController& motors, RobotMode& mode)
        : rgbLED(led), motors(motors), currentMode(mode), RGBLEDOn(false) {
    }

    // parseCommand examines commands and activates the appropriate module or motors.
    void parseCommand(char cmd);

private:
    // Data Members

    RGBLEDController& rgbLED;
    MotorController& motors;
    RobotMode& currentMode;
    bool RGBLEDOn;
};
