#pragma once
#include <Arduino.h>

// MotorController controls the Motor module.
class MotorController {
public:
    // MotorController Configuration

    static constexpr int BASE_SPEED{ 150 };

    // driveForwardWithCorrection readjusts robot path by examining the
    // distance to the target wall.
    void driveForwardWithCorrection(uint16_t distanceCm, uint16_t target, uint16_t correction);

    // driveForward moves the robot forwards.
    void driveForward(int speed = BASE_SPEED);
    // driveBackward moves the robot backwards.
    void driveBackward(int speed = BASE_SPEED);

    // pivotLeft turns the robot to the left.
    void pivotLeft(int speed);
    // pivotRight moves the robot to the right.
    void pivotRight(int speed);

    // pivotLeft90 turns the robot 90 degrees to the left.
    void pivotLeft90();
    // pivotRight90 turns the robot 90 degrees to the right.
    void pivotRight90();

    // stop makes the robot wheels stop moving.
    void stop();

private:
    // MotorController Configuration

    static constexpr int LEFT_TURN_90_SPEED{ 130 };
    static constexpr int LEFT_TURN_90_DELAY{ 500 };
    static constexpr int RIGHT_TURN_90_SPEED{ 130 };
    static constexpr int RIGHT_TURN_90_DELAY{ 500 };

    // Helper Methods

    // drive interfaces with the hardware to change the motor speeds, driving the robot.
    void drive(int leftSpeed, int rightSpeed);
};
