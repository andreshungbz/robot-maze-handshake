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

    case Mode::NORMAL: { // Regular application of right-hand rule while checking for island
        // 1. RIGHT OPEN then TURN RIGHT
        if (!rightWallDetected) {
            // do only when not on a 4th consecutive left turn (island check)
            if (leftTurnCounter != 4) {
                resetIslandCheck();

                // right turn movement
                ble.write("[NORMAL] 1. Right");

                if (!first) {
                    handleUltrasonicPositionOffset();
                    first = false;
                }

                motors.pivotRight90();
                handleRightOpeningOffset();

                break;
            }
        }

        // 2. RIGHT BLOCKED and FRONT BLOCKED then LEFT TURN
        if (frontWallDetected) {
            // when not on a 4th consecutive left turn (island check)
            if (leftTurnCounter != 4) {
                // record starting on index 1 since that is when the first segment is measured
                if (leftTurnCounter != 0) {
                    recordRectangleSegment();
                }

                ++leftTurnCounter;

                // set RGB to blue when traversing 4th segment
                if (leftTurnCounter == 4) {
                    rgbLED.setBlue();
                }

                // left turn movement
                ble.write("[NORMAL] 2.A Left");
                handleBackoff();
                motors.pivotLeft90();
            }
            // when on the 4th consecutive left turn (island check)
            else {
                // record the last rectangle segment
                recordRectangleSegment();

                // if the rectangle is valid, we are in an island
                if (rectangleSegmentIndex == 4 && validateRectangleIsland()) {
                    // change mode
                    rgbLED.setRed();
                    currentMode = Mode::IN_ISLAND;

                    // reverse direction movement
                    ble.write("[VALID RECTANGLE] 2.B Reverse");
                    handleBackoff();
                    motors.pivotLeft90();
                    motors.pivotLeft90();
                    handleRightOpeningOffset();
                }
                // invalid rectangle (e.g. omega form)
                else {
                    rgbLED.turnOff();

                    // if there happens to be a right opening, then go through it and resume right-hand rule
                    if (!rightWallDetected) {
                        // right turn movement
                        ble.write("[INVALID RECTANGLE] 2.C Right");
                        handleBackoff();
                        motors.pivotRight90();
                        handleRightOpeningOffset();

                        rgbLED.turnOff();
                    }
                    // else we need to retrace the last segment by reversing
                    else {
                        // change mode
                        currentMode = Mode::RETRACING;

                        // reverse direction movement (no forward offset as we won't check left anyways)
                        ble.write("[INVALID RECTANGLE] 2.D Reverse");
                        handleBackoff();
                        motors.pivotLeft90();
                        motors.pivotLeft90();
                    }
                }

                // regardless of 4th consecutive left turn outcome, reset counter and segments
                resetIslandCheck();
            }

            break;
        }

        // 3. FORWARD
        ++currentRectangleSegmentLength;// increment segment length that may be recorded
        // when not on a 4th consecutive left turn (island check), drive with correction
        if (leftTurnCounter != 4) {
            motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION);
        }
        // decreased movement correction
        else {
            motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, 10);
        }

        break;
    }

    case Mode::RETRACING: { // When retracing the 4th segment is necessary (rectangle validation failed)
        // keep going forward until the first wall is hit, then reverse and resume right hand rule
        if (frontWallDetected) {
            // change mode
            currentMode = Mode::NORMAL;

            // reverse direction movement
            ble.write("[RETRACING] Reverse");
            handleBackoff();
            motors.pivotLeft90();
            motors.pivotLeft90();

            rgbLED.turnOff();
        }
        else {
            motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION);
        }

        break;
    }

    case Mode::IN_ISLAND: { // Continue traversal until finish point is found (first dead end)
        // 1. RIGHT OPEN then TURN RIGHT
        if (!rightWallDetected) {
            rightWallBlockedCounter = 0;

            // right turn movement
            ble.write("[ISLAND] 1. Right");
            handleUltrasonicPositionOffset();
            motors.pivotRight90();
            handleRightOpeningOffset();

            break;
        }

        // 2. RIGHT BLOCKED and FRONT BLOCKED then CHECK GOAL or LEFT TURN
        if (frontWallDetected) {
            ble.write("[ISLAND] 2.A Checking Goal");
            // X. FINISH POINT (GOAL) DETECTION
            ++rightWallBlockedCounter; // one right wall was already detected
            // consecutively check for next 2 walls
            for (int i{ 1 }; i <= 2; ++i) {
                motors.pivotLeft90();
                uint16_t rDistance = usSensor.getDistanceCm();
                bool rWDetected = rDistance < RIGHT_OPEN_THRESHOLD;

                if (rWDetected) {
                    ++rightWallBlockedCounter;
                }
            }

            // undo right wall checks
            motors.pivotRight90();
            motors.pivotRight90();

            // if we reached a dead end in the island, set goal flag
            if (rightWallBlockedCounter >= GOAL_THRESHOLD) {
                ble.write("[ISLAND] 2.B Goal!");
                motors.stop();
                rgbLED.setGreen();
                reachedGoal = true;

                rightWallBlockedCounter = 0;

                break;
            }

            // if the goal wasn't detected, then do a regular left turn
            ble.write("[ISLAND] 2.C Left");
            handleBackoff();
            motors.pivotLeft90();

            rightWallBlockedCounter = 0;

            break;
        }

        // 3. FORWARD
        motors.driveForwardWithCorrection(rightDistance, RIGHT_WALL_DISTANCE_TARGET, MOVEMENT_CORRECTION);
        break;
    }
    }

    first = false;
}

bool MazeSolver::isGoalReached() const {
    return reachedGoal;
}

void MazeSolver::resetAll() {
    resetIslandCheck();
    rightWallBlockedCounter = 0;
    currentMode = Mode::NORMAL;
    first = true;
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

void MazeSolver::recordRectangleSegment() {
    if (rectangleSegmentIndex < 4) {
        // debugging
        char buffer[12];
        itoa(currentRectangleSegmentLength, buffer, 10);
        ble.write("[SEGMENT] Length ");
        ble.write(buffer);
        ble.write("\n");

        rectangleSegmentLengths[rectangleSegmentIndex++] = currentRectangleSegmentLength;
        currentRectangleSegmentLength = 0;
    }
}

bool MazeSolver::validateRectangleIsland() {
    int s0 = rectangleSegmentLengths[0];
    int s1 = rectangleSegmentLengths[1];
    int s2 = rectangleSegmentLengths[2];
    int s3 = rectangleSegmentLengths[3];

    // check opposite sides having more or less equal lengths
    bool oppositeMatch = abs(s0 - s2) <= RECTANGLE_SEGMENT_LENGTH_TOLERANCE &&
        abs(s1 - s3) <= RECTANGLE_SEGMENT_LENGTH_TOLERANCE;

    // check for strictly increasing and strictly decreasing values (not rectangle)
    bool monotonic = (s0 > s1 && s1 > s2 && s2 > s3) ||
        (s0 < s1 && s1 < s2 && s2 < s3);

    return oppositeMatch && !monotonic;
}

void MazeSolver::resetIslandCheck() {
    leftTurnCounter = 0;

    rectangleSegmentIndex = 0;
    currentRectangleSegmentLength = 0;
    for (int i = 0; i < 4; ++i) rectangleSegmentLengths[i] = 0;
}
