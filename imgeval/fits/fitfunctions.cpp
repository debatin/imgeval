#include "fitfunctions.h"


/*!  computes the residual of a gaussian:
f_i= (gaussian(x0,x1,x2)-y_i)/\sigma_i 
where x0,x1,x2 are Parameters of the gaussian
f=x0 exp(-(i-x1)^2/x2)

The Parameters are choosen in a way to minimize computation time.
so \sigma is sqrt(x2/2)
the center x1 is given in units of pixel from start of the sample
also the numbar of particles has to be normalized
\param x the values of the parameters xi
\param data struct containing the size of the data, 
the y values to be fit and the respective standard deviations
\param f returns the values f_i of the function
\author Markus Debatin
\date 09-2010
*************************************************************************/
int gaussian_f (const gsl_vector * param, void *data, gsl_vector * f)
{

  size_t n = ((struct data * )data)->n;
  double *y = ((struct data *)data)->y;
  double *xdat = ((struct data *)data)->x;
  double *sigma = ((struct data*) data)->sigma;

  double x0 = gsl_vector_get(param,0);
  double x1 = gsl_vector_get(param,1);
  double x2 = gsl_vector_get(param,2);

  size_t i;

  for(i=0; i<n;i++)
    {
     /* Model Yi=x0* exp(-(i-x1)*(i-x1)/x2    */
      double t=xdat[i];
      double Yi= x0 * exp(-(t-x1)*(t-x1)/x2);
      gsl_vector_set(f,i,(Yi-y[i])/sigma[i]);
    }
  return GSL_SUCCESS;
}

/*! computes the Jacobian matrix of the residal of the gaussian
     Jacobian matrix J(i,j) = dfi / dxj, 
     where fi = (Yi - yi)/sigma[i],     
     Yi= x0 * exp(-(i-x1)^2/x2)
     \param x the values of the parameters xi
     \param data struct containing the size of the data, 
     the y values to be fit and the respective standard deviations
     \param J returns the values J_ij of the matrix
\author Markus Debatin
\date 10-2010
*************************************************************************/
int gaussian_df (const gsl_vector * param, void *data, gsl_matrix * J)
{

  size_t n = ((struct data *)data)->n;
  double *sigma = ((struct data *) data)->sigma;
  double *xdat = ((struct data *) data)->x;

  double x0 = gsl_vector_get(param,0);
  double x1 = gsl_vector_get(param,1);
  double x2 = gsl_vector_get(param,2);
     
  size_t i;
  
  for (i = 0; i < n; i++)
    {
      /* Jacobian matrix J(i,j) = dfi / dxj, 
	 where fi = (Yi - yi)/sigma[i],     
	 Yi= x0 * exp(-(i-x1)^2/x2)   */
      double s = sigma[i];
      double t=xdat[i];
      double e = exp(-(t-x1)*(t-x1)/x2);
      gsl_matrix_set (J, i, 0, e/s); 
      gsl_matrix_set (J, i, 1, 2*(t-x1)/x2     *x0*e/s);
      gsl_matrix_set (J, i, 2,(t-x1)*(t-x1)/x2/x2*x0*e/s);
    }
  return GSL_SUCCESS;
}

int gaussian_fdf (const gsl_vector * param, void *data,
               gsl_vector * f, gsl_matrix * J)
{
  gaussian_f (param, data, f);
  gaussian_df (param, data, J);
  
  return GSL_SUCCESS;
}
