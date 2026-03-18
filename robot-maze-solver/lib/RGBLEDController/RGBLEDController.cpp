#include "RGBLEDController.h"

#include "MegaPiUtils.h"
#include "MeMegaPi.h"

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
