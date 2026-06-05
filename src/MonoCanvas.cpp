#include "MonoCanvas.h"
#if defined(USE_MONO_CANVAS)

const uint8_t* MonoCanvas::font(Scale s) const {
    switch (s) {
        case Scale::Large:  return u8g2_font_7x13B_tr;
        case Scale::Medium: return u8g2_font_6x10_tr;
        default:            return u8g2_font_5x7_tr;
    }
}

int MonoCanvas::lineHeight(Scale s) const {
    switch (s) { case Scale::Large: return 15; case Scale::Medium: return 12; default: return 9; }
}

void MonoCanvas::setInk(Ink ink) { _g.setDrawColor(ink == Ink::Bg ? 0 : 1); }

void MonoCanvas::clear()        { _g.clearBuffer(); }
void MonoCanvas::present()      { _g.sendBuffer(); }
void MonoCanvas::power(bool on) { _g.setPowerSave(on ? 0 : 1); }

void MonoCanvas::text(int x, int y, Scale s, Ink ink, const char* str) {
    _g.setFont(font(s));
    setInk(ink);
    _g.drawStr(_ox + x, _oy + y + _g.getAscent(), str);  // ascent => top-left semantics
}

int MonoCanvas::textWidth(Scale s, const char* str) {
    _g.setFont(font(s));
    return _g.getStrWidth(str);
}

void MonoCanvas::box(int x, int y, int w, int h, Ink ink, bool fill) {
    setInk(ink);
    if (fill) _g.drawBox(_ox + x, _oy + y, w, h);
    else      _g.drawFrame(_ox + x, _oy + y, w, h);
}

void MonoCanvas::hline(int x, int y, int w, Ink ink) {
    setInk(ink);
    _g.drawHLine(_ox + x, _oy + y, w);
}
#endif
