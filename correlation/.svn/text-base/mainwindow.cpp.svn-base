#include"mainwindow.h"
#include<QtGui>
#include<fstream>
#include<iostream>
using namespace std;
CorrelationGUI::CorrelationGUI(QWidget *parent)
{
    QVBoxLayout* lay = new QVBoxLayout;
    filename_b = new QPushButton("filename");
    lay->addWidget(filename_b);
    connect(filename_b,SIGNAL(clicked()),this,SLOT(setFilename()));
    QPushButton* calculate_b = new QPushButton("calculate");
    connect(calculate_b,SIGNAL(clicked()),this,SLOT(calculate()));
    lay->addWidget(calculate_b);
    setLayout(lay);
}

void CorrelationGUI::setFilename()
{
    QString ddir = QFileDialog::getExistingDirectory((QWidget*)this, tr("Open Directory"),"");
    filename_b->setText(ddir);
    dir.setPath(ddir);
    createFileList();
    // readfile();
}

void CorrelationGUI::createFileList()
{
    filelist.clear();
    QStringList filters;
    QStringList qfilenames;
    filters=QStringList( "*.blc");
    qfilenames=dir.entryList(filters,QDir::Readable|QDir::Files,QDir::Name);
    for(int i = 0; i < qfilenames.size();i++) filelist.push_back(dir.absolutePath().toStdString()+
                                                                 "\\" +qfilenames[i].toStdString());
    for(int i=0;i<filelist.size();i++) cout<<filelist[i]<<"\n";
}

void CorrelationGUI::calculate()
{
    if(filelist.size()<4)return;
    std::ifstream imgfile;
    try{
        imgfile.open(filelist[0].c_str(),std::ios::binary);
    }
    catch(...)
    { cout<<"file openin err\n"; return;
    }
    cout<<"opening "<<filelist[0]<<"\n";
    int imgwidth;
    int imgheight;
    int imgsize;
    int width;
    int height;
    imgfile.read((char*) &imgwidth,4);   //ignore header
    imgfile.read((char*) &imgwidth,4);   //read width
    imgfile.read((char*) &imgheight,4); //read height
    imgsize=imgwidth*imgheight;
    imgfile.close();
    cout<<"imgwidth"<<imgwidth<<"\n";
    cout<<"imgheight: "<<imgheight<<"\n";
    long memneed = sizeof(float)*imgsize*filelist.size();
    long sqmatrix_mem = filelist.size()*filelist.size()*sizeof(float);
    cout<<memneed/1000000<<"+"<<sqmatrix_mem/1000000<<"MB memory needed\n";
    float* bigmatrix = NULL;
    float* square = NULL;  //square matrix
    bigmatrix= new(nothrow) float[memneed/sizeof(float)];
    square= new(nothrow) float[sqmatrix_mem/sizeof(float)];
    if((bigmatrix == NULL)||(square == NULL))
    {
        cout<<"not enough memory\n";
        return;
    }
    int nimage =filelist.size(); //number of images
    cout<<"calculating for "<<nimage<<" images\n";
    cout<<"reading data "; cout.flush();
    for(int i =0;i<nimage;i++)   //read in data
    {
        try{
            imgfile.open(filelist[i].c_str(),std::ios::binary);
        }
        catch(...)
        { cout<<"file openin err\n"; return;
        }
        imgfile.read((char*) &width,4);   //ignore header
        imgfile.read((char*) &width,4);   //read width
        imgfile.read((char*) &height,4); //read height
        if((width!= imgwidth)||(height != imgheight)){
            cout<<"wrong dimensions image nr: "<<i<<"\n";
            return;
        }
        imgfile.read((char*) &bigmatrix[imgsize*i],imgsize*4);
        ///< reading data to position in big matrix;
        imgfile.close();
        cout<<"."; cout.flush();
    }
    // calculate A*A
    float* iter1; //define two iterators;
    float* iter2;
    float sum;
    cout<<"\n calculting correlation matrix "; cout.flush();
    for(int y = 0;y<nimage;y++){
        cout<<"."; cout.flush();
        for(int x = 0;x<nimage;x++)    {
          sum=0;
          iter1= &bigmatrix[x*imgsize];
          iter2= &bigmatrix[y*imgsize];
          for(int i = 0;i<imgsize;i++) sum+=*iter1++**iter2++; //multiply two iterator values increas iterators afterwards
          square[y*nimage+x]=sum;
      }
    }




    //    bool dimensionchange = !((imgwidth == oldimgwidth) && (imgheight= oldimgheight) && (samplesperpixel ==1));
    //    if ((dimensionchange)&&(imgdata != NULL)){
    //        delete[] imgdata;  //free memory from last image
    //        imgdata = new(nothrow) float[imgwidth*imgheight*samplesperpixel];  // allocate memory
    //    }
    //    samplesperpixel=1;                                  // only monochrome images
    //    if(imgdata != NULL) imgfile.read((char*) imgdata,imgwidth*imgheight*sizeof(float));  //read the imagedata
    //    else{ imgheight =0; imgwidth =0; cout<<"memallocation failed imgdata\n";}  // in case memory allocation failed
    //    imgfile.close();


    //  if(filelist.size()<4) return;   // some data needed
  /*  //! calculate memory needs;
    float* buffer = NULL;
    int i = 1000000000;
    while(buffer == NULL){
        i/= 2;
        buffer = new(nothrow) float[i];
    }
    cout<<i<<"mem allocated\n";
    float* buffer2 = NULL;
    int i2 = 1000000000;
    while(buffer2 == NULL){
        i2/= 2;
        buffer2 = new(nothrow) float[i2];
    }
    cout<<i2<<"mem allocated\n";*/
    //! find large memory blocks;


}



