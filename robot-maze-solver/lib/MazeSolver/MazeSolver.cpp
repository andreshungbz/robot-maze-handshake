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
        // 1. FRONT OPEN then FORWARD
        if (!frontWallDetected) {
            motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION_PROPORTION);
            break;
        }

        // 2. FRONT BLOCKED then DECIDE RIGHT TURN or LEFT TURN
        if (frontWallDetected) {
            // 2.A RIGHT OPEN then TURN RIGHT
            if (!rightWallDetected) {
                islandContainerWallCounter = 0;

                // right turn movement
                ble.write("[NORMAL] 2.A Right + Offset");
                handleBackoff();
                motors.pivotRight90();

                break;
            }

            // 2.B RIGHT BLOCKED then TURN LEFT
            ++islandContainerWallCounter; // increment counter
            if (islandContainerWallCounter == 4) {
                // change mode
                currentMode = Mode::IN_ISLAND;
                rgbLED.setRed();

                // double left turn movement (essentially reverse)
                ble.write("[ISLAND FOUND] 2.B Reverse + Offset");
                handleBackoff();
                motors.pivot180();
                handleRightOpeningOffset();
            }
            else {
                // left turn movement
                ble.write("[NORMAL] 2.B Left");
                handleBackoff();
                motors.pivotLeft90();
            }

            break;
        }

        break;
    }

    case Mode::IN_ISLAND: {
        // 1. FRONT BLOCKED then GOAL REACHED
        if (frontWallDetected) {
            handleBackoff();
            motors.stop();
            rgbLED.setGreen();
            reachedGoal = true;

            break;
        }

        // 2. FOLLOW RIGHT WALL
        motors.driveFollowRightWall(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION_PROPORTION, RIGHT_OPEN_THRESHOLD);
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
    islandContainerWallCounter = 0;
    rgbLED.turnOff();
}

// Helper Methods

void MazeSolver::handleRightOpeningOffset() {
    motors.driveForward();
    delay(1000);
}

void MazeSolver::handleUltrasonicPositionOffset() {
    motors.driveForward();
    delay(500);
}

void MazeSolver::handleBackoff() {
    motors.driveBackward();
    delay(100);
}
