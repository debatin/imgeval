#ifndef IMGDATA_H
#define IMGDATA_H
#include<vector>
#include<QPointF>
#include<iostream>
//!  class for loading and  storing image Data
/*! This class serves for storing and calculation of the images
  It can load only 16bit grayscale tiff (so far)
  Images are treated internally as an RxR matrix (double)

*/
using namespace std;
struct twopoints
{
    int x1; //position
    int y1; ///< position
    int wx; ///<width
    int wy; ///<height
};

class Imagedata
{
public:
    Imagedata();
    Imagedata(int width, int height); ///< creates an image of given size
    Imagedata(const Imagedata & img);
    ~Imagedata();
    //! load the given tiff file
    void load(const char* filename);  ///< loading 16 bit tif Files
    void load_bloc(const char* filename); ///< loads float files
    void load_matlab(const char* filename); ///< load matlab file
    void save(const char* filename);  ///< save 16 bit tif Files
    void save_matlab(const char* filename); ///< save as matlab file
    void save_bloc(const char* filename); ///< save as datablock
    string getArtist(){return artist;};  ///< returns the artist parameter
    inline float* ptr()const{return imgdata;};  ///< pointer to the raw image data
    inline int height(){return imgheight;}  ///<height of the image
    inline int width(){return imgwidth;}  ///< width of the image
    std::vector<float> rowsum();  ///< sums over the rows in the roi
    std::vector<float> colsum();  ///< sums over the cols in the roi
    void rowsmooth(int nsmooth);
    void colsmooth(int nsmooth);
    void xscale(float scale, string interpolation ="none"); ///< scales in x direction
    void yscale(float scale, string interpolation="none");  ///< scales in y direction
    void scale(float scale, string interpolation="none");  ///< scales in bot directions
    float sum();  ///< sums over the roi
    float max();  ///< returns the maximum element in the roi
    float contraction1D(Imagedata const &second)const;  ///< calculates sum x1*x2 x1,x2
    float contractionROI(Imagedata const &second)const;  ///< calculates sum x1*x2 x1,x2
    inline void clear(){imgwidth=0; imgheight =0;  setROI();
        if(imgdata!=NULL) delete[] imgdata; imgdata=NULL;}; ///<deletes the image;

    void setROI(int x=0, int y=0, int x2=-1, int y2=-1);
    void setROI(QPointF p1,QPointF p2){
        setROI((int)p1.x(),(int)p1.y(),(int)p2.x(),(int)p2.y());};

    bool sameSize(Imagedata const& other)const; ///<compares width height, sample
    bool empty(); ///< true when width = height =0;
    void cutimg();   ///<cuts the image to contain the ROI only
    Imagedata& operator=(Imagedata const& other);  ///<copy data
    Imagedata& operator+=(Imagedata const& other); ///<Elementwise
    Imagedata& operator-=(Imagedata const& other); ///<Elementwise
    Imagedata& operator*=(Imagedata const& other); ///<Elementwise
    Imagedata& operator/=(Imagedata const& other); ///<Elementwise
    Imagedata& operator+=(float const other);
    Imagedata& operator-=(float const other);
    Imagedata& operator*=(float const other);
    Imagedata& operator/=(float const other);
    inline float& operator()(int x, int y){ zero =0; if(empty())  return zero;
        if (x<0) return zero; if (y<0) return zero; if(x>=imgwidth) return zero;
        if(y>=imgheight)return zero;
        return imgdata[imgwidth*y+x]; }
    void ln(); ///< takes logarithm
    void rot90(); ///<rotates the image by 90 degrees counter clockwise
    void rot(float deg); ///< rotates the image by deg degrees (360 = full rotation)
    void flip_horizontal(); ///< flips the image left/right
    void makePositive();
protected:
    float zero;
    float* imgdata;  //the image will be stored here
    int imgwidth;   ///<width of the image
    int imgheight; ///<height of the image
    unsigned short bitspersample;  ///< also called depth (16 for or cam)
    unsigned short samplesperpixel;  ///< 1=gray; 3=rgb
    twopoints roi;
    string artist; ///<person who created the image used for distinction from new or old images

};
//! only used during file IO
struct IFD_Entry
{
    short tag;
    short type;
    int numval;
    int valofs;
};
#endif // IMGDATA_H
