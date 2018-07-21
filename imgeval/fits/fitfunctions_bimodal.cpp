#include "fitfunctions_bimodal.h"
/*! computes the residual of a Bimodal distribution:
\sigma_i is the statistical error of datapoint i
f_i= (gaussian(x0,x1,x2)+thomas_fermi()-y_i)/\sigma_i

where x0,x1,x2 are Parameters of the gaussian
f=x0 exp(-(i-x1)^2/x2)+x3

x3,x1,x4,x5 are Parameters of the Thomas Fermi
tf=x4*max(0,(1-((i-x1)^2/x5^2)^2)

The Parameters are choosen in a way to minimize computation time.
so \sigma is sqrt(x2/2)

the center x1 is given in units of pixel from start of the sample
also the number of particles has to be normalized
\param x the values of the parameters xi
\param data struct containing the size of the data,
the y values to be fit and the respective standard deviations
\param f returns the values f_i of the function
\author Markus Debatin
\date 10-2010
*************************************************************************/
int bimodal1d_f (const gsl_vector * param, void *data, gsl_vector * f)
{

  size_t n = ((struct data * )data)->n;
  double *y = ((struct data *)data)->y;
  double *xdat = ((struct data *)data)->x;
  double *sigma = ((struct data*) data)->sigma;

  double x0 = gsl_vector_get(param,0);
  double x1 = gsl_vector_get(param,1);
  double x2 = gsl_vector_get(param,2);
  double x3 = gsl_vector_get(param,3);
  double x4 = gsl_vector_get(param,4);
  double x5 = gsl_vector_get(param,5);

  size_t i;
double Yi;
  for(i=0; i<n;i++)
    {
     /* Model Yi=x0* exp(-(i-x1)*(i-x1)/x2    */
      double t=xdat[i];
      double u2=(t-x1)*(t-x1)/x5/x5;
      if(u2<=1){
         Yi= x0 * exp(-(t-x1)*(t-x1)/x2)+x4*(1-u2)*(1-u2)+x3;}
      else {Yi= x0 * exp(-(t-x1)*(t-x1)/x2)+x3;}
      gsl_vector_set(f,i,(Yi-y[i])/sigma[i]);
    }
  return GSL_SUCCESS;
}

/*! computes the Jacobian matrix of the residal of the gaussian
     Jacobian matrix J(i,j) = dfi / dxj,
     where fi = (Yi - yi)/sigma[i],
     Yi= bimodal function
     \param x the values of the parameters xi
     \param data struct containing the size of the data,
     the y values to be fit and the respective standard deviations
     \param J returns the values J_ij of the matrix
\author Markus Debatin
\date 10-2010
*************************************************************************/
int bimodal1d_df (const gsl_vector * param, void *data, gsl_matrix * J)
{

  size_t n = ((struct data *)data)->n;
  double *sigma = ((struct data *) data)->sigma;
  double *xdat = ((struct data *)data)->x;

  double x0 = gsl_vector_get(param,0);
  double x1 = gsl_vector_get(param,1);
  double x2 = gsl_vector_get(param,2);
  double x3 = gsl_vector_get(param,3);
  double x4 = gsl_vector_get(param,4);
  double x5 = gsl_vector_get(param,5);

  size_t i;

  for (i = 0; i < n; i++)
    {
      /* Jacobian matrix J(i,j) = dfi / dxj,
         where fi = (Yi - yi)/sigma[i],
         Yi= x0 * exp(-(i-x1)^2/x2)   */
      double s = sigma[i];
      double t=xdat[i];
      double e = exp(-(t-x1)*(t-x1)/x2);

      double u=(t-x1)/x5;
      double u2= u*u;
      if(u2<=1){

        gsl_matrix_set (J, i, 0, e/s);
        gsl_matrix_set (J, i, 1, 2*(t-x1)/x2*x0*e/s+4*x4*u/x5*(1-u2)/s);
        gsl_matrix_set (J, i, 2,(t-x1)*(t-x1)/x2/x2*x0*e/s);
        gsl_matrix_set (J, i, 3,1/s);
        gsl_matrix_set (J, i, 4,(1-u2)*(1-u2)/s);
        gsl_matrix_set (J, i, 5,+4*x4*(1-u2)*u*u/x5/s);
    }
          else{
        gsl_matrix_set (J, i, 0, e/s);
        gsl_matrix_set (J, i, 1, 2*(t-x1)/x2*x0*e/s);
        gsl_matrix_set (J, i, 2,(t-x1)*(t-x1)/x2/x2*x0*e/s);
        gsl_matrix_set (J, i, 3,1/s);
        gsl_matrix_set (J, i, 4,0);
        gsl_matrix_set (J, i, 5,0);
      }
  }
  return GSL_SUCCESS;
}

int bimodal1d_fdf (const gsl_vector * x, void *data,
               gsl_vector * f, gsl_matrix * J)
{
  bimodal1d_f (x, data, f);
  bimodal1d_df (x, data, J);

  return GSL_SUCCESS;
}


/*! computes the residual of a Thomas Fermi distribution:
\sigma_i is the statistical error of datapoint i

x0,x1,x2 are Parameters of the Thomas Fermi function
tf=x2*max(0,(1-((i-x1)^2/x0^2)^(3/2))
the center x1 is given in units of pixel from start of the sample
also the number of particles has to be normalized
\param x the values of the parameters xi
\param data struct containing the size of the data,
the y values to be fit and the respective standard deviations
\param f returns the values f_i of the function
\author Markus Debatin
\date 10-2010
*************************************************************************/
int thomas_fermi1d_f (const gsl_vector * param, void *data, gsl_vector * f)
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
      gsl_vector_set(f,i,(thomas_fermi1d(xdat[i],x0,x1,x2)-y[i])/sigma[i]);
   return GSL_SUCCESS;
}

/*! computes the Jacobian matrix of the residal of the gaussian
     Jacobian matrix J(i,j) = dfi / dxj,
     where fi = (Yi - yi)/sigma[i],
     Yi= bimodal function
     \param x the values of the parameters xi
     \param data struct containing the size of the data,
     the y values to be fit and the respective standard deviations
     \param J returns the values J_ij of the matrix
\author Markus Debatin
\date 10-2010
*************************************************************************/
int thomas_fermi1d_df (const gsl_vector * param, void *data, gsl_matrix * J)
{

  size_t n = ((struct data *)data)->n;
  double *sigma = ((struct data *) data)->sigma;
  double *xdat = ((struct data *)data)->x;

  double x0 = gsl_vector_get(param,0);
  double x1 = gsl_vector_get(param,1);
  double x2 = gsl_vector_get(param,2);
  size_t i;

  for (i = 0; i < n; i++)
    {
      /* Jacobian matrix J(i,j) = dfi / dxj,
         where fi = (Yi - yi)/sigma[i],
         Yi=    */
      double s = sigma[i];
      double t=xdat[i];

      double u=(t-x1)/x0;
      double u2= u*u;
      if(u2<=1){
        double s1u2=(1-u2);
        gsl_matrix_set (J, i, 0,4*x2*s1u2*u2/x0/s);
        gsl_matrix_set (J, i, 1, 4*x2*u/x0*s1u2/s);
        gsl_matrix_set (J, i, 2,(1-u2)*s1u2/s);
    }
          else{
        gsl_matrix_set (J, i, 0, 0);
        gsl_matrix_set (J, i, 1, 0);
        gsl_matrix_set (J, i, 2, 0);
      }
  }
  return GSL_SUCCESS;
}

int thomas_fermi1d_fdf (const gsl_vector * param, void *data,
               gsl_vector * f, gsl_matrix * J)
{
  thomas_fermi1d_f (param, data, f);
  thomas_fermi1d_df (param, data, J);

  return GSL_SUCCESS;
}
