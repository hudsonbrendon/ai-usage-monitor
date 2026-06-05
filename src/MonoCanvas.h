#pragma once
#if defined(USE_MONO_CANVAS)
#include "Canvas.h"
#include <U8g2lib.h>

// Canvas over a U8g2 monochrome buffer. offX/offY + logical w/h support cropped
// panels (e.g. a 72x40 viewport inside a 128x64 controller).
class MonoCanvas : public Canvas {
public:
    MonoCanvas(U8G2& g, int logicalW, int logicalH, int offX = 0, int offY = 0)
        : _g(g), _w(logicalW), _h(logicalH), _ox(offX), _oy(offY) {}
    int  width() const override  { return _w; }
    int  height() const override { return _h; }
    void clear() override;
    void present() override;
    void power(bool on) override;
    void text(int x, int y, Scale s, Ink ink, const char* str) override;
    int  textWidth(Scale s, const char* str) override;
    int  lineHeight(Scale s) const override;
    void box(int x, int y, int w, int h, Ink ink, bool fill) override;
    void hline(int x, int y, int w, Ink ink) override;
private:
    U8G2& _g; int _w, _h, _ox, _oy;
    const uint8_t* font(Scale s) const;
    void setInk(Ink ink);
};
#endif
