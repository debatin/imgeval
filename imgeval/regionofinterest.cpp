#include "regionofinterest.h"
#include<QtGui>
#include<iostream>
#include "atomimg.h"
using namespace std;
/*! constructs an imgmarker for full functionality use other constructor*/
imgmarker::imgmarker(QGraphicsItem* parent)
  :QGraphicsRectItem(parent)
{
  setBrush(QBrush(QColor(100,255,0)));
  setRect(QRect(0,0,10,10));
  // setAcceptDrops(true);
  atomimg = NULL;
}


/*! constructs an imgmarker
  \param name name of the marker (can be nything to identify it)
  \param img  Atomimg class this marker belongs to
  */
imgmarker::imgmarker(string name,Atomimg* img, QGraphicsItem* parent)
  :QGraphicsRectItem(parent)
{
  setBrush(QBrush(QColor(100,255,0)));
  setRect(QRect(0,0,10,10));
  this->name = name;
  this->atomimg=img;
  if(atomimg != NULL) atomimg->markerpos_changed(name, pos()); // aktuelle Position bekanntgeben
  // setAcceptDrops(true);
}
//imgmarker::~imgmarker()
//{}

/* void imgmarker::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
 {
     event->setAccepted(event->mimeData()->hasFormat("text/plain"));
     update();

     }*/
void imgmarker::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
   QGraphicsItem::mousePressEvent(event); 
   event->accept();
   cout<<"pressevent\n";
   clickpos=event->pos();

} 
/*! event handler used to move the marker
   */
void imgmarker::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
   QGraphicsItem::mouseMoveEvent(event); 
   setPos(event->scenePos()-clickpos);
   event->accept();
   //emit moved(pos());
   if(atomimg != NULL) atomimg->markerpos_changed(name, pos());
   cout<<"moveevent\n";

} 


/*! constructor for a Region of interest Rectangle */
ROI::ROI(QGraphicsItem* parent)
  :QGraphicsRectItem(parent)
{
  setPen(QPen(QColor(100,255,0)));
  setRect(QRect(0,0,10,10));
  setFlag(QGraphicsItem::ItemIsSelectable,true);
  grabsize=20;

  // setAcceptDrops(true);
}
//imgmarker::~imgmarker()
//{}

/* void imgmarker::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
 {
     event->setAccepted(event->mimeData()->hasFormat("text/plain"));
     update();

     }*/
void ROI::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    grabsize=rect().width()/8+2; //adjust grab region to size of rect
   QGraphicsItem::mousePressEvent(event); 
   clickpos=event->pos();
   if((clickpos.x()>(rect().width())-grabsize)&&(clickpos.y()>(rect().height()-grabsize))){
     mode = 2;  //resize
     event->accept();
   setSelected(true);
     return;
   }
   if((clickpos.x()<grabsize)||(clickpos.y()<grabsize)){
     mode = 1;  //move
     event->accept();
     setSelected(true);
     return;
   }


} 

/* event handler used to move and resize the ROI */
void ROI::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
   QGraphicsItem::mouseMoveEvent(event); 
   if(mode==1){
     setPos(event->scenePos()-clickpos);
  //   emit moved(pos());
 }
   if(mode==2){
     setRect(QRectF(QPointF(0,0),event->pos()+QPointF(1,1)));
  }
   event->accept();

} 

void ROI::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
  setSelected(false);
  cout<<"ROIrelease"<<event->pos().x()<<";"<<event->pos().y()<<"\n"; 
  QGraphicsItem::mouseMoveEvent(event); 
   if(mode==2){
     QPointF pos1 = pos();
     QPointF pos2 = event->pos()+pos();
     if(pos1.x()>pos2.x()) {
       float f=pos1.x(); pos1.setX(pos2.x()); pos2.setX(f);}
     if(pos1.y()>pos2.y()) {
       float f=pos1.y(); pos1.setY(pos2.y()); pos2.setY(f);}
     setPos(pos1);
     pos2 = pos2-pos();
     setRect(QRectF(QPointF(0,0),pos2+QPointF(1,1)));
  }
   mode=0;
   //  emit ROIChanged();
} 

