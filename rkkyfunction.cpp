#include "rkkyfunction.h"

rkkyFunction::rkkyFunction()
{

}

void rkkyFunction::setConstants(double lA0,double lk_i, double lDeltaH){
    this->A0 = lA0;
    this->k_i = lk_i;
    this->DeltaH = lDeltaH;
    this->E_i = P_h*P_h*lk_i*lk_i/2/P_mn;
    return;
}

rkkyFunction::s_functions rkkyFunction::getFunction(double t_x,double t_y,double o){
    s_functions retval;

    double o2 = o*o;
    double t_x2 = t_x*t_x;
    double t_y2 = t_y*t_y;


    return retval;
}
