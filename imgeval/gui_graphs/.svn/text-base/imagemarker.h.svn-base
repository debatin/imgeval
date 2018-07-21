#ifndef IMAGEMARKER_H
#define IMAGEMARKER_H

#include<QWidget>
#include <QGraphicsWidget>
#include<QGraphicsRectItem>
#include<QGraphicsItem>
#include<QGraphicsPathItem>
#include<QPointF>
#include<iostream>
//#include<QWidget>
#include <QBrush>
using namespace std;
class Atomimg;
class ImageMarker: public QGraphicsWidget
{
    Q_OBJECT
public:
  ImageMarker(QGraphicsItem* parent=0);
  ImageMarker(string name,Atomimg* img=NULL,  QGraphicsItem* parent=0);
  //  ~ImageMarker();
  // void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
  void createActions();
  void mousePressEvent ( QGraphicsSceneMouseEvent * event );
  void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
  void setBrush(const QBrush brush);
  string getname(){return name;};

public slots:
  void size10();
  void incsize();
  void decsize();
  void lock(bool);
protected:
  bool locked;
private:

  int test;
  string name;
  Atomimg* atomimg;
  QPointF clickpos;
  QGraphicsRectItem* qr;
public:
  QAction* markersizeAct;
  QAction* size10Act;
  QAction* sizeincAct;
  QAction* sizedecAct;
  QAction* lockAct;
  ImageMarker* t;
public slots:
    void testslot();
private:
    int tt;
};



#endif // IMAGEMARKER_H
