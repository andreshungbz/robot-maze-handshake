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
    enum class Mode { NORMAL, IN_ISLAND };
    Mode currentMode{ Mode::NORMAL };

    // Hardware Components

    BLEController& ble;
    LineSensor& lfSensor;
    MotorController& motors;
    RGBLEDController& rgbLED;
    UltrasonicSensor& usSensor;

    // Counters & Flags & Configurations

    bool reachedGoal{ false };
    bool rightTurnCooldown{ false };
    int islandContainerWallCounter{ 0 }; // detecting potential islands
    static constexpr int ISLAND_CONTAINER_WALL_THRESHOLD{ 4 }; // consecutive walls requiring left turns that indicate possible island
    static constexpr int RIGHT_OPEN_THRESHOLD{ 18 }; // distance considered a right opening (cm)
    static constexpr int RIGHT_WALL_DISTANCE_TARGET{ 6 }; // distance aimed against right wall while moving (cm)
    static constexpr float MOVEMENT_CORRECTION_PROPORTION{ 3.0f }; // motor speed correction when moving

    // Helper Methods

    // handleRightOpeningOffset drives the robot forward when making right turns so that
    // the ultrasonic sensor is not positioned against the opening from which the robot just came from.
    void handleRightOpeningOffset();
    // handleUltrasonicPositionOffset drives the robot forward when first detecting a right opening so
    // that the position of the ultrasonic sensor is accounted for.
    void handleUltrasonicPositionOffset();
    // handleBackoff drives the robot slightly backwards so that turns made when detecting the
    // front wall are not too close to the wall.
    void handleBackoff();
};
