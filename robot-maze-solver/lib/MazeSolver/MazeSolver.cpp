#include "MazeSolver.h"

MazeSolver::MazeSolver(MotorController& motors, UltrasonicSensor& usSensor, LineSensor& lineSensor)
    : motors(motors), usSensor(usSensor), lfSensor(lineSensor) {
}

void MazeSolver::update() {
    // READ SENSORS

    uint16_t rightDistance = usSensor.getDistanceCm();
    bool rightWallDetected = rightDistance < RIGHT_OPEN_THRESHOLD;
    bool frontWallDetected = lfSensor.isWallAhead();

    switch (currentMode) {

    case Mode::NORMAL: {
        // 1. RIGHT OPEN → TURN RIGHT
        if (!rightWallDetected) {
            if (leftTurnCounter != 4) { // normal case when not on the 4th left turn
                leftTurnCounter = 0;
                resetSegments();
                handleBackoff();
                handleTurnRight();
                return;
            }
        }

        // 2. FRONT BLOCKED + RIGHT BLOCKED → LEFT TURN
        if (frontWallDetected) {
            if (leftTurnCounter != 4) {
                if (leftTurnCounter != 0) {
                    recordSegment();
                }

                ++leftTurnCounter;
                handleBackoff();
                handleTurnLeft();
                return;
            }
            else { // we should have 4 segments at this point, so we check if it's a valid rectangle
                recordSegment(); // ← capture the final segment

                if (segmentIndex == 4 && validateRectangle()) {
                    currentMode = Mode::IN_ISLAND;

                    // essentially reverse direction
                    handleBackoff();
                    handleTurnLeft();
                    handleTurnLeft();

                    resetSegments();
                    leftTurnCounter = 0;
                }
                else {
                    if (!rightWallDetected) {
                        currentMode = Mode::NORMAL;

                        handleBackoff();
                        handleTurnRight();

                        resetSegments();
                        leftTurnCounter = 0;

                        return;
                    }
                    else {
                        currentMode = Mode::RETRACING;

                        // essentially reverse direction
                        handleBackoff();
                        handleTurnLeft();
                        handleTurnLeft();

                        resetSegments();
                        leftTurnCounter = 0;
                    }
                }

                return;
            }
        }

        // 3. FORWARD
        currentSegmentLength++;
        motors.driveForwardWithCorrection(rightDistance, TARGET, CORRECTION);
        break;
    }

    case Mode::RETRACING: {
        if (frontWallDetected) {
            currentMode = Mode::NORMAL;
            // essentially reverse direction
            handleBackoff();
            handleTurnLeft();
            handleTurnLeft();
            return;
        }
        else {
            motors.driveForwardWithCorrection(rightDistance, TARGET, CORRECTION);
            return;
        }
    }

    case Mode::IN_ISLAND: {
        // 1. RIGHT OPEN → TURN RIGHT
        // in the island, we could still make some right turns before reaching a dead end
        if (!rightWallDetected) {
            rightWallBlockedCounter = 0;
            handleBackoff();
            handleTurnRight();
            return;
        }

        // GOAL DETECTION
        // detecting 3 consecutive right walls
        ++rightWallBlockedCounter;
        if (rightWallBlockedCounter >= GOAL_THRESHOLD) {
            reachedGoal = true;
            resetCounters();
            handleTurnLeft();
            handleTurnLeft();
            return;
        }

        // 2. FRONT BLOCKED → LEFT TURN
        if (frontWallDetected) {
            handleBackoff();
            handleTurnLeft();
            return;
        }

        // 3. FORWARD
        motors.driveForwardWithCorrection(rightDistance, TARGET, CORRECTION);
        break;
    }

    }
}

bool MazeSolver::isGoalReached() const {
    return reachedGoal;
}

// =========================
// Helper Methods
// =========================

void MazeSolver::handleTurnRight() {
    motors.pivotRight90();
    handleForwardOffset();
}

void MazeSolver::handleTurnLeft() {
    motors.pivotLeft90();
}

void MazeSolver::handleForwardOffset() {
    motors.driveForward();
    delay(1000);
}

void MazeSolver::handleBackoff() {
    motors.driveBackward();
    delay(250);
}

void MazeSolver::resetCounters() {
    leftTurnCounter = 0;
    rightWallBlockedCounter = 0;
}

void MazeSolver::resetSegments() {
    segmentIndex = 0;
    currentSegmentLength = 0;
    for (int i = 0; i < 4; ++i) segmentLengths[i] = 0;
}

void MazeSolver::recordSegment() {
    if (segmentIndex < 4) {
        segmentLengths[segmentIndex++] = currentSegmentLength;
        currentSegmentLength = 0;
    }
}

bool MazeSolver::validateRectangle() {
    int s0 = segmentLengths[0];
    int s1 = segmentLengths[1];
    int s2 = segmentLengths[2];
    int s3 = segmentLengths[3];

    bool oppositeMatch = abs(s0 - s2) <= LENGTH_TOLERANCE &&
        abs(s1 - s3) <= LENGTH_TOLERANCE;

    bool monotonic = (s0 > s1 && s1 > s2 && s2 > s3) ||
        (s0 < s1 && s1 < s2 && s2 < s3);

    return oppositeMatch && !monotonic;
}
