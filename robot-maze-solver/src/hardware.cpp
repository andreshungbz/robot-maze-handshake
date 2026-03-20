#include "MeMegaPi.h"

/* Configuration: Me RGB LED Module */

MeRGBLed led(PORT_5);
constexpr uint8_t LED_COUNT{4};

// hw_set_rgb_led_color sets all LED components to one color.
void hw_set_rgb_led_color(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t t = 0; t < LED_COUNT; ++t) {
        led.setColorAt(t, r, g, b);
    }
    
    led.show();
}

/* Configuration: Me DC Motor */

MeMegaPiDCMotor leftMotor(PORT1B);
MeMegaPiDCMotor rightMotor(PORT2B);

// hw_set_motor_speed sets the running speed of the motors to turn the wheels.
void hw_set_motor_speed(int leftSpeed, int rightSpeed) {
    leftMotor.run(leftSpeed);
    rightMotor.run(-rightSpeed); // flip right motor since it is reversed on the robot
}

/* Configuration: Me Ultrasonic Sensor */

MeUltrasonicSensor ultrasonic(PORT_6);

// hw_get_ultrasonic_distance returns distance in cm.
uint16_t hw_get_ultrasonic_distance() {
    return ultrasonic.distanceCm();
}

/* Configuration: Me Line Follower Sensor */

MeLineFollower lineFinder(PORT_7);

// hw_read_line_sensor returns the sensor values for the MeLineFollower.
uint8_t hw_read_line_sensor() {
    // From the readSensors() documentation:
    // (0x00)-S1_IN_S2_IN:   sensor1 and sensor2 are both inside of black line
    // (0x01)-S1_IN_S2_OUT:  sensor1 is inside of black line and sensor2 is outside of black line
    // (0x02)-S1_OUT_S2_IN:  sensor1 is outside of black line and sensor2 is inside of black line
    // (0x03)-S1_OUT_S2_OUT: sensor1 and sensor2 are both outside of black line
    return lineFinder.readSensors();
}
