# claude-usage-monitor

Live Claude Code API usage on a tiny OLED — your 5-hour and 7-day rate-limit
utilization with reset countdowns, on the ideaspark ESP8266 0.96" OLED board.
Configured once over a captive Wi-Fi portal.

Designed to grow: a `Canvas`/`IBoard` abstraction keeps every screen
board-agnostic, so new boards are additive (see `docs/EXTENDING.md`).

## Hardware (today)
- ideaspark ESP8266 (ESP-12S) with integrated SSD1306 128x64 I2C OLED
  (VR:2.1: OLED on SDA=GPIO12, SCL=GPIO14; FLASH button on GPIO0).

## Build, test, flash (PlatformIO)
```
pio test -e native        # host unit tests (parsing, settings)
pio run  -e ideaspark     # build firmware
pio run  -e ideaspark -t upload
```

## Setup
1. First boot opens an open Wi-Fi AP `ClaudeUsage-XXXX`.
2. Join it, open `http://192.168.4.1`.
3. Enter your 2.4 GHz Wi-Fi and a Claude token (`claude setup-token`).
4. It reboots and shows the dashboard. Tap = screen on/off, long-press =
   refresh, hold 5 s = factory reset.

## License
MIT © Hudson Brendon
