# Adding a board

The firmware is board-agnostic above the `Canvas` (drawing) and `IBoard`
(display + input) seams. Adding a board never touches `Screens`, `Usage`,
`Settings`, `Storage`, or `UsageClient`.

## Steps
1. **Display tech already supported?** Mono OLEDs reuse `MonoCanvas`. A new
   tech (color TFT, e-paper) needs a new `Canvas` subclass implementing the
   same primitives (`text`, `box`, `hline`, `clear`, `present`, metrics),
   gated behind a `USE_<X>_CANVAS` flag.
2. **Create `src/boards/<name>.cpp`**, guarded by `#if defined(BOARD_<NAME>)`,
   implementing `IBoard` (construct the display object + a Canvas over it, map
   the board's buttons to `tapped`/`longPressed`/`held`) and defining
   `IBoard& Board()`.
3. **Add a PlatformIO env** in `platformio.ini` with `-DBOARD_<NAME>` plus the
   right `-DUSE_<X>_CANVAS` and display `lib_deps`.
4. **Build it:** `pio run -e <name>`. The host tests and all shared logic are
   unchanged.

## Notes
- `Screens` is responsive (uses `canvas.width()/height()` and semantic
  `Scale`/`Ink`), so a 72x40 OLED and a 320x170 TFT both render without
  per-board screen code.
- Storage is LittleFS on every chip; TLS in `UsageClient` is chip-guarded
  (BearSSL on ESP8266, mbedtls on ESP32) and already compiles for both.
