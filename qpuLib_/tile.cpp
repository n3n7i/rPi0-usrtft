
#include <QPULib.h>
#include <sys/time.h>
#include <stdlib.h>


void tileread_f(Int hop, Int step, Int tX, Int tY, Ptr<Float> p, Ptr<Float> q){ //  Ptr<Float> r, Ptr<Float> s Float xnv, Int prc

  Int jump = hop*step;
  Int l=0;

  For (Int j = 0, j <= step, j++) //i = i+g)

    l = hop*j + step*tY + jump*tX; 
    q[j*16 + index()] = p[l + index()];
    End;
  }


void tilewrite_f(Int hop, Int step, Int tX, Int tY, Ptr<Float> p, Ptr<Float> q){ //  Ptr<Float> r, Ptr<Float> s Float xnv, Int prc

  Int jump = hop*step;
  Int l=0;

  For (Int j = 0, j <= step, j++) //i = i+g)

    l = hop*j + step*tY + jump*tX; 
    q[l + index()] = p[j*16 + index()];
    End;
  }



int main()
{

  int cores;

  int blocks;

  int xrun;

  int prec = 16;

  int count;// = xrun * prec;


  int gridX = 240;
  int gridY = 320;

  int tileX = 16;

  count = gridX*gridY;
  cores=1;
//  blocks=2;
//  prec=16;
//  xrun = 16*cores*blocks;
// count = xrun * prec;

  timeval tvStart, tvEnd, tvDiff;

  gettimeofday(&tvStart, NULL);

  // Construct kernel
  auto k = compile(tileread_f);

  gettimeofday(&tvEnd, NULL);
  timersub(&tvEnd, &tvStart, &tvDiff);

  printf("compile time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);

  gettimeofday(&tvStart, NULL);

  // Allocate and initialise array shared between ARM and GPU
  SharedArray<float> array(count);
  SharedArray<float> arrayc(count);


  gettimeofday(&tvEnd, NULL);
  timersub(&tvEnd, &tvStart, &tvDiff);
  printf("smem alloc time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);


  for (int i = 0; i < count; i++){
    array[i]  = (float) i;     
    arrayc[i] = (float) i; }


  // Invoke the kernel and display the result
  k.setNumQPUs(cores);

  gettimeofday(&tvStart, NULL);


  k(gridX, tileX, 5,5, &arrayc, &array);


  gettimeofday(&tvEnd, NULL);


  for (int i = 0; i < 24*16; i++) {
    printf("%i: %.1f, ", i, array[i]); //%.1f.  ", i,  array[i], arrayb[i]); //array[i], arrayb[i],
    if(i%4 == 3) printf("\n");
    if(i%16 == 15) printf("\n");
    }
  
  timersub(&tvEnd, &tvStart, &tvDiff);
  printf("run time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);

  k.setNumQPUs(0);

  return 0;
}
