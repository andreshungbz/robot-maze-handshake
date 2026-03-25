#include "MeMegaPi.h"

/* CONFIGURATION: Me RGB LED Module */

MeRGBLed led{ PORT_6 };
constexpr uint8_t LED_COUNT{ 4 };

// hw_set_rgb_led_color sets all LED components to one color.
void hw_set_rgb_led_color(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t t = 0; t < LED_COUNT; ++t) {
        led.setColorAt(t, r, g, b);
    }

    led.show();
}

/* CONFIGURATION: Me DC Motor */

MeMegaPiDCMotor leftMotor{ PORT1B };
MeMegaPiDCMotor rightMotor{ PORT2B };

// hw_set_motor_speed sets the running speed of the motors to turn the wheels.
void hw_set_motor_speed(int leftSpeed, int rightSpeed) {
    leftMotor.run(leftSpeed);
    rightMotor.run(-rightSpeed); // flip right motor since it is reversed on the robot
}

/* CONFIGURATION: Me Ultrasonic Sensor */

MeUltrasonicSensor ultrasonic{ PORT_7 };

// hw_get_ultrasonic_distance returns distance in cm.
uint16_t hw_get_ultrasonic_distance() {
    return ultrasonic.distanceCm();
}

/* CONFIGURATION: Me Line Follower Sensor */

MeLineFollower lineFinder{ PORT_8 };

// hw_read_line_sensor returns the sensor values for the MeLineFollower.
uint8_t hw_read_line_sensor() {
    // From the readSensors() documentation:
    // (0x00)-S1_IN_S2_IN:   sensor1 and sensor2 are both inside of black line
    // (0x01)-S1_IN_S2_OUT:  sensor1 is inside of black line and sensor2 is outside of black line
    // (0x02)-S1_OUT_S2_IN:  sensor1 is outside of black line and sensor2 is inside of black line
    // (0x03)-S1_OUT_S2_OUT: sensor1 and sensor2 are both outside of black line
    return lineFinder.readSensors();
}

/* CONFIGURATION: Makeblock BLEV1.0_Z module */

// hw_ble_begin initializes the UART used by BLE module
void hw_ble_begin(long baudRate) {
    Serial3.begin(baudRate);
}

// hw_ble_available returns true if data is available.
bool hw_ble_available() {
    return Serial3.available() > 0;
}

// hw_ble_read reads one byte.
char hw_ble_read() {
    if (Serial3.available() > 0) {
        return Serial3.read();
    }
    return 0;
}

// hw_ble_write writes one byte.
void hw_ble_write(char c) {
    Serial3.write(c);
}
