#include "RGBLEDController.h"

#include "MeMegaPi.h"

static bool isValidPort(uint8_t port) {
    // MegaPi valid ports for modules is from 5 to 8
    return port >= 5 && port <= 8;
}

void setGreen(uint8_t port) {
    if (!isValidPort(port)) {
        return;
    }

    MeRGBLed led(port);

    // configure every LED to pure green
    for (uint8_t t{0}; t < LED_COUNT; ++t) {
        led.setColorAt(t, 0, 255, 0);
    }

    // activate LEDs
    led.show();
}
