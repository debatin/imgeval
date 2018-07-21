#include "fftwfilter.h"
#include "datastructure/imgdata.h"
/*! this code is not used !!
\author Markus Debatin
\date 10-2010
*************************************************************************/
fftwfilter::fftwfilter()
{
    in = NULL;
    out = NULL;
    width =0;
    height =0;
}

fftwfilter::~fftwfilter()
{
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
//    if (in != NULL) delete[] in;
//    if (out != NULL) delete[] out;
}

void fftwfilter::fft(Imagedata &real, Imagedata &imag)
{
    if((real.width()==0) || (real.height()==0)) return;
    if(! real.sameSize(imag)) return;
    generatePlan(real);
 int N=width* height;
    for(int i = 0; i<N;i++){
        in[i][0]=real.ptr()[i];
        in[i][1]=imag.ptr()[i];
    }
    fftw_execute(p);
    for(int i = 0; i<N;i++){
        real.ptr()[i]=out[i][0];
        imag.ptr()[i]=out[i][1];
    }
}

void fftwfilter::generatePlan(Imagedata & img)
{
   if((width == img.width()) && (height == img.height())) return;  // nothing to do in this case
   fftw_destroy_plan(p);
   fftw_free(in); fftw_free(out);
   width=img.width();
   height = img.height();
   int N=width* height;
   in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
   out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
   p=  fftw_plan_dft_2d(width, height, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
}
void fftwfilter::filter(Imagedata &img, Imagedata &fftmask)
{
    Imagedata temp = img;
    temp *=0;
   fft(img,temp);
   temp *=fftmask; img *= fftmask;
   fft(img,temp);
}
