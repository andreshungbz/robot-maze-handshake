#pragma once

#include "RGBLEDController.h"
#include "RobotMode.h"
#include "MotorController.h"

class CommandParser {
public:
    // constructor
    CommandParser(RGBLEDController& led, MotorController& motors, RobotMode& mode)
        : rgbLED(led), motors(motors), currentMode(mode), RGBLEDOn(false) {}

    void parseCommand(char cmd);

private:
    RGBLEDController& rgbLED;
    MotorController& motors;
    RobotMode& currentMode;
    bool RGBLEDOn;
};
