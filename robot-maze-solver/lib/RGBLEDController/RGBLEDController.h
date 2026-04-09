#pragma once
#include <Arduino.h>

// RGBLEDController controls the RGB LED module.
class RGBLEDController {
public:
    // setRed sets the color to pure red (255, 0, 0).
    void setRed();
    // setGreen sets the color to pure green (0, 255, 0).
    void setGreen();
    // setBlue sets the color to pure blue (0, 0, 255).
    void setBlue();
    // turnOff turns all LEDs off (0, 0, 0).
    void turnOff();

private:
    // Helper Methods

    // setColor sets the color to the RGB parameters.
    void setColor(uint8_t r, uint8_t g, uint8_t b);
};
