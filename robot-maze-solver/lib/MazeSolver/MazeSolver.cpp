#include "MazeSolver.h"

MazeSolver::MazeSolver(BLEController& ble, LineSensor& lineSensor, MotorController& motors, RGBLEDController& rgbLED, UltrasonicSensor& usSensor)
    : ble(ble), lfSensor(lineSensor), motors(motors), rgbLED(rgbLED), usSensor(usSensor) {
}

void MazeSolver::update() {
    // read sensors
    uint16_t rightDistance = usSensor.getDistanceCm();
    bool rightWallDetected = rightDistance < RIGHT_OPEN_THRESHOLD;
    bool frontWallDetected = lfSensor.isWallAhead();

    // if currently cooling down, stop when right wall is detected
    if (rightTurnCooldown == true) {
        rightTurnCooldown = rightWallDetected ? false : true;
    }

    switch (currentMode) {
    case Mode::NORMAL: {
        // 1. RIGHT OPEN (and not cooldown) then TURN RIGHT
        if (!rightWallDetected && !rightTurnCooldown) {
            islandContainerWallCounter = 0;
            rightTurnCooldown = true;

            // right turn movement
            ble.write("[NORMAL] 1. Right + Offset");
            motors.stop();
            handleUltrasonicPositionOffset();
            motors.pivotRight90();
            handleRightOpeningOffset();

            break;
        }

        // 2. RIGHT BLOCKED and FRONT BLOCKED then LEFT TURN
        if (frontWallDetected) {
            ++islandContainerWallCounter;

            if (islandContainerWallCounter == 4) {
                // change mode
                currentMode = Mode::IN_ISLAND;
                rightTurnCooldown = true;
                rgbLED.setRed();

                // double left turn movement (essentially reverse)
                ble.write("[ISLAND FOUND] 2.B Reverse + Offset");
                motors.stop();
                handleBackoff();
                motors.pivot180();
                handleRightOpeningOffset();
            }
            else {
                // left turn movement
                ble.write("[NORMAL] 2.A Left");
                motors.stop();
                handleBackoff();
                motors.pivotLeft90();
            }

            break;
        }

        // 3. FORWARD
        motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION_PROPORTION);
        break;
    }

    case Mode::IN_ISLAND: {
        // 1. RIGHT OPEN (and not cooldown) then TURN RIGHT
        if (!rightWallDetected && !rightTurnCooldown) {
            rightTurnCooldown = true;

            // right turn movement
            ble.write("[ISLAND] 1. Right + Offset");
            motors.stop();
            handleUltrasonicPositionOffset();
            motors.pivotRight90();
            handleRightOpeningOffset();

            break;
        }

        // 2. RIGHT BLOCKED and FRONT BLOCKED then GOAL REACHED
        if (frontWallDetected) {
            motors.stop();
            handleBackoff();
            motors.stop();
            rgbLED.setGreen();
            reachedGoal = true;

            break;
        }

        // 3. FORWARD
        motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION_PROPORTION);
        break;
    }
    }
}

bool MazeSolver::isGoalReached() const {
    return reachedGoal;
}

void MazeSolver::resetAll() {
    currentMode = Mode::NORMAL;
    reachedGoal = false;
    rightTurnCooldown = false;
    islandContainerWallCounter = 0;
    rgbLED.turnOff();
}

// Helper Methods

void MazeSolver::handleRightOpeningOffset() {
    motors.driveForward();
    delay(600);
}

void MazeSolver::handleUltrasonicPositionOffset() {
    motors.driveForward();
    delay(500);
}

void MazeSolver::handleBackoff() {
    motors.driveBackward();
    delay(100);
}
