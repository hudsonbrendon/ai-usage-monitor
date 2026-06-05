#pragma once
#include "../Provider.h"

// Anthropic provider: probes the Messages API and reads the
// anthropic-ratelimit-unified-5h/7d-* response headers.
class ClaudeProvider : public Provider {
public:
    const char* id() override { return "Claude"; }
    UsageStatus fetch(const String& token, char* errOut, size_t errLen) override;
};
