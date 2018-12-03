#ifndef RKKYFUNCTION_H
#define RKKYFUNCTION_H

#include <math.h>
#include <QDebug>

#include "function_cuda.h"

class rkkyFunction
{
public:
    rkkyFunction();
    void setConstants(double lA0,double lk_s, double lDeltaH,double lambda);

    struct s_functions{
        double func1;
        double func2;
    };

    s_functions getFunction(double t_x,double t_y,double o);

    double **map2D;

private:
    double k_i,k_s,A0,DeltaH,E_i;

};

#endif // RKKYFUNCTION_H
