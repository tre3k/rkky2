#include <cuda.h>

#include "function_cuda.h"

/* *** GPU Functions *** */

__device__ double func1(double *t_x,double *t_y,double *o){
  double retval;

  
  return retval;
}

__global__ void func_test(double *in,double *out){
  int tid = blockIdx.x;
  out[tid] = in[tid]*in[tid];
  return;
}



/* *** CPU Functions *** */

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

