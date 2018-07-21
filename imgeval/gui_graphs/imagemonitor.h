#ifndef IMAGEMONITOR_H
#define IMAGEMONITOR_H
#include <QGraphicsScene>
#include <QPixmap>
#include <QImage>
#include "datastructure/imgdata.h"
#include <QWidget>
#include <iostream>
class QGraphicsPixmapItem;
class QGraphicsSceneWheelEvent;
class Dataset;
class MainWindow;
#define uchar unsigned char
using namespace std;
struct ARGB
{
  unsigned char a;
  unsigned char r;
  unsigned char g;
  unsigned char b;
};


//! Class for displaying the images on the screen
/*! Stores the data in an Imagedata class. And displays it using different colorscheemes.
  Also zooming capabilities and regions of interes should be implemented here.

*/
class Imagemonitor: public QGraphicsScene
{
Q_OBJECT
public:
  Imagemonitor(Dataset* settings, MainWindow* mainapp, std::string name,QWidget* parent = 0);
    void updateimg();
    void readfile();
    void setImage(Imagedata* img);
    void createPalette();
    void clear();
    void toFile(string filename);
    void wheelEvent(QGraphicsSceneWheelEvent* event);
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void setColorScale(float newscale){if(newscale>0) colorscaling=newscale;}  //adjust the color scale
public slots:
    void autoscaleColor();  //autoscales the palette colors

private:
    Imagedata* img;  //data of the image
    std::string name; //name of the species;
    float zoomfactor;
    QPixmap pixmap;
    QGraphicsPixmapItem* pixmapitem;
    QImage image;
    uchar* pixdata;
    ARGB palette[1000]; 
    unsigned char* pal;
    float coloroffset;
    float colorscaling; //<to see the full image: maxvalue*colorscaling = 1000;
    Dataset* settings;
    MainWindow* mainapp;
};


#endif // IMAGEMONITOR_H
