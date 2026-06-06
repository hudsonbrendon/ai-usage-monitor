#pragma once
#include "../Provider.h"

class CodexProvider : public Provider {
public:
    const char* id() override { return "Codex"; }
    bool available(const Settings& s) override {
        return !s.codexAccountId.empty() && (!s.codexToken.empty() || !s.codexRefreshToken.empty());
    }
    UsageStatus fetch(const Settings& s, char* errOut, size_t errLen) override;

    // After a fetch, if the server rotated the refresh token, main reads it here
    // and persists it. Returns true once per rotation.
    bool takeRotatedRefreshToken(String& out);

private:
    String   _accessToken;
    String   _liveRefreshToken;     // current refresh token (may rotate)
    uint32_t _expiresAt = 0;        // unix epoch when _accessToken expires
    bool     _rotated = false;

    bool refreshAccessToken(const Settings& s, char* errOut, size_t errLen);
};
