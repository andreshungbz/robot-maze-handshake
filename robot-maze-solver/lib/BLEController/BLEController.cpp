#include "BLEController.h"

// forward declarations for functions in hardware.cpp
void hw_ble_begin(long baudRate);
bool hw_ble_available();
char hw_ble_read();
void hw_ble_write(char c);

// Public Methods

void BLEController::begin(long baudRate) {
    hw_ble_begin(baudRate);
}

bool BLEController::available() const {
    return hw_ble_available();
}

char BLEController::read() {
    return hw_ble_read();
}

void BLEController::write(char c) {
    hw_ble_write(c);
}

void BLEController::write(const char* str) {
    while (*str) {
        hw_ble_write(*str++);
    }
}
