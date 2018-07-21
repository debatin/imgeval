#ifndef PERFORM_FITS_H
#define PERFORM_FITS_H
#include<vector>
#include<iostream>
using namespace std;
//! computes the fits etc.

//wrapper function to use with y-values only
int perform_fits (const vector<float> data ,
                  vector<float> &params,string fitname="gaussian");
//wrapper function for the function below to use it with vectors
int perform_fits (const vector<float> xdata ,const vector<float> ydata ,
                  vector<float> &params, string type="gaussian");
//performs various fits on the data given
int perform_fits ( double* x,double* y , const int ny,
		  double* params,const  int nparams,
                  double* fitteddata,string type="gaussian");
#endif
