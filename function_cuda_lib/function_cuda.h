#ifndef FUNCTION_CUDA_H
#define FUNCTION_CUDA_H

class FunctionCuda{
 public:
  double A;                 // Stiffness RKKY
  double DeltaH;            // (H-Hc2)
  double k_i;               // spiral wave vector
  
  double *test(double *in,int N);
  void getRoot1(double *t_x,double *t_y, double *omega);
  void getRoot2(double *t_x,double *t_y, double *omega);
  
 private:
  
};


#endif // FUNCTION_CUDA_H
