#include "imgdata.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<math.h>
#include"constants.h"
#include "numeric_templates.h"
#include "assert.h"

#define USEMATLAB  //comment out this line if you don't have matlab libraries

#ifdef USEMATLAB
#include "mat.h"
#endif
using namespace std;

/*! constructor of an empty image
\author Markus Debatin
\date 10-2010
*************************************************************************/
Imagedata::Imagedata()
{
    imgdata=NULL;  //safe default values
    imgwidth=0;
    imgheight=0;
    bitspersample=8;
    samplesperpixel=1;
    setROI();
    //roi coordinates are always
}

/*! constructor of an image of given size
\author Markus Debatin
\date 10-2010
*************************************************************************/
Imagedata::Imagedata(int width, int height)
{
    if(width <1) return; if(height<1)return;
    Imagedata();
    samplesperpixel=1;
    imgdata= new(nothrow) float[width*height];
    if(imgdata == NULL){
        cout<<"imgdata allocation error\n";
        imgwidth =0; imgheight =0;
        return;
    }
    imgwidth=width;
    imgheight=height;
    for(int i =0;i<width*height;i++) imgdata[i]=0;  //initialize
    setROI();
    //roi coordinates are always
}
/*! constructor of an image of given size
\author Markus Debatin
\date 10-2010
*************************************************************************/
Imagedata::Imagedata(const Imagedata & img)
{
    Imagedata();
    this->operator =(img);
    //roi coordinates are always
}
/*! destructor frees memory
\author Markus Debatin
\date 10-2010
*************************************************************************/
Imagedata::~Imagedata()
{
    LOG
            if(imgdata != NULL) delete[] imgdata;  //free memory on destruction
}


/*! load block file
see write block file for format
\param filename  Tiff file to be read
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::load_bloc(const char* filename)
{
    char ch;
    std::ifstream imgfile;
    imgfile.open(filename,std::ios::binary);
    imgfile>>ch;  //reading header information
    imgfile>>ch;  //ignoring the information
    imgfile>>ch;
    imgfile>>ch;
    int oldimgwidth=imgwidth;
    int oldimgheight = imgheight;
    imgfile.read((char*) &imgwidth,sizeof(imgwidth));   //read width
    imgfile.read((char*) &imgheight,sizeof(imgheight)); //read height
    bool dimensionchange = !((imgwidth == oldimgwidth) && (imgheight= oldimgheight) && (samplesperpixel ==1));
    if ((dimensionchange)&&(imgdata != NULL)){
        delete[] imgdata;  //free memory from last image
        imgdata = new(nothrow) float[imgwidth*imgheight*samplesperpixel];  // allocate memory
    }
    samplesperpixel=1;                                  // only monochrome images
    if(imgdata != NULL) imgfile.read((char*) imgdata,imgwidth*imgheight*sizeof(float));  //read the imagedata
    else{ imgheight =0; imgwidth =0; cout<<"memallocation failed imgdata\n";}  // in case memory allocation failed
    imgfile.close();
}


/*! load TIFF file
  can load 16bit grayscale tiffs. The rest of the standard is not fully implemented
\param filename  Tiff file to be read
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::load(const char* filename)
{
    string fname = filename;
    if(fname.substr(fname.size()-4)==".mat")
    {
        load_matlab(filename);   //load matlab file
        return;
    }

    artist = "";  //default value for tag
    std::ifstream imgfile;
    imgfile.open(filename,std::ios::binary);
    std::ostringstream oss;
    char ch;
    int intbuffer;
    char* imgbuffer;
    //for reading tiff files  see also Tiff6.pdf

    int ifd1;   //here the ifd is located (for tiff files) see filespecs
    short Number_Entries;   //Number of Entries in the ifd
    unsigned short compression;  //Number indexing the compression in the file
    unsigned int rowsperstrip;   // for tiff files
    unsigned int stripoffset;
    std::vector<int> stripoffsets;
    std::vector<int> stripbytecounts;

    //********************* Parsing the "HEADER" Information *******************************
    imgfile>>ch;
    imgfile>>ch;
    imgfile>>ch;
    imgfile>>ch;
    imgfile.read((char*) &ifd1,4);

    //******************************* READING the IFD *********************************************
    imgfile.seekg(ifd1);
    imgfile.read((char*) &Number_Entries,2);
    IFD_Entry ife;
    std::vector<IFD_Entry> ifd_entries;

    for(int i = 0; i< Number_Entries;i++) //reading the ifd entries
    {
        imgfile.read((char*) &ife,12);
        ifd_entries.push_back(ife);
    }
    //********************************* Evaluating ifd Entries ******************************************
    for(int i = 0; i< Number_Entries;i++) //reading the ifd entries
    {
        ife=ifd_entries[i];
        switch(ife.tag)
        {
        case 256:  //img width
            imgwidth=(int) ife.valofs;
            break;
        case 257:  //img height
            imgheight=(int) ife.valofs;
            break;
        case 258:  //bits per sample (16 in this case)
            bitspersample=ife.valofs;
            break;
        case 259:  //Compression
            compression=(unsigned short) ife.valofs;
            break;
        case 262: //Color
            break;
        case 273:  //reading in stripoffset values
            // stripoffset=ife.valofs;
            if(ife.numval==1) {stripoffsets.push_back(ife.valofs);}
            else{  //for larger files there are mostly many strips
                imgfile.seekg(ife.valofs);
                for(int j= 0;j< ife.numval;j++)
                {
                    imgfile.read((char*) &stripoffset,4);
                    stripoffsets.push_back(stripoffset);
                }
            }
            break;
             case 274: //orientation
                 break;
             case 277:  //Samples per pixel 1= grayscale 3=RGB
                 samplesperpixel=ife.valofs;
                 break;
             case 278:   //rows per strip
                 rowsperstrip=ife.valofs;
                 break;
             case 279:
                 if(ife.numval==1) {stripbytecounts.push_back(ife.valofs);}
                 else{  //for larger files there are mostly many strips
                     imgfile.seekg(ife.valofs);
                     for(int j= 0;j< ife.numval;j++)
                     {
                         imgfile.read((char*) &intbuffer,4);
                         stripbytecounts.push_back(intbuffer);
                     }
                 }
                 break;
             case 315:
                 if(ife.numval==4) {
                     artist = (char*)&ife.valofs;
                 }
                 else
                     break;
             default:
                 break;
                 //unknown tag
             }
    }
    //******************** reading the Image Data using PackBits ********************
    if (imgfile.bad()) throw 1;
    if(imgwidth>4000) throw 3;
    if(imgheight>4000) throw 3;
    if(samplesperpixel>10) throw 3;
    if(imgwidth>4000) throw 3;
    if (imgdata != NULL) delete[] imgdata;
    imgdata = new float[imgwidth*imgheight*samplesperpixel];
    imgbuffer = new char[imgwidth*imgheight*bitspersample/8*samplesperpixel];
    char* imgbufferpointer=imgbuffer;

    for(int i=0; i< (int)stripoffsets.size();i++)  //parsing through the strips
    {
        if(i+1==(int)stripoffsets.size()) rowsperstrip=imgheight-rowsperstrip*i;
        //number of rows different for last strip
        imgfile.seekg(stripoffsets[i]);    //go to start of strip;
        int bytecount=0;
        int bytesperstrip= imgwidth*rowsperstrip*bitspersample/8*samplesperpixel;
        char c;
        if(compression ==32773 ){
            while (bytecount<bytesperstrip){
                imgfile.read(&c,1);
                int n=c;
                if(c>=0) {imgfile.read(imgbufferpointer,n+1);
                    imgbufferpointer+=n+1;
                    bytecount+=n+1;
                }
                else if(c>-128){
                    imgfile.read((char*)&c,1);
                    for(int j=0;j<1-n;j++){
                        *imgbufferpointer++ = c;
                        bytecount++;
                    }
                }
            }
        }
        else if(compression ==1){    //no compression
            for(int j =0;j<rowsperstrip;j++){ //read rows separately
                imgfile.read(imgbufferpointer,imgwidth*bitspersample/8*samplesperpixel);
                imgbufferpointer += imgwidth*bitspersample/8*samplesperpixel;
                if((imgwidth*bitspersample/8*samplesperpixel)%2==1)imgfile>>ch;  //if number of bytes per row odd
            }
        }
        else throw(7);
    }
    imgfile.close();
    //transfer data to data section
    float* imgend = &imgdata[imgwidth*imgheight*samplesperpixel];
    unsigned short* bufiter= (unsigned short*) imgbuffer;
    for(float* imgiter = imgdata;imgiter<imgend;imgiter++)
        *imgiter= *bufiter++;
    delete[] imgbuffer; imgbuffer=NULL;
    setROI();
}


/*! load the image from matlab_file
\attention needs Matlab libraries to be installed
\param filename file the image is saved to
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::load_matlab(const char* filename)
{
    samplesperpixel = 1;
    imgwidth =0; imgheight =0;
    if(imgdata != NULL) delete[] imgdata;
#ifdef USEMATLAB
    MATFile *pmat;
    //   mxArray  *pa2;
    //   int status;
    pmat = matOpen(filename, "r");  //open mat-file for reading
    if (pmat == NULL) {
        cout<<"file could not be opened .mat\m";
        return;
    }                              //on error return
    const char* cvarname = new char[1000];
    string varname=filename;
    varname=varname.substr(varname.size()-10,6);
    //    mxArray * matimg = matGetVariable(pmat, ("m"+varname).c_str());
    mxArray * matimg = matGetNextVariable(pmat, &cvarname);
    cout<<"Imagedata: varname of img: "<<cvarname<<endl;

    int* dimensions =(int*)  mxGetDimensions(matimg);
    imgwidth=dimensions[0];
    imgheight=dimensions[1];
    // cout<<"mat read in width: "<<dimensions[0]<<"\n";
    // cout<<"mat read in height: "<<dimensions[1]<<"\n";
    //  pa2 = mxCreateNumericMatrix(imgwidth,imgheight,mxSINGLE_CLASS,mxREAL);
    imgdata = new float[imgwidth*imgheight];
    memcpy((void *)imgdata,((void *)mxGetPr(matimg)),  imgwidth*imgheight*sizeof(float));


    //status = matPutVariable(pmat,("m"+varname).c_str() , pa2);
    //    status = matPutVariable(pmat,"test" , pa2);
    mxDestroyArray(matimg);

    if (matClose(pmat) != 0) {
        //  printf("Error closing file %s\n",file);

    }
    return;
#endif
}

/*! saves the image as uncompressed TIFF file
\param filename file the image is saved to
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::save(const char* filename)
{
    std::ofstream file;                         //file to be written
    file.open(filename,std::ios::binary | std::ios_base::trunc);
    int intbuffer;
    short Number_Entries;                       //Number of Entries in the ifd

    //********************* Writing the "HEADER" Information *******************************
    file<<'I'<<'I';
    file<<(char)42<<(char)0;
    intbuffer =imgwidth*imgheight*2*samplesperpixel+24;
    file.write((char*) &intbuffer,4);                    //writing position of the ifd
    intbuffer =100;  file.write((char*) &intbuffer,4); //X-Resolution
    intbuffer =1;    file.write((char*) &intbuffer,4); //X-Resolution
    intbuffer =100;  file.write((char*) &intbuffer,4); //Y-Resolution
    intbuffer =1;    file.write((char*) &intbuffer,4); //Y-Resolution

    //****************************  WritingData ***************************************
    assert(imgwidth <2000); assert(imgheight<2000);
    char* imgbuffer = new char[imgwidth*imgheight*2*samplesperpixel];
    unsigned short* bufiter= (unsigned short*) imgbuffer;
    float* imgend = &imgdata[imgwidth*imgheight*samplesperpixel];
    for(float* imgiter = imgdata;imgiter<imgend;imgiter++){
        *bufiter++ = *imgiter;
        assert(bufiter<=(unsigned short*) &imgbuffer[imgwidth*imgheight*2*samplesperpixel]);
    }
    file.write(imgbuffer,imgwidth*imgheight*2*samplesperpixel);
    delete[] imgbuffer;
    //******************************* Writing the IFD *********************************************
#define IFD_RATIONAL ife.type=5;   //shortcuts for use below
#define IFD_LONG ife.type=4;
#define IFD_SHORT ife.type=3;
#define IFD_ASCII ife.type=2;
#define W_IFE  file.write((char*) &ife,12);
#define MAD_ASCII char* cc=((char*)&ife.valofs); for(int i =0;i<4;i++) cc[i]="MaD"[i];
    Number_Entries=11; file.write((char*) &Number_Entries,2);
    IFD_Entry ife;
    ife.tag=256; IFD_LONG;     ife.numval=1; ife.valofs=imgwidth;
    W_IFE //width
            ife.tag=257; IFD_LONG;     ife.numval=1; ife.valofs=imgheight;
    W_IFE  //height
            ife.tag=258; IFD_SHORT;    ife.numval=1; ife.valofs=16;
    W_IFE//bits per sample
            ife.tag=259; IFD_SHORT;    ife.numval=1; ife.valofs=1;
    W_IFE//no compression
            ife.tag=273; IFD_LONG;     ife.numval=1; ife.valofs=24;
    W_IFE//Offset of the data
            ife.tag=278; IFD_LONG;     ife.numval=1; ife.valofs=imgheight;
    W_IFE //rows per strip
            ife.tag=279; IFD_LONG;     ife.numval=1; ife.valofs=imgheight*imgwidth*2;
    W_IFE  //strip byte count
            ife.tag=282; IFD_RATIONAL; ife.numval=1; ife.valofs=8;
    W_IFE//X-Resolution
            ife.tag=283; IFD_RATIONAL; ife.numval=1; ife.valofs=16;
    W_IFE //Y-Resolution
            ife.tag=296; IFD_SHORT;    ife.numval=1; ife.valofs=3;
    W_IFE //Resolution Unit cm
            ife.tag=315; IFD_SHORT;    ife.numval=4;
    MAD_ASCII           W_IFE //ARTIST
            int zero=0; file.write((char*) &zero,4);
    file.close();
}


/*! saves the image as matlab_file
\attention needs Matlab libraries to be installed
\param filename file the image is saved to
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::save_matlab(const char* filename)
{
#ifdef USEMATLAB
    MATFile *pmat;
    mxArray  *pa2;
    int status;
    pmat = matOpen(filename, "w");
    if (pmat == NULL) {
        return;
    }

    string varname=filename;
    varname=varname.substr(varname.size()-10,6);
    cout<<"Imagedata: "<<varname<<endl;
    pa2 = mxCreateNumericMatrix(imgwidth,imgheight,mxSINGLE_CLASS,mxREAL);
    memcpy((void *)(mxGetPr(pa2)), (void *)imgdata, imgwidth*imgheight*4);
    status = matPutVariable(pmat,("m"+varname).c_str() , pa2);
    //    status = matPutVariable(pmat,"test" , pa2);
    mxDestroyArray(pa2);
    if (matClose(pmat) != 0) {
        //  printf("Error closing file %s\n",file);

    }
    return;
#endif
}

/*! saves the image as bloc file
a block file consists of four letters "bloc" followed
by two integers giving the dimensions in x and y direction.
The rest of the file contains x*y float values
\param filename file the image is saved to
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::save_bloc(const char* filename)
{
    std::ofstream file;                         //file to be written
    file.open(filename,std::ios::binary | std::ios_base::trunc);
    file<<'b'<<'l'<<'o'<<'c';                                     // Writing the "HEADER"
    file.write((char*) &imgwidth,sizeof(imgwidth));               //writing width of image
    file.write((char*) &imgheight,sizeof(imgheight));             //writing height of image
    file.write((char*) imgdata,sizeof(float)*imgwidth*imgheight); //writing image data
    file.close();
}

/*! sums over the rows returning a column vector
Only data in the  ROI (region of interest) is used
\author Markus Debatin
\date 10-2010
*************************************************************************/
std::vector<float> Imagedata::rowsum()
{
    std::vector<float> sum(roi.wy,0);
    float* buffer_iterator=  &imgdata[imgwidth*roi.y1];
    for(int i = 0;i<roi.wy;i++)
    {
        for(int j =0;j<roi.x1;j++)
            buffer_iterator++;
        for(int j =0;j<roi.wx;j++)
            sum[i]+= *buffer_iterator++;
        for(int j =roi.wx+roi.x1;j<imgwidth;j++)
            buffer_iterator++;
    }
    return sum;
}

/*! sums over the cols returning a row vector
Only data in the ROI is used
\author Markus Debatin
\date 10-2010
*************************************************************************/

std::vector<float> Imagedata::colsum()
{
    std::vector<float>sum(roi.wx,0);
    cout<<"colsum"<<roi.wx<<";"<<roi.x1<<";"<<roi.wy<<";"<<roi.y1<<"\n";
    float* buffer_iterator=  &imgdata[imgwidth*roi.y1];
    for(int i =0;i<roi.wy;i++)
    {
        for(int j =0;j<roi.x1;j++)
            buffer_iterator++;
        for(int j =0;j<roi.wx;j++)
            sum[j]+= *buffer_iterator++;
        for(int j =roi.wx+roi.x1;j<imgwidth;j++)
            buffer_iterator++;
    }
    return sum;
}

/*! sum over all the values in the ROI
\author Markus Debatin
\date 10-2010
*************************************************************************/
float Imagedata::sum()
{
    float sum;
    vector<float> csum = colsum(); //colsum uses ROI this is where it comes from
    for(int i = 0;i<csum.size();i++) sum+=csum[i];
    return sum;
}

/*! smoothens the image in one direction
x is replaced by the average ove nsmooth pixels
\param nsmooth number of pixels to be averaged
\author Markus Debatin
\date 2010
*************************************************************************/
void Imagedata::rowsmooth(int nsmooth)
{
    float* tmp = new float[imgwidth*imgheight*samplesperpixel];
    int spp=samplesperpixel;
    //float tmp[imgwidth*imgheight*spp];
    for(int y=0;y<imgheight;y++)
        for(int x=0; x<imgwidth;x++){
        for(int s=0;s<spp;s++)
            tmp[y*imgwidth*spp+x*spp+s]=0;
        for(int i = -nsmooth/2;i<nsmooth/2;i++)
            for(int s=0;s<spp;s++){
            tmp[y*imgwidth*spp+x*spp+s]+=imgdata[y*imgwidth*spp+((x+i+imgwidth)%imgwidth)*spp+s];
        }
    }
    delete[] imgdata;
    imgdata=tmp;
}

/*! smoothens the image in one direction
x is replaced by the average ove nsmooth pixels
\param nsmooth number of pixels to be averaged
\author Markus Debatin
\date 2010
*************************************************************************/
void Imagedata::colsmooth(int nsmooth)
{
    float* tmp = new float[imgwidth*imgheight*samplesperpixel];
    int spp=samplesperpixel;
    for(int y=0;y<imgheight;y++)
        for(int x=0; x<imgwidth;x++){
        for(int s=0;s<spp;s++)
            tmp[y*imgwidth*spp+x*spp+s]=0;
        for(int i = -nsmooth/2;i<nsmooth/2;i++)
            for(int s=0;s<spp;s++){
            tmp[y*imgwidth*spp+x*spp+s]+=imgdata[((y+i+imgheight)%imgheight)*imgwidth*spp+x*spp+s];
        }
    }
    delete[] imgdata;
    imgdata=tmp;
}

/*! scales the image in one direction
the procedure reduces/increases the amount of data
it's not lossless and hence irreversible
\param scale factor  smaller than 1 shrinks the image
\param interpolation so far not used
\author Markus Debatin
\date 12-10-2010
*************************************************************************/
void Imagedata::xscale(float scale, string interpolation)
{
    int newwidth = imgwidth*scale;
    //cout<<"scalingx"<<newwidth<<"; "<<imgwidth<<": "<<(newwidth-1)/scale
    //       <<";"<<(int)(newwidth-1)/scale<<"\n";
    float* tmp = new float[newwidth*imgheight*samplesperpixel];
    int spp=samplesperpixel;
    for(int y=0;y<imgheight;y++)
        for(int x=0; x<newwidth;x++)
            for(int s=0;s<spp;s++)
                tmp[y*newwidth*spp+x*spp+s]=imgdata[y*imgwidth*spp+((int)(x/scale))*spp+s];
    delete[] imgdata;
    imgdata=tmp;
    imgwidth=newwidth;
}

/*! scales the image in one direction
the procedure reduces/increases the amount of data
it's not lossless and hence irreversible
\param scale factor  smaller than 1 shrinks the image
\param interpolation so far not used
\author Markus Debatin
\date 12-10-2010
*************************************************************************/
void Imagedata::yscale(float scale, string interpolation)
{
    int newheight = imgheight*scale;
    // cout<<"scalingy"<<newheight<<"; "<<imgheight<<"\n";
    float* tmp = new float[imgwidth*newheight*samplesperpixel];
    int spp=samplesperpixel;
    for(int y=0;y<newheight;y++)
        for(int x=0; x<imgwidth;x++)
            for(int s=0;s<spp;s++)
                tmp[y*imgwidth*spp+x*spp+s]=imgdata[((int)(y/scale))*imgwidth*spp+x*spp+s];
    delete[] imgdata;
    imgdata=tmp;
    imgheight=newheight;
}

/*! scales the image in both directions by calling scalex and scaley
\param scale factor  smaller than 1 shrinks the image
\param interpolation so far not used
\author Markus Debatin
\date 12-10-2010
*************************************************************************/
void Imagedata::scale(float scale, string interpolation)
{
    xscale(scale,interpolation);
    yscale(scale,interpolation);
}


/*! returns the maximum Element in the roi
 returns the maximum value in the region of interest
\author Markus Debatin
\date 2010
*************************************************************************/
float Imagedata::max()
{
    float max=0;
    if(imgdata == NULL) return 0;
    max = imgdata[(roi.y1*imgwidth+roi.x1)*samplesperpixel];
    unsigned int x2= roi.x1+roi.wx;
    unsigned int y2= roi.y1+roi.wy;
    float* iterator;
    for(unsigned int y=roi.y1;y<y2;y++){
        iterator = &imgdata[imgwidth*y+roi.x1];
        for(unsigned int x=roi.x1;x<x2;x++){
            if(max < *iterator) max=*iterator;
            iterator++;
        }
    }
    return max;
}


/*! calculates sum x_i*y_i
  where x_i is the data of this image and y_i is the
  data of the second image. The whole data is treated
  no difference is made for columns or rows.
 returns the maximum value in the region of interest
\param second second image
\author Markus Debatin
\date 2010
*************************************************************************/
float Imagedata::contraction1D(Imagedata const &second)const
{
    if (!sameSize(second)) return 0;
    if(imgwidth*imgheight ==0) return 0;
    float* d2 = second.ptr();
    float* d1 = imgdata;
    assert(d2 != NULL); assert(d1 != NULL);
    double sum = 0;
    for(int i =imgwidth*imgheight;i>0;i--){
        //        sum += *d1++**d2++;
        sum += *d1 * *d2;
        d1++; d2++;

    }
    return sum;
}

float Imagedata::contractionROI(const Imagedata &second) const
{
    if (!sameSize(second)) return 0;
    if(imgwidth*imgheight ==0) return 0;
    float* d2 = second.ptr();
    float* d1 = imgdata;
    assert(d2 != NULL); assert(d1 != NULL);

    std::vector<float>sum(roi.wx,0);
    //cout<<"colsum"<<roi.wx<<";"<<roi.x1<<";"<<roi.wy<<";"<<roi.y1<<"\n";
    float* buffer_iterator=  &imgdata[imgwidth*roi.y1];
    //float
    for(int i =0;i<roi.wy;i++)
    {
        for(int j =0;j<roi.x1;j++)
            buffer_iterator++;
        for(int j =0;j<roi.wx;j++)
            sum[j]+= *buffer_iterator++;
        for(int j =roi.wx+roi.x1;j<imgwidth;j++)
            buffer_iterator++;
    }
  //  return sum;
}

//! sets the Region of interest
/*!
  sets the Region of interest
coordinates are the top left edge and the bottom right edge
  coordinates are given in pixels of the image
\author Markus Debatin
\date 2010
*************************************************************************/
void Imagedata::setROI(int x1, int y1, int x2, int y2)
{
    if (x2>imgwidth) x2=imgwidth;
    if (y2>imgheight) y2=imgheight;
    if (x1<0) x1=0;
    if (y1<0) y1=0;
    if((x1>x2)||(y1>y2))
    { x1=0; x2=imgwidth; y1=0; y2=imgheight;}
    roi.x1=x1; roi.y1=y1; roi.wx=x2-x1; roi.wy=y2-y1;

}

//! just check whether the other image has the same dimensions
/*!
\param image to be compared with this one
\author Markus Debatin
\date 2010
*************************************************************************/
bool Imagedata::sameSize(Imagedata const& other)const
{
    return((imgwidth == other.imgwidth)&&
           (imgheight == other.imgheight)&&
           (samplesperpixel == other.samplesperpixel));
}

/*! checks whether the image is empty
\author Markus Debatin
\date 2010
*************************************************************************/
bool::Imagedata::empty()
{
    if(imgwidth ==0) return true;
    if(imgheight ==0)return true;
    return false;
}

/*!
  cuts the image to the size of the roi
\author Markus Debatin
\date 2010
*************************************************************************/
void Imagedata::cutimg()
{
    float* tmp = new float[roi.wx*roi.wy*samplesperpixel];
    int spp=samplesperpixel;
    float* buff_iter= tmp;
    //float tmp[imgwidth*imgheight*spp];
    for(int y=0;y<roi.wy;y++)
        for(int x=0; x<roi.wx;x++)
            for(int s=0;s<spp;s++){
        *buff_iter= imgdata[(y+roi.y1)*imgwidth*spp+(roi.x1+x)*spp+s];
        buff_iter++;
    }
    delete[] imgdata;
    imgdata=tmp;
    roi.x1=0; roi.y1=0; imgwidth=roi.wx; imgheight=roi.wy;
}

//! copies the image from the other image
/*! for a=b copies b to a
\author Markus Debatin
\date 2010
*************************************************************************/
Imagedata& Imagedata::operator=(Imagedata const& other)
                               {
    imgwidth = other.imgwidth;
    imgheight = other.imgheight;
    samplesperpixel = other.samplesperpixel;
    setROI();
    if (imgdata!=NULL) delete[] imgdata;
    if(imgwidth*imgheight*samplesperpixel>0){
        imgdata = new(nothrow) float[imgwidth*imgheight*samplesperpixel];
        if(imgdata == NULL)cout<<"imgdata allocation error\n";
    }
    else imgdata = NULL;
    float* buff_iter = imgdata;
    float* other_iter = other.ptr();
    for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--){
        *buff_iter++=*other_iter++;
        assert(buff_iter <= &imgdata[imgwidth*imgheight*samplesperpixel]);
    }
    return *this;
}

//! add another image pixelwise to this image
/*!

Nothing happens if dimensions don't match
\author Markus Debatin
\date 10-2010
*************************************************************************/
Imagedata& Imagedata::operator+=(Imagedata const& other)
                                {
    if(empty()) operator=( other);
    else if (sameSize(other)){
        float* buff_iter =  imgdata;
        float* other_iter = other.ptr();
        for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
            *buff_iter++ += *other_iter++;
    }
    return *this;
}
//! see +=
/*!
 see +=
\author Markus Debatin
\date 09-2010
*************************************************************************/
Imagedata& Imagedata::operator-=(Imagedata const& other)
                                {
    if (sameSize(other)){
        float* buff_iter =  imgdata;
        float* other_iter = other.ptr();
        for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
            *buff_iter++ -= *other_iter++;
    }
    return *this;
}
//! Elementwise multiplikation
Imagedata& Imagedata::operator*=(Imagedata const& other)
                                {
    if (sameSize(other)){
        float* buff_iter =  imgdata;
        float* other_iter = other.ptr();
        for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
            *buff_iter++ *= *other_iter++;
    }
    return *this;
}
//! Elementwise division
Imagedata& Imagedata::operator/=(Imagedata const& other)
                                {
    if (sameSize(other)){
        float* buff_iter =  imgdata;
        float* other_iter = other.ptr();
        for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
            *buff_iter++ /= *other_iter++;
    }
    return *this;
}
//! add a constant value to each element
Imagedata& Imagedata::operator+=(float const other)
                                {
    assert((imgdata!=NULL)||(imgwidth==0)||(imgheight==0));
    float* buff_iter =  imgdata;
    for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
        *buff_iter++ += other;
    return *this;
}
//! subtract a value (other) from each element
Imagedata& Imagedata::operator-=(float const other)
                                {
    assert((imgdata!=NULL)||(imgwidth==0)||(imgheight==0));
    float* buff_iter =  imgdata;
    for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
        *buff_iter++ -= other;
    return *this;
}
//! scalar multiplication
Imagedata& Imagedata::operator*=(float const other)
                                {
    assert((imgdata!=NULL)||(imgwidth==0)||(imgheight==0));
    float* buff_iter =  imgdata;
    for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
        *buff_iter++ *= other;
    return *this;
}
//! scalar division
Imagedata& Imagedata::operator/=(float const other)
                                {
    assert((imgdata!=NULL)||(imgwidth==0)||(imgheight==0));
    float* buff_iter =  imgdata;
    for(int i= imgwidth*imgheight*samplesperpixel;i>0;i--)
        *buff_iter++ /= other;
    return *this;
}
//! return value at certain xy position
//float Imagedata::operator()(int x, int y)
//{
//return imgdata[imgwidth*y+x];
//}


//! take logarithm of each element
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::ln()
{
    for(float* buff_iter= &imgdata[imgwidth*imgheight*samplesperpixel];
        buff_iter>imgdata; --buff_iter)
        *buff_iter = log(*buff_iter);
}

/*! rotate image by 90 degrees
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::rot90()
{
    float* tmp = new float[imgwidth*imgheight*samplesperpixel];
    int spp=samplesperpixel;
    float* buff_iter= imgdata;
    //float tmp[imgwidth*imgheight*spp];
    for(int y=0;y<imgheight;y++)
        for(int x=0; x<imgwidth;x++)
            for(int s=0;s<spp;s++){
        tmp[(imgwidth-x-1)*imgheight*spp+y*spp+s]=*buff_iter;
        buff_iter++;
    }
    delete[] imgdata;
    imgdata=tmp;
    int t=imgwidth; imgwidth=imgheight; imgheight=t;
}

/*! rotates the image by deg degrees
  es wird das gewichtete Mittel aus den umliegenden Nachbarn gebildet.
\param deg number of degrees by which the image should be rotated
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::rot(float deg)
{
    float cosa= cos(Pi*2*deg/360.0);
    float sina= sin(Pi*2*deg/360.0);  // sin and cosine take time this is to speed up
    float ox, oy,rx, ry;                     // x and y in the old image
    int ix, iy;
    float* tmp = new float[imgwidth*imgheight*samplesperpixel];   //temporary buffer
    int spp=samplesperpixel;                   // is eg. 3 for rgb pictures
    float* buff_iter= tmp;                //iterator
    //float tmp[imgwidth*imgheight*spp];
    for(int y=0;y<imgheight;y++)      // y in new image
        for(int x=0; x<imgwidth;x++){
        ox=(x-imgwidth/2.0)*cosa-(y-imgheight/2.0)*sina+imgwidth/2.0 ;
        oy=(x-imgwidth/2.0)*sina+(y-imgheight/2.0)*cosa+imgheight/2.0;
        rx= ox-((int)ox); ry=oy-((int)oy);
        ix = ox; iy=oy;
        for(int s=0;s<spp;s++){
            (*buff_iter)=0;
            if((ix<imgwidth-1)&&(iy<imgheight-1)&&(ix>=0)&&(iy>=0)){
                (*buff_iter)+= imgdata[iy*imgwidth*spp+ix*spp+s]*(1-rx)*(1-ry);
                (*buff_iter)+= imgdata[iy*imgwidth*spp+(ix+1)*spp+s]*rx*(1-ry);
                (*buff_iter)+= imgdata[(iy+1)*imgwidth*spp+ix*spp+s]*(1-rx)*ry;
                (*buff_iter)+= imgdata[(iy+1)*imgwidth*spp+(ix+1)*spp+s]*rx*ry;
            }
            buff_iter++;
        }
    }
    delete[] imgdata;
    imgdata=tmp;
}

/*! flips the image
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::flip_horizontal()
{
    int spp=samplesperpixel;
    float* tmp = new float[imgwidth*imgheight*spp];
    float* buff_iter= imgdata;
    //float tmp[imgwidth*imgheight*spp];
    for(int y=0;y<imgheight;y++)
        for(int x=0; x<imgwidth;x++)
            for(int s=0;s<spp;s++){
        tmp[((y+1)*imgwidth-x-1)*spp+s]=*buff_iter;
        buff_iter++;
    }
    delete[] imgdata;
    imgdata=tmp;
}

/*! substitue negative values by 0
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagedata::makePositive()
{
    for(float* buff_iter= &imgdata[imgwidth*imgheight*samplesperpixel];
        buff_iter>imgdata; --buff_iter)
        if(*buff_iter<0)  *buff_iter = 0;
}

