
#include"errpattern.h"
#include <stdio.h>
#include <gsl/gsl_linalg.h>
#include<iostream>
#include"datastructure/imgdata.h"
#include<vector>
#include<assert.h>
/*!  calculates a Matrix which is used later on for error calculations
X^ik=sum_j x^i_j*x^k_j; where x^k is errpatterns[k];
\param errimg is a vector containing the errorpatterns to be fit
\author Markus Debatin
\date 2010
*************************************************************************/
void generate_errpattern_matrix(const vector<Imagedata> &errimg, Imagedata &errmatrix)
{
    cout<<"errimg size: "<<errimg.size()<<"\n";
     int n=errimg.size();  //simple substitution
     if (n==0) return;
      errmatrix = Imagedata(n,n);
      cout<<errmatrix.width()<<"rrmwidth\n";
      assert(errmatrix.width()==n);
      assert(errmatrix.height()==n);
      for(int i =0;i<n;i++)
         for(int j = 0 ;j<n;j++)
             errmatrix(i,j)=(float) errimg[i].contraction1D(errimg[j]);
    return;
 }

 /*!  calculates a Matrix which is used later on for error calculations
 X^ik=sum_j x^i_j*x^k_j; where x^k is errpatterns[k];
 \param errimg is a vector containing the errorpatterns to be fit
 \author Markus Debatin
 \date 2010
 *************************************************************************/
 void fitErrPattern (vector<Imagedata>const  &errimg, Imagedata &img, Imagedata & fitimg)
 {
     int n=errimg.size();
     if(n==0)return;
     Imagedata errmat;
     generate_errpattern_matrix(errimg,errmat);
     assert(errmat.width()==n); assert(errmat.height()==n);
     double *a_data = new double[n*n];
     double *b_data = new double[n];
     float* errmat_ptr = errmat.ptr();
     assert(errmat_ptr != NULL);
     for(int i = 0;i<n*n;i++)
       a_data[i] = errmat_ptr[i];
     cout<<"contractions: ";
     for(int i =0;i<n;i++){
         b_data[i]= img.contraction1D(errimg[i]);
         cout<<b_data[i]<<"; ";
     }
     cout<<"n: "<<n<<";"<<errmat(0,0)<<"\n";
     for(int i =0;i<n;i++){
         for(int j =0;j<n;j++)
            cout<<errmat(j,i)<<"  ";
          cout<<"<\n";
     }
    /* double a_data[] = { 0.18, 0.60, 0.57, 0.96,
                         0.41, 0.24, 0.99, 0.58,
                         0.14, 0.30, 0.97, 0.66,
                         0.51, 0.13, 0.19, 0.85 };


     double b_data[] = { 1.0, 2.0, 3.0, 4.0 };*/
     cout<<"run\n";

     gsl_matrix_view m = gsl_matrix_view_array (a_data, n, n);  //solve Ax=b
     gsl_vector_view b = gsl_vector_view_array (b_data, n);
     gsl_vector *x = gsl_vector_alloc (n);
     int s;
     gsl_permutation * p = gsl_permutation_alloc (n);

     gsl_linalg_LU_decomp (&m.matrix, p, &s);
     gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);
     cout<<"still run\n";
     printf ("x = \n");
     gsl_vector_fprintf (stdout, x, "%g");
     //generate fitted pattern img;
     cout<<"creating fitimg "<<img.width()<<" \n";
     Imagedata tmp;
     fitimg=img; fitimg*=0;
     for(int i = 0;i<n;i++){
         tmp=errimg[i]; tmp*=x->data[i];
         fitimg+=tmp;
     }
     cout<<"subtracting fitimg "<<fitimg.width()<<" x "<<fitimg.height()<<"\n";
     //img-=fitimg;
     gsl_permutation_free (p);
     gsl_vector_free (x);
     return;
     //return 0;
 }
 /*!  calculates a Matrix which is used later on for error calculations
 X^ik=sum_j x^i_j*x^k_j; where x^k is errpatterns[k];

 this time only simple
 \param errimg is a vector containing the errorpatterns to be fit
 \author Markus Debatin
 \date 2010
 *************************************************************************/
 void fitErrPattern2 (vector<Imagedata>const  &errimg, Imagedata &img, Imagedata & fitimg)
 {
     int n=errimg.size();
     if(n==0)return;
     //generate fitted pattern img;
     cout<<"creating fitimg "<<img.width()<<" \n";
     Imagedata tmp;
     fitimg=img; fitimg*=0;
     double factor;
     for(int i = 0;i<n;i++){
         factor=img.contraction1D(errimg[i])/errimg[i].contraction1D(errimg[i]);
         cout<<"factor "<<i<<" : "<<factor<<"\n";
         tmp=errimg[i];
         tmp*=(float)factor;
         fitimg+=tmp;
     }
     cout<<"subtracting fitimg "<<fitimg.width()<<" x "<<fitimg.height()<<"\n";
     //img-=fitimg;
     return;
     //return 0;
 }

 void search_errorpatterns(string )
 {




 }
