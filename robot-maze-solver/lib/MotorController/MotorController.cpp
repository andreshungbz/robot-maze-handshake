#include "MotorController.h"

// forward declaration for function in hardware.cpp
void hw_set_motor_speed(int leftSpeed, int rightSpeed);

// Public Methods

void MotorController::driveForwardWithCorrection(uint16_t distanceCm, uint16_t target, uint16_t correction) {
    if (distanceCm > target) {
        // if too far from the wall, steer right
        drive(BASE_SPEED + correction, BASE_SPEED);
    }
    else if (distanceCm < target) {
        // if too close to the wall, steer left
        drive(BASE_SPEED, BASE_SPEED + correction);
    }
    else {
        // if at designated distance from the wall, move forward
        driveForward(BASE_SPEED);
    }
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
    drive(PIVOT_180_SPEED, -PIVOT_180_SPEED);
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
