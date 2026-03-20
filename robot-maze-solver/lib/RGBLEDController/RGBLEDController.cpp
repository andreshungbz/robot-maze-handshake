#include "RGBLEDController.h"

// forward declaration for function in hardware.cpp
void hw_set_rgb_led_color(uint8_t r, uint8_t g, uint8_t b);

void RGBLEDController::setGreen() {
    setColor(0, 255, 0);
}

void RGBLEDController::setColor(uint8_t r, uint8_t g, uint8_t b) {
    hw_set_rgb_led_color(r, g, b);
}
