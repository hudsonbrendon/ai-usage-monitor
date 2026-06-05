#pragma once
#include "Settings.h"

class Storage {
public:
    bool begin();                  // mount LittleFS (format on first use)
    bool load(Settings& out);      // read /settings.json; false if absent/bad
    bool save(const Settings& s);  // write /settings.json
    void wipe();                   // remove /settings.json (factory reset)
};
