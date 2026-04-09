#pragma once
#include <Arduino.h>

#include "BLEController.h"
#include "LineSensor.h"
#include "MotorController.h"
#include "RGBLEDController.h"
#include "UltrasonicSensor.h"

// MazeSolver directs a robot to solve a maze using the right-hand rule.
class MazeSolver {
public:
    // constructor
    MazeSolver(BLEController& ble, LineSensor& lineSensor, MotorController& motors, RGBLEDController& rgbLED, UltrasonicSensor& usSensor);

    // update keeps track of the maze state and directs the appropriate robot movement.
    void update();
    // isGoalReached returns whether or not the designated finish point is reached.
    bool isGoalReached() const;
    // resetAll reverts all flags, counters, and values to their initial states.
    void resetAll();

private:
    // Mode distinguishes the different logic taken by MazeSolver.
    enum class Mode { NORMAL, IN_ISLAND, RETRACING };
    Mode currentMode{ Mode::NORMAL };

    // Hardware Components
    BLEController& ble;
    LineSensor& lfSensor;
    MotorController& motors;
    RGBLEDController& rgbLED;
    UltrasonicSensor& usSensor;

    // Finish Point Counters & Flags
    bool reachedGoal{ false };
    int rightWallBlockedCounter{ 0 }; // detecting dead end in island (finish point)

    // Island Counters & Flags
    int leftTurnCounter{ 0 }; // detecting potential islands
    int rectangleSegmentLengths[4]{ 0, 0, 0, 0 }; // lengths between left turns
    int rectangleSegmentIndex{ 0 }; // current segment being recorded
    int currentRectangleSegmentLength{ 0 }; // running length of current segment

    // MazeSolver Configuration
    static constexpr int GOAL_THRESHOLD{ 3 }; // consecutive right wall blocks in an island that confirms finish point
    static constexpr int LEFT_TURN_ISLAND_THRESHOLD{ 4 }; // consecutive left turns that indicate possible island
    static constexpr int RIGHT_OPEN_THRESHOLD{ 12 }; // distance considered a right opening (cm)
    static constexpr int RIGHT_WALL_DISTANCE_TARGET{ 6 }; // distance aimed against right wall while moving (cm)
    static constexpr int MOVEMENT_CORRECTION{ 35 }; // motor speed correction value when moving
    static constexpr int RECTANGLE_SEGMENT_LENGTH_TOLERANCE{ 3 }; // allowed deviation in rectangle segment lengths

    // Helper Methods

    // handleForwardOffset drives the robot forward when making right turns so that
    // the ultrasonic sensor is not positioned against the opening from which the robot just came from.
    void handleForwardOffset();
    // handleBackoff drives the robot slightly backwards so that turns made when detecting the
    // front wall are not too close to the wall.
    void handleBackoff();

    // recordRectangleSegment takes note of the accumulated counter as a length.
    void recordRectangleSegment();
    // validateRectangleSegment validates a rectangle through its opposite sides and monotonicity.
    bool validateRectangleIsland();
    // resetIslandCheck clears the recorded rectangle segments and the left turn counter.
    void resetIslandCheck();
};
