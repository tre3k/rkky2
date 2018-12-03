/*
 * Kirill Pshenichny aka tre3k (c) 11.2018
 * This is part for calculationt the neutron scattering in the samples
 * with RKKY interaction.
 *
 */


#include <cuda.h>

#include "function_cuda.h"

/* *** GPU Functions *** */

__device__ double dE_i,dDeltaH,dA0,dk_s,dk_i;

__device__ double *dMap;                      // variable for dispersion 


struct sFunc{
  double func1,func2;
};

__global__ void calcConst(){
  dDeltaH = dDeltaH*P_g*P_uB;
  dE_i = P_h*P_h*dk_i*dk_i/2/P_mn;
  return;
}

__device__ struct sFunc func(double *lt_x,double *lt_y,double *lo){
  struct sFunc retval;

  double o = *lo;
  double t_x = *lt_x;
  double t_y = *lt_y;

  double o2 = o*o;
  double t_x2 = t_x*t_x;
  double t_y2 = t_y*t_y;

  double A = t_x2*dk_i*dk_i;
  double B = t_y2*dk_i*dk_i;
  double C = P_h*P_h*o2*dk_i*dk_i/4/dE_i/dE_i;
  double D = dk_s*dk_s; 

  retval.func1 = o - dA0*(A+B+C-D)*(A+B+C-D) - dDeltaH;
  retval.func2 = o + dA0*(A+B+C-D)*(A+B+C-D) + dDeltaH;
  
  return retval;
}

__global__ void generateMap(double *from_var, double *to_var, int *N){
  dMap = new double [*N];
  double dvar = (*to_var - *from_var)/ (double)(*N);
  for(int i=0;i<*N;i++) dMap[i] = *from_var + (double)i*dvar;
}

__global__ void deleteMap(){
  delete [] dMap;
}

__global__ void dispersionAtTheta_x(double *out){
  
  return;
}

__global__ void func_test(double *in,double *out){
  int tid = blockIdx.x;
  out[tid] = in[tid]*in[tid];

  return;
}



/* *** CPU Functions *** */

void FunctionCuda::memcpyConst(){

  double *lA0,*lDeltaH,*lk_i,*lk_s;
  lA0 = &dA0;
  lDeltaH = &dDeltaH;
  lk_i = &dk_i;
  lk_s = &dk_s;

  cudaMemcpy(lA0, &A, sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(lDeltaH, &DeltaH, sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(lk_i, &k_i, sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(lk_s, &k_s, sizeof(double), cudaMemcpyHostToDevice);
  
  calcConst<<<1,1>>>();
  
  return;
}

void FunctionCuda::getDispersion(double *data1,double *data2,
				 int flags, double from_var, double to_var, int N){

  memcpyConst();

  
  return;
}

double *FunctionCuda::test(double *in,int N){
  
  double *ret = new double [N];
  double *dev_in, *dev_out;

  cudaMalloc((void **)&dev_in,N*sizeof(double));
  cudaMalloc((void **)&dev_out,N*sizeof(double));

  cudaMemcpy(dev_in,in,N*sizeof(double),cudaMemcpyHostToDevice);

  func_test<<<N,1>>>(dev_in,dev_out);
  
  cudaMemcpy(ret,dev_out,N*sizeof(double),cudaMemcpyDeviceToHost);
  
  /*
  for(int i=0;i<N;i++){
    ret[i] = in[i]*in[i];
  }
  */

  
  return ret;
}

