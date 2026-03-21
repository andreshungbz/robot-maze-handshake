#include <Arduino.h>

#include "BLEController.h"
#include "LineSensor.h"
#include "MazeSolver.h"
#include "MotorController.h"
#include "RGBLEDController.h"
#include "UltrasonicSensor.h"

// Mode represents the configurations of the program.
enum class Mode {
    MANUAL, // the robot is controlled via the robot-bt-controller program.
    AUTONOMOUS // the robot navigates the maze automatically using the right-hand rule.
};

// Mode Configuration

constexpr unsigned long AUTONOMOUS_MAIN_LOOP_DELAY{50};

// Component initializations
BLEController ble{};
LineSensor lfs{};
MotorController motors{};
RGBLEDController rgbLED{};
UltrasonicSensor us{};

// MazeSolver initialization
MazeSolver mazeSolver{motors, us, lfs};

// Toggle-able variables and their initial values
Mode currentMode{Mode::MANUAL};
bool RGBLEDOn{false};

void setup() {
    Serial.begin(9600); // debugging/monitoring
    ble.begin(115200); // Makeblock BLEV1.0_Z module

    Serial.println("[robot-maze-solver] Starting in [MANUAL] mode...");
}

void loop() {
    // [MANUAL] Mode
    if (currentMode == Mode::MANUAL) {
        // parse available BLE data
        if (ble.available()) {
            char cmd = ble.read();
            switch (cmd) {
                // RGB LED Toggle
                case 'G':
                    if (RGBLEDOn) {
                        rgbLED.turnOff();
                        Serial.println("[MANUAL]: RGB LED turned OFF");
                    } else {
                        rgbLED.setGreen();
                        Serial.println("[MANUAL]: RGB LED set to GREEN");
                    }
                    RGBLEDOn = !RGBLEDOn;
                    break;

                // [AUTONOMOUS] Mode
                case 'A':
                    currentMode = Mode::AUTONOMOUS;
                    Serial.println("[robot-maze-solver] switched to [AUTONOMOUS] mode");
                    break;
            }
        }
    }

    // [AUTONOMOUS] Mode
    else if (currentMode == Mode::AUTONOMOUS) {
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
