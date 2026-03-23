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

// lower-level objects
BLEController ble{};
LineSensor lfs{};
MotorController motors{};
RGBLEDController rgbLED{};
UltrasonicSensor us{};
// higher-level objects
MazeSolver mazeSolver{motors, us, lfs};
CommandParser parser{rgbLED, motors, currentMode};

// forward declaration for introduction
void introduction();

void setup() {
    Serial.begin(9600); // debugging/monitoring
    ble.begin(115200); // Makeblock BLEV1.0_Z module

    introduction();
}

void loop() {
    // [MANUAL] Mode
    if (currentMode == RobotMode::MANUAL) {
        // parse available BLE data
        if (ble.available()) {
            char cmd{ble.read()};
            Serial.print("[MANUAL] Received Command: ");
            Serial.print(cmd);
            Serial.println();

            parser.parseCommand(cmd);
        }
    }

    // [AUTONOMOUS] Mode
    else if (currentMode == RobotMode::AUTONOMOUS) {
        // check for [MANUAL] mode override (input of 'M')
        if (ble.available()) {
            char cmd{ble.read()};
            if (cmd == 'M') {
                motors.stop();
                currentMode = RobotMode::MANUAL;
                Serial.println("[AUTONOMOUS] Overriding to [MANUAL] mode");
                return;
            }
        }

        // continue maze navigation using the right-hand rule.
        mazeSolver.update();

        // if the final point is achieved, turn on RGB LED and stop motors indefinitely
        if (mazeSolver.isGoalReached()) {
            motors.stop();
            rgbLED.setGreen();
            currentMode = RobotMode::MANUAL;
            Serial.println("[AUTONOMOUS] Goal reached! Switching to [MANUAL] mode");
            return;
        }
    }

    delay(AUTONOMOUS_MAIN_LOOP_DELAY);
}

// introduction displays project information and links.
void introduction() {
    const char* PURPLE = "\x1B[95m";
    const char* RESET  = "\x1B[0m";

    Serial.print(PURPLE);
    Serial.print("[robot-maze-solver]");
    Serial.print(RESET);
    Serial.println("Program that navigates a robot through a maze using the right-hand rule (or manually)");

    Serial.print(PURPLE);
    Serial.print("[GitHub Repository]");
    Serial.print(RESET);
    Serial.println(" https://github.com/andreshungbz/robot-maze-handshake");

    Serial.print(PURPLE);
    Serial.print("[Project Subfolder]");
    Serial.print(RESET);
    Serial.println(" https://github.com/andreshungbz/robot-maze-handshake/tree/main/robot-maze-solver");

    Serial.println();
}
