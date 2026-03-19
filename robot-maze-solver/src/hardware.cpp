#include "MeMegaPi.h"

/* Configuration: Me RGB LED Module */

MeRGBLed led(PORT_8);
constexpr uint8_t LED_COUNT{4};

// hw_set_rgb_led_color sets all LED components to one color.
void hw_set_rgb_led_color(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t t = 0; t < LED_COUNT; ++t) {
        led.setColorAt(t, r, g, b);
    }
    
    led.show();
}
