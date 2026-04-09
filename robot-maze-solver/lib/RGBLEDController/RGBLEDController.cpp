#include "RGBLEDController.h"

// forward declaration for function in hardware.cpp
void hw_set_rgb_led_color(uint8_t r, uint8_t g, uint8_t b);

// Public Methods

void RGBLEDController::setRed() {
    setColor(255, 0, 0);
}

void RGBLEDController::setGreen() {
    setColor(0, 255, 0);
}

void RGBLEDController::setBlue() {
    setColor(0, 0, 255);
}

void RGBLEDController::turnOff() {
    setColor(0, 0, 0);
}

// Private Helper Methods

void RGBLEDController::setColor(uint8_t r, uint8_t g, uint8_t b) {
    hw_set_rgb_led_color(r, g, b);
}
