#pragma once
#include <stdint.h>

// Semantic colors — a mono panel maps everything but Bg to "on".
enum class Ink : uint8_t { Fg, Dim, Accent, Warn, Bg };
// Semantic text sizes — each Canvas maps to a real font / text scale.
enum class Scale : uint8_t { Small, Medium, Large };

// Abstract drawing surface. (x, y) is always the TOP-LEFT of what is drawn.
class Canvas {
public:
    virtual ~Canvas() {}
    virtual int  width() const = 0;
    virtual int  height() const = 0;
    virtual void clear() = 0;
    virtual void present() = 0;                  // flush to the panel
    virtual void power(bool on) = 0;             // screen on/off
    virtual void text(int x, int y, Scale s, Ink ink, const char* str) = 0;
    virtual int  textWidth(Scale s, const char* str) = 0;
    virtual int  lineHeight(Scale s) const = 0;
    virtual void box(int x, int y, int w, int h, Ink ink, bool fill) = 0;
    virtual void hline(int x, int y, int w, Ink ink) = 0;
};
