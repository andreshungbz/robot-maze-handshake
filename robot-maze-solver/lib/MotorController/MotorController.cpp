#include "MotorController.h"

// forward declaration for function in hardware.cpp
void hw_set_motor_speed(int leftSpeed, int rightSpeed);

// Public Methods

void MotorController::driveForwardWithCorrection(uint16_t distanceCm, uint16_t target, float kP) {
    int error = (int)target - (int)distanceCm;

    // proportional control signal
    int turn = (int)(error * kP);

    // clamp correction to avoid extreme motor imbalance
    if (turn > 40) turn = 40;
    if (turn < -40) turn = -40;

    int leftSpeed = BASE_SPEED + turn;
    int rightSpeed = BASE_SPEED - turn;

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
