#include "Usage.h"
#include <cstdlib>
#include <cstdio>

static bool nonEmpty(const char* s) { return s != nullptr && s[0] != '\0'; }

UsageStatus parseUsage(const char* h5util, const char* h5reset,
                       const char* d7util, const char* d7reset) {
    UsageStatus s;
    if (nonEmpty(h5util)) { s.h5Percent = (float)atof(h5util) * 100.0f; s.valid = true; }
    if (nonEmpty(d7util)) { s.d7Percent = (float)atof(d7util) * 100.0f; s.valid = true; }
    if (nonEmpty(h5reset)) s.h5Reset = (uint32_t)strtoul(h5reset, nullptr, 10);
    if (nonEmpty(d7reset)) s.d7Reset = (uint32_t)strtoul(d7reset, nullptr, 10);
    return s;
}

void formatCountdown(uint32_t epoch, uint32_t now, char* out, size_t outLen) {
    if (epoch == 0)   { snprintf(out, outLen, "--");  return; }
    if (epoch <= now) { snprintf(out, outLen, "now"); return; }
    uint32_t diff = epoch - now;
    uint32_t d = diff / 86400, h = (diff % 86400) / 3600, m = (diff % 3600) / 60;
    if (d > 0)      snprintf(out, outLen, "%ud%uh", d, h);
    else if (h > 0) snprintf(out, outLen, "%uh%02um", h, m);
    else            snprintf(out, outLen, "%um", m);
}
