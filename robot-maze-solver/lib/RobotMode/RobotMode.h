#pragma once

// RobotMode represents the configurations of the program.
enum class RobotMode {
    MANUAL, // the robot is controlled via the robot-bt-controller program.
    AUTONOMOUS // the robot navigates the maze automatically using the right-hand rule.
};
