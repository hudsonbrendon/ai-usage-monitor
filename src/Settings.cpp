#include "Settings.h"
#include <ArduinoJson.h>

bool settingsToJson(const Settings& s, char* out, size_t outLen) {
    JsonDocument doc;
    doc["ssid"] = s.ssid;
    doc["password"] = s.password;
    doc["token"] = s.token;
    doc["pollSeconds"] = s.pollSeconds;
    size_t n = serializeJson(doc, out, outLen);
    return n > 0 && n < outLen;
}

bool settingsFromJson(const char* json, Settings& s) {
    JsonDocument doc;
    if (deserializeJson(doc, json)) return false;
    s.ssid     = doc["ssid"]     | "";
    s.password = doc["password"] | "";
    s.token    = doc["token"]    | "";
    s.pollSeconds = doc["pollSeconds"] | (uint16_t)120;
    s.configured = !s.ssid.empty() && !s.token.empty();
    return true;
}
