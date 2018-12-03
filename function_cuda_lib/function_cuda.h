#ifndef FUNCTION_CUDA_H
#define FUNCTION_CUDA_H

/* flags */
#define F_DISPERSION_AT_THETA_X  0
#define F_DISPERSION_AT_THETA_Y  1
#define F_DISPERSION_AT_OMEGA    2

/* physical constant */
#define P_h            1.054571800e-34     // Joul*s  plank const
#define P_mn           1.674927471e-27     // kg  neutron mass
#define P_uB           927.400968e-26      // Joul/T  Bor magneton
#define P_g            -2.00231930436153   // g-factor
#define P_e            1.60217662e-19      // e Kulon

class FunctionCuda{
 public:
  double A;                 // Stiffness RKKY
  double DeltaH;            // (H-Hc2)
  double k_i;               // spiral wave vector
  double k_s;
  
  double *test(double *in,int N);
  void getRoot1(double *t_x,double *t_y, double *omega);
  void getRoot2(double *t_x,double *t_y, double *omega);

  void getDispersion(double *data1,double *data2,
		     int flags, double from_var, double to_var, int N);
  
 private:
  void memcpyConst(void);
  
};


#endif // FUNCTION_CUDA_H
