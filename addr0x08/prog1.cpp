
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

int icRec[5];

int main(int argc, char* argv[]){

//  int xdev = wiringPiI2CSetup(0x08);

//  if(bcm2835_init())

//    if(bcm2835_i2c_begin())

  char* fn = "/dev/i2c-1";
  int  addr = 0x08;
  int fd;

  char buff[30];

  if((fd = open(fn, O_RDWR)) >= 0)
  
    if(ioctl(fd, I2C_SLAVE, addr) >= 0)

      buff[0] = 101;

//      if(ioctl(fd, I2C_SLAVE, ))
    
      printf("success?! %i", buff[0]);

  if(buff[0]==101){

    read(fd, buff, 6);

    printf("read success?! %s \n", &buff[0]);

    }

  printf("response? %s", &buff[0]);

  write(fd, buff, 4);

  
  }