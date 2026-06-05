#pragma once
#include "Usage.h"
#include <Arduino.h>

class UsageClient {
public:
    // One HTTPS probe with the OAuth token. On failure returns valid=false and
    // copies a short reason into errOut.
    UsageStatus fetch(const String& token, char* errOut, size_t errLen);
};
