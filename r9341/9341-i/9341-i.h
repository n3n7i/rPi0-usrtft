
#include <stdio.h>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

//#include "graphkit.h"

using namespace std;

class n3n_ILI9341 {
  public:
    n3n_ILI9341();
	
    void lcd(int rst, int cs, int cmd);
    void rasSpi(int mhz);

    void writeCommandB(uint8_t com);
    void spiWriteB(uint8_t b);

    void spiWriteS(uint8_t com);

    void TFTInit(void);

   //---------

    void setAddrWin(int16_t x0, int16_t y0, int16_t x1, int16_t y1);	
    void spiWrite16(uint16_t com);
    void spiWrite16f(uint16_t com);
    
    //uint8_t tinyfont_getind(char c);

  private:
    int _pin_rst, _pin_cs, _pin_cmd;
  };

class n3n_XP2046 {
  public:
    n3n_XP2046(){};
    void touchscreen( int cs, int irq);
    void read(void);

  private:
    int _pin_cs, _pin_irq;
  };


/*	

//	void clock_modifier(uint8_t xdiv);
//	void usb_modifier(uint8_t xsetting);
	
//	void writeCommand(uint8_t com);

//	uint8_t spiBang(uint8_t inp);
//	uint8_t readCommand(uint8_t com, uint8_t getbyte);
	
//    void spiWrite(uint8_t b);

//	void spiWrite16(uint16_t b);
//	void spiWriteN(uint8_t b[], uint8_t n);
//	void spiBufferN(uint8_t b[16], uint8_t n);
//	void spiBufferN2(int n);
//	uint8_t spiRead(uint8_t b);	


	uint8_t tinyfont_getind(char c);
//	int tinyfont(uint8_t ind);
	uint8_t tinyfont_Sbuff(char buff[], uint8_t start, uint8_t fsize, uint8_t len, uint16_t wid, uint8_t row);
	uint8_t tinyfont_Scalc(uint8_t fsize, uint16_t wid);
	void tinyfont_Sbuff_GFX(uint8_t clen, uint8_t fsize, uint8_t fsize_h, uint16_t fcol, uint16_t bcol, uint16_t wid);
	
	void tinyfont_print(char strx[], uint8_t strlen, uint8_t fsize, uint16_t fcol, uint16_t bcol, uint16_t winwid);
	

	void setAddrWin(int16_t x0, int16_t y0, int16_t x1, int16_t y1);	
	void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

//	void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
	void drawLineB(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

    uint16_t s1(uint16_t xA, int16_t xB, uint8_t xN);
	uint16_t s2(uint16_t xA, uint16_t xB, uint16_t xC, uint8_t xN);
	
	void drawCurve(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
	void drawWin(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colorBack, uint16_t colorBorder);
	
	n7i collectCurve(n7i_curve curveX, uint8_t pointx);
	void drawSurf(n7i_surf surfN, uint16_t colour);
		
	uint8_t _buffN[16]; //2 byte per pixel?
	uint8_t _buffS[32]; //2 char per byte
	

//  Morse(int pin);
//    void dot();
//    void dash();

*/

// These are the ILI9341 control registers
#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0A
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_VSCRDEF 0x33
#define ILI9341_MADCTL  0x36
#define ILI9341_VSCRSADD 0x37
#define ILI9341_PIXFMT  0x3A

#define ILI9341_RAMWR_C   0x3C
#define ILI9341_RAMRD_C   0x3E


#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

