
#include "9341-i.h"

#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>

n3n_ILI9341::n3n_ILI9341(void) {
  //wiringPiSetup();
  }

void n3n_ILI9341::lcd(int rst, int cs, int cmd){
  pinMode(rst, OUTPUT);
  digitalWrite(rst, HIGH);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  pinMode(cmd, OUTPUT);
  digitalWrite(cmd, HIGH);
  _pin_rst = rst;
  _pin_cs = cs;  
  _pin_cmd = cmd;  
}

void n3n_ILI9341::rasSpi(int mhz){

  wiringPiSetup();
  wiringPiSPISetup (0, mhz);
  
  }

void n3n_ILI9341::writeCommandB(unsigned char com){
    
  unsigned char xsend[2];
  xsend[0] = com;
  digitalWrite(_pin_cmd, LOW);
  digitalWrite(_pin_cs, LOW);
	
  wiringPiSPIDataRW(0, xsend, 1);
  digitalWrite(_pin_cs, HIGH);
  }


void n3n_ILI9341::spiWriteB(uint8_t com){

  unsigned char xsend[2];
  xsend[0] = com;

  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);	

  wiringPiSPIDataRW(0, xsend, 1);
  
  //digitalWrite(_pin_cs, HIGH);
  }

void n3n_ILI9341::spiWriteS(uint8_t com){

  unsigned char xsend[2];
  xsend[0] = com;

//  digitalWrite(_pin_cmd, HIGH);
//  digitalWrite(_pin_cs, LOW);	

  wiringPiSPIDataRW(0, xsend, 1);
  
//  digitalWrite(_pin_cs, HIGH);
  }

void n3n_ILI9341::spiWrite16(uint16_t com){

  uint8_t xsend[2];
  xsend[1] = com & 0xff;
  xsend[0] = (com>>8) & 0xff;


  digitalWrite(_pin_cmd, HIGH);
  digitalWrite(_pin_cs, LOW);	

  wiringPiSPIDataRW(0, xsend, 2);
  
  //digitalWrite(_pin_cs, HIGH);
  }

void n3n_ILI9341::spiWrite16f(uint16_t com){

  uint8_t xsend[2];
  xsend[1] = com & 0xff;
  xsend[0] = (com>>8) & 0xff;

//  digitalWrite(_pin_cmd, HIGH);
//  digitalWrite(_pin_cs, LOW);	

  wiringPiSPIDataRW(0, xsend, 2);
  
//  digitalWrite(_pin_cs, HIGH);
  }




void n3n_ILI9341::TFTInit(void){
	
//	Serial.println("Init begin!!");
	
//	SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));

    // toggle RST low to reset
    //if (1){ //(_pin_rst >= 0) {
//        pinMode(_rst, OUTPUT);
        digitalWrite(_pin_rst, HIGH);
        delay(50);
        digitalWrite(_pin_rst, LOW);
        digitalWrite(_pin_cmd, LOW);
	  digitalWrite(_pin_cs, LOW);
        delay(250);
        digitalWrite(_pin_rst, HIGH);
 	digitalWrite(_pin_cmd, HIGH);
 	digitalWrite(_pin_cs, HIGH);
        
        delay(50);
    //}

    //startWrite();

    //writeCommand(0xEF); //?
    //spiWrite(0x03);
    //spiWrite(0x80);
    //spiWrite(0x02);
	
	//noInterrupts();
//	SREG &= ~(1<<7);

    writeCommandB(0xCF);  // power control B
    spiWriteB(0x00); //static
    //spiWriteB(0XC1); // 0x99?
	spiWriteB(0x99); // 0x99?
    spiWriteB(0x30); // discharge enable

    writeCommandB(0xED); // power on sequence control
    spiWriteB(0x64); // cp1 first frame
    //spiWriteB(0x03); // en vcl / en ddvd
	spiWriteB(0x13); // en vcl / en ddvd
    spiWriteB(0x12); // en vgh / vgl
    spiWriteB(0x81); // ddvdh enhance?

    writeCommandB(0xE8);  // driver timing control A
    spiWriteB(0x85); //+1 unit no-overlap
    spiWriteB(0x00); //1 unit/1 unit eq timing
    spiWriteB(0x78); // 2 unit precharge

    writeCommandB(0xCB); // power control A
    spiWriteB(0x39);
    spiWriteB(0x2C);
    spiWriteB(0x00);
    spiWriteB(0x34);  //vcore
    spiWriteB(0x02);  // ddvdh = 5.6v

    writeCommandB(0xF7); // pump ratio control
    spiWriteB(0x20); // ddvd = 2*vcl

    writeCommandB(0xEA); // driver timing control B
    spiWriteB(0x00); // default 1/2/1/2
    spiWriteB(0x00); // ?


    writeCommandB(ILI9341_PWCTR1);    //Power control 0xC0
    spiWriteB(0x23);   //VRH[5:0]  gvdd 4.5V

    writeCommandB(ILI9341_PWCTR2);    //Power control 0xC1
    spiWriteB(0x10);   //SAP[2:0];BT[3:0] vgh = vcl*7 / vgl = -vcl*4

    writeCommandB(ILI9341_VMCTR1);    //VCM control 0xc5
    spiWriteB(0x3e); //vcomH = 4.25V
    spiWriteB(0x28); //vcomL = -1.5V

    writeCommandB(ILI9341_VMCTR2);    //VCM control2
    spiWriteB(0x86);  //-- minus 58?

    writeCommandB(ILI9341_MADCTL);    // Memory Access Control
    spiWriteB(0x48);

    writeCommandB(ILI9341_VSCRSADD); // Vertical scroll
    //SPI_WRITE16(0);                 // Zero
	//spiWriteB16(0x0000);
	spiWriteB(0x00);
	spiWriteB(0x00);
	//*/
	
    writeCommandB(ILI9341_PIXFMT);
    spiWriteB(0x55);

    writeCommandB(ILI9341_FRMCTR1); //0xB1
    spiWriteB(0x00); // frame rate div 1
    spiWriteB(0x18); // 79Hz

    writeCommandB(ILI9341_DFUNCTR);    // Display Function Control 0xB6
    spiWriteB(0x08);
    spiWriteB(0x82);
    spiWriteB(0x27);

    writeCommandB(0xF2);    // 3Gamma Function Disable
    spiWriteB(0x00);

    writeCommandB(ILI9341_GAMMASET);    //Gamma curve selected 0x26
    spiWriteB(0x01);

    writeCommandB(ILI9341_GMCTRP1);    //Set Gamma
    spiWriteB(0x0F);
    spiWriteB(0x31);
    spiWriteB(0x2B);
    spiWriteB(0x0C);
    spiWriteB(0x0E);
    spiWriteB(0x08);
    spiWriteB(0x4E);
    spiWriteB(0xF1);
    spiWriteB(0x37);
    spiWriteB(0x07);
    spiWriteB(0x10);
    spiWriteB(0x03);
    spiWriteB(0x0E);
    spiWriteB(0x09);
    spiWriteB(0x00);

    writeCommandB(ILI9341_GMCTRN1);    //Set Gamma
    spiWriteB(0x00);
    spiWriteB(0x0E);
    spiWriteB(0x14);
    spiWriteB(0x03);
    spiWriteB(0x11);
    spiWriteB(0x07);
    spiWriteB(0x31);
    spiWriteB(0xC1);
    spiWriteB(0x48);
    spiWriteB(0x08);
    spiWriteB(0x0F);
    spiWriteB(0x0C);
    spiWriteB(0x31);
    spiWriteB(0x36);
    spiWriteB(0x0F);

    writeCommandB(ILI9341_SLPOUT);    //Exit Sleep
	
//	SREG |= (1<<7);
	//interrupts();
	
    delay(120);
//	SREG &= ~(1<<7);
    writeCommandB(ILI9341_DISPON);    //Display on
//	SREG |= (1<<7);
    delay(120);
	
//	Serial.println("display init complete?");
    //endWrite();

    //_width  = ILI9341_TFTWIDTH;
    //_height = ILI9341_TFTHEIGHT;
	
//	SPI.endTransaction();

}

void n3n_ILI9341::setAddrWin(int16_t x0, int16_t y0, int16_t x1, int16_t y1){

  writeCommandB(ILI9341_CASET);
  spiWrite16(x0);
  spiWrite16(x1);
  
  writeCommandB(ILI9341_PASET);
  spiWrite16(y0);
  spiWrite16(y1);
  
  //writeCommandB(ILI9341_RAMRD);
  writeCommandB(ILI9341_RAMWR);
  //writeCommandB(ILI9341_RAMRD_C);
  
  }

void n3n_XP2046::touchscreen(int cs, int irq){
//  pinMode(rst, OUTPUT);
//  digitalWrite(rst, HIGH);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  pinMode(irq, INPUT);
  //digitalWrite(cmd, HIGH);
  //_pin_rst = rst;
  _pin_cs = cs;  
  _pin_irq = irq;
  
  //digitalWrite(cs, LOW);
  //wiringPiSPIDataRW(0, xsend, 1);
  
   wiringPiSPISetup (1, 2000000);

}

void n3n_XP2046::read(void){

unsigned char ts[16] = {0xB1, 0,0,0, 0xC1, 0,0,0, 0x91, 0,0,0, 0xD0, 0,0,0};
unsigned int x1, y1;

  digitalWrite(_pin_cs, LOW);
  wiringPiSPIDataRW(1, ts, 16);
 
  
  for(int i=0; i<16; i++){ printf("%i ", ts[i]);}
  x1 = ts[9]<<4 | ts[10]>>4;
  y1 = ts[13]<<4 | ts[14]>>4;
  
  //x1 = x1>>4;
  //y1 = y1>>4;
  
  printf("\n read! %i %i \n", x1, y1);
  
  digitalWrite(_pin_cs, HIGH);
}
