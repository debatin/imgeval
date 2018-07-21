#ifndef FITFUNCTIONS_BIMODAL_H
#define FITFUNCTIONS_BIMODAL_H

#include <gsl/gsl_errno.h>
#include <gsl/gsl_multifit_nlin.h>
#include "fitstructs.h"


//! bimodal residual function used for fitting
int bimodal1d_f(const gsl_vector * x, void *data, gsl_vector * f);
//! corresponding Jacobian matrix
int bimodal1d_df (const gsl_vector * x, void *data, gsl_matrix * J);
//! function calling both fitting functions
int bimodal1d_fdf (const gsl_vector * x, void *data,
              gsl_vector * f, gsl_matrix * J);

//! thomas fermi residual function used for fitting
int thomas_fermi1d_f(const gsl_vector * x, void *data, gsl_vector * f);
//! corresponding Jacobian matrix
int thomas_fermi1d_df (const gsl_vector * x, void *data, gsl_matrix * J);
//! function calling both fitting functions
int thomas_fermi1d_fdf (const gsl_vector * x, void *data,
              gsl_vector * f, gsl_matrix * J);


inline double thomas_fermi1d(const double &x,
                             const double &x0,
                             const double &x1,
                             const double &x2)
{
      double u2=(x-x1)*(x-x1)/x0/x0;
      if(u2<=1) return x2*(1-u2)*(1-u2);
      return 0;
}

#endif // FITFUNCTIONS_BIMODAL_H
