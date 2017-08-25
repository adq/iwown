#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include "gfxfont.h"

extern uint32_t OLED_physWidth;
extern uint32_t OLED_physHeight;
extern uint32_t OLED_curWidth;
extern uint32_t OLED_curHeight;
extern uint8_t OLED_textsize;
extern uint8_t OLED_rotation;

// This MUST be defined by the subclass:
extern void OLED_drawPixel(int16_t x, int16_t y, uint16_t color);

// TRANSACTION API / CORE DRAW API
// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
void OLED_writePixel(int16_t x, int16_t y, uint16_t color);
void OLED_writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void OLED_writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void OLED_writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void OLED_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

// CONTROL API
// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
void OLED_setRotation(uint8_t r);
void OLED_invertDisplay(bool i);

// BASIC DRAW API
// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
void OLED_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void OLED_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void OLED_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void OLED_fillScreen(uint16_t color);
void OLED_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void OLED_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

// These exist only with Adafruit_GFX (no subclass overrides)
void OLED_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void OLED_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void OLED_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void OLED_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void OLED_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void OLED_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void OLED_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void OLED_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void OLED_drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void OLED_drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void OLED_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
void OLED_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
void OLED_drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
void OLED_drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h);
void OLED_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void OLED_setCursor(int16_t x, int16_t y);
void OLED_setTextColor(uint16_t c);
void OLED_setTextColor(uint16_t c, uint16_t bg);
void OLED_setTextSize(uint8_t s);
void OLED_setTextWrap(bool w);
void OLED_cp437(bool x);
void OLED_setFont(const GFXfont *f);
void OLED_getTextBounds(char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

void OLED_write(uint8_t);

// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
int16_t OLED_getCursorX(void);
int16_t OLED_getCursorY(void);

void OLED_charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);

#endif // _ADAFRUIT_GFX_H


/*

const int16_t
WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
int16_t
_width, _height, // Display w/h as modified by current rotation
cursor_x, cursor_y;
uint16_t
textcolor, textbgcolor;
uint8_t
textsize,
rotation;
boolean
wrap,   // If set, 'wrap' text at right edge of display
_cp437; // If set, use correct CP437 charset (default is off)
GFXfont
*gfxFont;
*/
