#include <QtGui>
#include "imagemonitor.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include"datastructure/dataset.h"
#include"mainwindow.h"
using namespace std;
Imagemonitor::Imagemonitor(Dataset* setting, MainWindow* mainapp, std::string name, QWidget* parent)
    :QGraphicsScene(parent)
{
    this->name=name;
    settings=setting;
    this->mainapp = mainapp;
    pixdata=NULL;
    img=NULL;
    zoomfactor=1;
    readfile();
    //updateimg();
    pixmapitem = new QGraphicsPixmapItem;
    addItem(pixmapitem);
    createPalette();
    pal=(unsigned char*)palette;
    coloroffset=0;
    colorscaling=1;
}

void Imagemonitor::readfile()
{
}


//! sets the new image to be diplayed and updates the display
void Imagemonitor::setImage(Imagedata* newimg)
{
    img=newimg;
    if (pixdata!=NULL) delete[] pixdata;
    pixdata=new unsigned char[img->height()*img->width()*4];
    updateimg();
}

//!generates RGB image and displays it
/*! An RGB image is generated from the float data using
  the given palette, colorscalings, etc.
  The RGB  image is then displayed
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagemonitor::updateimg()
{
    if(img == NULL)return;

    float* imgbuffer= img->ptr();
    for(int i = 0;i<img->height();i++)
        for(int j=0;j<img->width();j++){
        int dat=(((int)((imgbuffer[i*img->width()+j])*colorscaling+coloroffset))%1000);
        if(dat<0) dat=0;
        for(int k=0;k<4;k++)
            pixdata[i*img->width()*4+j*4+k]= pal[4*dat+3-k];
    }
    image=QImage(pixdata,img->width(),img->height(),QImage::Format_RGB32);
    pixmap=pixmap.fromImage(image);
    pixmapitem->setPixmap(pixmap.scaled((int)(img->width()*zoomfactor),
                                        (int)(img->height()*zoomfactor)));
}

//! creates the palette from a vector of colors
/*! creates the Palette vrom a vector of colors stored in the central settings
The vector should have n*3 entries which are interpreted as R1 G1 B1 R2 G2.....
where R1 is the red component of the first color.
The colors are placed equidistant in the number space and a smooth transition
between the colors is created.
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagemonitor::createPalette()
{
    //setpal contains n*3 numbers describing rgb colors
    vector<double> setpal;
    setpal=(*settings)["palette"].vd;
    // cout<<pal->size()<<endl;
    int ncolors=1000;  //corresponds to the definition in the class!!
    int npal = setpal.size()/3-1; //anzahl der Teilpaletten
    int i=0;
    for(int j=0;j<npal;j++){
        float s=0; //scale to vary from 0 to 1
        while(i<ncolors*(j+1)/(npal)){
            palette[i].a=0;
            palette[i].r=setpal[3*j  ]*(1-s)+setpal[3*j+3]*s;
            palette[i].g=setpal[3*j+1]*(1-s)+setpal[3*j+4]*s;
            palette[i].b=setpal[3*j+2]*(1-s)+setpal[3*j+5]*s;
            i++;
            s+=(float)npal/1000.0;
        }
    }
}

/*! removes all items and deletes them
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagemonitor::clear()
{
    //cout<<"imgmonitor clear\n";
    if (pixdata!=NULL) delete[] pixdata;
    pixdata=NULL;
    QList<QGraphicsItem *>it = items();
    for(int i = 0; i<it.size();i++){
        removeItem(it[i]);
        delete it[i];

    }
    img=NULL;
}

/*! saves the image to a file
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagemonitor::toFile(string filename)
{
    if(filename.length()<5) return;    //there should be at least *.??? z.B.: a.png
    if(filename.substr(filename.length()-4,4) == ".mat")
        img->save_matlab(filename.c_str());
    else
        pixmap.save(QString(filename.c_str()),0,settings->get("saveimg_quality",(int)-1).i);
}

//! if ALT is pressed and the mouse wheel is turned this function is called
/*! from the rotation of the wheel a scalingfactor is calculated, which is used
  for colorscaling or coloroffset (if shift is pressed)
\author Markus Debatin
\date 10-2009
*************************************************************************/
void Imagemonitor::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    int numsteps =event->delta()/30;   //1° corresponds delta=8. Our mouse 1Step=15°=120delta
    float scalingfactor=1;
    if (numsteps>0)
        for(int i = 0;i<numsteps;i++)
            scalingfactor*=1.03;
    else
        for(int i = 0;i<-numsteps;i++)
            scalingfactor/=1.03;
    event->accept();
    if(event->modifiers()& Qt::ShiftModifier)
    {coloroffset+=numsteps*10;}
    else
        colorscaling*=scalingfactor;
    updateimg();
}

/*! used to show the mouse position on the status bar
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagemonitor::mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    if(img==NULL) return;
    ostringstream os;
    int x =(int)mouseEvent->scenePos().x();
    int y = (int)mouseEvent->scenePos().y();
    os<<"("<<x<<", "<<y<<")  "<<(*img)(x,y);
    mainapp->statusBar()->showMessage(os.str().c_str());
}

/*! used to autoscale the color
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Imagemonitor::autoscaleColor()
{
    if(img == NULL) return;  //if no image set
    float maxval = img->max();
    if(maxval >0)
        setColorScale(1000.0/maxval);
    updateimg();

}
