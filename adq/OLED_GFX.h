#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include "gfxfont.h"

extern uint32_t OLED_physWidth;
extern uint32_t OLED_physHeight;
extern uint32_t OLED_curWidth;
extern uint32_t OLED_curHeight;
extern uint8_t OLED_textsize;
extern uint8_t OLED_rotation;
extern int16_t OLED_cursor_x;
extern int16_t OLED_cursor_y;
extern uint16_t OLED_textcolor;
extern uint16_t OLED_textbgcolor;
extern bool OLED_wrap;
extern GFXfont *OLED_gfxFont;

// This MUST be defined by the subclass:
extern void OLED_drawPixel(int16_t x, int16_t y, uint16_t color);

// TRANSACTION API / CORE DRAW API
// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
extern void OLED_writePixel(int16_t x, int16_t y, uint16_t color);
extern void OLED_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

// CONTROL API
// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
extern void OLED_setRotation(uint8_t r);
extern void OLED_invertDisplay(bool i);

// BASIC DRAW API
// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
extern void OLED_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
extern void OLED_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
extern void OLED_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
extern void OLED_fillScreen(uint16_t color);
extern void OLED_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
extern void OLED_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

// These exist only with Adafruit_GFX (no subclass overrides)
extern void OLED_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
extern void OLED_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
extern void OLED_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
extern void OLED_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
extern void OLED_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void OLED_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
extern void OLED_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
extern void OLED_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
extern void OLED_drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
extern void OLED_drawBitmapBg(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
extern void OLED_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
extern void OLED_drawGrayscaleBitmapMask(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
extern void OLED_drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
extern void OLED_drawRGBBitmapMask(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
extern void OLED_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
extern void OLED_setCursor(int16_t x, int16_t y);
extern void OLED_setTextColor(uint16_t c);
extern void OLED_setTextColorBg(uint16_t c, uint16_t bg);
extern void OLED_setTextSize(uint8_t s);
extern void OLED_setTextWrap(bool w);
extern void OLED_setFont(GFXfont *f);
extern void OLED_getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

extern void OLED_write(uint8_t);

extern void OLED_charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);

#endif // _ADAFRUIT_GFX_H
