#include "CodexProvider.h"
#include "globalsign_root.h"
#include "../Usage.h"
#include <ArduinoJson.h>
#include <time.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClientSecureBearSSL.h>
  #include <ESP8266HTTPClient.h>
#else
  #include <WiFi.h>
  #include <WiFiClientSecure.h>
  #include <HTTPClient.h>
#endif

static const char* CODEX_ENDPOINT       = "https://chatgpt.com/backend-api/codex/usage";
static const char* CODEX_TOKEN_ENDPOINT = "https://auth.openai.com/oauth/token";
static const char* CODEX_CLIENT_ID      = "app_EMoamEEZ73f0CkXaXp7hrann";

static uint32_t nowEpoch() { time_t t; time(&t); return (uint32_t)t; }

#if defined(ESP8266)
static void attachTls(BearSSL::WiFiClientSecure& c) {
    static BearSSL::X509List ca(GLOBALSIGN_ROOT_CA);
    c.setTrustAnchors(&ca);
}
#endif

bool CodexProvider::takeRotatedRefreshToken(String& out) {
    if (!_rotated) return false;
    out = _liveRefreshToken;
    _rotated = false;
    return true;
}

bool CodexProvider::refreshAccessToken(const Settings& s, char* errOut, size_t errLen) {
    if (_liveRefreshToken.length() == 0) _liveRefreshToken = s.codexRefreshToken.c_str();
    if (_liveRefreshToken.length() == 0) { snprintf(errOut, errLen, "no_refresh"); return false; }

#if defined(ESP8266)
    BearSSL::WiFiClientSecure client; attachTls(client);
#else
    WiFiClientSecure client; client.setCACert(GLOBALSIGN_ROOT_CA);
#endif
    HTTPClient https;
    if (!https.begin(client, CODEX_TOKEN_ENDPOINT)) { snprintf(errOut, errLen, "tls_init"); return false; }
    https.addHeader("Content-Type", "application/json");
    JsonDocument reqDoc;                       // serialize so the token is JSON-escaped
    reqDoc["grant_type"]    = "refresh_token";
    reqDoc["refresh_token"] = _liveRefreshToken.c_str();
    reqDoc["client_id"]     = CODEX_CLIENT_ID;
    String body;
    serializeJson(reqDoc, body);
    int code = https.POST(body);
    if (code != 200) { snprintf(errOut, errLen, (code == 400 || code == 401) ? "refresh_denied" : "refresh_%d", code); https.end(); return false; }
    String resp = https.getString();
    https.end();

    OAuthToken t = parseOAuthToken(resp.c_str());
    if (!t.ok) { snprintf(errOut, errLen, "refresh_parse"); return false; }
    _accessToken = t.accessToken.c_str();
    _expiresAt   = nowEpoch() + (t.expiresIn ? t.expiresIn : 3600);
    if (t.refreshToken.length() && t.refreshToken != _liveRefreshToken.c_str()) {
        _liveRefreshToken = t.refreshToken.c_str();
        _rotated = true;
    }
    return true;
}

UsageStatus CodexProvider::fetch(const Settings& s, char* errOut, size_t errLen) {
    UsageStatus bad;

    String accessToken;
    bool usingRefresh = !s.codexRefreshToken.empty();
    if (usingRefresh) {
        if (_accessToken.length() == 0 || nowEpoch() + 60 >= _expiresAt) {
            if (!refreshAccessToken(s, errOut, errLen)) return bad;
        }
        accessToken = _accessToken;
    } else {
        accessToken = s.codexToken.c_str();
    }

    for (int attempt = 0; attempt < 2; attempt++) {
#if defined(ESP8266)
        BearSSL::WiFiClientSecure client; attachTls(client);
#else
        WiFiClientSecure client; client.setCACert(GLOBALSIGN_ROOT_CA);
#endif
        HTTPClient https;
        if (!https.begin(client, CODEX_ENDPOINT)) { snprintf(errOut, errLen, "tls_init"); return bad; }
        https.addHeader("Authorization", String("Bearer ") + accessToken);
        https.addHeader("chatgpt-account-id", s.codexAccountId.c_str());
        https.addHeader("originator", "codex_cli_rs");
        https.addHeader("OpenAI-Beta", "responses=experimental");
        https.addHeader("User-Agent", "codex_cli_rs/0.137.0 (ai-usage-monitor; esp8266)");
        https.addHeader("Accept", "application/json");
        https.setTimeout(15000);

        int code = https.GET();
        if (code == 401 && usingRefresh && attempt == 0) {
            https.end();
            _expiresAt = 0;
            if (!refreshAccessToken(s, errOut, errLen)) return bad;
            accessToken = _accessToken;
            continue;
        }
        if (code <= 0) { snprintf(errOut, errLen, "http_%d", code); https.end(); return bad; }
        if (code != 200) { snprintf(errOut, errLen, code == 401 ? "auth_failed" : "http_%d", code); https.end(); return bad; }

        String resp = https.getString();
        https.end();
        UsageStatus u = parseCodexUsage(resp.c_str());
        if (!u.valid) { snprintf(errOut, errLen, "no_usage"); return bad; }
        return u;
    }
    snprintf(errOut, errLen, "auth_failed");
    return bad;
}
