#ifndef RKKYFUNCTION_H
#define RKKYFUNCTION_H

#include "function_cuda.h"

class rkkyFunction
{
public:
    rkkyFunction();
    void setConstants(double lA0,double lk_i, double lDeltaH);

    struct s_functions{
        double func1;
        double func2;
    };

    s_functions getFunction(double t_x,double t_y,double o);

    double **map2D;

private:
    double k_i,A0,DeltaH,E_i;

};

#endif // RKKYFUNCTION_H
