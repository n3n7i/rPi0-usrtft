
//---

#include <stdio.h>
#include <stdint.h>

#include <wiringPiSPI.h>
#include <wiringPi.h>

#include "9341-i/9341-i.cpp"
#include "9341-i/graphkit.cpp"

typedef n3n_ILI9341 xtft;

//uint8_t xarr[16]={0x01,0x11, 0x02,0x21, 0x03,0x31, 0x05,0x51, 0x07,0x71, 0x0a,0xa1, 0x0b,0xb1, 0x0c,0xc1};

 xtft ntft;

 graphkit tftbuff;
 
 n3n_XP2046 ntouch;
 
 screenbuffer xstore;

void burstspi(screenbuffer xframe){
  for(int k=0; k<320; k+=2)
    wiringPiSPIDataRW(0, xframe.buff[k], 960);
  }

void xtft_setup(){ //xtft mtft) {

  ntft = n3n_ILI9341();
  
  ntouch = n3n_XP2046();
  
  //n3n_ILI9341 ntft = n3n_ILI9341();
  
  printf("tft setpins \n");

// cs:2, res:0, dc:7  
//  n3n_ILI9341 ntft = n3n_ILI9341();
  
//  SPI.begin();
//  SPI.beginTransaction(SPISettings(12000000, MSBFIRST, SPI_MODE0));

  ntft.rasSpi(32000000);
  ntft.lcd(0, 2, 7);
  ntouch.touchscreen(4, 5);
  ntft.TFTInit();
  ntft.writeCommandB(ILI9341_RAMWR);
  

  tftbuff = graphkit();
  tftbuff.window(0,0,80,60);    
  tftbuff.clear(0xF00F);
  
  printf("test %i ", tftbuff._buffer[1]);
  printf("spz: %i", tftbuff._buffer[2] & 0xff);
  printf("spx: %i", (tftbuff._buffer[2])>>8 & 0xff);

  //tftbuff.subclear(10,10,10,10, 0xF00F);

  //unsigned char tftburst[500];
  minibuffer tftb; unsigned char zz=0;
  minibuffer tfts;
  
  //ntft.setAddrWin(40, 20, 200, 300);
  //for(int xloop = 0; xloop<25; xloop++){ 
  f565 picker; // = rgb16(0, 127, 0);
  
  for(int k=0; k<480; k+=2) { 
    picker = rgb16(k%128, 127, k%250);
    tftb.buff[k]=picker.high; 
    tftb.buff[k+1]=picker.low;
    }
  
  picker = rgb16(127, 0,0);
  for(int k=40; k<360; k+=2) {
     picker = rgb16(255 - k%28, k%250, 127);
     tfts.buff[k]=picker.high; tfts.buff[k+1]=picker.low; }
  
  for(int x1=0;x1<320;x1++)
    for(int x2=0;x2<480;x2++){
      if((x1%50) < 25) xstore.buff[x1][x2] = tftb.buff[x2];
      if((x1%50) >= 25) xstore.buff[x1][x2] = tfts.buff[x2];
    }
    //tftburst[k] = (unsigned char) k%255;
  
  ntft.setAddrWin(0, 0, 240, 320);
  ntft.spiWrite16(0);
  for(int zk=0; zk<320; zk+=1){
    
    burstspi(xstore);
  /*for(int k=0; k<320; k+=8){
  //for(int m=0; m<2; m++){
  //for(int j=0; j<60; j++){
    //tftb = tftbuff.subpatch(k,1, 5,5);
    //printf("testj %i ", tftburst[1]);  
    //for(int k2=0; k2<480; k2++) tfts.buff[k2+((k%24)-5)*(k%2)] = tftb.buff[k2];
    //tfts = scanline(tfts, xstore, k);
    //wiringPiSPIDataRW(0, tfts.buff, 480);
    wiringPiSPIDataRW(0, xstore.buff[k], 3840);
    //delay(25);
    //ntft.spiWrite16f(0xFFFF);
    };*/
    //ntft.setAddrWin(40, 20, 100, 280);
  //  if(zk==1)ntft.writeCommandB( ILI9341_RAMRD);
  //  if(zk==2)ntft.writeCommandB( ILI9341_RAMWR);
  
  /*for(int k=0; k<480; k+=2) { 
    picker = rgb16(k%128, 127, k%250);
    tftb.buff[k]=picker.high; 
    tftb.buff[k+1]=picker.low;
    }
  
  picker = rgb16(127, 0,0);
  for(int k=40; k<360; k+=2) {
     picker = rgb16(255 - k%28, k%250, 127);
     tfts.buff[k]=picker.high; tfts.buff[k+1]=picker.low; }
  */
  for(int x1=zk;x1<320;x1++)
    for(int x2=x1;x2<480-x1;x2++){
      if(((x1-zk)%50) < 25) xstore.buff[x1][x2] = tftb.buff[x2];
      if(((x1-zk)%50) >= 25) xstore.buff[319-x1][x2] = tfts.buff[x2];
    }
  
  
  }
  //ntft.setAddrWin(40, 20, 60, 180);
  ntft.setAddrWin(40, 20, 200, 300);
  
  printf("test?!");
  ntouch.read();
  printf("done?!");
  
  int xcomp = 0;
  int zcomp =0;
  while(xcomp < 250){
    delay(75);
    zcomp = digitalRead(5);
    if(zcomp == 0){ printf(" Touch! %i \n", zcomp); xcomp++; ntouch.read(); }
    fflush(stdout);
    //xcomp += zcomp;
    }
    
  
  //ntft.spiWrite16(0);
  //for (int xi=0; xi<200; xi++) wiringPiSPIDataRW(0, tftb.buff, 480);
//  uint16_t x=0;
//  uint16_t y=0;
//  uint8_t x8=0;

  //return 0;
  }

void xwin_setup(void) {

  tftbuff = graphkit();
  tftbuff.window(0,0,80,60);
    
  tftbuff.clear(0);

  }



int main(int argc, char *argv[]){

  //ntft.n3n_ILI9341();
  
/*  ntft.rasSpi(12000000);
  ntft.lcd(0, 2, 7);
  ntft.TFTInit();

  ntft.writeCommandB(ILI9341_RAMWR);
*/

  //ntft = n3n_ILI9341{};

  xtft_setup(); //ntft);
  //xwin_setup();
  
  char tx[] = {"testsgring"};
  
  printf(" %s", tx);

  xcod zz =  stringmap(tx, 8);
  
  zchar zx;
  
  for (int i=0; i<8; i++) printf(" %i", zz.stringcode[i]);
  
  zx = charmap(zz.stringcode[1], zx); 
  
  printf("\n~ %i \n", zz.stringcode[1]);

  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
        printf(" %i, ", (zx.bitcode[i][j]));} printf("\n");}

  return 0;
  }

