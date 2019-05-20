
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


void absmooth(Int n, Int c, Float xnv, Ptr<Float> p, Ptr<Float> q, Ptr<Float> r){

  Int j=0, k=me()<<4, l=0, g=c*16;

  Float x, z;

    x = q[j+k];
    z = p[j+k];

    x = x-z;


//  Int i=0;
  For (Int i = 0, i < n, i = i+g)

    l = index() + k+i;

    r[l] = z + x*toFloat(j)*xnv;

    j = j+1;
  End
}


void acsmooth(Int n, Int c, Float xnv, Ptr<Float> p, Ptr<Float> q, Ptr<Float> r, Ptr<Float> s){

  Int j=0, k=me()<<4, l=0, g=c*16;

  Float xA, yA, zA, pxy, pyz, tA, tB;

    xA = p[j+k];
    yA = q[j+k];
    zA = r[j+k];

    pxy = yA-xA;
    pyz = zA-yA;


//  Int i=0;
  For (Int i = 0, i < n, i = i+g)

    l = index() + k+ i;

    tA = xA + pxy*toFloat(j)*xnv;
    tB = yA + pyz*toFloat(j)*xnv;

    s[l] = tA*(1-toFloat(j)*xnv) + tB*toFloat(j)*xnv;
    j = j+1;
  End
}


void aesmooth(Int nb, Int c, Float xnv, Int prc, Ptr<Float> p, Ptr<Float> q, Ptr<Float> r, Ptr<Float> s){

  Int j=0, k=me()<<4, l=0, g=c*16;

  Float xA, yA, zA, pxy, pyz, tA, tB;

//  Int i=0;

 For (Int i = 0, i < nb, i++)

    xA = p[k + g*i];
    yA = q[k + g*i];
    zA = r[k + g*i];

    pxy = yA-xA;
    pyz = zA-yA;

  For (j = 0, j <= prc, j++) //i = i+g)

    l = index() + k + g*(i*prc+j);

    tA = xA + pxy*toFloat(j)*xnv;
    tB = yA + pyz*toFloat(j)*xnv;

    s[l] = tA*(1-toFloat(j)*xnv) + tB*toFloat(j)*xnv;
    //j = j+1;
  End;
 End;
}



int main()
{

  int cores = 2;

  int blocks = 10;

  int xrun = 16*cores*blocks;

  int prec = 240;

  int count = xrun * prec;


  timeval tvStart, tvEnd, tvDiff;

  gettimeofday(&tvStart, NULL);

  // Construct kernel
  auto k = compile(aesmooth);

  gettimeofday(&tvEnd, NULL);
  timersub(&tvEnd, &tvStart, &tvDiff);

  printf("compile time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);

  gettimeofday(&tvStart, NULL);

  // Allocate and initialise array shared between ARM and GPU
  SharedArray<float> array(count);
  SharedArray<float> arrayb(count);
//  SharedArray<float> arrayc(count);
//  SharedArray<float> arrayd(count);


  SharedArray<float> xdelt(xrun);
  SharedArray<float> ydelt(xrun);
  SharedArray<float> zdelt(xrun);


  gettimeofday(&tvEnd, NULL);
  timersub(&tvEnd, &tvStart, &tvDiff);
  printf("smem alloc time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);


  for (int i = 0; i < count; i++){
    array[i] = i;     arrayb[i] = count-i; }
//    arrayc[i] = 0;}

  for (int i = 0; i < xrun; i++){
    xdelt[i] = i*5;
    ydelt[i] = (i+10)*4+20;
    zdelt[i] = i*3+10;
    printf("xyz: %i [%.0f %.0f %.0f] ",i, xdelt[i], ydelt[i], zdelt[i]);
    }

  // Invoke the kernel and display the result
  k.setNumQPUs(cores);

  gettimeofday(&tvStart, NULL);

  float zz = 1.0/(prec -1); //, zxa[1]={0.0}, zxb[1]={10.0};*cores

//  float zya[1]={20.0}, zyb[1]={5.0};

  printf("xf: %f\n", zz);

  k(blocks, cores, zz, prec, &xdelt, &ydelt, &zdelt, &array);

//  k(16*xrun*cores, cores, zz, &ydelt, &arrayb);


  gettimeofday(&tvEnd, NULL);


  for (int i = 0; i < count; i++) {
    printf("%i: %.1f, ", i, array[i]); //%.1f.  ", i,  array[i], arrayb[i]); //array[i], arrayb[i],
    if(i%4 == 3) printf("\n");
    if(i%16 == 15) printf("\n");
    }
  
  timersub(&tvEnd, &tvStart, &tvDiff);
  printf("run time: %ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);

//  free(&array);
//  free(&arrayb);
//  free(&arrayc);

  k.setNumQPUs(0);



  return 0;
}
