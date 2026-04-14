#include "MazeSolver.h"

MazeSolver::MazeSolver(BLEController& ble, LineSensor& lineSensor, MotorController& motors, RGBLEDController& rgbLED, UltrasonicSensor& usSensor)
    : ble(ble), lfSensor(lineSensor), motors(motors), rgbLED(rgbLED), usSensor(usSensor) {
}

void MazeSolver::update() {
    // read sensors
    uint16_t rightDistance = usSensor.getDistanceCm();
    bool rightWallDetected = rightDistance < RIGHT_OPEN_THRESHOLD;
    bool frontWallDetected = lfSensor.isWallAhead();

    switch (currentMode) {
    case Mode::NORMAL: {
        // 1. RIGHT OPEN then TURN RIGHT
        if (!rightWallDetected) {
            islandContainerWallCounter = 0;

            // right turn movement
            ble.write("[NORMAL] 1. Right + Offset");
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
                rgbLED.setRed();

                // double left turn movement (essentially reverse)
                ble.write("[ISLAND FOUND] 2.B Reverse + Offset");
                handleBackoff();
                motors.pivotLeft90();
                motors.pivotLeft90();
                handleRightOpeningOffset();
            }
            else {
                // left turn movement
                ble.write("[NORMAL] 2.A Left");
                handleBackoff();
                motors.pivotLeft90();
            }

            break;
        }

        // 3. FORWARD
        motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION);
        break;
    }

    case Mode::IN_ISLAND: {
        // 1. RIGHT OPEN then TURN RIGHT
        if (!rightWallDetected) {
            rightWallBlockedCounter = 0;

            // right turn movement
            ble.write("[ISLAND] 1. Right + Offset");
            handleUltrasonicPositionOffset();
            motors.pivotRight90();
            handleRightOpeningOffset();

            break;
        }

        // 2. RIGHT BLOCKED and FRONT BLOCKED then CHECK GOAL or LEFT TURN
        if (frontWallDetected) {
            ++rightWallBlockedCounter; // one right wall was already detected
            handleBackoff();

            // consecutively check for the next 2 walls
            for (int i{ 1 }; i <= 2; ++i) {
                motors.pivotLeft90();
                uint16_t rDistance = usSensor.getDistanceCm();
                bool rWDetected = rDistance < RIGHT_OPEN_THRESHOLD;

                if (rWDetected) {
                    ++rightWallBlockedCounter;
                }
            }

            // X. GOAL REACHED (dead end in island)
            if (rightWallBlockedCounter >= GOAL_THRESHOLD) {
                ble.write("[ISLAND] 2.B Goal!");
                // reverse back to dead end, stop, and set RGB to green
                motors.pivotRight90();
                motors.pivotRight90();
                motors.stop();
                rgbLED.setGreen();
                reachedGoal = true;
            }
            // if the goal wasn't detected, then it must be a left turn
            else {
                ble.write("[ISLAND] 2.C Left");
                motors.pivotRight90();
            }

            rightWallBlockedCounter = 0;
            break;
        }

        // 3. FORWARD
        motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION);
        break;
    }
    }
}

bool MazeSolver::isGoalReached() const {
    return reachedGoal;
}

void MazeSolver::resetAll() {
    islandContainerWallCounter = 0;
    rightWallBlockedCounter = 0;
    currentMode = Mode::NORMAL;
    reachedGoal = false;

    rgbLED.turnOff();
}

// Helper Methods

void MazeSolver::handleRightOpeningOffset() {
    motors.driveForward();
    delay(550);
}

void MazeSolver::handleUltrasonicPositionOffset() {
    motors.driveForward();
    delay(350);
}

void MazeSolver::handleBackoff() {
    motors.driveBackward();
    delay(100);
}
