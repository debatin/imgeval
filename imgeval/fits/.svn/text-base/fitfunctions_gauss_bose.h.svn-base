#ifndef FITFUNCTIONS_GAUSS_BOSE_H
#define FITFUNCTIONS_GAUSS_BOSE_H



#include <gsl/gsl_errno.h>
#include <gsl/gsl_multifit_nlin.h>
#include "fitstructs.h"



//! bose enhanced gaussian residual function used for fitting
int bose_gaussian_f(const gsl_vector * x, void *data, gsl_vector * f);
//! corresponding Jacobian matrix
int bose_gaussian_df (const gsl_vector * x, void *data, gsl_matrix * J);
//! function calling both fitting functions
int bose_gaussian_fdf (const gsl_vector * x, void *data,
                       gsl_vector * f, gsl_matrix * J);
//Y=x0*g_5/2(e^((x-x1)^2/x2))+x3
#define BOSE_GAUSSIAN_NR 5  //Nr of elements to approximate the function
//const float g_2_1 = 1+1/4.0+1/9.0+1/16.0+1/25.0;  //removed.  wrong. TT.
const float g_2_1 = 1+1/8.0+1/27.0+1/64.0+1/625.0;  //Riemann zeta(3) approximation used in Nfit (Bose thermal)

inline double bose_gaussian(const double &x,
                        const double &x0,
                        const double &x1,
                        const double &x2,
                        const double &x3)
{
      const double n52[5] = {1,2*2*sqrt(2),3*3*sqrt(3),4*4*sqrt(4),5*5*sqrt(5)};  //n^(5/2);
      double e= exp(-(x-x1)*(x-x1)/x2);
      double ytemp=1;
      double Yi=0;//g_(5/2)(Y1)
      for(int j=0;j<BOSE_GAUSSIAN_NR;j++) {ytemp*=e; Yi+=ytemp/n52[j];}
      Yi*=x0; Yi+=x3;
      return Yi;
}


#endif // FITFUNCTIONS_GAUSS_BOSE_H
