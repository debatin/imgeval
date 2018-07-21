#ifndef FFTWFILTER_H
#define FFTWFILTER_H
#include "includes/fftw3.h"
#include "datastructure/imgdata.h"
//! container to manage 2d fft
class fftwfilter
{
public:
    fftwfilter();
    ~fftwfilter();
    void fft_real(Imagedata &img){Imagedata temp=img; temp *=0; fft(img,temp); };    //returns real part of fft in the given image
    void fft_imag(Imagedata &img){Imagedata temp=img; temp *=0; fft(img,temp); img=temp;};    //returns imaginary part of fft in the given image
    void fft(Imagedata &real, Imagedata &complex);    //returns
    void filter(Imagedata &img, Imagedata &fftmask);  //transforms image masks out pixel, transforms back
    void generatePlan( Imagedata &img);
protected:
    fftw_plan p;  //plan to be used with fftw
    int width;  //width of current plan
    int height; //height of current plan
    fftw_complex* in; //buffer for calculation of fft
    fftw_complex* out;
};




#endif // FFTWFILTER_H
