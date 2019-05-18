
#include <QPULib.h>
#include <sys/time.h>
#include <stdlib.h>


/*
void rot3D_1(Int n, Float cosTheta, Float sinTheta, Ptr<Float> x, Ptr<Float> y)
{
  For (Int i = 0, i < n, i = i+16)
    Float xOld = x[i];
    Float yOld = y[i];
    x[i] = xOld * cosTheta - yOld * sinTheta;
    y[i] = yOld * cosTheta + xOld * sinTheta;
  End
}
*/

// Define function that runs on the GPU.

//p + (me() << 4);
//    *p = me();
//    q = q + (me() << 4);
//    r = r + (me() << 4);


void hellob(Int n, Int c, Ptr<Int> p, Ptr<Int> q, Ptr<Int> r)
{
  Int j=0, k=me()<<4, l;
  For (Int i = 0, i < n, i = i+ (16*c))
    l = k+i;
    p[l] = me(); 
    q[l] = index();
    r[l] = j;
    j = j+1;
  End
}


void absmooth(Int n, Int c, Float xnv, Float p, Float q, Ptr<Float> r){

  Int j=0, k=me()<<4, l=0, g=c*16;

  Float x = (q-p), z=p;

  For (Int i = 0, i < n, i = i+g)
    l = index() + k+i;

//    x = x + xnv;

    r[l] = z + x*toFloat(l)*xnv;

    //z = z + x; //*index();

//    p[l+g] = p[j];
//    q[l+g] = q[j];

    //q[l] = index();
    //r[l] = j;
    j = j+1;
  End
}




int main()
{

  int cores = 2;

  int xrun = 1;

  int count = 16*cores*xrun;

  timeval tvStart, tvEnd, tvDiff;

  gettimeofday(&tvStart, NULL);

  // Construct kernel
  auto k = compile(absmooth);

  gettimeofday(&tvEnd, NULL);
  timersub(&tvEnd, &tvStart, &tvDiff);

  printf("compile time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);

  gettimeofday(&tvStart, NULL);

  // Allocate and initialise array shared between ARM and GPU
  SharedArray<float> array(count);
  SharedArray<float> arrayb(count);
  SharedArray<float> arrayc(count);

  gettimeofday(&tvEnd, NULL);
  timersub(&tvEnd, &tvStart, &tvDiff);
  printf("smem alloc time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);


  for (int i = 0; i < count; i++){
    array[i] = i;     arrayb[i] = count-i; 
    arrayc[i] = 0;}

  // Invoke the kernel and display the result
  k.setNumQPUs(cores);

  gettimeofday(&tvStart, NULL);

  float zz = 1.0/((16*cores) -1), zx=0.0, zy=10.0;

  printf("xf: %f\n", zz);

  k(16*xrun*cores, cores, zz, zx, zy, &arrayc);

  gettimeofday(&tvEnd, NULL);


  for (int i = 0; i < count; i++) {
    printf("%i: %f.  ", i,  arrayc[i]); //array[i], arrayb[i],
    if(i%4 == 3) printf("\n");
    }
  
  timersub(&tvEnd, &tvStart, &tvDiff);
  printf("run time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);

//  free(&array);
//  free(&arrayb);
//  free(&arrayc);



  return 0;


}
