#pragma once
#include "Usage.h"
#include <Arduino.h>

// A usage data source (Claude, Codex, ...). One fetch returns the 5h/7d windows.
// Implementations live in src/providers/. main holds one active Provider*.
class Provider {
public:
    virtual ~Provider() {}
    virtual const char* id() = 0;   // short label, e.g. "Claude"
    virtual UsageStatus fetch(const String& token, char* errOut, size_t errLen) = 0;
};
