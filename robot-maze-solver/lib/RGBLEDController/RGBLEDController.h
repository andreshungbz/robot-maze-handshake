#pragma once

#include <Arduino.h>

// LED_COUNT represents the number of LEDs on the RGB LED module.
constexpr uint8_t LED_COUNT = 4;

// setGreen sets all LEDs on the RGB LED module to pure green.
void setGreen(uint8_t port);
