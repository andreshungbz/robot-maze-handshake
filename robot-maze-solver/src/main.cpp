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
constexpr unsigned long AUTONOMOUS_MAIN_LOOP_DELAY{ 20 }; // main loop delay
RobotMode currentMode{ RobotMode::MANUAL }; // starting mode

// Object Initializations

// lower-level objects
BLEController ble{};
LineSensor lfs{};
MotorController motors{};
RGBLEDController rgbLED{};
UltrasonicSensor us{};
// higher-level objects
MazeSolver mazeSolver{ ble, lfs, motors, rgbLED, us };
CommandParser parser{ ble, rgbLED, motors, currentMode };

// forward declaration for introduction
void introduction();

void setup() {
    Serial.begin(9600); // debugging/monitoring
    ble.begin(115200); // Makeblock BLEV1.0_Z module

    introduction();

    // testing forward offset (debugging)
    // motors.driveForward();
    // delay(1200);
    // motors.stop();
}

void loop() {
    // [MANUAL] Mode
    if (currentMode == RobotMode::MANUAL) {
        // sensor tests (debugging)
        // Serial.println(lfs.getSensors());
        // Serial.println(us.getDistanceCm());

        // parse available BLE data
        while (ble.available()) {
            char cmd{ ble.read() };

            // echo command back (debugging)
            // ble.write(cmd);

            parser.parseCommand(cmd);
        }
    }

    // [AUTONOMOUS] Mode
    else if (currentMode == RobotMode::AUTONOMOUS) {
        // check for [MANUAL] mode override (input of 'M')
        if (ble.available()) {
            char cmd{ ble.read() };
            if (cmd == 'M') {
                motors.stop();
                mazeSolver.resetAll();
                currentMode = RobotMode::MANUAL;
                ble.write("[AUTONOMOUS] Overriding to [MANUAL] mode");
                return;
            }
        }

        // continue maze navigation using the right-hand rule.
        mazeSolver.update();

        // if the final point is achieved, switch back to [MANUAL] mode
        if (mazeSolver.isGoalReached()) {
            mazeSolver.resetAll();
            currentMode = RobotMode::MANUAL;
            ble.write("[AUTONOMOUS] Goal reached! Switching to [MANUAL] mode");
            return;
        }
    }

    delay(AUTONOMOUS_MAIN_LOOP_DELAY);
}

// introduction displays project information and links.
void introduction() {
    Serial.println("[robot-maze-solver] Program that navigates a robot through a maze using the right-hand rule (or manually)");
    Serial.println("[GitHub Repository] https://github.com/andreshungbz/robot-maze-handshake");
    Serial.println("[Project Subfolder] https://github.com/andreshungbz/robot-maze-handshake/tree/main/robot-maze-solver");
    Serial.println();
}
