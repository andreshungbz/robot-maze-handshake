#include "MotorController.h"

// forward declaration
void hw_set_motor_speed(int leftSpeed, int rightSpeed);

void MotorController::drive(int leftSpeed, int rightSpeed) {
    setSpeed(leftSpeed, rightSpeed);
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

void MotorController::stop() {
    drive(0, 0);
}

void MotorController::setSpeed(int leftSpeed, int rightSpeed) {
    hw_set_motor_speed(leftSpeed, rightSpeed);
}
