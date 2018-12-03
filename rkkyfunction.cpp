#include "rkkyfunction.h"

rkkyFunction::rkkyFunction()
{

}

void rkkyFunction::setConstants(double lA0,double lk_s, double lDeltaH, double lambda){
    this->A0 = -lA0;
    //lambda *= 1e-10;
    this->k_i = 2.0*M_PI/lambda;
    this->DeltaH = P_g*P_uB*lDeltaH;
    this->E_i = P_h*P_h*this->k_i*this->k_i/2/P_mn/1e-20;

    this->E_i = 1000*this->E_i/P_e; //to meV
    this->DeltaH = 1000*this->DeltaH/P_e;

    qDebug () << "E_i: " << this->E_i << " meV";
    qDebug () << "guH: " << -this->DeltaH << " meV";

    this->k_s = lk_s/10;                                            // in A^-1

    qDebug () << "k_i: " << this->k_i << " A^-1";
    qDebug () << "k_s: " << this->k_s << " A^-1";
    qDebug () << this->A0 << " meVA^-4";

    return;
}

rkkyFunction::s_functions rkkyFunction::getFunction(double t_x,double t_y,double o){
    s_functions retval;

    double o2 = o*o;
    double t2_x = t_x*t_x;
    double t2_y = t_y*t_y;
    double k2_i = k_i*k_i;

    double h = 1000*P_h/P_e;

    double A = k2_i*t2_x;
    double B = k2_i*t2_y;
    double C = h*h*o2/k2_i/4/E_i/E_i;
    double D = k_s*k_s;


    retval.func1 = h*o - A0*(A+B+C-D)*(A+B+C-D) - DeltaH;
    retval.func2 = h*o + A0*(A+B+C-D)*(A+B+C-D) + DeltaH;
    return retval;
}
