
#include <stdio.h>
#include <stdint.h>
//#include <wiringPiSPI.h>
//#include <wiringPi.h>

using namespace std;

const int gkMM = 2400;

const uint16_t tinyfont_numerals[38] = 
     {0x757, 0x222, 0x623, 0x616, 0x571, 0x326,
     0x466, 0x712, 0x376, 0x331, 0x002, 0x255, 0x466,
     0x747, 0x656, 0x343, 0x764, 0x657, 0x575, 0x727,
     0x726, 0x565, 0x447, 0x364, 0x174, 0x757, 0x664,
     0x662, 0x665, 0x326, 0x722, 0x557, 0x552, 0x136,
     0x525, 0x522, 0x623 };						  

typedef struct{ 
    unsigned char buff[1024];
    int len;
    } minibuffer;
    
typedef struct{ 
    unsigned char buff[512][3];
    int len;
    } channelbuffer;
    
typedef struct{ 
    unsigned char buff[320][480];
    } screenbuffer;

typedef struct{ 
    unsigned char high;
    unsigned char low;
    } f565;
    
typedef struct{ 
    uint16_t stringcode[40];
    } xcod;


typedef struct{ 
    bool bitcode[4][4];
    } zchar;



typedef channelbuffer rgb;

typedef minibuffer rft;

class graphkit {
  public:
    graphkit(){};
    void window(int x, int y, int w, int h);
    void clear(uint16_t bgcolor);
    void subclear(int x, int y, int w, int h, uint16_t bgcolor);
    void subpatch(minibuffer sp, int x, int y, int w, int h);
    
    uint16_t _buffer[gkMM];
	
  private:
    
    int _winx, _winy, _winw, _winh;
    int _winrx, _winry, _winlim;
    
    } ;
