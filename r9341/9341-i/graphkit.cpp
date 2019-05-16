
#include "graphkit.h"

#include <stdio.h>
#include <stdint.h>

//graphkit::graphkit(void){}

void graphkit::window(int x, int y, int w, int h){
  _winx = x;   _winy = y;   _winw = w;   _winh = h;
  _winrx = 4;   _winry = 4; _winlim = w * h; 
  }

void graphkit::clear(uint16_t bgcolor){
  for (int i=0; i<_winlim; i++){
    _buffer[i] = bgcolor+0;
    }
  }
  
void graphkit::subclear(int x, int y, int w, int h, uint16_t bgcolor){
  int x1 = x+1; int x2=x+w;
  int y1 = y; int y2=y+h;
  int k;
  for (int i=x1; i<x2; i++){
    k = _winh * (i-1);
    for (int j=k+y1; j<k+y2; j++){
      _buffer[j] = bgcolor;
      }
    }
  }
  

void graphkit::subpatch(minibuffer sp, int x, int y, int w, int h){
  //minibuffer sp;
  int x1 = x+1; int x2=x+w;
  int y1 = y; int y2=x+h;
  int k; int vx=0;
  uint16_t xtemp;
  for (int i=x1; i<x2; i++){
    k = _winh * (i-1);
    for (int j=k+y1; j<k+y2; j++){
      xtemp = _buffer[j];
      for(int m=0; m<_winrx; m++){
        sp.buff[vx+1] = 0xff & xtemp;
        sp.buff[vx] = ((xtemp)>>8) & 0xff ;        
        vx+=2;        
        }
      }
    }
  //printf("sp: %i", burst[1]);
  //printf("spx: %i", _buffer[2]);
  //printf("spz: %i", _buffer[2] & 0xff);
  //printf("spx: %i", (_buffer[2])>>8 & 0xff);
  //return sp;
  }

void thrxtwo(rgb chan3, rft out2){
  int j=chan3.len;
  unsigned char r1, r2, r3;
  for (int i=0; i<j; i++){
    r1 = chan3.buff[i][0];
    r2 = chan3.buff[i][1];
    r3 = chan3.buff[i][2];
    out2.buff[i*2] = r1 & 0xF0 | r2>>5;
    out2.buff[i*2 +1] = r2 & 0xB0 | r3>>3;
    }
  out2.len = 2*j;
  }

rft scanline(rft outp, screenbuffer xinp, int col){
  for(int i=0; i<480; i++)
    outp.buff[i] = xinp.buff[col][i];
  return outp;
  }
  
void scanline_win(rft outp, screenbuffer xinp, int col, int start, int stop){
  for(int i=start; i<start+stop; i++)
    outp.buff[i] = xinp.buff[col][i];
  }

f565 rgb16(uint8_t r1, uint8_t r2, uint8_t r3){
  f565 out2;
  out2.high = r1 & 0xF0 | (r2+0)>>5;
  out2.low  = r2 & 0xB0 | (r3+0)>>3;
  return out2;
  }
  
uint8_t tinyfont_getind(char c){
  uint8_t r1 = c - '0'; //((int) c) - ((int) '0'); 
  if(r1>9){ // || (r1>9)){
    r1 = c - 'A'+11; //10 + ((int) c) - ((int) 'A'); 
    if((r1<12) || (r1>37))
      r1 = c-'a'+11; 
	if(r1<10) r1+37;
  }
  return r1;
  }

xcod stringmap(char getstr[], int len){
  xcod xd;
  for(int i=0; i<len; i++){
    xd.stringcode[i] = tinyfont_numerals[tinyfont_getind(getstr[i])];
    }
  return xd;
  }

zchar charmap(uint16_t scode, zchar zcode){
  unsigned int n=0x0001;  
  printf("%i \n", scode);
  for (int i=3; i>=0; i--){
    for (int j=3; j>=0; j--){
      printf("%i", ((scode & n) > 0));
      zcode.bitcode[i][j] = 0+((scode & n) > 0);
      n=n<<1;
      }      printf("\n");}
  return zcode;
  }
    


void textscan(xcod inp, int len, screenbuffer xout, int stcol, f565 color){
  int zcol = stcol;
  
  for(int i=0; i<len; i++){
    int xinp = inp.stringcode[i];
    for(int j=0; j<12; j++){
      
    
    //xout.buff[i][zcol] = ;
  }}


}
