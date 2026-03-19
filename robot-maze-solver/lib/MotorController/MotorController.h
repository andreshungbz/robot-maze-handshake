#pragma once
#include <Arduino.h>

// MotorController controls the Motor module.
class MotorController {
public:
    // drive is a lower-level method for setting wheel speeds independently.
    void drive(int leftSpeed, int rightSpeed);

    // Higher-level Methods using drive.

    // driveForward moves the robot forwards.
    void driveForward(int speed);
    // driveBackward moves the robot backwards.
    void driveBackward(int speed);

    // pivotLeft turns the robot to the left.
    void pivotLeft(int speed);
    // pivotLeft90 turns the robot 90 degrees to the left.
    void pivotLeft90();
    // pivotRight moves the robot to the right.
    void pivotRight(int speed);
    // pivotRight90 turns the robot 90 degrees to the right.
    void pivotRight90();
    
    // stop makes the robot wheels stop moving.
    void stop();

private:
    // setSpeed interfaces with the hardware to change the motor speeds.
    void setSpeed(int leftSpeed, int rightSpeed);
};
