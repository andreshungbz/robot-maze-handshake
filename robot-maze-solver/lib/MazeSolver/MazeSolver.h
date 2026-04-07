#pragma once
#include <Arduino.h>

#include "LineSensor.h"
#include "MotorController.h"
#include "UltrasonicSensor.h"

// MazeSolver directs a robot to solve a maze using the right-hand rule.
class MazeSolver {
public:
    // constructor
    MazeSolver(MotorController& motors, UltrasonicSensor& usSensor, LineSensor& lineSensor);

    // update keeps track of the maze state and directs the appropriate robot movement.
    void update();
    // isGoalReached returns whether or not the designated finish point is reached.
    bool isGoalReached() const;

private:
    enum class Mode { NORMAL, IN_ISLAND, RETRACING };

    // Data Members

    MotorController& motors;
    UltrasonicSensor& usSensor;
    LineSensor& lfSensor;
    Mode currentMode{ Mode::NORMAL };

    // Counters & Flags

    bool reachedGoal{ false };
    int leftTurnCounter{ 0 }; // consecutive left turns to detect island
    int rightWallBlockedCounter{ 0 }; // consecutive right walls blocked to check inside an island

    // Segments (for confirming rectangular island)

    int segmentLengths[4]{ 0, 0, 0, 0 }; // lengths between left turns
    int segmentIndex{ 0 };           // current segment being recorded
    int currentSegmentLength{ 0 };   // running length of current segment

    // MazeSolver Configuration

    static constexpr int RIGHT_OPEN_THRESHOLD{ 12 }; // distance considered an opening to the right in cm
    static constexpr int LEFT_TURN_ISLAND_THRESHOLD{ 4 }; // consecutive left turns indicate a possible island
    static constexpr int LENGTH_TOLERANCE{ 3 }; // allowed deviation in segment lengths
    static constexpr int GOAL_THRESHOLD{ 3 };   // consecutive right wall blocks in an island confirms finish point
    static constexpr int TARGET{ 6 };           // target right-wall distance in cm
    static constexpr int CORRECTION{ 35 };      // motor correction factor

    // Helper Methods

    void handleTurnRight();
    void handleTurnLeft();
    void handleForwardOffset();
    void handleBackoff();

    void resetCounters();
    void resetSegments();

    // verification helpers
    void recordSegment();
    bool validateRectangle();
};
