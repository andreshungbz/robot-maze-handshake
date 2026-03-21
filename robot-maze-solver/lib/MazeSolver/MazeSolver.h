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
    // It is currently considered to be the first dead end encountered in the first island reached.
    bool isGoalReached() const;

private:
    // Data Members

    MotorController& motors;
    UltrasonicSensor& usSensor;
    LineSensor& lfSensor;

    // Counters & Flags

    bool reachedGoal{false};
    int leftTurnCounter{0}; // consecutive left turns to detect island
    bool inIsland{false}; // true if robot is accounting for an island
    int rightWallEventCounter{0}; // consecutive right-wall events inside an island

    // MazeSolver Configuration

    static constexpr int RIGHT_OPEN_THRESHOLD{10}; // distance considered an opening to the right in cm
    // island and goal configuration
    static constexpr int LEFT_TURN_ISLAND_THRESHOLD{4}; // consecutive left turns indicate a possible island
    static constexpr int GOAL_THRESHOLD{3}; // consecutive right wall events while in an island confirms goal
    // right wall hugging correction configuration
    static constexpr int TARGET{6}; // target right-wall distance in cm
    static constexpr int CORRECTION{30}; // motor correction factor

    // Helper Methods

    // handleTurnRight pivots the robot to the right in the maze and moves forward a bit.
    void handleTurnRight();
    // handleTurnLeft pivots the robot to the left in the maze.
    void handleTurnLeft();
};
