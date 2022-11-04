//============================================================================
// lcd.h: Adapted from the lcdwiki.com examples.
//============================================================================

#ifndef __LCD_H
#define __LCD_H
#include <stdint.h>
// shorthand notation for 8-bit and 16-bit unsigned integers
typedef uint8_t u8;
typedef uint16_t u32;

// The LCD device structure definition.
//
typedef struct
{
    u32 width;
    u32 height;
    u32 id;
    u8  dir;
    u32  wramcmd;
    u32  setxcmd;
    u32  setycmd;
    void (*reset)(int);
    void (*select)(int);
    void (*reg_select)(int);
} lcd_dev_t;

// The LCD device.
// This will be initialized by LCD_direction() so that the
// width and height will be appropriate for the rotation.
// The setxcmd and setycmd will be set so that cursor selection
// is defined properly for the rotation.
extern lcd_dev_t lcddev;

// Rotation:
// 0: rotate 0
// 1: rotate: 90
// 2: rotate: 180
// 3: rotate 270
#define USE_HORIZONTAL       0

// The dimensions of the display.
#define LCD_W 480
#define LCD_H 320

// Some popular colors
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define YELLOW      0XFFE0
#define GBLUE       0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define GRAY        0X8430
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458
#define LIGHTGREEN  0X841F
#define LIGHTGRAY   0XEF5B
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12


#define ILI9488_NOP     0x00
#define ILI9488_SWRESET 0x01
#define ILI9488_RDDID   0x04
#define ILI9488_RDDST   0x09

#define ILI9488_SLPIN   0x10
#define ILI9488_SLPOUT  0x11
#define ILI9488_PTLON   0x12
#define ILI9488_NORON   0x13

#define ILI9488_RDMODE  0x0A
#define ILI9488_RDMADCTL  0x0B
#define ILI9488_RDPIXFMT  0x0C
#define ILI9488_RDIMGFMT  0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  0x20
#define ILI9488_INVON   0x21
#define ILI9488_GAMMASET 0x26
#define ILI9488_DISPOFF 0x28
#define ILI9488_DISPON  0x29

#define ILI9488_CASET   0x2A
#define ILI9488_PASET   0x2B
#define ILI9488_RAMWR   0x2C
#define ILI9488_RAMRD   0x2E

#define ILI9488_PTLAR   0x30
#define ILI9488_MADCTL  0x36
#define ILI9488_PIXFMT  0x3A

#define ILI9488_FRMCTR1 0xB1
#define ILI9488_FRMCTR2 0xB2
#define ILI9488_FRMCTR3 0xB3
#define ILI9488_INVCTR  0xB4
#define ILI9488_DFUNCTR 0xB6

#define ILI9488_PWCTR1  0xC0
#define ILI9488_PWCTR2  0xC1
#define ILI9488_PWCTR3  0xC2
#define ILI9488_PWCTR4  0xC3
#define ILI9488_PWCTR5  0xC4
#define ILI9488_VMCTR1  0xC5
#define ILI9488_VMCTR2  0xC7

#define ILI9488_RDID1   0xDA
#define ILI9488_RDID2   0xDB
#define ILI9488_RDID3   0xDC
#define ILI9488_RDID4   0xDD

#define ILI9488_GMCTRP1 0xE0
#define ILI9488_GMCTRN1 0xE1


#define ILI9488_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9488_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9488_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9488_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9488_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9488_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9488_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9488_RED         0xF800      /* 255,   0,   0 */
#define ILI9488_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9488_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9488_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9488_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0xF81F

void LCD_Setup(void);
void LCD_Init(void (*reset)(int), void (*select)(int), void (*reg_select)(int));
void LCD_Clear(u32 Color);
void LCD_DrawPoint(u32 x,u32 y,u32 c);
void LCD_DrawLine(u32 x1, u32 y1, u32 x2, u32 y2, u32 c);
void LCD_DrawRectangle(u32 x1, u32 y1, u32 x2, u32 y2, u32 c);
void LCD_DrawFillRectangle(u32 x1, u32 y1, u32 x2, u32 y2, u32 c);
void LCD_Circle(u32 xc, u32 yc, u32 r, u32 fill, u32 c);
void LCD_DrawTriangle(u32 x0,u32 y0, u32 x1,u32 y1, u32 x2,u32 y2, u32 c);
void LCD_DrawFillTriangle(u32 x0,u32 y0, u32 x1,u32 y1, u32 x2,u32 y2, u32 c);
void LCD_DrawChar(u32 x,u32 y,u32 fc, u32 bc, char num, u8 size, u8 mode);
void LCD_DrawString(u32 x,u32 y, u32 fc, u32 bg, const char *p, u8 size, u8 mode);
void LCD_SetWindow(uint32_t xStart, uint32_t yStart, uint32_t xEnd, uint32_t yEnd);
void LCD_WriteData16_Prepare();
void LCD_WriteData16_End();
void LCD_WriteData16(u32 data);
void fillRect(u32 sx, u32 sy, u32 ex, u32 ey, u32 color);
void setAddrWindow(uint32_t xStart, uint32_t yStart, uint32_t xEnd, uint32_t yEnd);
void write16BitColor(uint16_t color);
void spiwrite(uint8_t data);
//===========================================================================
// C Picture data structure.
//===========================================================================
typedef const struct {
    unsigned int   width;
    unsigned int   height;
    unsigned int   bytes_per_pixel; // 2:RGB16, 3:RGB, 4:RGBA
    unsigned char  pixel_data[0]; // variable length array
} Picture;

void LCD_DrawPicture(u32 x0, u32 y0, const Picture *pic);

#endif
