#pragma once
#include <string>
#include <cstdint>
#include <cstddef>

struct Settings {
    std::string ssid;
    std::string password;
    std::string token;            // sk-ant-oat01-...
    uint16_t    pollSeconds = 120;
    bool        configured   = false;
};

bool settingsToJson(const Settings& s, char* out, size_t outLen);
// Missing fields keep defaults; `configured` = ssid AND token present.
bool settingsFromJson(const char* json, Settings& s);
