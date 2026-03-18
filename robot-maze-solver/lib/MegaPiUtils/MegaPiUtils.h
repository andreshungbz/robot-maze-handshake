#pragma once
#include <Arduino.h>

// isValidPort validates against the valid ports for connecting modules
// to the MegaPi, which is port 5 to 8.
bool isValidPort(uint8_t port);
