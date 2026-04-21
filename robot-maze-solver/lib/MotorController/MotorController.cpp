#include "MotorController.h"

// forward declaration for function in hardware.cpp
void hw_set_motor_speed(int leftSpeed, int rightSpeed);

// Public Methods

void MotorController::driveForwardWithCorrection(uint16_t distanceCm, uint16_t target, float kP) {
    int error = (int)target - (int)distanceCm;
    int turn = (int)(error * kP);

    int leftSpeed = BASE_SPEED - turn;
    int rightSpeed = BASE_SPEED + turn;

    drive(leftSpeed, rightSpeed);
}

void MotorController::driveFollowRightWall(uint16_t distanceCm, uint16_t target, float kP, int openThreshold) {
    int leftSpeed, rightSpeed;

    // normal proportional control
    if (distanceCm < openThreshold) {
        int error = (int)target - (int)distanceCm;
        int turn = (int)(error * kP);

        leftSpeed = BASE_SPEED - turn;
        rightSpeed = BASE_SPEED + turn;
    }
    // skew right with appropriate strength
    else {
        leftSpeed = BASE_SPEED + 30;
        rightSpeed = BASE_SPEED - 20;
    }

    // clamp speeds
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    drive(leftSpeed, rightSpeed);
}

void MotorController::driveForward(int speed) {
    drive(speed, speed);
}

void MotorController::driveBackward(int speed) {
    drive(-speed, -speed);
}

void MotorController::pivotLeft(int speed) {
    drive(-speed, speed);
}

void MotorController::pivotRight(int speed) {
    drive(speed, -speed);
}

void MotorController::pivotLeft90() {
    pivotLeft(LEFT_TURN_90_SPEED);
    delay(LEFT_TURN_90_DELAY);
    stop();
    delay(200);
}

void MotorController::pivotRight90() {
    pivotRight(RIGHT_TURN_90_SPEED);
    delay(RIGHT_TURN_90_DELAY);
    stop();
    delay(200);
}

void MotorController::pivot180() {
    drive(-PIVOT_180_SPEED, PIVOT_180_SPEED);
    delay(PIVOT_180_DELAY);
    stop();
    delay(200);
}

void MotorController::stop() {
    drive(0, 0);
}

// Private Helper Methods

void MotorController::drive(int leftSpeed, int rightSpeed) {
    hw_set_motor_speed(leftSpeed, rightSpeed);
}
