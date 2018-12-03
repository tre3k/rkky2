#ifndef RKKYFUNCTION_H
#define RKKYFUNCTION_H

#include <math.h>
#include <QDebug>
#include <QVector>

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
    void calculateMap2D(double from_theta, double to_theta, int N);

    double **map2D;
    QVector<double> vRootThetaX1;
    QVector<double> vRootThetaY1;
    QVector<double> vRootOmega1;
    QVector<double> vRootThetaX2;
    QVector<double> vRootThetaY2;
    QVector<double> vRootOmega2;

private:
    double k_i,k_s,A0,DeltaH,E_i;
    int findRoots(double *func,int *index,int N);

};

#endif // RKKYFUNCTION_H
