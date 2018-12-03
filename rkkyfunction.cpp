#include "rkkyfunction.h"

rkkyFunction::rkkyFunction()
{

}

void rkkyFunction::setConstants(double lA0,double lk_s, double lDeltaH, double lambda){
    this->A0 = -lA0;
    this->k_i = 2.0*M_PI/lambda;
    this->DeltaH = P_g*P_uB*lDeltaH;
    this->E_i = P_h*P_h*this->k_i*this->k_i/2/P_mn/1e-20;

    this->E_i = 1000*this->E_i/P_e; //to meV
    this->DeltaH = 1000*this->DeltaH/P_e;

    qDebug () << "E_i: " << this->E_i << " meV";
    qDebug () << "guH: " << -this->DeltaH << " meV";

    //this->k_s = lk_s/10;                                            // in A^-1
    this->k_s = lk_s;

    qDebug () << "k_i: " << this->k_i << " A^-1";
    qDebug () << "k_s: " << this->k_s << " A^-1";
    qDebug () << this->A0 << " meVA^-4";

    return;
}

void rkkyFunction::calculateMap2D(double from_theta, double to_theta, int N){
    map2D = new double * [N];
    for(int i=0;i<N;i++) map2D[i] = new double [N];
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++) map2D[i][j] = 0.0;
    }

    double dt = (to_theta-from_theta)/(double) N;
    double t_x,t_y,o;
    double *func1 = new double [N];
    double *func2 = new double [N];
    double *omega = new double [N];
    int *index = new int [N];

    double geomFactor;

    int root_counts;

    struct s_functions funcs;

    vRootOmega1.clear();
    vRootOmega2.clear();
    vRootThetaX1.clear();
    vRootThetaY1.clear();
    vRootThetaX2.clear();
    vRootThetaY2.clear();

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            t_x = from_theta + dt*i;
            t_y = from_theta + dt*j;

            for(int k=0;k<N;k++){
                o = from_theta + dt*k;
                omega[k] = o;
                funcs = getFunction(t_x,t_y,omega[k]);
                func1[k] = funcs.func1;
                func2[k] = funcs.func2;
            }
            // find roots
            root_counts = findRoots(func1,index,N);
            for(int k=0;k<root_counts;k++){
                geomFactor = t_x*t_x/(t_x*t_x + t_y*t_y + omega[index[k]]*omega[index[k]]);
                map2D[i][j] += geomFactor/omega[index[k]];
                if(t_y==0){
                    vRootThetaX1.append(t_x);
                    vRootThetaY1.append(t_y);
                    vRootOmega1.append(omega[index[k]]);
                }
            }
            root_counts = findRoots(func2,index,N);
            for(int k=0;k<root_counts;k++){
                geomFactor = t_x*t_x/(t_x*t_x + t_y*t_y + omega[index[k]]*omega[index[k]]);
                map2D[i][j] += geomFactor/omega[index[k]];
                if(t_y==0){
                    vRootThetaX2.append(t_x);
                    vRootThetaY2.append(t_y);
                    vRootOmega2.append(omega[index[k]]);
                }
            }

        }
    }

    /*
    t_x = 0.0; t_y = 0.00;

    for(int k=0;k<N;k++){
        o = from_theta + dt*k;
        omega[k] = o;
        funcs = getFunction(t_x,t_y,o);
        func1[k] = funcs.func1;
        func2[k] = funcs.func2;
    }
    // find roots
    root_counts = findRoots(func1,index,N);
    for(int k=0;k<root_counts;k++){
        qDebug() << t_x << " " << t_y << " " << omega[index[k]];
    }
    qDebug () << root_counts;
    */

    /*
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++) map2D[i][j] = i*i+j*j;
    }
    */
    return;
}

int rkkyFunction::findRoots(double *func,int *index,int N){
    int count_roots = 0;
    /*
    double eps = 0.025;
    //double eps = 0.005;
    for(int i=0;i<N;i++){
        if(func[i] <= eps && func[i] >= -eps){
            index[count_roots] = i;
            count_roots++;
        }
    }
    */

    for(int i=1;i<N;i++){
        if((func[i-1]>=0 && func[i] <= 0)|| (func[i-1]<=0 && func[i] >= 0)){
            index[count_roots] = i;
            count_roots++;
        }
    }
    return count_roots;
}

rkkyFunction::s_functions rkkyFunction::getFunction(double t_x,double t_y,double o){
    s_functions retval;

    double o2 = o*o;
    double t2_x = t_x*t_x;
    double t2_y = t_y*t_y;
    double k2_i = k_i*k_i;

    double A = k2_i*t2_x;
    double B = k2_i*t2_y;
    double C = k2_i*o2;
    double D = k_s*k_s;
    //double D = k_s*k_s*k_i*k_i;


    retval.func1 = 2*E_i*o - A0*(A+B+C-D)*(A+B+C-D) - DeltaH;
    retval.func2 = 2*E_i*o + A0*(A+B+C-D)*(A+B+C-D) + DeltaH;

    return retval;
}
