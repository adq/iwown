/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "OLED_GFX.h"
#include "glcdfont.c"

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

uint32_t OLED_physWidth;
uint32_t OLED_physHeight;
uint32_t OLED_curWidth;
uint32_t OLED_curHeight;
uint8_t OLED_textsize = 1;
uint8_t OLED_rotation = 0;
int16_t OLED_cursor_x = 0;
int16_t OLED_cursor_y = 0;
uint16_t OLED_textcolor = 0xffff;
uint16_t OLED_textbgcolor = 0xffff;
bool OLED_wrap = true;
GFXfont *OLED_gfxFont = NULL;


/*
OLED_Adafruit_GFX(int16_t w, int16_t h):
WIDTH(w), HEIGHT(h)
{
    // _width    = WIDTH;
    // _height   = HEIGHT;
    // rotation  = 0;
    cursor_y  = cursor_x    = 0;
    // textsize  = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap      = true;
    gfxFont   = NULL;
}
*/

// Bresenham's algorithm - thx wikpedia
void OLED_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
        uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            OLED_drawPixel(y0, x0, color);
        } else {
            OLED_drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

// (x,y) is topmost point; if unsure, calling function
// should sort endpoints or call drawLine() instead
void OLED_drawFastVLine(int16_t x, int16_t y,
        int16_t h, uint16_t color) {
    // Update in subclasses if desired!
    OLED_writeLine(x, y, x, y+h-1, color);
}

// (x,y) is leftmost point; if unsure, calling function
// should sort endpoints or call drawLine() instead
void OLED_drawFastHLine(int16_t x, int16_t y,
        int16_t w, uint16_t color) {
    // Update in subclasses if desired!
    OLED_writeLine(x, y, x+w-1, y, color);
}

void OLED_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t color) {
    // Update in subclasses if desired!
    for (int16_t i=x; i<x+w; i++) {
        OLED_drawFastVLine(i, y, h, color);
    }
}

void OLED_fillScreen(uint16_t color) {
    // Update in subclasses if desired!
    OLED_fillRect(0, 0, OLED_curWidth, OLED_curHeight, color);
}

void OLED_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
        uint16_t color) {
    // Update in subclasses if desired!
    if(x0 == x1){
        if(y0 > y1) _swap_int16_t(y0, y1);
        OLED_drawFastVLine(x0, y0, y1 - y0 + 1, color);
    } else if(y0 == y1){
        if(x0 > x1) _swap_int16_t(x0, x1);
        OLED_drawFastHLine(x0, y0, x1 - x0 + 1, color);
    } else {
        OLED_writeLine(x0, y0, x1, y1, color);
    }
}

// Draw a circle outline
void OLED_drawCircle(int16_t x0, int16_t y0, int16_t r,
        uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    OLED_drawPixel(x0  , y0+r, color);
    OLED_drawPixel(x0  , y0-r, color);
    OLED_drawPixel(x0+r, y0  , color);
    OLED_drawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        OLED_drawPixel(x0 + x, y0 + y, color);
        OLED_drawPixel(x0 - x, y0 + y, color);
        OLED_drawPixel(x0 + x, y0 - y, color);
        OLED_drawPixel(x0 - x, y0 - y, color);
        OLED_drawPixel(x0 + y, y0 + x, color);
        OLED_drawPixel(x0 - y, y0 + x, color);
        OLED_drawPixel(x0 + y, y0 - x, color);
        OLED_drawPixel(x0 - y, y0 - x, color);
    }
}

void OLED_drawCircleHelper( int16_t x0, int16_t y0,
        int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            OLED_drawPixel(x0 + x, y0 + y, color);
            OLED_drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            OLED_drawPixel(x0 + x, y0 - y, color);
            OLED_drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            OLED_drawPixel(x0 - y, y0 + x, color);
            OLED_drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            OLED_drawPixel(x0 - y, y0 - x, color);
            OLED_drawPixel(x0 - x, y0 - y, color);
        }
    }
}

void OLED_fillCircle(int16_t x0, int16_t y0, int16_t r,
        uint16_t color) {
    OLED_drawFastVLine(x0, y0-r, 2*r+1, color);
    OLED_fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void OLED_fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
        uint8_t cornername, int16_t delta, uint16_t color) {

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1) {
            OLED_drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
            OLED_drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2) {
            OLED_drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
            OLED_drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}

// Draw a rectangle
void OLED_drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
    uint16_t color) {
    OLED_drawFastHLine(x, y, w, color);
    OLED_drawFastHLine(x, y+h-1, w, color);
    OLED_drawFastVLine(x, y, h, color);
    OLED_drawFastVLine(x+w-1, y, h, color);
}

// Draw a rounded rectangle
void OLED_drawRoundRect(int16_t x, int16_t y, int16_t w,
    int16_t h, int16_t r, uint16_t color) {
    // smarter version
    OLED_drawFastHLine(x+r  , y    , w-2*r, color); // Top
    OLED_drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    OLED_drawFastVLine(x    , y+r  , h-2*r, color); // Left
    OLED_drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    OLED_drawCircleHelper(x+r    , y+r    , r, 1, color);
    OLED_drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    OLED_drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    OLED_drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void OLED_fillRoundRect(int16_t x, int16_t y, int16_t w,
                        int16_t h, int16_t r, uint16_t color) {
    // smarter version
    OLED_fillRect(x+r, y, w-2*r, h, color);

    // draw four corners
    OLED_fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    OLED_fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void OLED_drawTriangle(int16_t x0, int16_t y0,
        int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    OLED_drawLine(x0, y0, x1, y1, color);
    OLED_drawLine(x1, y1, x2, y2, color);
    OLED_drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void OLED_fillTriangle(int16_t x0, int16_t y0,
        int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        OLED_drawFastHLine(a, y0, b-a+1, color);
        return;
    }

    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa   = 0,
    sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        OLED_drawFastHLine(a, y, b-a+1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        OLED_drawFastHLine(a, y, b-a+1, color);
    }
}

// BITMAP / XBITMAP / GRAYSCALE / RGB BITMAP FUNCTIONS ---------------------

// Draw a RAM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground color (unset bits are transparent).
void OLED_drawBitmap(int16_t x, int16_t y,
  uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++ ) {
            if(i & 7) byte <<= 1;
            else      byte   = bitmap[j * byteWidth + i / 8];
            if(byte & 0x80) OLED_drawPixel(x+i, y, color);
        }
    }
}

// Draw a RAM-resident 1-bit image at the specified (x,y) position,
// using the specified foreground (for set bits) and background (unset
// bits) colors.
void OLED_drawBitmapBg(int16_t x, int16_t y,
  uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) {

    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++ ) {
            if(i & 7) byte <<= 1;
            else      byte   = bitmap[j * byteWidth + i / 8];
            OLED_drawPixel(x+i, y, (byte & 0x80) ? color : bg);
        }
    }
}

// Draw a RAM-resident 8-bit image (grayscale) at the specified (x,y)
// pos.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
void OLED_drawGrayscaleBitmap(int16_t x, int16_t y,
  uint8_t *bitmap, int16_t w, int16_t h) {
    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++ ) {
            OLED_drawPixel(x+i, y, bitmap[j * w + i]);
        }
    }
}

// Draw a RAM-resident 8-bit image (grayscale) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) pos.
// BOTH buffers (grayscale and mask) must be RAM-resident, no mix-and-
// match.  Specifically for 8-bit display devices such as IS31FL3731;
// no color reduction/expansion is performed.
void OLED_drawGrayscaleBitmapMask(int16_t x, int16_t y,
  uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h) {
    int16_t bw   = (w + 7) / 8; // Bitmask scanline pad = whole byte
    uint8_t byte = 0;
    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++ ) {
            if(i & 7) byte <<= 1;
            else      byte   = mask[j * bw + i / 8];
            if(byte & 0x80) {
                OLED_drawPixel(x+i, y, bitmap[j * w + i]);
            }
        }
    }
}

// Draw a RAM-resident 16-bit image (RGB 5/6/5) at the specified (x,y)
// position.  For 16-bit display devices; no color reduction performed.
void OLED_drawRGBBitmap(int16_t x, int16_t y,
  uint16_t *bitmap, int16_t w, int16_t h) {
    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++ ) {
            OLED_drawPixel(x+i, y, bitmap[j * w + i]);
        }
    }
}

// Draw a RAM-resident 16-bit image (RGB 5/6/5) with a 1-bit mask
// (set bits = opaque, unset bits = clear) at the specified (x,y) pos.
// BOTH buffers (color and mask) must be RAM-resident, no mix-and-match.
// For 16-bit display devices; no color reduction performed.
void OLED_drawRGBBitmapMask(int16_t x, int16_t y,
  uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h) {
    int16_t bw   = (w + 7) / 8; // Bitmask scanline pad = whole byte
    uint8_t byte = 0;
    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++ ) {
            if(i & 7) byte <<= 1;
            else      byte   = mask[j * bw + i / 8];
            if(byte & 0x80) {
                OLED_drawPixel(x+i, y, bitmap[j * w + i]);
            }
        }
    }
}

// TEXT- AND CHARACTER-HANDLING FUNCTIONS ----------------------------------

// Draw a character
void OLED_drawChar(int16_t x, int16_t y, unsigned char c,
  uint16_t color, uint16_t bg, uint8_t size) {

    if(!OLED_gfxFont) { // 'Classic' built-in font

        if((x >= OLED_curWidth)            || // Clip right
           (y >= OLED_curHeight)           || // Clip bottom
           ((x + 6 * size - 1) < 0) || // Clip left
           ((y + 8 * size - 1) < 0))   // Clip top
            return;

        for(int8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for(int8_t j=0; j<8; j++, line >>= 1) {
                if(line & 1) {
                    if(size == 1)
                        OLED_drawPixel(x+i, y+j, color);
                    else
                        OLED_fillRect(x+i*size, y+j*size, size, size, color);
                } else if(bg != color) {
                    if(size == 1)
                        OLED_drawPixel(x+i, y+j, bg);
                    else
                        OLED_fillRect(x+i*size, y+j*size, size, size, bg);
                }
            }
        }
        if(bg != color) { // If opaque, draw vertical line for last column
            if(size == 1) OLED_drawFastVLine(x+5, y, 8, bg);
            else          OLED_fillRect(x+5*size, y, size, 8*size, bg);
        }

    } else { // Custom font

        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= (uint8_t)pgm_read_byte(&OLED_gfxFont->first);
        GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&OLED_gfxFont->glyph))[c]);
        uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&OLED_gfxFont->bitmap);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t  w  = pgm_read_byte(&glyph->width),
                 h  = pgm_read_byte(&glyph->height);
        int8_t   xo = pgm_read_byte(&glyph->xOffset),
                 yo = pgm_read_byte(&glyph->yOffset);
        uint8_t  xx, yy, bits = 0, bit = 0;
        int16_t  xo16 = 0, yo16 = 0;

        if(size > 1) {
            xo16 = xo;
            yo16 = yo;
        }

        // Todo: Add character clipping here

        // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
        // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
        // has typically been used with the 'classic' font to overwrite old
        // screen contents with new data.  This ONLY works because the
        // characters are a uniform size; it's not a sensible thing to do with
        // proportionally-spaced fonts with glyphs of varying sizes (and that
        // may overlap).  To replace previously-drawn text when using a custom
        // font, use the getTextBounds() function to determine the smallest
        // rectangle encompassing a string, erase the area with fillRect(),
        // then draw new text.  This WILL infortunately 'blink' the text, but
        // is unavoidable.  Drawing 'background' pixels will NOT fix this,
        // only creates a new set of problems.  Have an idea to work around
        // this (a canvas object type for MCUs that can afford the RAM and
        // displays supporting setAddrWindow() and pushColors()), but haven't
        // implemented this yet.

        for(yy=0; yy<h; yy++) {
            for(xx=0; xx<w; xx++) {
                if(!(bit++ & 7)) {
                    bits = pgm_read_byte(&bitmap[bo++]);
                }
                if(bits & 0x80) {
                    if(size == 1) {
                        OLED_drawPixel(x+xo+xx, y+yo+yy, color);
                    } else {
                        OLED_fillRect(x+(xo16+xx)*size, y+(yo16+yy)*size,
                          size, size, color);
                    }
                }
                bits <<= 1;
            }
        }

    } // End classic vs custom font
}

void OLED_write(uint8_t c) {
    if(!OLED_gfxFont) { // 'Classic' built-in font

        if(c == '\n') {                        // Newline?
            OLED_cursor_x  = 0;                     // Reset x to zero,
            OLED_cursor_y += OLED_textsize * 8;          // advance y one line
        } else if(c != '\r') {                 // Ignore carriage returns
            if(OLED_wrap && ((OLED_cursor_x + OLED_textsize * 6) > OLED_curWidth)) { // Off right?
                OLED_cursor_x  = 0;                 // Reset x to zero,
                OLED_cursor_y += OLED_textsize * 8;      // advance y one line
            }
            OLED_drawChar(OLED_cursor_x, OLED_cursor_y, c, OLED_textcolor, OLED_textbgcolor, OLED_textsize);
            OLED_cursor_x += OLED_textsize * 6;          // Advance x one char
        }

    } else { // Custom font

        if(c == '\n') {
            OLED_cursor_x  = 0;
            OLED_cursor_y += (int16_t)OLED_textsize *
                        (uint8_t)pgm_read_byte(&OLED_gfxFont->yAdvance);
        } else if(c != '\r') {
            uint8_t first = pgm_read_byte(&OLED_gfxFont->first);
            if((c >= first) && (c <= (uint8_t)pgm_read_byte(&OLED_gfxFont->last))) {
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(
                  &OLED_gfxFont->glyph))[c - first]);
                uint8_t   w     = pgm_read_byte(&glyph->width),
                          h     = pgm_read_byte(&glyph->height);
                if((w > 0) && (h > 0)) { // Is there an associated bitmap?
                    int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                    if(OLED_wrap && ((OLED_cursor_x + OLED_textsize * (xo + w)) > OLED_curWidth)) {
                        OLED_cursor_x  = 0;
                        OLED_cursor_y += (int16_t)OLED_textsize *
                          (uint8_t)pgm_read_byte(&OLED_gfxFont->yAdvance);
                    }
                    OLED_drawChar(OLED_cursor_x, OLED_cursor_y, c, OLED_textcolor, OLED_textbgcolor, OLED_textsize);
                }
                OLED_cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)OLED_textsize;
            }
        }

    }
}

void OLED_setCursor(int16_t x, int16_t y) {
    OLED_cursor_x = x;
    OLED_cursor_y = y;
}

void OLED_setTextSize(uint8_t s) {
    OLED_textsize = (s > 0) ? s : 1;
}

void OLED_setTextColor(uint16_t c) {
    // For 'transparent' background, we'll set the bg
    // to the same as fg instead of using a flag
    OLED_textcolor = OLED_textbgcolor = c;
}

void OLED_setTextColorBg(uint16_t c, uint16_t b) {
    OLED_textcolor   = c;
    OLED_textbgcolor = b;
}

void OLED_setTextWrap(bool w) {
    OLED_wrap = w;
}

void OLED_setRotation(uint8_t x) {
    OLED_rotation = (x & 3);
    switch(OLED_rotation) {
        case 0:
        case 2:
            OLED_curWidth  = OLED_physWidth;
            OLED_curHeight = OLED_physHeight;
            break;
        case 1:
        case 3:
            OLED_curWidth  = OLED_physHeight;
            OLED_curHeight = OLED_physWidth;
            break;
    }
}

void OLED_setFont(GFXfont *f) {
    if(f) {            // Font struct pointer passed in?
        if(!OLED_gfxFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            OLED_cursor_y += 6;
        }
    } else if(OLED_gfxFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        OLED_cursor_y -= 6;
    }
    OLED_gfxFont = (GFXfont *)f;
}

// Broke this out as it's used by both the PROGMEM- and RAM-resident
// getTextBounds() functions.
void OLED_charBounds(char c, int16_t *x, int16_t *y,
  int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {

    if(OLED_gfxFont) {

        if(c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += OLED_textsize * (uint8_t)pgm_read_byte(&OLED_gfxFont->yAdvance);
        } else if(c != '\r') { // Not a carriage return; is normal char
            uint8_t first = pgm_read_byte(&OLED_gfxFont->first),
                    last  = pgm_read_byte(&OLED_gfxFont->last);
            if((c >= first) && (c <= last)) { // Char present in this font?
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(
                  &OLED_gfxFont->glyph))[c - first]);
                uint8_t gw = pgm_read_byte(&glyph->width),
                        gh = pgm_read_byte(&glyph->height),
                        xa = pgm_read_byte(&glyph->xAdvance);
                int8_t  xo = pgm_read_byte(&glyph->xOffset),
                        yo = pgm_read_byte(&glyph->yOffset);
                if(OLED_wrap && ((*x+(((int16_t)xo+gw)*OLED_textsize)) > OLED_curWidth)) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += OLED_textsize * (uint8_t)pgm_read_byte(&OLED_gfxFont->yAdvance);
                }
                int16_t ts = (int16_t)OLED_textsize,
                        x1 = *x + xo * ts,
                        y1 = *y + yo * ts,
                        x2 = x1 + gw * ts - 1,
                        y2 = y1 + gh * ts - 1;
                if(x1 < *minx) *minx = x1;
                if(y1 < *miny) *miny = y1;
                if(x2 > *maxx) *maxx = x2;
                if(y2 > *maxy) *maxy = y2;
                *x += xa * ts;
            }
        }

    } else { // Default font

        if(c == '\n') {                     // Newline?
            *x  = 0;                        // Reset x to zero,
            *y += OLED_textsize * 8;             // advance y one line
            // min/max x/y unchaged -- that waits for next 'normal' character
        } else if(c != '\r') {  // Normal char; ignore carriage returns
            if(OLED_wrap && ((*x + OLED_textsize * 6) > OLED_curWidth)) { // Off right?
                *x  = 0;                    // Reset x to zero,
                *y += OLED_textsize * 8;         // advance y one line
            }
            int x2 = *x + OLED_textsize * 6 - 1, // Lower-right pixel of char
                y2 = *y + OLED_textsize * 8 - 1;
            if(x2 > *maxx) *maxx = x2;      // Track max x, y
            if(y2 > *maxy) *maxy = y2;
            if(*x < *minx) *minx = *x;      // Track min x, y
            if(*y < *miny) *miny = *y;
            *x += OLED_textsize * 6;             // Advance x one char
        }
    }
}

// Pass string and a cursor position, returns UL corner and W,H.
void OLED_getTextBounds(char *str, int16_t x, int16_t y,
        int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = OLED_curWidth, miny = OLED_curHeight, maxx = -1, maxy = -1;

    while((c = *str++))
        OLED_charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}
