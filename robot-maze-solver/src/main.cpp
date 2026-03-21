#include <Arduino.h>

#include "BLEController.h"
#include "CommandParser.h"
#include "LineSensor.h"
#include "MazeSolver.h"
#include "MotorController.h"
#include "RGBLEDController.h"
#include "RobotMode.h"
#include "UltrasonicSensor.h"

// Main Configuration
constexpr unsigned long AUTONOMOUS_MAIN_LOOP_DELAY{50}; // main loop delay
RobotMode currentMode{RobotMode::MANUAL}; // starting mode

// Object Initializations

// components
BLEController ble{};
LineSensor lfs{};
MotorController motors{};
RGBLEDController rgbLED{};
UltrasonicSensor us{};
// Maze Solver
MazeSolver mazeSolver{motors, us, lfs};
// Command Parser
CommandParser parser{rgbLED, motors, currentMode};

void setup() {
    Serial.begin(9600); // debugging/monitoring
    ble.begin(115200); // Makeblock BLEV1.0_Z module

    Serial.println("[robot-maze-solver] Starting in [MANUAL] mode...");
}

void loop() {
    // [MANUAL] Mode
    if (currentMode == RobotMode::MANUAL) {
        // parse available BLE data
        if (ble.available()) {
            char cmd = ble.read();
            Serial.print("[MANUAL] Received Command: ");
            Serial.print(cmd);
            Serial.println();

            parser.parseCommand(cmd);
        }
    }

    // [AUTONOMOUS] Mode
    else if (currentMode == RobotMode::AUTONOMOUS) {
        // continue maze navigation using the right-hand rule.
        mazeSolver.update();

        // if the final point is achieved, turn on RGB LED and stop motors indefinitely
        if (mazeSolver.isGoalReached()) {
            rgbLED.setGreen();
            motors.stop();

            Serial.println("[AUTONOMOUS] Goal reached! Robot stopped.");
            while (true) delay(1000);
        }
    }

    delay(AUTONOMOUS_MAIN_LOOP_DELAY);
}
