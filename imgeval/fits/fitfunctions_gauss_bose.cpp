#include "fitfunctions_gauss_bose.h"




/*! computes the residual of a bose enhanced gaussian:

f_i= (g5/2(gaussian(x0,x1,x2,x))-y_i)/\sigma_i
where x0,x1,x2 are Parameters of the gaussian
f=x0 exp(-(i-x1)^2/x2)

The Parameters are choosen in a way to minimize computation time.
\param x the values of the parameters xi
\param data struct containing the data to be fitted (see header file)
\param f returns the values f_i of the function
\author Markus Debatin
\date 10-2010
*************************************************************************/
int bose_gaussian_f (const gsl_vector * param, void *data, gsl_vector * f)
{
  size_t n = ((struct data * )data)->n;
  double *xdat = ((struct data *)data)->x;
  double *y = ((struct data *)data)->y;
  double *sigma = ((struct data*) data)->sigma;
  double x0 = gsl_vector_get(param,0);
  double x1 = gsl_vector_get(param,1);
  double x2 = gsl_vector_get(param,2);
  double x3 = gsl_vector_get(param,3);
  size_t i;

  for(i=0; i<n;i++)
    {
      double x=xdat[i];
      gsl_vector_set(f,i,(bose_gaussian(x,x0,x1,x2,x3)-y[i])/sigma[i]);
    }
  return GSL_SUCCESS;
}

/*! computes the Jacobian matrix of the residal of the function
     Jacobian matrix J(i,j) = dfi / dxj,
     where fi = (Yi - yi)/sigma[i],
     Yi=g5/2(f)
     f=x0 exp(-(i-x1)^2/x2)
     \param x the values of the parameters xi
     \param data struct containing data,
     \param J returns the values J_ij of the matrix
\author Markus Debatin
\date 10-2010
*************************************************************************/
int bose_gaussian_df (const gsl_vector * param, void *data, gsl_matrix * J)
{

  size_t  n     = ((struct data*) data)->n;
  double *sigma = ((struct data*) data)->sigma;
  double *xdat     = ((struct data*) data)->x;

  double x0 = gsl_vector_get(param,0);
  double x1 = gsl_vector_get(param,1);
  double x2 = gsl_vector_get(param,2);
  double x3 = gsl_vector_get(param,3);
  size_t i;
  const double n52[5] = {1,2*2*sqrt(2),3*3*sqrt(3),4*4*sqrt(4),5*5*sqrt(5)};  //n^(5/2);
  const double n32[5] = {1,2*sqrt(2),3*sqrt(3),4*sqrt(4),5*sqrt(5)};  //n^(3/2);

  for (i = 0; i < n; i++)
    {
      double s = sigma[i];
      double x = xdat[i];
      double e = exp(-(x-x1)*(x-x1)/x2);
      double ytemp=1;
      double Yi=0;//g_(5/2)(Y1)
      double Y32=0;
      for(int j=0;j<BOSE_GAUSSIAN_NR;j++) {ytemp*=e; Yi+=ytemp/n52[j]; }
      ytemp=1;
      for(int j=0;j<BOSE_GAUSSIAN_NR;j++) {ytemp*=e; Y32+=ytemp/n32[j]; }  //e*g_(3/2)(e)

      gsl_matrix_set (J, i, 0, Yi/s);
      gsl_matrix_set (J, i, 1, 2*(x-x1)/x2       *x0*Y32/s);
      gsl_matrix_set (J, i, 2,(x-x1)*(x-x1)/x2/x2*x0*Y32/s);
      gsl_matrix_set (J, i, 3,1/s);
    }
  return GSL_SUCCESS;
}


/*! wrapper function to call both functions

\author Markus Debatin
\date 10-2010
*************************************************************************/
int bose_gaussian_fdf (const gsl_vector * param, void *data,
               gsl_vector * f, gsl_matrix * J)
{
  bose_gaussian_f (param, data, f);
  bose_gaussian_df (param, data, J);

  return GSL_SUCCESS;
}
