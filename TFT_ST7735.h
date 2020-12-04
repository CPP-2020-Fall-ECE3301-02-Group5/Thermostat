#ifndef TFT_ST7735_H
#define TFT_ST7735_H

void drawRoundRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char r, unsigned int color);
void fillRoundRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char r, unsigned int color);
void drawTriangle(signed int x0, signed int y0, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);
void fillTriangle(signed int x0, signed int y0, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);
void setTextWrap(BOOL );
void invertDisplay(BOOL );
void NormalDisplay();
int Color565(int r, int g, int b);
void spiwrite(unsigned char spidata);
void write_command(unsigned char cmd_);
void write_data(unsigned char data_);
void Rcmd1();
void Rcmd2red();
void Rcmd2green();
void Rcmd3();
void setScrollDefinition(unsigned char top_fix_height, unsigned char bottom_fix_height, BOOL _scroll_direction);
void setAddrWindow(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1);
void drawPixel(unsigned char x, unsigned char y, unsigned int color);
void drawFastVLine(unsigned char x, unsigned char y, unsigned char h, unsigned int color);
void fillRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned int color);
void drawtext(unsigned char x, unsigned char y, char *_text, unsigned int color, unsigned int bg, unsigned char size);
void drawChar(unsigned char x, unsigned char y, unsigned char c, unsigned int color, unsigned int bg,  unsigned char size);
void fillRectangle(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned int color);
void drawCircle(signed int x0, signed int y0, signed int r, unsigned int color);
void drawCircleHelper(signed int x0, signed int y0, signed int r, unsigned int cornername, unsigned int color);
void fillCircle(signed int x0, signed int y0, signed int r, unsigned int color);
void fillCircleHelper(signed int x0, signed int y0, signed int r, unsigned int cornername, signed int delta, unsigned int color);
void fillScreen(unsigned int color) ;
void drawFastHLine(unsigned char x, unsigned char y, unsigned char w, unsigned int color);
void VerticalScroll(unsigned char _vsp);
void TFT_BlackTab_Initialize();
void TFT_GreenTab_Initialize();
void TFT_RedTab_Initialize(void);
void TFT_ST7735B_Initialize(void);
void LCD_Reset(void);
void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);

BOOL wrap = TRUE;


unsigned char colstart = 0, rowstart = 0, _tft_type;
#define _swap(a, b) { signed int t; t = a; a = b; b = t;}
#define _width         128
#define _height        160
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
#define ST7735_PTLAR   0x30
#define ST7735_VSCRDEF 0x33
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
#define ST7735_VSCRSADD 0x37
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD
#define ST7735_PWCTR6  0xFC
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define   ST7735_BLACK   0x0000
#define   ST7735_BLUE    0x001F
#define   ST7735_RED     0xF800
#define   ST7735_GREEN   0x07E0
#define   ST7735_CYAN    0x07FF
#define   ST7735_MAGENTA 0xF81F
#define   ST7735_YELLOW  0xFFE0
#define   ST7735_WHITE   0xFFFF

#endif /* TFT_ST7735_H */