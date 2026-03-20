#include "MazeSolver.h"

MazeSolver::MazeSolver(MotorController& motors, UltrasonicSensor& usSensor, LineSensor& lineSensor)
    : motors(motors), usSensor(usSensor), lfSensor(lineSensor) {}

void MazeSolver::update() {
    if (reachedGoal) return;

    uint16_t rightDistance{usSensor.getDistanceCm()};
    bool rightWallDetected{rightDistance < RIGHT_OPEN_THRESHOLD};
    bool frontWallDetected{lfSensor.isWallAhead()};

    // 1. RIGHT=OPEN: turn right
    if (!rightWallDetected) {
        leftTurnCounter = 0; // reset island check counter
        handleTurnRight();
        return;
    }

    // 2. RIGHT=BLOCKED & FRONT=BLOCKED: turn left
    if (frontWallDetected) {
        // final goal finish point check
        if (inIsland) {
            // increment right-wall event if right sensor detects wall
            if (rightWallDetected) {
                ++rightWallEventCounter;
            } else {
                rightWallEventCounter = 0; // reset right wall check counter
            }

            // the first dead end in the first island is considered the goal,
            if (rightWallEventCounter >= GOAL_THRESHOLD) {
                reachedGoal = true;

                // align robot back towards dead end, then exit
                handleTurnLeft();
                handleTurnLeft();
                return;
            }
        }

        handleTurnLeft();
        ++leftTurnCounter; // increment island check counter

        // if enough consecutive left turns are made to assume an island,
        // do one additional turn left to essentially reverse direction, then set island flag
        if (!inIsland && leftTurnCounter >= LEFT_TURN_ISLAND_THRESHOLD) {
            inIsland = true;
            leftTurnCounter = 0; // reset island check counter
            handleTurnLeft();
            return;
        }

        return;
    }

    // 3. RIGHT=BLOCKED & FRONT=OPEN: move forward with correction
    leftTurnCounter = 0; // reset island check counter
    rightWallEventCounter = 0; // reset right wall check counter
    motors.driveForwardWithCorrection(rightDistance, TARGET, CORRECTION);
}

bool MazeSolver::isGoalReached() const {
    return reachedGoal; 
}

// Helper Methods

void MazeSolver::handleTurnRight() {
    motors.pivotRight90();

    // allow some driving forward to account for the previous opening when turning right
    motors.driveForward();
    delay(200);
}

void MazeSolver::handleTurnLeft() {
    motors.pivotLeft90();
}
