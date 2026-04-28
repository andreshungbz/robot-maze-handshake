#include "MotorController.h"

// forward declaration for function in hardware.cpp
void hw_set_motor_speed(int leftSpeed, int rightSpeed);

// Public Methods

void MotorController::driveForwardWithCorrection(uint16_t distanceCm, uint16_t target, float kP) {
    static float lastTurn = 0;  // smoothing memory
    int error = (int)target - (int)distanceCm;

    // ignore smaller differences
    const int DEADZONE = 2;
    if (abs(error) < DEADZONE) {
        drive(BASE_SPEED, BASE_SPEED);
        lastTurn = 0;
        return;
    }

    // use proportional control
    float turn = error * kP;

    // clamp protection
    const float MAX_TURN = 30.0f;
    if (turn > MAX_TURN) turn = MAX_TURN;
    if (turn < -MAX_TURN) turn = -MAX_TURN;

    // smooth correction
    const float SMOOTHING = 0.7f; // closer to 1 = smoother
    turn = SMOOTHING * lastTurn + (1.0f - SMOOTHING) * turn;
    lastTurn = turn;

    // apply correction
    int leftSpeed = BASE_SPEED - (int)turn;
    int rightSpeed = BASE_SPEED + (int)turn;

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
