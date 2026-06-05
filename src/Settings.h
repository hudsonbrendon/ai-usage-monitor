#pragma once
#include <string>
#include <cstdint>
#include <cstddef>

struct Settings {
    std::string ssid;
    std::string password;
    std::string token;            // Claude OAuth token (claude setup-token)
    std::string codexToken;       // Codex access_token (~/.codex/auth.json)
    std::string codexAccountId;   // Codex account_id
    uint16_t    pollSeconds = 120;
    bool        configured   = false;
};

bool settingsToJson(const Settings& s, char* out, size_t outLen);
// Missing fields keep defaults; `configured` = ssid AND at least one provider's creds.
bool settingsFromJson(const char* json, Settings& s);
