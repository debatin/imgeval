#ifndef REGIONOFINTEREST
#define REGIONOFINTEREST
#include<QGraphicsRectItem>
#include<QGraphicsItem>
#include<QGraphicsPathItem>
#include<QPointF>
#include<iostream>
#include<QWidget>
#include <QBrush>
#include <QGraphicsWidget>
class Atomimg;
class QContextMenuEvent;
using namespace std;



class ROI: public QGraphicsRectItem
{
//Q_OBJECT
    public:
  ROI(QGraphicsItem* parent=0);
  //  ~imgmarker();
  // void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent ( QGraphicsSceneMouseEvent * event ); 
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event ); 
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ); 
    QPointF pos2(){return pos()+rect().bottomRight();};
    float width(){return rect().width();};
    float height(){return rect().height();};

private:
  int test;
  QPointF clickpos;
  QPointF clicksize;
  int mode;
  int grabsize; //size of the region the mouse grabs to
};

#endif
