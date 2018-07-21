#ifndef FITFUNCTIONS_H
#define FITFUNCTIONS_H
#include <gsl/gsl_errno.h>
#include <gsl/gsl_multifit_nlin.h>
#include "fitstructs.h"
#include "fitfunctions_gauss_bose.h"
#include "fitfunctions_bimodal.h"



//! gaussian residual function used for fitting
int gaussian_f(const gsl_vector * x, void *data, gsl_vector * f);
//! corresponding Jacobian matrix
int gaussian_df (const gsl_vector * x, void *data, gsl_matrix * J);
//! function calling both fitting functions
int gaussian_fdf (const gsl_vector * x, void *data, 
	      gsl_vector * f, gsl_matrix * J);




#endif
